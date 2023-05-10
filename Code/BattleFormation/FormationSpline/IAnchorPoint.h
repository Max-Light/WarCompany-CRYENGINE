#pragma once


struct IAnchorPoint
{
	IAnchorPoint() = default;
	virtual ~IAnchorPoint() = default;

	// Return the position of the anchor point
	virtual Vec3 GetPos() const = 0;
};