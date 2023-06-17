#pragma once

struct IFormationUnit
{
	using TUnitClassType = typename int;
	using TArmorClassType = typename int;

	virtual uint32 GetId() const = 0;
	virtual Vec3 GetWorldPos() const = 0;
	virtual Vec3 GetSize() const { return Vec3(5, 3, 2); }
	virtual AABB GetAABB() const = 0;
	virtual void MoveInFormation(const Vec3& gridPos) = 0;
};