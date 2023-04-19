#pragma once

#include "IBattleFormation.h"
#include "FormationSlot.h"
#include "IFormationUnit.h"


class CBattleFormation : public IBattleFormation
{
public:
	struct IFormationColumn
	{
		virtual float GetXPos() const = 0;
		virtual float GetWidth() const = 0;
	};

	// shifts the position of columns up and down the formation line
	struct IColumnVerticalityHandler
	{
		float GetColumnVerticalOffset(IFormationColumn* pColumn) const { return 0; }
	};

	struct IBattleFormationColumnObserver
	{
		enum class EActionType
		{
			ColumnAdd,
			ColumnRemove,
		};

		virtual void OnFormationColumn(const EActionType& action, IFormationColumn* pColumn) = 0;
	};
protected:
	class CFormationColumn : public IFormationColumn
	{
	public:
		using TSlotCollection = std::vector<CFormationSlot*>;
	public:
		CFormationColumn() = default;
		virtual ~CFormationColumn() = default;

		// IFormationColumn
		virtual float GetXPos() const override { return m_xPosition; }
		virtual float GetWidth() const override { return m_width; }
		// ~IFormationColumn

		// Return a pointer to the collection of slots in the column
		const TSlotCollection* GetSlots() const { return &m_slots; }
		TSlotCollection* GetSlots() { return &m_slots; }

		// Set the x position of the column in the formation
		void SetXPos(float xPos) { m_xPosition = xPos; }

		// Set the width of the column
		void SetWidth(float width) { m_width = width; }
	protected:
		TSlotCollection m_slots;
		float m_xPosition = 0;
		float m_width = 0;
	};
	using TColumnCollection = std::vector<CFormationColumn>;
	using TSlotCollection = CFormationColumn::TSlotCollection;
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
	virtual IFormationSlot* InsertColumnAndUnit(SColumnUnitInsertionParam& insertionParams) override;
	virtual IFormationSlot* InsertUnitInColumn(SUnitInsertionParam& insertionParams) override;
	virtual void RemoveSlot(uint x, uint y) override;
	virtual void RemoveSlot(IFormationSlot* pSlot) override;
	// ~IBattleFormation

	// Register a column verticality handler
	void RegisterColumnVerticalityHandler(IColumnVerticalityHandler* pHandler) { m_columnVerticalHandlers.push_back(pHandler); }

	// Unregister a column verticality handler
	void UnregisterColumnVerticalityHandler(IColumnVerticalityHandler* pHandler) { m_columnVerticalHandlers.erase(std::find(m_columnVerticalHandlers.begin(), m_columnVerticalHandlers.end(), pHandler)); }
protected:
	// Parameter to be passed to the SpawnSlot function
	struct SSlotSpawnParams : public SSlotSpawnBaseParam
	{
		TColumnCollection::iterator colItr;
		TSlotCollection::iterator slotItr;
	};
	// Spawns a slot local to the formation
	CFormationSlot* SpawnSlot(const SSlotSpawnParams& slotParams);

	// Inserts a new column in the formation
	TColumnCollection::iterator InsertColumnAt(TColumnCollection::iterator colItr, float columnWidth, const EColumnShiftType& shiftType);

	// Shift columns right and left on the horizontal axis in the formation
	void ShiftColumnsAt(const TColumnCollection::iterator& colItr, float offset, const EColumnShiftType& shiftType = EColumnShiftType::Right);

	// Shift all slots in the column range by the offset
	void ShiftSlotsInColumnRange(TColumnCollection::iterator startItr, const TColumnCollection::iterator& endItr, float offset);

	// Shift slots within a column up and down
	void ShiftSlotsInColumn(TSlotCollection::iterator startItr, const TSlotCollection::iterator& endItr, float offset);

	// Retrieve the column's vertical offset
	float GetColumnOffset(IFormationColumn* pColumn) const;

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