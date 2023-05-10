
#include "StdAfx.h"
#include "BattleLineSpline.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <DefaultComponents/Input/InputComponent.h>

namespace
{
    static void RegisterBattleLineSplineComponent(Schematyc::IEnvRegistrar& registrar)
    {
        Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
        {
            Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CBattleLineSpline));
        }
    }
    CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterBattleLineSplineComponent);
}


void CBattleLineSpline::Initialize()
{
    m_pFormation = m_pEntity->GetComponent<CBattleFormation>();

    Cry::DefaultComponents::CInputComponent* pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
    pInput->RegisterAction("battle_formation", "init", [this](int activationMode, float value)
        {
            CryLog("Initializing Spline");
            InitializeSpline();
        });
    pInput->BindAction("battle_formation", "init", EActionInputDevice::eAID_KeyboardMouse, EKeyId::eKI_I, true, false, false);
    pInput->RegisterAction("battle_formation", "insert", [this](int activationMode, float value)
        {
            InsertCurve(GetFormation()->GetColumn(2), 0.5f);
        });
    pInput->BindAction("battle_formation", "insert", EActionInputDevice::eAID_KeyboardMouse, EKeyId::eKI_O, true, false, false);
}

Cry::Entity::EventFlags CBattleLineSpline::GetEventMask() const
{
    return Cry::Entity::EEvent::Update;
}

void CBattleLineSpline::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Update:
        break;
    }
}

void CBattleLineSpline::InsertCurve(IFormationColumn* pColumn, float normalizedOffset)
{
    float battleLineLength = pColumn->GetXPos() + (normalizedOffset * pColumn->GetWidth() / 2);
    CCurveIterator curveItr = FindCurve(battleLineLength);

    CRY_ASSERT(curveItr != GetEndCurve(), "Cannot insert a new curve at the end of a spline!");
    if (curveItr == GetEndCurve())
    {
        CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Cannot insert a new curve at the end of a spline!");
        return;
    }

    CVertexPoint* pLeftVertex = *curveItr.m_vertexItr;
    CVertexPoint* pRightVertex = *(curveItr.m_vertexItr + 1);
    CCurveLine* pCurveLine = *curveItr.m_curveLineItr;
    float t = (battleLineLength - pLeftVertex->GetBattleLineXPos()) / (pRightVertex->GetBattleLineXPos() - pLeftVertex->GetBattleLineXPos());
    SVertexSpawnParams vertexParams;
    vertexParams.pSpline = this;
    vertexParams.gridPosition = CalculateCurvePosition(curveItr, t);
    CVertexPoint* pNewVertex = CVertexPoint::SpawnVertex(vertexParams);

    float curveMagnitude = (pRightVertex->GetPos() - pLeftVertex->GetPos()).GetLength2D();
    Vec2 outAnchorOffset = pCurveLine->GetOutAnchor()->GetPos() - pLeftVertex->GetPos();
    Vec2 inAnchorOffset = pCurveLine->GetInAnchor()->GetPos() - pRightVertex->GetPos();
    float outAnchorMagnitude = outAnchorOffset.GetLength();
    float inAnchorMagnitude = inAnchorOffset.GetLength();
    Vec2 newOutAnchorPos = outAnchorOffset * (outAnchorMagnitude / curveMagnitude) + pLeftVertex->GetPos();
    Vec2 newInAnchorPos = inAnchorOffset * (inAnchorMagnitude / curveMagnitude) + pRightVertex->GetPos();
    pCurveLine->GetOutAnchor()->SetPos(newOutAnchorPos);
    pCurveLine->GetInAnchor()->SetPos(newInAnchorPos);
    
    Vec2 vertexTangent = CalculateCurveTangent(curveItr, t).Normalize();
    SAnchorSpawnParams anchorParams;
    anchorParams.pSpline = this;
    float newOutAnchorMagnitude = m_normalizedAnchorLength * (pNewVertex->GetPos() - pRightVertex->GetPos()).GetLength2D();
    anchorParams.gridPosition = newOutAnchorMagnitude * vertexTangent + pNewVertex->GetPos();
    CAnchorPoint* pNewOutAnchor = CAnchorPoint::SpawnAnchor(anchorParams);
    float newInAnchorMagnitude = m_normalizedAnchorLength * (pNewVertex->GetPos() - pLeftVertex->GetPos()).GetLength2D();
    anchorParams.gridPosition = newInAnchorMagnitude * -vertexTangent + pNewVertex->GetPos();
    CAnchorPoint* pNewInAnchor = CAnchorPoint::SpawnAnchor(anchorParams);
    
    CCurveLine* pNewCurveLine = new CCurveLine(pNewOutAnchor, pCurveLine->GetInAnchor(), pCurveLine->GetCurveMode());
    pCurveLine->SetInAnchor(pNewInAnchor);
    
    m_vertexPoints.insert(curveItr.m_vertexItr + 1, pNewVertex);
    m_curveSegments.insert(curveItr.m_curveLineItr + 1, pNewCurveLine);
}

void CBattleLineSpline::InsertCurve(float battleLineXPos)
{
}

void CBattleLineSpline::RemoveCurve(const IVertexPoint* vertex, const ECurveDirection& dir)
{
}

void CBattleLineSpline::ApplyCurveMode(ICurveSegment* curveSegment, const ICurveSegment::ECurveMode& mode)
{
}

void CBattleLineSpline::MoveVertex(IVertexPoint* pVertex, const Vec2& pos)
{
    CCurveIterator curveItr = FindCurve(pVertex);
}

void CBattleLineSpline::MoveAnchor(IAnchorPoint* pAnchor, const Vec2& pos)
{
}

CCurveIterator CBattleLineSpline::FindCurve(const IVertexPoint* vertex)
{
    CCurveIterator curveItr = GetStartCurve();
    while (curveItr != GetEndCurve())
    {
        if (curveItr.GetStartVertex() == vertex)
        {
            return curveItr;
        }
    }
    return curveItr;
}

CCurveIterator CBattleLineSpline::FindCurve(const IFormationColumn* pColumn, float normalizedOffset)
{
    CCurveIterator curveItr;
    for (curveItr = GetStartCurve(); curveItr != GetEndCurve(); ++curveItr)
    {
        if (curveItr.GetEndVertex()->GetBattleLineXPos() > pColumn->GetXPos())
        {
            return curveItr;
        }
    }
    return curveItr;
}

CCurveIterator CBattleLineSpline::FindCurve(float battleLineLength)
{
    CCurveIterator curveItr;
    for (curveItr = GetStartCurve(); curveItr != GetEndCurve(); ++curveItr)
    {
        if (curveItr.GetEndVertex()->GetBattleLineXPos() > battleLineLength)
        {
            return curveItr;
        }
    }
    return curveItr;
}

Vec3 CBattleLineSpline::GetOutAnchorOffset(const CCurveIterator& curveItr) const
{
    return curveItr.GetOutAnchor()->GetPos() - curveItr.GetStartVertex()->GetPos();
}

Vec3 CBattleLineSpline::GetInAnchorOffset(const CCurveIterator& curveItr) const
{
    return curveItr.GetInAnchor()->GetPos() - curveItr.GetEndVertex()->GetPos();
}

Vec3 CBattleLineSpline::CalculateCurvePosition(const CCurveIterator& curveItr, float t1) const
{
    CRY_ASSERT(curveItr != GetEndCurve(), "Curve iterator out of range in spline!");
    t1 = crymath::clamp(t1, 0.0f, 1.0f);

    Vec3 startVertexPos = curveItr.GetStartVertex()->GetPos();
    Vec3 outAnchorPos = curveItr.GetOutAnchor()->GetPos();
    Vec3 inAnchorPos = curveItr.GetInAnchor()->GetPos();
    Vec3 endVertexPos = curveItr.GetEndVertex()->GetPos();
    return Vec3::CreateCubicCurve(startVertexPos, outAnchorPos, inAnchorPos, endVertexPos, t1);
}

Vec3 CBattleLineSpline::CalculateCurveTangent(const CCurveIterator& curveItr, float t1) const
{
    CRY_ASSERT(curveItr != GetEndCurve(), "Curve iterator out of range in spline!");
    t1 = crymath::clamp(t1, 0.0f, 1.0f);

    Vec3 startVertexPos = curveItr.GetStartVertex()->GetPos();
    Vec3 outAnchorPos = curveItr.GetOutAnchor()->GetPos();
    Vec3 inAnchorPos = curveItr.GetInAnchor()->GetPos();
    Vec3 endVertexPos = curveItr.GetEndVertex()->GetPos();
    float t0 = 1.0f - t1;
    return 3.0f * t0 * t0 * (outAnchorPos - startVertexPos) + 6.0f * t0 * t1 * (inAnchorPos - outAnchorPos) + 3.0f * t1 * t1 * (endVertexPos - inAnchorPos);
}

void CBattleLineSpline::InitializeSpline()
{
    SVertexSpawnParams vertexParams;
    vertexParams.pSpline = this;
    vertexParams.gridPosition = Vec2(0, 0);
    CVertexPoint* pStartVertex = CVertexPoint::SpawnVertex(vertexParams);
    vertexParams.gridPosition = Vec2(m_pFormation->GetBattleLineLength(), 0);
    CVertexPoint* pEndVertex = CVertexPoint::SpawnVertex(vertexParams);

    m_vertexPoints.push_back(pStartVertex);
    m_vertexPoints.push_back(pEndVertex);

    SAnchorSpawnParams anchorParams;
    anchorParams.pSpline = this;
    Vec2 curveDifference = pEndVertex->GetPos() - pStartVertex->GetPos();
    Vec2 anchorDifference = curveDifference * m_normalizedAnchorLength;

    anchorParams.gridPosition = pStartVertex->GetPos() + anchorDifference;
    CAnchorPoint* pOutAnchor = CAnchorPoint::SpawnAnchor(anchorParams);
    anchorParams.gridPosition = pEndVertex->GetPos() + anchorDifference.flip();
    CAnchorPoint* pInAnchor = CAnchorPoint::SpawnAnchor(anchorParams);

    CCurveLine* pCurveLine = new CCurveLine(pOutAnchor, pInAnchor);
    m_curveSegments.push_back(pCurveLine);
}

void CBattleLineSpline::RestrictVertexAngle(const CCurveIterator& curveItr)
{
    Vec3 leftVertexPos = (curveItr - 1).GetStartVertex()->GetPos();
    Vec3 rightVertexPos = curveItr.GetEndVertex()->GetPos();

    Vec3 baseCurveLine = rightVertexPos - leftVertexPos;
    Vec3 leftCurveLine = curveItr.GetStartVertex()->GetPos() - leftVertexPos;
    Vec3 rightCurveLine = curveItr.GetStartVertex()->GetPos() - rightVertexPos;

    Quat leftRotation = Quat::CreateRotationV0V1(baseCurveLine, leftCurveLine);
    Quat rightRotation = Quat::CreateRotationV0V1(-baseCurveLine, rightCurveLine);

    Ang3 maxLeftAngle = Ang3(leftRotation).Normalize() * m_maxVertexAngleOffset;
    Ang3 maxRightAngle = Ang3(rightRotation).Normalize() * m_maxVertexAngleOffset;
    Quat maxLeftRotation = Quat::CreateRotationXYZ(maxLeftAngle);
    Quat maxRightRotation = Quat::CreateRotationXYZ(maxRightAngle);
}
