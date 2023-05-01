#pragma once

#include "BattleFormation/IFormationSlot.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/CoreAPI.h>


class CFormationSlot : public IEntityComponent, public IFormationSlot
{
public:
	CFormationSlot() = default;
	virtual ~CFormationSlot() override = default;

	static void ReflectType(Schematyc::CTypeDesc<CFormationSlot>& desc)
	{
		desc.SetGUID("{DABF095A-BB2F-4692-87DC-4B8E8B4CE637}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Formation Slot");
		desc.SetComponentFlags(EntityComponentFlags(
			{ 
				EEntityComponentFlags::HiddenFromUser, 
				EEntityComponentFlags::HideFromInspector 
			}
		));
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IFormationSlot
	virtual IFormationUnit* GetUnit() const override { return m_pUnit; }
	virtual Vec3 GetSize() const override { return m_boundingBox.GetSize(); }
	virtual Vec3 GetPos() const override { return m_pEntity->GetPos(); }
	virtual bool IsFormationReady() const override;
	// ~IFormationSlot

	// Set the world position of the slot
	void SetPos(const Vec2& gridPos);

	// Offset the position of the slot
	void OffsetPos(const Vec2& movement);

	// Update the position of the slot
	void UpdatePos();

	// Set the size of the slot
	void SetSize(const Vec3& size);

	// Assign the unit occupying this slot
	void AssignUnit(IFormationUnit* pUnit) { m_pUnit = pUnit; }
protected:
	IFormationUnit* m_pUnit = nullptr;
	AABB m_boundingBox;
};

