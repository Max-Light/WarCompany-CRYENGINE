#pragma once

#include "StdAfx.h"


struct ISplineControlPoint
{
	virtual ~ISplineControlPoint() = default;
	virtual Vec3 GetPosition() const = 0;
	virtual void SetPosition(const Vec3& position) = 0;
};