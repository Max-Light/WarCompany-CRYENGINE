
#include "VertexPoint.h"


Vec3 CVertexPoint::CalculateCurvePosition(float t)
{
	return Vec3::CreateCubicCurve(GetPosition(), m_outAnchor.GetPosition(), m_pNextVertex->GetPosition(), m_pNextVertex->m_inAnchor.GetPosition(), t);
}

Vec3 CVertexPoint::CalculateCurvePosition(CVertexPoint& vertex, float t)
{
	return vertex.CalculateCurvePosition(t);
}

Vec3 CVertexPoint::CalculateCurveTangent(float t)
{
	return Vec3();
}

Vec3 CVertexPoint::CalculateCurveTangent(CVertexPoint& vertex, float t)
{
	return Vec3();
}

CVertexPoint::TCurve CVertexPoint::GetCurve()
{
	if (m_pNextVertex == nullptr)
	{
		return TCurve();
	}

	CBattleLineSlot* slot = m_pSlot;
	float curveLength = 0;
	int nodeCount = 1;
	while (slot != m_pNextVertex->m_pSlot)
	{
		curveLength += slot->Radius();
		slot = slot->NextSlot();
		curveLength += slot->Radius();
		++nodeCount;
	}

	return TCurve
	{
		curveLength,
		nodeCount
	};
}

CVertexPoint::TCurve CVertexPoint::GetReverseCurve()
{
	if (m_pNextVertex == nullptr)
	{
		return TCurve();
	}

	CBattleLineSlot* slot = m_pSlot;
	float curveLength = 0;
	int nodeCount = 1;
	while (slot != m_pPrevVertex->m_pSlot)
	{
		curveLength += slot->Radius();
		slot = slot->PrevSlot();
		curveLength += slot->Radius();
		++nodeCount;
	}

	return TCurve
	{
		curveLength,
		nodeCount
	};
}

void CVertexPoint::Translate(const Vec3& position)
{
	Vec3 inAnchorDisplacement = m_inAnchor.GetPosition() - GetPosition();
	Vec3 outAnchorDisplacement = m_outAnchor.GetPosition() - GetPosition();

	SetPosition(position);
	m_inAnchor.SetPosition(inAnchorDisplacement + GetPosition());
	m_outAnchor.SetPosition(outAnchorDisplacement + GetPosition());
}

void CVertexPoint::TranslateSpline(CVertexPoint& vertex, const Vec3& newPosition)
{
	if (vertex.m_pNextVertex == nullptr)
	{
		vertex.Translate(newPosition);
	}
	else
	{
		Vec3 curveDisplacement = vertex.m_pNextVertex->GetPosition() - vertex.GetPosition();
		vertex.Translate(newPosition);
		TranslateSpline(*(vertex.m_pNextVertex), curveDisplacement + vertex.GetPosition());
	}
}

void CVertexPoint::ReverseTranslateSpline(CVertexPoint& vertex, const Vec3& newPosition)
{
	if (vertex.m_pPrevVertex == nullptr)
	{
		vertex.Translate(newPosition);
	}
	else
	{
		Vec3 curveDisplacement = vertex.m_pPrevVertex->GetPosition() - vertex.GetPosition();
		vertex.Translate(newPosition);
		TranslateSpline(*(vertex.m_pPrevVertex), curveDisplacement + vertex.GetPosition());
	}
}