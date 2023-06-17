#pragma once

#include "IBattleLineSpline.h"
#include "BattleFormation/BattleFormation.h"
#include "VertexPoint.h"
#include "AnchorPoint.h"
#include "CurveLine.h"

// For reference
#include <DefaultComponents/Physics/Vehicles/VehicleComponent.h>


typedef std::vector<CVertexPoint*> VertexCollection;
typedef std::vector<CCurveLine*> CurveLineCollection;

class CLocalCurveIterator : public ICurveSection
{
public:
	CLocalCurveIterator(VertexCollection::const_iterator vertexItr, CurveLineCollection::const_iterator curveLineItr)
		: m_vertexItr(vertexItr)
		, m_curveLineItr(curveLineItr)
	{
	}
	CLocalCurveIterator() = default;
	virtual ~CLocalCurveIterator() = default;

	// ICurveSection
	inline virtual IVertexPoint* GetStartVertex() const override { return *m_vertexItr; }
	inline virtual IVertexPoint* GetEndVertex() const override { return *(m_vertexItr + 1); }
	inline virtual IAnchorPoint* GetOutAnchor() const override { return (*m_curveLineItr)->GetOutAnchor(); }
	inline virtual IAnchorPoint* GetInAnchor() const override { return (*m_curveLineItr)->GetInAnchor(); }
	inline virtual ICurveProperties* GetCurveLine() const override { return *m_curveLineItr; }
	// ~ICurveSection

	inline const VertexCollection::const_iterator& GetVertexItr() const { return m_vertexItr; }
	inline const CurveLineCollection::const_iterator& GetCurveLineItr() const { return m_curveLineItr; }

	inline CLocalCurveIterator& operator++() 
	{
		++m_vertexItr;
		++m_curveLineItr;
		return *this;
	}
	inline virtual CLocalCurveIterator& operator--()
	{
		--m_vertexItr;
		--m_curveLineItr;
		return *this;
	}
	inline CLocalCurveIterator operator++(int)
	{
		CLocalCurveIterator temp = *this;
		++(*this);
		return temp;
	}

	inline CLocalCurveIterator operator--(int)
	{
		CLocalCurveIterator temp = *this;
		--(*this);
		return temp;
	}

	inline CLocalCurveIterator& operator+=(const int offset)
	{
		m_vertexItr += offset;
		m_curveLineItr += offset;
		return *this;
	}

	inline CLocalCurveIterator operator+(const int offset) const
	{
		CLocalCurveIterator temp = *this;
		temp += offset;
		return temp;
	}

	inline CLocalCurveIterator& operator-=(const int offset)
	{
		m_vertexItr -= offset;
		m_curveLineItr -= offset;
		return *this;
	}

	inline CLocalCurveIterator operator-(const int offset) const
	{
		CLocalCurveIterator temp = *this;
		temp -= offset;
		return temp;
	}
	inline bool operator==(const CLocalCurveIterator& rhs) const { return m_vertexItr == rhs.m_vertexItr; }
	inline bool operator!=(const CLocalCurveIterator& rhs) const { return m_vertexItr != rhs.m_vertexItr; }
private:
	VertexCollection::const_iterator m_vertexItr;
	CurveLineCollection::const_iterator m_curveLineItr;
};

class CCurveIterator : public ICurveIterator, public _reference_target_t
{
public:
	CCurveIterator(VertexCollection* pVertexCollection, CurveLineCollection* pCurveLineCollection)
		: m_pVertexCollection(pVertexCollection)
		, m_pCurveLineCollection(pCurveLineCollection)
		, m_curveItr(CLocalCurveIterator(pVertexCollection->begin(), pCurveLineCollection->begin()))
	{
	}
	virtual ~CCurveIterator() = default;

	// ICurveIterator
	inline virtual void AddRef() override { _reference_target_t::AddRef(); }
	inline virtual void Release() override { _reference_target_t::Release(); }
	inline virtual const ICurveSection& GetCurve() const override { return m_curveItr; }
	inline virtual void Next() override { ++m_curveItr; }
	inline virtual bool IsEnd() const { return m_curveItr.GetCurveLineItr() == m_pCurveLineCollection->end(); }
	inline virtual void MoveToFirst() { m_curveItr = CLocalCurveIterator(m_pVertexCollection->begin(), m_pCurveLineCollection->begin()); }
	// ~ICurveIterator
private:
	CurveLineCollection* m_pCurveLineCollection = nullptr;
	VertexCollection* m_pVertexCollection = nullptr;
	CLocalCurveIterator m_curveItr;
};

class CCurveFragment : IColumnVerticalOffsetEffect
{
public:
	CCurveFragment(float min, float max)
		: m_minVal(min)
		, m_maxVal(max)
	{
	}
	virtual ~CCurveFragment() = default;

	virtual float GetVerticalOffset(float t) const override;
private:
	float m_minVal;
	float m_maxVal;
};

class CBattleLineSpline : public IBattleLineSpline
{
public:
	typedef CLocalCurveIterator CurveItr;
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
	virtual void InsertCurve(float battleLineXPos) override;
	virtual void RemoveCurve(const IVertexPoint* pVertex, const ECurveDirection& dir = ECurveDirection::Right) override;
	virtual void Reset() override;
	virtual ICurveIteratorPtr GetCurveIterator() override { return new CCurveIterator(&m_vertexPoints, &m_curveSegments); }
	inline virtual float GetMaxVertexAngularRestriction() const override { return s_maxVertexAngleOffset; }
	inline virtual float GetMaxAnchorAngularRestriction() const override { return s_maxAnchorAngleOffset; }
	inline virtual float GetDefaultNormalizedAnchorLength() const override { return s_normalizedAnchorLength; }
	// ~IBattleLineSpline

	// Calculate the curve position interpolated in the range [0, 1]
	static Vec3 CreateCurvePos(const CurveItr& curveItr, float t);

	// Calculate the curve tangent interpolated in the range [0, 1]
	static Vec3 CreateCurveTangent(const CurveItr& curveItr, float t);

	// Returns a curve iterator to the first curve instance in the spline
	CurveItr GetStartCurve() const { return CurveItr(m_vertexPoints.begin(), m_curveSegments.begin()); }

	// Returns a curve iterator to the last curve instance in the spline
	CurveItr GetEndCurve() const { return CurveItr(m_vertexPoints.end() - 1, m_curveSegments.end()); }

	// Returns the curve iterator where the vertex is found
	CurveItr FindCurve(const IVertexPoint* pVertex) const;

	// Returns the curve iterator where the anchor is found
	CurveItr FindCurve(const IAnchorPoint* pAnchor) const;

	// Returns the curve iterator where the curve segment is found
	CurveItr FindCurve(const ICurveProperties* curveLine) const;

	// Returns the curve iterator where the spline's battle line x position is located
	CurveItr FindCurve(float battleLineXPos) const;
protected:
	// Return the out anchor offset
	Vec3 GetOutAnchorOffset(const CurveItr& curveItr) const;

	// Return the in anchor offset
	Vec3 GetInAnchorOffset(const CurveItr& curveItr) const;

	// Initializes the spline by creating the first curve
	void InitializeSpline();

	// Restricts middle vertex points to an angle between adjacent vertices
	void RestrictVertexAngle(const CurveItr& curveItr);
protected:
	CBattleFormation* m_pFormation = nullptr;
	VertexCollection m_vertexPoints;
	CurveLineCollection m_curveSegments;

	constexpr static float s_maxVertexAngleOffset = 33.34f;
	constexpr static float s_maxAnchorAngleOffset = 45.0f;
	constexpr static float s_normalizedAnchorLength = 0.33f;
};

