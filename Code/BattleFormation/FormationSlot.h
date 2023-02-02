#pragma once

#include <StdAfx.cpp>
#include "BattleFormation/IFormationUnit.h"


class CFormationSlot
{
public:
	CFormationSlot() = default;
	virtual ~CFormationSlot() = default;

	IFormationUnit* FormationUnit() const { return m_pUnit; }
	Vec3 Position() const { return m_position; }
	AABB GetAABB() const;

	void SetPosition(const Vec3& position) { m_position = position; }
	void SetOrientation(const Matrix33& orientation);
	void Rotate(Quat& rotation);
	void LookDirection(Vec2& direction);
protected:
	IFormationUnit* m_pUnit = nullptr;
	Vec3 m_position;
	Vec2 m_lookDirection;
};