#pragma once

#include <StdAfx.cpp>

class CFormationBuildGroup
{
public:
	CFormationBuildGroup() = default;
	virtual ~CFormationBuildGroup() = default;

	RectF GetRect() const { return m_rectangle; }
	bool IntersectsWidth(float x) const;

	void SetPosition(Vec2 position);
private:
	RectF m_rectangle;
};