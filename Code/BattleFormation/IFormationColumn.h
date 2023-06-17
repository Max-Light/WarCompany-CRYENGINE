#pragma once

struct IColumnVerticalOffsetEffect
{
	IColumnVerticalOffsetEffect() = default;
	virtual ~IColumnVerticalOffsetEffect() = default;

	// Return the vertical offset interpolated within column width
	virtual float GetVerticalOffset(float t) const = 0;
};

struct IFormationColumn
{
	IFormationColumn() = default;
	virtual ~IFormationColumn() = default;

	// Returns the column's x position local to the formation
	virtual float GetXPos() const = 0;

	// Returns the leftmost x position of the column local to the formation
	virtual float GetMinXPos() const { return GetXPos() - GetWidth() / 2; }

	// Returns the rightmost x position of the column local to the formation
	virtual float GetMaxXPos() const { return GetXPos() + GetWidth() / 2; }

	// Returns the dedicated width of the column
	virtual float GetWidth() const = 0;

	// Returns the column's y position head local to the formation
	virtual float GetYPos() const = 0;

	// Set a column vertical offset to the column
	virtual void SetColumnVerticalOffset(const IColumnVerticalOffsetEffect& verticalEffect) = 0;
};