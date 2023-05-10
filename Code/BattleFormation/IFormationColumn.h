#pragma once

struct IFormationColumn
{
	// Returns the column's x position local to the formation
	virtual float GetXPos() const = 0;

	// Returns the leftmost x position of the column local to the formation
	float GetMinXPos() const { return GetXPos() - GetWidth() / 2; }

	// Returns the rightmost x position of the column local to the formation
	float GetMaxXPos() const { return GetXPos() + GetWidth() / 2; }

	// Returns the dedicated width of the column
	virtual float GetWidth() const = 0;

	// Returns the column's y position head local to the formation
	virtual float GetYPos() const = 0;
};