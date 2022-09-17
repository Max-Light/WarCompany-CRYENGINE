#pragma once

#include "ISplineControlPoint.h"
#include "AnchorPoint.h"
#include "BattleLineSlot.h"


struct SCurve
{
	float curveLength;
	int nodeCount;
};


class CVertexPoint : public ISplineControlPoint
{
public:
	using TSlot = typename CBattleLineSlot;
	using TCurve = typename SCurve;
public:
	CVertexPoint(CBattleLineSlot* slot)
		: m_pSlot(slot)
	{ }
	virtual ~CVertexPoint() override = default;

	// ISplineControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_pSlot->Position(); }
	virtual void SetPosition(const Vec3& position) override { m_pSlot->SetPosition(position); }
	// ~ISplineControlPoint implementation

	CVertexPoint* PrevVertex() const { return m_pPrevVertex; }
	CVertexPoint* NextVertex() const { return m_pNextVertex; }
	const CAnchorPoint& InAnchor() const { return m_inAnchor; }
	const CAnchorPoint& OutAnchor() const { return m_outAnchor; }
	TSlot* BattleLineSlot() const { return m_pSlot; }

	CVertexPoint** PrevVertexPointer() { return &m_pPrevVertex; }
	CVertexPoint** NextVertexPointer() { return &m_pNextVertex; }
	void SetBattleLineNode(TSlot* node) { m_pSlot = node; }

	Vec3 CalculateCurvePosition(float t);
	static Vec3 CalculateCurvePosition(CVertexPoint& vertex, float t);
	Vec3 CalculateCurveTangent(float t);
	static Vec3 CalculateCurveTangent(CVertexPoint& vertex, float t);

	TCurve GetCurve();
	TCurve GetReverseCurve();

	void Translate(const Vec3& position);
	static void TranslateSpline(CVertexPoint& vertex, const Vec3& newPosition);
	static void ReverseTranslateSpline(CVertexPoint& vertex, const Vec3& newPosition);
private:
	CVertexPoint* m_pPrevVertex = nullptr;
	CVertexPoint* m_pNextVertex = nullptr;
	CAnchorPoint m_inAnchor = CAnchorPoint(this, CAnchorPoint::EAnchorType::eInAnchor);
	CAnchorPoint m_outAnchor = CAnchorPoint(this, CAnchorPoint::EAnchorType::eOutAnchor);
	TSlot* m_pSlot = nullptr;
};