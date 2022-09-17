#pragma once

#include <CryEntitySystem/IEntityComponent.h>


class CVertexPoint;

class CBattleLineSlot : public IEntityComponent
{
public:
	CBattleLineSlot() = default;
	virtual ~CBattleLineSlot() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleLineSlot>& desc)
	{
		desc.SetGUID("{4E4EFEC0-465E-4D91-BD37-8CBEEE5713D1}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Line Slot");
		desc.SetComponentFlags(
			{
			EEntityComponentFlags::Schematyc,
			EEntityComponentFlags::NoCreationOffset
			}
		);
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	CBattleLineSlot* PrevSlot() const { return m_pPrevSlot; }
	CBattleLineSlot* NextSlot() const { return m_pNextSlot; }
	CVertexPoint* Vertex() const { return m_pVertex; }
	Vec3 Position() const { return m_pEntity->GetWorldPos(); }
	Quat Rotation() const { return m_pEntity->GetWorldRotation(); }
	float Radius() const { return m_radius; }

	CBattleLineSlot** PrevSlotPointer() { return &m_pPrevSlot; }
	CBattleLineSlot** NextSlotPointer() { return &m_pNextSlot; }
	CVertexPoint** VertexPointer() { return &m_pVertex; }

	void LookDirection(const Vec3& position);
	void SetPosition(const Vec3& position) { m_pEntity->GetWorldTM().TransformPoint(position); }
	void SetRotation(const Quat& rotation) { m_pEntity->SetRotation(rotation); }
	void SetRadius(float radius);
private:
	CBattleLineSlot* m_pPrevSlot = nullptr;
	CBattleLineSlot* m_pNextSlot = nullptr;
	CVertexPoint* m_pVertex = nullptr;
	float m_radius = 1;
};