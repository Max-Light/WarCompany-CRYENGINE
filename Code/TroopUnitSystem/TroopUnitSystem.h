#pragma once

#include "ITroopUnitSystem.h"

#include <map>

class CTroopUnitSystem : public ITroopUnitSystem
{
public:
	CTroopUnitSystem();
	virtual ~CTroopUnitSystem() = default;

	// ITroopUnitSystem
	virtual ITroopUnit* SpawnTroopUnit(const STroopUnitSpawnParams& troopUnitSpawnParams) override;
	inline virtual ITroopUnitClass* GetTroopUnitClass(TroopId troopUnitId) const 
	{
		auto troopUnitClassItr = m_troopUnitClasses.find(troopUnitId);
		if (troopUnitClassItr == m_troopUnitClasses.end())
		{
			return nullptr;
		}
		return troopUnitClassItr->second;
	}
	// ~ITroopUnitSystem

	inline void RegisterTroopUnit(ITroopUnitClass* pTrooUnitClass) 
	{
		m_troopUnitClasses.emplace(std::make_pair(pTrooUnitClass->GetTroopId(), pTrooUnitClass));
	}
protected:
	std::map<TroopId, ITroopUnitClass*> m_troopUnitClasses;
};