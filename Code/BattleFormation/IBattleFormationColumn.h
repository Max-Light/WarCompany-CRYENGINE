#pragma once

struct IBattleFormationColumn
{
	virtual float GetXPos() const = 0;
	virtual float GetWidth() const = 0;
};