#pragma once

#include <StdAfx.cpp>

class CFormationBuildGroup
{
public:
	CFormationBuildGroup() = default;
	virtual ~CFormationBuildGroup() = default;

	RectF GetRect() const { return m_rectangle; }
	bool IntersectsWidth(float x) const;

	void SetPos(Vec2 gridPosition);
private:
	RectF m_rectangle;
};