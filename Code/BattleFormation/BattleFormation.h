#pragma once

#include "IBattleFormation.h"
#include "FormationColumn.h"

typedef std::vector<CFormationColumn*> ColumnCollection;

// Wrapper column iterator for IFormationColumn
class CColumnIterator
{
protected:
	typedef ColumnCollection::iterator Iterator;
public:
	CColumnIterator() = default;
	CColumnIterator(const Iterator& colItr)
		: m_columnItr(colItr)
	{}
	~CColumnIterator() = default;

	IFormationColumn& operator*() const { return **m_columnItr; }
	IFormationColumn* operator->() const { return *m_columnItr; }
	CColumnIterator& operator++() { ++m_columnItr; return *this; }
	CColumnIterator& operator--() { --m_columnItr; return *this; }
	CColumnIterator& operator+=(const int offset) { m_columnItr += offset; return *this; }
	CColumnIterator& operator-=(const int offset) { m_columnItr -= offset; return *this; }
private:
	Iterator m_columnItr;
};

class CBattleFormation : public IBattleFormation
{
public:
	CBattleFormation() = default;
	virtual ~CBattleFormation() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleFormation>& desc)
	{
		desc.SetGUID("{4354C586-96B8-4D5F-8410-B1E845995BF0}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Formation");
		desc.SetDescription("Create an organized unit formation.");
		desc.AddBase<IBattleFormation>();
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IBattleFormation
	virtual IFormationSlot* GetSlot(uint x, uint y) const override { return (*m_formationColumns[x])[y]; }
	virtual IFormationColumn* GetColumn(uint x) const override { return m_formationColumns[x]; }
	virtual CColumnIterator GetBeginColumn() override { return CColumnIterator(m_formationColumns.begin()); }
	virtual CColumnIterator GetEndColumn() override { return CColumnIterator(m_formationColumns.end()); }
	virtual uint GetColumnCount() const override { return m_formationColumns.size(); }
	virtual uint GetSlotCount() const override;
	virtual IFormationSlot* InsertColumnAndUnit(uint col, SSlotSpawnParams& slotParams, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual IFormationSlot* InsertUnitInColumn(uint col, uint depth, SSlotSpawnParams& slotParams) override;
	virtual void RemoveSlot(uint x, uint y) override;
	virtual void RemoveSlot(IFormationSlot* pSlot) override;
	// ~IBattleFormation
protected:
	// Spawns a slot local to the formation
	CFormationSlot* SpawnSlot(const SSlotSpawnParams& slotParams, const Vec2& gridPos);

	// Inserts a new column in the formation
	ColumnCollection::iterator InsertColumnAt(ColumnCollection::iterator colItr, float columnWidth, const EColumnShiftType& shiftType);

	// Shift columns right and left on the horizontal axis in the formation
	void ShiftColumnsAt(const ColumnCollection::iterator& colItr, float offset, const EColumnShiftType& shiftType = EColumnShiftType::Right);

	// Shift all slots in the column range by the offset
	void ShiftColumnRange(ColumnCollection::iterator startItr, const ColumnCollection::iterator& endItr, float offset);

	// Update slot positions within the column range
	void UpdateSlotsInColumnRange(ColumnCollection::iterator startItr, const ColumnCollection::iterator& endItr);

	// Iterate over all slots in the formation
	void IterateSlots(std::function<void(CFormationSlot& slot)> func) const;

	// Update all slots to the new position
	void UpdateAllSlotPos();
protected:
	ColumnCollection m_formationColumns;
};

