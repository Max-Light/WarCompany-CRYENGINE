#pragma once

#include "StdAfx.h"

enum class ETroopType
{
	Infantry,
	Range,
	Cavalry,
	RangeCavalry
};

struct STroopCompanyClassParams
{
	Vec3 troopSize;
	ETroopType troopType;
};

class CTroopCompanyClass
{
public:
	CTroopCompanyClass(const STroopCompanyClassParams& params);
	virtual ~CTroopCompanyClass() = default;

	Vec3 TroopSize() const { return m_troopSize; }
protected:
	Vec3 m_troopSize;
	ETroopType m_troopType;
};