#pragma once

#include "IBattleLineSpline.h"
#include "BattleFormation/BattleFormation.h"
#include "VertexPoint.h"
#include "AnchorPoint.h"


class CBattleLineSpline : public IBattleLineSpline
{
protected:
	enum class ECurveMode : uint8
	{
		Linear = 0,
		Quadratic = 1,
		Cubic = 2
	};

	struct SCurveSegment
	{
		CAnchorPoint outAnchor;
		CAnchorPoint inAnchor;
		ECurveMode curveMode = ECurveMode::Linear;
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

		TVertexCollection::iterator GetStartVertexIterator() const { return m_vertexItr; }
		TVertexCollection::iterator GetEndVertexIterator() const { return m_vertexItr + 1; }
		TCurveSegmentCollection::iterator GetCurveSegmentIterator() const { return m_curveSegmentItr; }

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
		desc.SetLabel("Spline");
		desc.SetDescription("Control the shape of splineable battle formations.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// return the starting curve iterator
	CCurveIterator GetStartCurve() { return CCurveIterator(m_vertexPoints.begin(), m_curveSegments.begin()); }

	// return the ending curve iterator
	CCurveIterator GetEndCurve() { return CCurveIterator(m_vertexPoints.end() - 1, m_curveSegments.end()); }

	// returns the total amount of curves in the spline
	uint GetCurveCount() const { return m_curveSegments.size(); }

	// return the out anchor offset
	Vec3 GetOutAnchorOffset(const CCurveIterator& curveItr) const;

	// return the in anchor offset
	Vec3 GetInAnchorOffset(const CCurveIterator& curveItr) const;

	// Calculate the position on the curve
	Vec3 CalculateCurvePosition(const CCurveIterator& curveItr, float normalizedValue) const;

	// get the curve length between two formation references
	float GetColumnLength(const CVertexPoint::SFormationRef& start, const CVertexPoint::SFormationRef& end) const;

	// inserts a new curve in the spline
	void InsertCurve(CVertexPoint::SFormationRef& formationRef);

	// removes a curve from the spline
	// deletes the starting vertex and following curve segment
	// does not delete the starting or ending vertex
	void RemoveCurve(const CCurveIterator& curveItr);

	// sets the curve mode at the specified curve index
	void ApplyCurveMode(ECurveMode mode, const CCurveIterator& curveItr);

	// returns the curve iterator where the relative formation position belongs
	CCurveIterator FindCurve(const CVertexPoint::SFormationRef& formationRef);
protected:
	CBattleFormation* m_pFormation = nullptr;
	TVertexCollection m_vertexPoints;
	TCurveSegmentCollection m_curveSegments;
};

