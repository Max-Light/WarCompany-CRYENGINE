#pragma once

#include "BattleFormationSlot.h"

class CBattleFormation final : public IEntityComponent
{
public:
	friend class CBattleFormationSlot;
protected:
	CBattleLineSpline m_battleLineSpline;
};