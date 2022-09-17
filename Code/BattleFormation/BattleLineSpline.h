#pragma once

#include <CryEntitySystem/IEntityComponent.h>

#include "BattleLineSlot.h"
#include "VertexPoint.h"


class CBattleLineSpline;

class CBattleLineSplineIterator
{
public:
	using TSlot = typename CBattleLineSlot;
	using TVertex = typename CVertexPoint;
	using TNodePointer = typename CBattleLineSlot*;
	using TNodeReference = typename CBattleLineSlot&;

	CBattleLineSplineIterator() noexcept
		: m_pSlot(nullptr)
	{}

	CBattleLineSplineIterator(TSlot* node)
		: m_pSlot(node)
	{}

	TNodeReference operator*() const noexcept
	{
		return *m_pSlot;
	}

	TNodePointer operator->() const noexcept
	{
		return m_pSlot;
	}

	CBattleLineSplineIterator& operator++() noexcept
	{
		m_pSlot = m_pSlot->NextSlot();
		return *this;
	}

	CBattleLineSplineIterator operator++(int) noexcept
	{
		CBattleLineSplineIterator iterator = *this;
		m_pSlot = m_pSlot->NextSlot();
		return iterator;
	}

	CBattleLineSplineIterator& operator--() noexcept
	{
		m_pSlot = m_pSlot->PrevSlot();
		return *this;
	}

	CBattleLineSplineIterator operator--(int) noexcept
	{
		CBattleLineSplineIterator iterator = *this;
		m_pSlot = m_pSlot->PrevSlot();
		return iterator;
	}

	bool operator==(const CBattleLineSplineIterator& right) const noexcept
	{
		return m_pSlot == right.m_pSlot;
	}

	bool operator!=(const CBattleLineSplineIterator& right) const noexcept
	{
		return m_pSlot != right.m_pSlot;
	}

	TSlot* Slot() const { return m_pSlot; }
	TVertex* Vertex() const { return m_pSlot->Vertex(); }
private:
	TSlot* m_pSlot = nullptr;
};

class CBattleLineSpline : public IEntityComponent
{
public:
	using TIterator = typename CBattleLineSplineIterator;
	using TSlot = typename CBattleLineSlot;
public:
	CBattleLineSpline() = default;
	virtual ~CBattleLineSpline() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleLineSpline>& desc)
	{
		desc.SetGUID("{ABAE3702-F302-4122-A470-639ABD50938B}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Line Spline");
		desc.SetDescription("A battle line formation in the form of a spline. Control the battle line with vertex and anchor points.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	TIterator operator[](const int index) const noexcept { return GetVertexAt(index); }
	TIterator Start() const { return TIterator(m_pStartVertex->BattleLineSlot()); }
	TIterator End() const { return TIterator(m_pEndVertex->BattleLineSlot()->NextSlot()); }
	TIterator GetVertexAt(int index) const;
	int CurveCount() const { return m_curveCount; }
	int SlotCount() const { return m_slotCount; }

	static TSlot* SpawnSlot();

	CVertexPoint* SetVertex(const TIterator& iterator);
	void RemoveVertex(const TIterator& iterator);

	void SetSlotAmount(int slots);
	void Clear();

	void PushSlotFront(TSlot* slot);
	void PushSlotBack(TSlot* slot);
	void InsertSlot(const TIterator& iterator, TSlot* slot);
	TSlot* PopSlotFront();
	TSlot* PopSlotBack();
	TSlot* PopSlot(const TIterator& iterator);
private:
	int m_slotCount = 0;
	int m_curveCount = 0;
	CVertexPoint* m_pStartVertex;
	CVertexPoint* m_pEndVertex;
};