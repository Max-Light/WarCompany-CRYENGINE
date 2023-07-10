
#include "TroopUnitSystem.h"

#include "TroopUnit/TroopUnit.h"
#include "TroopUnit/TroopUnitClass.h"

#include <CryEntitySystem/IEntitySystem.h>

CTroopUnitSystem::CTroopUnitSystem()
{
	STroopNavigationProperties navProperties;

	STroopUnitClassParams params;
	params.troopId = (TroopId)ETroopUnit::Placeholder;
	params.troopType = ETroopType::Infantry;
	params.navProperties = navProperties;

	CTroopUnitClass* pClass = new CTroopUnitClass(params);
	RegisterTroopUnit(pClass);
}

ITroopUnit* CTroopUnitSystem::SpawnTroopUnit(const STroopUnitSpawnParams& troopUnitSpawnParams)
{
	ITroopUnitClass* pTroopUnitClass = GetTroopUnitClass(troopUnitSpawnParams.troopId);
	CRY_ASSERT(pTroopUnitClass, "Troop Unit Class with ID %i was not found", troopUnitSpawnParams.troopId);
	if (!pTroopUnitClass)
	{
		return nullptr;
	}

	SEntitySpawnParams troopUnitEntitySpawnParams;
	troopUnitEntitySpawnParams.vPosition = troopUnitSpawnParams.position;
	IEntity* pTroopUnitEntity = gEnv->pEntitySystem->SpawnEntity(troopUnitEntitySpawnParams);

	CTroopUnit* pTroopUnit = pTroopUnitEntity->CreateComponent<CTroopUnit>();
	pTroopUnit->SetTroopUnitType(pTroopUnitClass);

	return pTroopUnit;
}
