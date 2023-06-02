#pragma once

#include "TroopUnit.h"
#include "TroopCompanyClass.h"
#include "BattleFormation/IFormationUnit.h"

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/CoreAPI.h>

class CTroopCompany : public IEntityComponent, public IFormationUnit
{
public:
	using TTroop = CTroopUnit;
public:
	CTroopCompany()
	{
		STroopCompanyClassParams tcc;
		tcc.troopSize = Vec3(1, 1, 2);
		tcc.troopType = ETroopType::Infantry;
		m_pTroopCompanyClass = new CTroopCompanyClass(tcc);

		m_troopUnits = std::vector<TTroop>(100, TTroop());

		m_troopLineCount = 15;
	}
	virtual ~CTroopCompany() = default;

	static void ReflectType(Schematyc::CTypeDesc<CTroopCompany>& desc)
	{
		desc.SetGUID("{0C0BF3A3-479F-47AB-8B8C-71C85F8BFE43}"_cry_guid);
		desc.SetEditorCategory("Company Units");
		desc.SetLabel("Troop Company");
		desc.SetDescription("A company of troops");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IFormationUnit
	virtual EntityId GetId() const override { return GetEntityId(); }
	virtual Vec3 GetWorldPos() const override { return m_pEntity->GetWorldPos(); }
	virtual Vec3 GetSize() const override;
	virtual AABB GetAABB() const override;
	virtual void MoveInFormation(const Vec3& pos) override { m_pEntity->SetPos(pos); };
	// ~IFormationUnit

	// returns the troop within the company grid
	// returns nullptr if outside the grid position
	const TTroop* GetTroopInGrid(uint y, uint x) const;
	// returns the amount of troops in the company
	uint GetTroopCount() const { return m_troopUnits.size(); }
	// returns the troop capacity
	uint GetTroopCapacity() const { return m_troopUnits.capacity(); }
	// adds the troop to the company
	void AddTroop(TTroop& troop);
	// adds troops to the company
	void AddTroops(std::vector<TTroop>& troops);
	// resizes the troop line width
	void ResizeTroopLine(uint troopCount);
	// set the maximum capacity
	void SetTroopCapacity(uint capacity);
protected:
	CTroopCompanyClass* m_pTroopCompanyClass;
	std::vector<TTroop> m_troopUnits;
	uint m_troopLineCount;
};