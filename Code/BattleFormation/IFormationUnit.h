#pragma once

struct IFormationUnit
{
	using TUnitClassType = typename int;
	using TArmorClassType = typename int;

	virtual Vec3 GetWorldPos() const = 0;
	virtual Vec3 GetSize() const = 0;
	virtual AABB GetAABB() const = 0;
	virtual void MoveInFormation(const Vec3& gridPos) = 0;
};