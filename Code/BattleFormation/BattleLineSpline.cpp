
#include "StdAfx.h"
#include "BattleLineSpline.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CryCore/StaticInstanceList.h>

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryPhysics/IPhysics.h>
#include <DefaultComponents/Physics/BoxPrimitiveComponent.h>


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
    TSlot* start = SpawnSlot();
    TSlot* end = SpawnSlot();

    m_pEntity->AttachChild(start->GetEntity());
    m_pEntity->AttachChild(end->GetEntity());

    start->GetEntity()->SetPos(Vec3(0, 0, 0));
    end->GetEntity()->SetPos(Vec3(start->Radius() + end->Radius(), 0, 0));

    m_pStartVertex = new CVertexPoint(start);
    m_pEndVertex = new CVertexPoint(end);
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
        gEnv->pRenderer->GetIRenderAuxGeom()->SetRenderFlags(SAuxGeomRenderFlags());
        gEnv->pRenderer->GetIRenderAuxGeom()->DrawSphere(m_pEntity->GetWorldPos(), 0.5f, ColorB(0, 0, 255), false);
    }
    break;
    }
}

CBattleLineSpline::TSlot* CBattleLineSpline::SpawnSlot()
{
    SEntitySpawnParams slotSpawnParams;
    slotSpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
    slotSpawnParams.sName = "Slot";

    IEntity* pSlotEntity = gEnv->pEntitySystem->SpawnEntity(slotSpawnParams);
    if (slotSpawnParams.spawnResult == EEntitySpawnResult::Success)
    {
        TSlot* pSlotComponent = pSlotEntity->GetOrCreateComponent<TSlot>();
        return pSlotComponent;
    }
    return nullptr;
}

CBattleLineSpline::TIterator CBattleLineSpline::GetVertexAt(int index) const
{
    if (index < m_curveCount / 2)
    {
        TIterator iterator = TIterator(m_pStartVertex->BattleLineSlot());
        for (int i = 0; i < index; i++)
        {
            iterator++;
        }
        return iterator;
    }
    else
    {
        TIterator iterator = TIterator(m_pEndVertex->BattleLineSlot());
        for (int i = 0; i < m_curveCount - index; i++)
        {
            iterator--;
        }
        return iterator;
    }
}

CVertexPoint* CBattleLineSpline::SetVertex(const TIterator& iterator)
{
    CVertexPoint* vertex = new CVertexPoint(iterator.Slot());
    *(iterator.Slot()->Vertex()->NextVertex()->PrevVertexPointer()) = vertex;
    *(vertex->NextVertexPointer()) = iterator.Slot()->Vertex()->NextVertex();
    *(iterator.Slot()->Vertex()->NextVertexPointer()) = vertex;
    *(vertex->PrevVertexPointer()) = iterator.Slot()->Vertex();

    TSlot* slot = vertex->BattleLineSlot();
    while (slot != vertex->NextVertex()->BattleLineSlot())
    {
        *(slot->VertexPointer()) = vertex;
        slot = slot->NextSlot();
    }

    m_curveCount++;
    return vertex;
}

void CBattleLineSpline::RemoveVertex(const TIterator& iterator)
{
    CVertexPoint* vertex = iterator.Slot()->Vertex();
    CRY_ASSERT(vertex != m_pStartVertex && vertex != m_pEndVertex);

    TSlot* slot = vertex->BattleLineSlot();
    while (slot != vertex->NextVertex()->BattleLineSlot())
    {
        *(slot->VertexPointer()) = vertex->PrevVertex();
        slot = slot->NextSlot();
    }

    *(vertex->NextVertex()->PrevVertexPointer()) = vertex->PrevVertex();
    *(vertex->PrevVertex()->NextVertexPointer()) = vertex->NextVertex();

    m_curveCount--;
    delete vertex;
}

void CBattleLineSpline::SetSlotAmount(int slotAmount)
{
    if (slotAmount < 2)
    {
        CryWarning(EValidatorModule::VALIDATOR_MODULE_ENTITYSYSTEM, EValidatorSeverity::VALIDATOR_ERROR, "");
        return;
    }

    while (slotAmount > 0)
    {
        TSlot* slot = SpawnSlot();
        PushSlotBack(slot);
        slotAmount--;
    }
}

void CBattleLineSpline::Clear()
{

}

void CBattleLineSpline::PushSlotFront(TSlot* slot)
{
    m_pEntity->AttachChild(slot->GetEntity());

    *(m_pStartVertex->BattleLineSlot()->PrevSlotPointer()) = slot;
    *(slot->NextSlotPointer()) = m_pStartVertex->BattleLineSlot();
    *(slot->VertexPointer()) = m_pStartVertex;
    m_pStartVertex->SetBattleLineNode(slot);

    m_slotCount++;
}

void CBattleLineSpline::PushSlotBack(TSlot* slot)
{
    m_pEntity->AttachChild(slot->GetEntity());

    *(m_pEndVertex->BattleLineSlot()->NextSlotPointer()) = slot;
    *(slot->PrevSlotPointer()) = m_pEndVertex->BattleLineSlot();
    *(m_pEndVertex->BattleLineSlot()->VertexPointer()) = m_pEndVertex->PrevVertex();
    *(slot->VertexPointer()) = m_pEndVertex;
    m_pEndVertex->SetBattleLineNode(slot);

    m_slotCount++;
}

void CBattleLineSpline::InsertSlot(const TIterator& iterator, TSlot* slot)
{
    if (iterator.Slot() == m_pStartVertex->BattleLineSlot())
    {
        PushSlotFront(slot);
    }
    else if (iterator.Slot() == m_pEndVertex->BattleLineSlot())
    {
        PushSlotBack(slot);
    }
    else
    {
        m_pEntity->AttachChild(slot->GetEntity());

        *(iterator.Slot()->PrevSlot()->NextSlotPointer()) = slot;
        *(iterator.Slot()->NextSlot()->PrevSlotPointer()) = slot;
        *(slot->PrevSlotPointer()) = iterator.Slot()->PrevSlot();
        *(slot->NextSlotPointer()) = iterator.Slot()->NextSlot();
        *(slot->VertexPointer()) = iterator.Slot()->Vertex();

        m_slotCount++;
    }
}

CBattleLineSpline::TSlot* CBattleLineSpline::PopSlotFront()
{
    TSlot* slot = m_pStartVertex->BattleLineSlot();

    slot->GetEntity()->DetachThis();

    m_pStartVertex->SetBattleLineNode(slot->NextSlot());
    *(slot->NextSlotPointer()) = nullptr;
    *(m_pStartVertex->BattleLineSlot()->PrevSlotPointer()) = nullptr;
    *(slot->VertexPointer()) = nullptr;

    m_slotCount--;
    return slot;
}

CBattleLineSpline::TSlot* CBattleLineSpline::PopSlotBack()
{
    TSlot* slot = m_pEndVertex->BattleLineSlot();

    slot->GetEntity()->DetachThis();

    m_pEndVertex->SetBattleLineNode(slot->PrevSlot());
    *(slot->PrevSlotPointer()) = nullptr;
    *(m_pEndVertex->BattleLineSlot()->NextSlotPointer()) = nullptr;
    *(slot->VertexPointer()) = nullptr;

    m_slotCount--;
    return slot;
}

CBattleLineSpline::TSlot* CBattleLineSpline::PopSlot(const TIterator& iterator)
{
    if (iterator.Slot() == m_pStartVertex->BattleLineSlot())
    {
        return PopSlotFront();
    }
    else if (iterator.Slot() == m_pEndVertex->BattleLineSlot())
    {
        return PopSlotBack();
    }
    else
    {
        TSlot* slot = iterator.Slot();

        slot->GetEntity()->DetachThis();

        if (slot == slot->Vertex()->BattleLineSlot())
        {
            RemoveVertex(iterator);
        }
        *(slot->NextSlot()->PrevSlotPointer()) = slot->PrevSlot();
        *(slot->PrevSlot()->NextSlotPointer()) = slot->NextSlot();
        *(slot->PrevSlotPointer()) = nullptr;
        *(slot->NextSlotPointer()) = nullptr;
        *(slot->VertexPointer()) = nullptr;

        m_slotCount--;
        return slot;
    }
}
