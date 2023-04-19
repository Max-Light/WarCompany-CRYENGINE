#pragma once

#include <CryEntitySystem/IEntityComponent.h>

struct IBattleLineSpline : public IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IBattleLineSpline>& desc)
	{
		desc.SetGUID("{ABAE3702-F302-4122-A470-639ABD50938B}"_cry_guid);
	}
};