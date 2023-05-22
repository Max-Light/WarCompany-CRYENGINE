#include "DebugBattleLineSpline.h"

namespace
{
    static void RegisterDebugBattleLineSplineComponent(Schematyc::IEnvRegistrar& registrar)
    {
        Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
        {
            Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDebugBattleLineSpline));
        }
    }
    CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterDebugBattleLineSplineComponent);
}

void CDebugBattleLineSpline::Initialize()
{
	/*m_pSpline = GetEntity()->GetOrCreateComponent<IBattleLineSpline>();

    if (!m_pCurveItr) 
    {
        m_pCurveItr = m_pSpline->GetCurveIterator();
    }*/

    /*if (!m_pCurveItr->IsEnd())
    {
        while (!m_pCurveItr->IsEnd())
        {
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetStartVertex()->GetPos());
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetOutAnchor()->GetPos());
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetInAnchor()->GetPos());
            m_pCurveItr->Next();
        }
        m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetStartVertex()->GetPos());
    }
    m_pCurveItr->MoveToFirst();*/
}

Cry::Entity::EventFlags CDebugBattleLineSpline::GetEventMask() const
{
    return
        Cry::Entity::EEvent::EditorPropertyChanged
        | Cry::Entity::EEvent::Update;
}

void CDebugBattleLineSpline::ProcessEvent(const SEntityEvent& event)
{
    switch (event.event)
    {
    case Cry::Entity::EEvent::EditorPropertyChanged:
        UpdateSpline();
        break;
    case Cry::Entity::EEvent::Update:
        break;
    }
}

void CDebugBattleLineSpline::UpdateSpline()
{
    /*uint pointIndex = 0;
    while (!m_pCurveItr->IsEnd())
    {
        m_pCurveItr->GetCurve().GetStartVertex()->SetPos(m_splinePoints.At(pointIndex));
        m_pCurveItr->GetCurve().GetOutAnchor()->SetPos(m_splinePoints.At(pointIndex + 1));
        m_pCurveItr->GetCurve().GetInAnchor()->SetPos(m_splinePoints.At(pointIndex + 2));
        pointIndex += 3;
        m_pCurveItr->Next();
    }
    m_pCurveItr->GetCurve().GetEndVertex()->SetPos(m_splinePoints.At(pointIndex));

    if (pointIndex != m_splinePoints.Size() - 1)
    {
        while (pointIndex < m_splinePoints.Size())
        {
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetStartVertex()->GetPos());
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetOutAnchor()->GetPos());
            m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetInAnchor()->GetPos());
            m_pCurveItr->Next();
        }
        m_splinePoints.PushBack(m_pCurveItr->GetCurve().GetStartVertex()->GetPos());
    }
    m_pCurveItr->MoveToFirst();*/
}
