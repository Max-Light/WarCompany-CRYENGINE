#pragma once

#include "ITroopCompany.h"
#include "ITroopCompanyClass.h"
#include "TroopUnitSystem/TroopUnit/ITroopUnit.h"
#include "TroopUnitSystem/TroopUnit/TroopUnitClass.h"
#include "BattleFormation/IFormationUnit.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/CoreAPI.h>


struct STroopCompanyParams
{
	STroopCompanyParams() = default;
	virtual ~STroopCompanyParams() = default;

	ITroopUnitClass* pTroopUnitClass = nullptr;
	ITroopCompanyClass* pTroopCompanyClass = nullptr;
	uint lineLength;
};

class CTroopCompany : 
	public IEntityComponent, 
	public ITroopCompany
{
public:
	CTroopCompany() = default;
	virtual ~CTroopCompany() = default;

	static void ReflectType(Schematyc::CTypeDesc<CTroopCompany>& desc)
	{
		desc.SetGUID("{0C0BF3A3-479F-47AB-8B8C-71C85F8BFE43}"_cry_guid);
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

	// ITroopCompany
	inline virtual ITroopCompanyClass* GetTroopCompanyClass() const override { return m_pTroopCompanyClass; }
	inline virtual ITroopUnitClass* GetTroopUnitClass() const override { return m_pTroopUnitClass; }
	inline virtual Vec3 GetWorldPos() const override { return GetEntity()->GetWorldPos(); }
	inline virtual uint GetTroopCount() const override { return m_troopUnits.size(); }
	inline virtual uint GetLineLength() const override { return m_lineLength; }
	virtual ITroopUnit* GetTroopInGrid(Vec2i gridCoord) const override;
	inline virtual Vec3 GetSize() const override;
	virtual void AddTroop(ITroopUnit* pTroopUnit) override;
	virtual void RemoveTroop(ITroopUnit* pTroopUnit) override;
	// ~ITroopCompany

	// IFormationUnit
	//virtual Vec3 GetWorldPos() const override { return m_pEntity->GetWorldPos(); }
	//virtual Vec3 GetSize() const override;
	//virtual AABB GetAABB() const override;
	//virtual void MoveInFormation(const Vec3& pos) override { m_pEntity->SetPos(pos); };
	// ~IFormationUnit

	// Translate the unit index value to a company grid coordinate
	Vec2i TranslateToCompanyGridCoord(uint unitIndex) const;

	// Calulcate the position of the unit in the grid
	Vec3 CalculateUnitPosition(uint unitIndex) const;

	// Spawn new troop units to available spaces within the company grid
	void SpawnTroops();

	// Initialize the troop company
	void InitializeTroopCompany(const STroopCompanyParams& troopCompanyParams);
protected:
	// Format all troops in the company grid
	void RepositionTroops();
protected:
	ITroopUnitClass* m_pTroopUnitClass = nullptr;
	ITroopCompanyClass* m_pTroopCompanyClass = nullptr;
	std::vector<ITroopUnit*> m_troopUnits;
	uint m_lineLength;
};