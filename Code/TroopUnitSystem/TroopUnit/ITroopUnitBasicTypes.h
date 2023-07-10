#pragma once

#include "StdAfx.h"

using TroopId = uint32;

enum class ETroopUnit : TroopId
{
	Placeholder = 0
};

static void ReflectType(Schematyc::CTypeDesc<ETroopUnit>& desc)
{
	desc.SetGUID("{95BCDDC2-CCF3-46BE-822C-D02D9ED1087C}"_cry_guid);
	desc.SetLabel("Troop Unit");
	desc.SetDefaultValue(ETroopUnit::Placeholder);
	desc.AddConstant(ETroopUnit::Placeholder, "Placeholder", "Placeholder");
}

enum class ETroopType
{
	Infantry,
	Range,
	Cavalry,
	RangeCavalry
};
