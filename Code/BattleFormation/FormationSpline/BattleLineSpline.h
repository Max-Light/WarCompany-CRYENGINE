#pragma once

#include "IBattleLineSpline.h"
#include "BattleFormation/BattleFormation.h"
#include "VertexPoint.h"
#include "AnchorPoint.h"


class CBattleLineSpline : public IBattleLineSpline
{
protected:
	struct SCurveSegment
	{
		CAnchorPoint outAnchor;
		CAnchorPoint inAnchor;
		ICurveSegment::ECurveMode curveMode = ICurveSegment::ECurveMode::Linear;
	};
protected:
	using TVertexCollection = std::vector<CVertexPoint*>;
	using TCurveSegmentCollection = std::vector<SCurveSegment*>;

	class CCurveIterator
	{
	public:
		CCurveIterator(TVertexCollection::iterator vertexItr, TCurveSegmentCollection::iterator curveSegmentItr) 
			: m_vertexItr(vertexItr)
			, m_curveSegmentItr(curveSegmentItr)
		{}
		CCurveIterator() = default;
		~CCurveIterator() = default;

		CVertexPoint* GetStartVertex() { return *m_vertexItr; }
		CVertexPoint* GetEndVertex() { return *(m_vertexItr + 1); }
		SCurveSegment* GetCurveSegment() { return *m_curveSegmentItr; }

		CCurveIterator& operator++() 
		{ 
			++m_vertexItr; 
			++m_curveSegmentItr;
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
			--m_curveSegmentItr;
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
			m_curveSegmentItr += offset;
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
			m_curveSegmentItr -= offset;
			return *this;
		}

		CCurveIterator operator-(const int offset) const
		{
			CCurveIterator temp = *this;
			temp -= offset;
			return temp;
		}

		friend bool operator==(const CCurveIterator& a, const CCurveIterator& b) { return a.m_curveSegmentItr == b.m_curveSegmentItr; };
		friend bool operator!=(const CCurveIterator& a, const CCurveIterator& b) { return a.m_curveSegmentItr != b.m_curveSegmentItr; };
	private:
		TVertexCollection::iterator m_vertexItr;
		TCurveSegmentCollection::iterator m_curveSegmentItr;
	};
public:
	CBattleLineSpline() = default;
	virtual ~CBattleLineSpline() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleLineSpline>& desc)
	{
		desc.SetGUID("{E1621D2E-AAAA-4D42-A112-E745C3CD46C7}"_cry_guid);
		desc.SetEditorCategory("Battle Formation/Battle Line Spline");
		desc.SetLabel("Battle Line Spline");
		desc.SetDescription("Creates a spline to shape battle formations");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IBattleLineSpline
	virtual void InsertCurve(IFormationColumn& column, float normalVal = 0.5f) override;
	virtual void RemoveCurve(IVertexPoint& vertex, const ECurveDirection& dir = ECurveDirection::Right) override;
	virtual void ApplyCurveMode(ICurveSegment& curveSegment, const ICurveSegment::ECurveMode& mode) override;
	// ~IBattleLineSpline
protected:
	// Return the starting curve iterator
	CCurveIterator GetStartCurve() { return CCurveIterator(m_vertexPoints.begin(), m_curveSegments.begin()); }

	// Return the ending curve iterator
	CCurveIterator GetEndCurve() { return CCurveIterator(m_vertexPoints.end() - 1, m_curveSegments.end()); }

	// Returns the curve iterator where the vertex is found
	CCurveIterator FindCurve(const CVertexPoint& vertex);
	// Returns the curve iterator where the anchor is found
	CCurveIterator FindCurve(const CAnchorPoint& anchor);
	// Returns the curve iterator where the curve segment is found
	CCurveIterator FindCurve(const SCurveSegment& curveSegment);
	// Returns the curve iterator where the formation column belongs
	CCurveIterator FindCurve(const IFormationColumn& column);

	// Return the out anchor offset
	Vec3 GetOutAnchorOffset(const CCurveIterator& curveItr) const;

	// Return the in anchor offset
	Vec3 GetInAnchorOffset(const CCurveIterator& curveItr) const;

	// Return the curve length from start to end vertex points
	float GetCurveLength(const CVertexPoint& start, const CVertexPoint& end) const;

	// Calculate the position on the curve
	Vec3 CalculateCurvePosition(const CCurveIterator& curveItr, float normalizedValue) const;
protected:
	CBattleFormation* m_pFormation = nullptr;
	TVertexCollection m_vertexPoints;
	TCurveSegmentCollection m_curveSegments;
};

