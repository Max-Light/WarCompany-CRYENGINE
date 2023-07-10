#pragma once

#include "StdAfx.h"
#include "ITroopCompanyClass.h"

class CTroopCompanyClass : public ITroopCompanyClass
{
public:
	CTroopCompanyClass(uint troopCount)
		: m_companyTroopCount(troopCount)
	{}
	virtual ~CTroopCompanyClass() = default;

	// ITroopCompanyClass
	inline virtual uint GetTroopCount() const override { return m_companyTroopCount; }
	// ~ITroopCompanyClass
protected:
	uint m_companyTroopCount = 100;
};