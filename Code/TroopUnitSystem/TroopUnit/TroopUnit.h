#pragma once

#include "StdAfx.h"
#include "ITroopUnit.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Physics/CylinderPrimitiveComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

class CTroopUnit : 
	public IEntityComponent, 
	public ITroopUnit
{
public:
	CTroopUnit() = default;
	virtual ~CTroopUnit() = default;

#ifndef RELEASE
	
#endif

	static void ReflectType(Schematyc::CTypeDesc<CTroopUnit>& desc)
	{
		desc.SetGUID("{D34142D0-9859-4326-A6F1-05E590E9A316}"_cry_guid);
		desc.SetEditorCategory("Troop Unit System");
		desc.SetLabel("Troop Unit");

		desc.AddMember(&CTroopUnit::m_troopType, 'unit', "TroopUnit", "Troop Unit", nullptr, 0);
		desc.AddMember(&CTroopUnit::m_health, 'heal', "Health", "Health", "Health amount of troop unit", 100);
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// ITroopUnit
	inline virtual TroopId GetTroopId() const override { return m_pTroopUnitClass->GetTroopId(); }
	inline virtual Vec3 GetWorldPos() const override { return GetEntity()->GetWorldPos(); }
	inline Vec3 GetSize() const override { return Vec3(m_pCollider->m_radius * 2, m_pCollider->m_radius * 2, m_pCollider->m_height); }
	inline virtual uint GetHealth() const override { return m_health; }
	inline virtual void SetHealth(uint health) override { m_health = health; }
	inline virtual void SetPos(Vec3 position) override { GetEntity()->SetPos(position); }
	inline virtual void Heal(uint healthAmount) override { m_health = crymath::clamp(m_health + healthAmount, (uint)0, UINT_MAX - m_health); }
	inline virtual void Damage(uint damageAmount) override { m_health = crymath::clamp(m_health - damageAmount, (uint)0, m_health); }
	inline virtual void MoveToPosition(Vec3 position) override { m_pNavigationComponent->NavigateTo(position); }
	// ~ITroopUnit

	void SetTroopUnitType(ITroopUnitClass* pTroopUnitClass) { m_pTroopUnitClass = pTroopUnitClass; }
protected:
	ETroopUnit m_troopType;
	ITroopUnitClass* m_pTroopUnitClass = nullptr;
	uint m_health;
	Cry::DefaultComponents::CCylinderPrimitiveComponent* m_pCollider = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationMeshComponent = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;
};