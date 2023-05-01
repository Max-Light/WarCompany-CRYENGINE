#pragma once

#include "StdAfx.h"

struct ICurveSegment
{
	enum class ECurveMode : uint8
	{
		Linear = 0,
		Quadratic = 1,
		Cubic = 2
	};

	virtual ECurveMode GetCurveMode() = 0;
};