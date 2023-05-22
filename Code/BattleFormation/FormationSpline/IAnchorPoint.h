#pragma once

#include "ISplinePoint.h"

struct IAnchorPoint : public ISplinePoint
{
	IAnchorPoint() = default;
	virtual ~IAnchorPoint() = default;
};