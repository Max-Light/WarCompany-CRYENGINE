#pragma once

#include "StdAfx.h"

struct IControlPoint
{
	virtual ~IControlPoint() = default;
	virtual Vec3 GetPosition() const = 0;
	virtual void SetPosition(Vec3 position) = 0;
};
