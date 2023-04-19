
#include "StdAfx.h"
#include "BattleLineSpline.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>
#include <CryEntitySystem/IEntitySystem.h>

#include <CryRenderer/IRenderAuxGeom.h>


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
    {
        /*gEnv->pRenderer->GetIRenderAuxGeom()->SetRenderFlags(SAuxGeomRenderFlags());
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawSphere(m_pEntity->GetWorldPos(), 0.5f, ColorB(0, 0, 255), false);*/
    }
    break;
    }
}

Vec3 CBattleLineSpline::GetOutAnchorOffset(const CCurveIterator& curveItr) const
{
    return (*curveItr.GetCurveSegmentIterator())->outAnchor.GetPosition() - (*curveItr.GetStartVertexIterator())->GetPosition();
}

Vec3 CBattleLineSpline::GetInAnchorOffset(const CCurveIterator& curveItr) const
{
    return (*curveItr.GetCurveSegmentIterator())->inAnchor.GetPosition() - (*curveItr.GetEndVertexIterator())->GetPosition();
}

Vec3 CBattleLineSpline::CalculateCurvePosition(const CCurveIterator& curveItr, float normalizedValue) const
{
    Vec3 startVertexPos = (*curveItr.GetStartVertexIterator())->GetPosition();
    Vec3 outAnchorPos = (*curveItr.GetCurveSegmentIterator())->outAnchor.GetPosition();
    Vec3 inAnchorPos = (*curveItr.GetCurveSegmentIterator())->inAnchor.GetPosition();
    Vec3 endVertexPos = (*curveItr.GetEndVertexIterator())->GetPosition();
    return Vec3::CreateCubicCurve(startVertexPos, outAnchorPos, inAnchorPos, endVertexPos, normalizedValue);
}

float CBattleLineSpline::GetColumnLength(const CVertexPoint::SFormationRef& start, const CVertexPoint::SFormationRef& end) const
{
    float curveLength = 0;

    /*curveLength += (1 - start.normalizedValue) * m_pFormation->GetColumnWidth(start.columnIndex);
    curveLength += end.normalizedValue * m_pFormation->GetColumnWidth(end.columnIndex);

    for (uint col = start.columnIndex + 1; col < end.columnIndex; ++col)
    {
        curveLength += m_pFormation->GetColumnWidth(col);
    }*/
    return curveLength;
}

void CBattleLineSpline::InsertCurve(CVertexPoint::SFormationRef& formationRef)
{
    CCurveIterator curveItr = FindCurve(formationRef);
    float columnLength = GetColumnLength((*curveItr.GetStartVertexIterator())->GetFormationReference(), formationRef);
    float totalcolumnLength = GetColumnLength((*curveItr.GetStartVertexIterator())->GetFormationReference(), (*curveItr.GetEndVertexIterator())->GetFormationReference());

    SEntitySpawnParams spawnParams;
    spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
    spawnParams.sName = string().Format("Vertex%" PRISIZE_T, GetCurveCount() + 1);
    spawnParams.pParent = this->m_pEntity;
    spawnParams.vPosition = CalculateCurvePosition(curveItr, columnLength / totalcolumnLength);

    IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
    CVertexPoint* pVertex = pEntity->GetOrCreateComponent<CVertexPoint>();

    SCurveSegment* pCurve = new SCurveSegment();

    m_vertexPoints.insert(curveItr.GetEndVertexIterator(), pVertex);
    m_curveSegments.insert(curveItr.GetCurveSegmentIterator(), pCurve);
}

void CBattleLineSpline::RemoveCurve(const CCurveIterator& curveItr)
{
    CRY_ASSERT(GetCurveCount() > 1, "Cannot delete a vertex below the minimum number of spline vertex points allowed (2).");

    CCurveIterator leftCurve;
    CCurveIterator rightCurve;
    ECurveMode curveMode;
    if (curveItr == GetStartCurve())
    {
        leftCurve = GetStartCurve();
        rightCurve = leftCurve + 1;
        curveMode = (*rightCurve.GetCurveSegmentIterator())->curveMode;
        Vec3 outAnchorOffset = GetOutAnchorOffset(rightCurve);
        (*leftCurve.GetCurveSegmentIterator())->outAnchor.SetPosition(outAnchorOffset + (*leftCurve.GetStartVertexIterator())->GetPosition());
    }
    else 
    {
        if (curveItr == GetEndCurve())
        {
            leftCurve = GetEndCurve() - 2;
            rightCurve = leftCurve + 1;
            curveMode = (*leftCurve.GetCurveSegmentIterator())->curveMode;
        }
        else
        {
            leftCurve = GetEndCurve() - 2;
            rightCurve = leftCurve + 1;
            curveMode = std::max((*leftCurve.GetCurveSegmentIterator())->curveMode, (*rightCurve.GetCurveSegmentIterator())->curveMode);
        }
        Vec3 inAnchorOffset = GetInAnchorOffset(rightCurve);
        (*leftCurve.GetCurveSegmentIterator())->inAnchor.SetPosition(inAnchorOffset + (*rightCurve.GetStartVertexIterator())->GetPosition());
    }
    delete *rightCurve.GetCurveSegmentIterator();
    m_curveSegments.erase(rightCurve.GetCurveSegmentIterator());
    delete *rightCurve.GetStartVertexIterator();
    m_vertexPoints.erase(rightCurve.GetStartVertexIterator());
}

void CBattleLineSpline::ApplyCurveMode(ECurveMode mode, const CCurveIterator& curveItr)
{
}

CBattleLineSpline::CCurveIterator CBattleLineSpline::FindCurve(const CVertexPoint::SFormationRef& formationRef)
{
    for (CCurveIterator curveItr = GetStartCurve(); curveItr != GetEndCurve(); ++curveItr)
    {
        if ((*curveItr.GetEndVertexIterator())->GetFormationReference() < formationRef)
        {
            return curveItr;
        }
    }
    return GetEndCurve();
}
