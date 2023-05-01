#pragma once

struct IFormationColumn
{
	// Returns the x position within the formation
	virtual float GetXPos() const = 0;

	// Returns the leftmost x position of the column
	float GetMinXPos() const { return GetXPos() - GetWidth() / 2; }

	// Returns the rightmost x position of the column
	float GetMaxXPos() const { return GetXPos() + GetWidth() / 2; }

	// Returns the dedicated width of the column
	virtual float GetWidth() const = 0;
};