#pragma once

struct IFormationUnit
{
	using TUnitClassType = typename int;
	using TArmorClassType = typename int;

	virtual Vec3 Position() const = 0;
	virtual Vec3 Dimensions() const = 0;
	virtual Vec3 HalfDimensions() const = 0;
	virtual TUnitClassType UnitClassType() const = 0;
	virtual TArmorClassType ArmorClassType() const = 0;
};