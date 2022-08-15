#pragma once

#include "BattleFormation.h"
#include "StdAfx.h"

class CBattleFormationSlot
{
public:
	friend class CBattleFormation;
public:
	Vec3 GetPosition() const { return m_position; }
	Quat GetRotation() const { return m_rotation; }
protected:
	Vec3 m_position = ZERO;
	Quat m_rotation = IDENTITY;
};