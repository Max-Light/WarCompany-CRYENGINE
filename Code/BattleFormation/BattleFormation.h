#pragma once

#include "IBattleFormation.h"
#include "BattleFormationColumn.h"

class CBattleFormation : public IBattleFormation
{
public:
	// shifts the position of columns up and down the formation line
	struct IColumnVerticalityHandler
	{
		float GetColumnVerticalOffset(IBattleFormationColumn* pColumn) const { return 0; }
	};

	struct IBattleFormationColumnObserver
	{
		enum class EActionType
		{
			ColumnAdd,
			ColumnRemove,
		};

		virtual void OnFormationColumn(const EActionType& action, IBattleFormationColumn* pColumn) = 0;
	};
protected:
	using TColumnCollection = std::vector<CBattleFormationColumn>;
	using TSlotCollection = CBattleFormationColumn::TSlotCollection;

	struct SBattleFormationIterator
	{
		SBattleFormationIterator() = default;
		SBattleFormationIterator(const TColumnCollection::iterator& colItr)
			: colItr(colItr)
			, slotItr(colItr->GetSlots()->begin())
		{}
		SBattleFormationIterator(const TColumnCollection::iterator& colItr, const TSlotCollection::iterator& slotItr)
			: colItr(colItr)
			, slotItr(slotItr)
		{}
		virtual ~SBattleFormationIterator() = default;

		TColumnCollection::iterator colItr;
		TSlotCollection::iterator slotItr;
	};
public:
	CBattleFormation() = default;
	virtual ~CBattleFormation() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleFormation>& desc)
	{
		desc.SetGUID("{4354C586-96B8-4D5F-8410-B1E845995BF0}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Formation");
		desc.SetDescription("Create an organized unit formation.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IBattleFormation
	virtual IFormationSlot* GetSlot(uint x, uint y) const override { return (*m_formationColumns[x].GetSlots())[y]; }
	virtual uint GetColumnCount() const override { return m_formationColumns.size(); }
	virtual uint GetSlotCountInColumn(uint col) const override { return m_formationColumns[col].GetSlots()->size(); }
	virtual uint GetSlotCount() const override;
	virtual IFormationSlot* InsertColumnAndUnit(uint col, SSlotSpawnParams& slotParams, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual IFormationSlot* InsertUnitInColumn(uint col, uint depth, SSlotSpawnParams& slotParams) override;
	virtual void RemoveSlot(uint x, uint y) override;
	virtual void RemoveSlot(IFormationSlot* pSlot) override;
	// ~IBattleFormation

	// Register a column verticality handler
	void RegisterColumnVerticalityHandler(IColumnVerticalityHandler* pHandler) { m_columnVerticalHandlers.push_back(pHandler); }

	// Unregister a column verticality handler
	void UnregisterColumnVerticalityHandler(IColumnVerticalityHandler* pHandler) { m_columnVerticalHandlers.erase(std::find(m_columnVerticalHandlers.begin(), m_columnVerticalHandlers.end(), pHandler)); }
protected:
	// Spawns a slot local to the formation
	CFormationSlot* SpawnSlot(const SSlotSpawnParams& slotParams, const Vec3& pos);

	// Query the positional point where a slot would be placed in the formation grid
	Vec3 QuerySlotPosition(const SBattleFormationIterator& itr, const SSlotSpawnParams& slotParams) const;

	// Inserts a new column in the formation
	TColumnCollection::iterator InsertColumnAt(TColumnCollection::iterator colItr, float columnWidth, const EColumnShiftType& shiftType);

	// Shift columns right and left on the horizontal axis in the formation
	void ShiftColumnsAt(const TColumnCollection::iterator& colItr, float offset, const EColumnShiftType& shiftType = EColumnShiftType::Right);

	// Shift all slots in the column range by the offset
	void ShiftSlotsInColumnRange(TColumnCollection::iterator startItr, const TColumnCollection::iterator& endItr, float offset);

	// Shift slots within a column up and down
	void ShiftSlotsInColumn(TSlotCollection::iterator startItr, const TSlotCollection::iterator& endItr, float offset);

	// Retrieve the column's vertical offset
	float GetColumnOffset(IBattleFormationColumn* pColumn) const;

	// Returns the terrain point position local to the formation
	Vec3 GetTerrainPosition(Vec2 gridPosition) const;

	// Iterate over all slots in the formation
	void IterateSlots(std::function<void(CFormationSlot& slot)> func) const;

	// Update all slots to the new position
	void UpdateAllSlotPos() const;
protected:
	TColumnCollection m_formationColumns;
	std::vector<IColumnVerticalityHandler*> m_columnVerticalHandlers;
};