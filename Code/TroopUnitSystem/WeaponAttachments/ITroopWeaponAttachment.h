#pragma once

#include "StdAfx.h"

struct ITroopWeaponAttachment
{
	virtual uint GetMinDamage() = 0;
	virtual uint GetMaxDamage() = 0;
	virtual string GetName() = 0;
};