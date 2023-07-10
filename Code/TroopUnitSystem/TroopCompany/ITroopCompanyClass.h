#pragma once

#include "StdAfx.h"

struct ITroopCompanyClass
{
	ITroopCompanyClass() = default;
	virtual ~ITroopCompanyClass() = default;

	virtual uint GetTroopCount() const = 0;
};