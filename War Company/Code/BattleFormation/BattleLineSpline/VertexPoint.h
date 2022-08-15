#pragma once

#include "BattleLineSlot.h"
#include "IControlPoint.h"
#include "AnchorPoint.h"
#include "BattleLineSpline.h"


class CVertexPoint : public CBattleLineSlot, public IControlPoint
{
public:
	friend class CBattleLineSpline;
public:
	CVertexPoint(const CBattleLineSpline& battleLine)
		: m_battleLine(&battleLine)
		, m_inAnchorPoint(*this)
		, m_outAnchorPoint(*this)
	{
	}
	~CVertexPoint() = default;
	CAnchorPoint& GetInAnchor() { return m_inAnchorPoint; }
	CAnchorPoint& GetOutAnchor() { return m_outAnchorPoint; }
	CVertexPoint* GetLeftVertex() const { return m_leftVertexPoint; };
	CVertexPoint* GetRightVertex() const { return m_rightVertexPoint; }

	//////////////////////////////////////////////////////////
	// IControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_position; }
	virtual void SetPosition(Vec3 position) override;
	//////////////////////////////////////////////////////////
protected:
	const CBattleLineSpline* const m_battleLine = nullptr;
	CVertexPoint* m_leftVertexPoint;
	CVertexPoint* m_rightVertexPoint;
	CAnchorPoint m_inAnchorPoint;
	CAnchorPoint m_outAnchorPoint;
};
