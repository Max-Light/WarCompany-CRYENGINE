#pragma once

#include "ITroopUnitClass.h"

class CTroopUnitClass : public ITroopUnitClass
{
public:
	CTroopUnitClass(const STroopUnitClassParams& troopUnitClassParams);
	virtual ~CTroopUnitClass() = default;

	// ITroopUnitClass
	inline virtual TroopId GetTroopId() const override { return m_troopId; }
	inline virtual Vec3 GetSize() const override { return GetAABB().GetSize(); }
	inline virtual AABB GetAABB() const override { return AABB(Vec3(-0.25, -0.25, 0), Vec3(0.25f, 0.25f, 2)); }
	inline virtual STroopNavigationProperties GetNavProperties() const { return m_navProperties; }
	// ~ITroopUnitClass
private:
	TroopId m_troopId;
	ETroopType m_troopType;
	STroopNavigationProperties m_navProperties;
};