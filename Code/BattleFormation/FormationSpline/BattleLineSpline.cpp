
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
    if (!m_pFormation)
    {
        CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_WARNING, "Battle Line Spline requires an existing Battle Formation");
        CryLog("Creating Battle Formation...");
        m_pFormation = m_pEntity->CreateComponent<CBattleFormation>();
    }

    InitializeSpline();
    Cry::DefaultComponents::CInputComponent* pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
    pInput->RegisterAction("battle_formation", "insert", [this](int activationMode, float value)
        {
            InsertCurve(GetFormation()->GetBattleLineLength() / 2);
        });
    pInput->BindAction("battle_formation", "insert", EActionInputDevice::eAID_KeyboardMouse, EKeyId::eKI_O, true, false, false);
}

Cry::Entity::EventFlags CBattleLineSpline::GetEventMask() const
{
    return 
        Cry::Entity::EEvent::Update
        | Cry::Entity::EEvent::Initialize;
}

void CBattleLineSpline::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::Initialize:
    {
        
    }
    case Cry::Entity::EEvent::Update:
        {
            static Vec3 points[20];
            Vec3 parentPos = GetEntity()->GetPos();
            for (auto start = GetStartCurve(); start != GetEndCurve(); ++start)
            {
                points[0] = CreateCurvePos(start, 0) + parentPos;
                uint frac = 1;
                for (int val = 1; val < 19; val += 2)
                {
                    Vec3 temp = CreateCurvePos(start, frac / 10.0f) + parentPos;
                    points[val] = temp;
                    points[val + 1] = temp;
                    ++frac;
                }
                points[19] = CreateCurvePos(start, 1) + parentPos;
                gEnv->pAuxGeomRenderer->DrawLines(points, 20, ColorB(0, 255, 0), 30.0f);
            }
            break;
        }
    }
}

void CBattleLineSpline::InsertCurve(float battleLineXPos)
{
    CurveItr curveItr = FindCurve(battleLineXPos);

    CRY_ASSERT(curveItr != GetEndCurve(), "Cannot insert a new curve at the end of a spline!");
    if (curveItr == GetEndCurve())
    {
        CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "Cannot insert a new curve at the end of a spline!");
        return;
    }

    IVertexPoint* pLeftVertex = curveItr.GetStartVertex();
    IVertexPoint* pRightVertex = curveItr.GetEndVertex();
    IAnchorPoint* pOutAnchor = curveItr.GetOutAnchor();
    IAnchorPoint* pInAnchor = curveItr.GetInAnchor();
    ICurveProperties* pCurveLine = curveItr.GetCurveLine();
    float t = (battleLineXPos - pLeftVertex->GetBattleLineXPos()) / (pRightVertex->GetBattleLineXPos() - pLeftVertex->GetBattleLineXPos());
    SVertexSpawnParams vertexParams;
    vertexParams.pSpline = this;
    vertexParams.gridPosition = CreateCurvePos(curveItr, t);
    CVertexPoint* pNewVertex = CVertexPoint::CreateVertex(vertexParams);

    float curveMagnitude = (pRightVertex->GetPos() - pLeftVertex->GetPos()).GetLength2D();
    Vec2 outAnchorOffset = pOutAnchor->GetPos() - pLeftVertex->GetPos();
    Vec2 inAnchorOffset = pInAnchor->GetPos() - pRightVertex->GetPos();
    float outAnchorMagnitude = outAnchorOffset.GetLength();
    float inAnchorMagnitude = inAnchorOffset.GetLength();
    Vec2 newOutAnchorPos = outAnchorOffset * (outAnchorMagnitude / curveMagnitude) + pLeftVertex->GetPos();
    Vec2 newInAnchorPos = inAnchorOffset * (inAnchorMagnitude / curveMagnitude) + pRightVertex->GetPos();
    pOutAnchor->SetPos(newOutAnchorPos);
    pInAnchor->SetPos(newInAnchorPos);

    Vec2 vertexTangent = CreateCurveTangent(curveItr, t).Normalize();
    SAnchorSpawnParams anchorParams;
    anchorParams.pSpline = this;
    float newOutAnchorMagnitude = GetDefaultNormalizedAnchorLength() * (pNewVertex->GetPos() - pRightVertex->GetPos()).GetLength2D();
    anchorParams.gridPosition = newOutAnchorMagnitude * vertexTangent + pNewVertex->GetPos();
    CAnchorPoint* pNewOutAnchor = CAnchorPoint::CreateAnchor(anchorParams);
    float newInAnchorMagnitude = GetDefaultNormalizedAnchorLength() * (pNewVertex->GetPos() - pLeftVertex->GetPos()).GetLength2D();
    anchorParams.gridPosition = newInAnchorMagnitude * -vertexTangent + pNewVertex->GetPos();
    CAnchorPoint* pNewInAnchor = CAnchorPoint::CreateAnchor(anchorParams);

    CCurveLine* pNewCurveLine = new CCurveLine(pNewOutAnchor, (*curveItr.GetCurveLineItr())->GetInAnchor(), pCurveLine->GetCurveMode());
    (*curveItr.GetCurveLineItr())->SetInAnchor(pNewInAnchor);

    m_vertexPoints.insert(curveItr.GetVertexItr() + 1, pNewVertex);
    m_curveSegments.insert(curveItr.GetCurveLineItr() + 1, pNewCurveLine);
}

void CBattleLineSpline::RemoveCurve(const IVertexPoint* vertex, const ECurveDirection& dir)
{
}

void CBattleLineSpline::Reset()
{
    auto startVertItr = m_vertexPoints.begin() + 1;
    auto endVertItr = m_vertexPoints.end() - 1;
    for (auto vertexItr = startVertItr; vertexItr != endVertItr; ++vertexItr)
    {
        gEnv->pEntitySystem->RemoveEntity((*vertexItr)->GetEntityId());
    }
    m_vertexPoints.erase(startVertItr, endVertItr);

    auto startCurveLineItr = m_curveSegments.begin() + 1;
    auto endCurveLineItr = m_curveSegments.end();
    for (auto curveLineItr = startCurveLineItr; startCurveLineItr != endCurveLineItr; ++curveLineItr)
    {
        gEnv->pEntitySystem->RemoveEntity((*curveLineItr)->GetOutAnchor()->GetEntityId());
        gEnv->pEntitySystem->RemoveEntity((*curveLineItr)->GetInAnchor()->GetEntityId());
        delete *curveLineItr;
    }
    m_curveSegments.erase(startCurveLineItr, endCurveLineItr);

    CCurveLine* pCurveLine = *m_curveSegments.begin();
    CVertexPoint* pStartVertex = *m_vertexPoints.begin();
    CVertexPoint* pEndVertex = *m_vertexPoints.end() - 1;
    Vec2 anchorGridDisplacement = (pEndVertex->GetPos() - pStartVertex->GetPos()) * s_normalizedAnchorLength;
    pCurveLine->GetOutAnchor()->SetPos(pStartVertex->GetGridPos() + anchorGridDisplacement);
    pCurveLine->GetInAnchor()->SetPos(pEndVertex->GetGridPos() + -anchorGridDisplacement);
    pCurveLine->ApplyCurveMode(ECurveMode::Linear);
}

Vec3 CBattleLineSpline::CreateCurvePos(const CurveItr& curveItr, float t)
{
    CRY_ASSERT(curveItr.GetStartVertex() != nullptr, "A starting vertex is required to calculated a curve position");
    CRY_ASSERT(curveItr.GetOutAnchor() != nullptr, "A outgoing anchor is required to calculated a curve position");
    CRY_ASSERT(curveItr.GetInAnchor() != nullptr, "A incoming anchor is required to calculated a curve position");
    CRY_ASSERT(curveItr.GetEndVertex() != nullptr, "A ending vertex is required to calculated a curve position");
    t = crymath::clamp(t, 0.0f, 1.0f);

    Vec3 startVertexPos = curveItr.GetStartVertex()->GetPos();
    Vec3 outAnchorPos = curveItr.GetOutAnchor()->GetPos();
    Vec3 inAnchorPos = curveItr.GetInAnchor()->GetPos();
    Vec3 endVertexPos = curveItr.GetEndVertex()->GetPos();
    return Vec3::CreateCubicCurve(startVertexPos, outAnchorPos, inAnchorPos, endVertexPos, t);
}

Vec3 CBattleLineSpline::CreateCurveTangent(const CurveItr& curveItr, float t)
{
    CRY_ASSERT(curveItr.GetStartVertex() != nullptr, "A starting vertex is required to calculated a curve tangent");
    CRY_ASSERT(curveItr.GetOutAnchor() != nullptr, "A outgoing anchor is required to calculated a curve tangent");
    CRY_ASSERT(curveItr.GetInAnchor() != nullptr, "A incoming anchor is required to calculated a curve tangent");
    CRY_ASSERT(curveItr.GetEndVertex() != nullptr, "A ending vertex is required to calculated a curve tangent");
    t = crymath::clamp(t, 0.0f, 1.0f);

    Vec3 startVertexPos = curveItr.GetStartVertex()->GetPos();
    Vec3 outAnchorPos = curveItr.GetOutAnchor()->GetPos();
    Vec3 inAnchorPos = curveItr.GetInAnchor()->GetPos();
    Vec3 endVertexPos = curveItr.GetEndVertex()->GetPos();
    float t0 = 1.0f - t;
    return 3.0f * t0 * t0 * (outAnchorPos - startVertexPos) + 6.0f * t0 * t * (inAnchorPos - outAnchorPos) + 3.0f * t * t * (endVertexPos - inAnchorPos);
}

CBattleLineSpline::CurveItr CBattleLineSpline::FindCurve(const IVertexPoint* vertex) const
{
    CurveItr curveItr = GetStartCurve();
    while (curveItr != GetEndCurve())
    {
        if (curveItr.GetStartVertex() == vertex)
        {
            return curveItr;
        }
        ++curveItr;
    }
    return curveItr;
}

CBattleLineSpline::CurveItr CBattleLineSpline::FindCurve(const IAnchorPoint* pAnchor) const
{
    CurveItr curveItr = GetStartCurve();
    while (curveItr != GetEndCurve())
    {
        if (curveItr.GetOutAnchor() == pAnchor || curveItr.GetInAnchor() == pAnchor)
        {
            return curveItr;
        }
        ++curveItr;
    }
    return curveItr;
}

CBattleLineSpline::CurveItr CBattleLineSpline::FindCurve(const ICurveProperties* curveLine) const
{
    CurveItr curveItr = GetStartCurve();
    while (curveItr != GetEndCurve())
    {
        if (curveItr.GetCurveLine() == curveLine)
        {
            return curveItr;
        }
        ++curveItr;
    }
    return curveItr;
}

CBattleLineSpline::CurveItr CBattleLineSpline::FindCurve(float battleLineXPos) const
{
    CurveItr curveItr = GetStartCurve();
    for (curveItr; curveItr != GetEndCurve(); ++curveItr)
    {
        if (curveItr.GetEndVertex()->GetBattleLineXPos() > battleLineXPos)
        {
            return curveItr;
        }
    }
    return curveItr;
}

Vec3 CBattleLineSpline::GetOutAnchorOffset(const CurveItr& curveItr) const
{
    return curveItr.GetOutAnchor()->GetPos() - curveItr.GetStartVertex()->GetPos();
}

Vec3 CBattleLineSpline::GetInAnchorOffset(const CurveItr& curveItr) const
{
    return curveItr.GetInAnchor()->GetPos() - curveItr.GetEndVertex()->GetPos();
}

void CBattleLineSpline::InitializeSpline()
{
    SVertexSpawnParams vertexParams;
    vertexParams.pSpline = this;
    vertexParams.gridPosition = Vec2(0, 0);
    CVertexPoint* pStartVertex = CVertexPoint::CreateVertex(vertexParams);

    vertexParams.gridPosition = Vec2(m_pFormation->GetBattleLineLength(), 0);
    CVertexPoint* pEndVertex = CVertexPoint::CreateVertex(vertexParams);

    m_vertexPoints.push_back(pStartVertex);
    m_vertexPoints.push_back(pEndVertex);

    SAnchorSpawnParams anchorParams;
    anchorParams.pSpline = this;
    Vec2 curveDifference = pEndVertex->GetPos() - pStartVertex->GetPos();
    Vec2 anchorDifference = curveDifference * GetDefaultNormalizedAnchorLength();

    anchorParams.gridPosition = pStartVertex->GetPos() + anchorDifference;
    CAnchorPoint* pOutAnchor = CAnchorPoint::CreateAnchor(anchorParams);
    anchorParams.gridPosition = pEndVertex->GetPos() + -anchorDifference;
    CAnchorPoint* pInAnchor = CAnchorPoint::CreateAnchor(anchorParams);

    CCurveLine* pCurveLine = new CCurveLine(pOutAnchor, pInAnchor);
    m_curveSegments.push_back(pCurveLine);
}

void CBattleLineSpline::RestrictVertexAngle(const CurveItr& curveItr)
{
    /*Vec3 leftVertexPos = (curveItr - 1).GetStartVertex()->GetPos();
    Vec3 rightVertexPos = curveItr.GetEndVertex()->GetPos();

    Vec3 baseCurveLine = rightVertexPos - leftVertexPos;
    Vec3 leftCurveLine = curveItr.GetStartVertex()->GetPos() - leftVertexPos;
    Vec3 rightCurveLine = curveItr.GetStartVertex()->GetPos() - rightVertexPos;

    Quat leftRotation = Quat::CreateRotationV0V1(baseCurveLine, leftCurveLine);
    Quat rightRotation = Quat::CreateRotationV0V1(-baseCurveLine, rightCurveLine);

    Ang3 maxLeftAngle = Ang3(leftRotation).Normalize() * s_maxVertexAngleOffset;
    Ang3 maxRightAngle = Ang3(rightRotation).Normalize() * s_maxVertexAngleOffset;
    Quat maxLeftRotation = Quat::CreateRotationXYZ(maxLeftAngle);
    Quat maxRightRotation = Quat::CreateRotationXYZ(maxRightAngle);*/
}
