#pragma once

#include "IBattleLineSpline.h"
#include "BattleFormation/BattleFormation.h"
#include "VertexPoint.h"
#include "AnchorPoint.h"
#include "CurveLine.h"


typedef std::vector<CVertexPoint*> VertexCollection;
typedef std::vector<CCurveLine*> CurveLineCollection;

class CCurveIterator
{
public:
	friend class CBattleLineSpline;

	CCurveIterator(VertexCollection::const_iterator vertexItr, CurveLineCollection::const_iterator curveLineItr)
		: m_vertexItr(vertexItr)
		, m_curveLineItr(curveLineItr)
	{}
	CCurveIterator() = default;
	~CCurveIterator() = default;

	IVertexPoint* GetStartVertex() const { return *m_vertexItr; }
	IVertexPoint* GetEndVertex() const { return *(m_vertexItr + 1); }
	IAnchorPoint* GetOutAnchor() const { return (*m_curveLineItr)->GetOutAnchor(); }
	IAnchorPoint* GetInAnchor() const { return (*m_curveLineItr)->GetInAnchor(); }
	ICurveLine* GetCurveLine() const { return *m_curveLineItr; }

	CCurveIterator& operator++()
	{
		++m_vertexItr;
		++m_curveLineItr;
		return *this;
	}

	CCurveIterator operator++(int)
	{
		CCurveIterator temp = *this;
		++(*this);
		return temp;
	}

	CCurveIterator& operator--()
	{
		--m_vertexItr;
		--m_curveLineItr;
		return *this;
	}

	CCurveIterator operator--(int)
	{
		CCurveIterator temp = *this;
		--(*this);
		return temp;
	}

	CCurveIterator& operator+=(const int offset)
	{
		m_vertexItr += offset;
		m_curveLineItr += offset;
		return *this;
	}

	CCurveIterator operator+(const int offset) const
	{
		CCurveIterator temp = *this;
		temp += offset;
		return temp;
	}

	CCurveIterator& operator-=(const int offset)
	{
		m_vertexItr -= offset;
		m_curveLineItr -= offset;
		return *this;
	}

	CCurveIterator operator-(const int offset) const
	{
		CCurveIterator temp = *this;
		temp -= offset;
		return temp;
	}

	friend bool operator==(const CCurveIterator& a, const CCurveIterator& b) { return a.m_curveLineItr == b.m_curveLineItr; };
	friend bool operator!=(const CCurveIterator& a, const CCurveIterator& b) { return a.m_curveLineItr != b.m_curveLineItr; };
private:
	VertexCollection::const_iterator m_vertexItr;
	CurveLineCollection::const_iterator m_curveLineItr;
};


class CBattleLineSpline : public IBattleLineSpline
{
public:
	CBattleLineSpline() = default;
	virtual ~CBattleLineSpline() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleLineSpline>& desc)
	{
		desc.SetGUID("{E1621D2E-AAAA-4D42-A112-E745C3CD46C7}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Line Spline");
		desc.SetDescription("Creates a spline to shape battle formations");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IBattleLineSpline
	virtual IBattleFormation* GetFormation() const override { return m_pFormation; }
	virtual void InsertCurve(IFormationColumn* pColumn, float normalVal = 0.0f) override;
	virtual void InsertCurve(float battleLineXPos) override;
	virtual void RemoveCurve(const IVertexPoint* pVertex, const ECurveDirection& dir = ECurveDirection::Right) override;
	virtual void ApplyCurveMode(ICurveSegment* pCurveSegment, const ICurveSegment::ECurveMode& mode) override;
	virtual void MoveVertex(IVertexPoint* pVertex, const Vec2& pos) override;
	virtual void MoveAnchor(IAnchorPoint* pAnchor, const Vec2& pos) override;
	// ~IBattleLineSpline

	// Return the starting curve iterator
	CCurveIterator GetStartCurve() const { return CCurveIterator(m_vertexPoints.begin(), m_curveSegments.begin()); }

	// Return the ending curve iterator
	CCurveIterator GetEndCurve() const { return CCurveIterator(m_vertexPoints.end() - 1, m_curveSegments.end()); }

protected:
	// Returns the curve iterator where the vertex is found
	CCurveIterator FindCurve(const IVertexPoint* pVertex);
	// Returns the curve iterator where the anchor is found
	CCurveIterator FindCurve(const IAnchorPoint* pAnchor);
	// Returns the curve iterator where the curve segment is found
	CCurveIterator FindCurve(const ICurveLine& curveLine);
	// Returns the curve iterator where the formation column plus its normalized offset from the center [-1, 1] belongs
	CCurveIterator FindCurve(const IFormationColumn* pColumn, float normalizedOffset);
	// Returns the curve iterator where the curve's horizontal (x) length is located
	CCurveIterator FindCurve(float battleLineLength);

	// Return the out anchor offset
	Vec3 GetOutAnchorOffset(const CCurveIterator& curveItr) const;

	// Return the in anchor offset
	Vec3 GetInAnchorOffset(const CCurveIterator& curveItr) const;

	// Calculate the position on the curve
	Vec3 CalculateCurvePosition(const CCurveIterator& curveItr, float normalizedValue) const;

	// Calculate the tangent on the curve
	Vec3 CalculateCurveTangent(const CCurveIterator& curveItr, float normalizedValue) const;

	// Initializes the spline by creating the first curve
	void InitializeSpline();

	// Restricts middle vertex points to an angle between adjacent vertices
	void RestrictVertexAngle(const CCurveIterator& curveItr);
protected:
	CBattleFormation* m_pFormation = nullptr;
	VertexCollection m_vertexPoints;
	CurveLineCollection m_curveSegments;

	constexpr static float m_maxVertexAngleOffset = 33.34f;
	constexpr static float m_maxAnchorAngleOffset = 45.0f;
	constexpr static float m_normalizedAnchorLength = 0.33f;
};

