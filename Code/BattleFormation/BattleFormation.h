#pragma once

#include "IBattleFormation.h"
#include "FormationColumn.h"

typedef CFormationColumn FormationColumn;
typedef std::vector<FormationColumn*> ColumnCollection;

class CBattleFormation : public IBattleFormation
{
public:
	struct SFormationProperties
	{
		void Serialize(Serialization::IArchive& archive)
		{
			archive(columnProperties, "Columns", "Columns");
		}

		std::vector<FormationColumn::SColumnProperties> columnProperties;
	};

	using ColumnItr = ColumnCollection::const_iterator;
	using SlotItr = FormationColumn::SlotItr;

	class CFormationIterator
	{
	public:
		CFormationIterator() = default;
		CFormationIterator(const ColumnItr& columnItr, const SlotItr& slotItr)
			: m_columnItr(columnItr)
			, m_slotItr(slotItr)
		{
		}

		// Shift the iterator to the next column
		inline void NextColumn() { ++m_columnItr; m_slotItr = (*m_columnItr)->GetStartSlot(); }

		// Shift the iterator to the previous column
		inline void PrevColumn() { --m_columnItr; m_slotItr = (*m_columnItr)->GetStartSlot(); }

		// Shift the slot iterator down in the current column
		inline void MoveDown() { ++m_slotItr; }

		// Shift the slot iterator up in the current column
		inline void MoveUp() { --m_slotItr; }

		// Return the column at the iterator location
		inline CFormationColumn* GetColumn() const { return *m_columnItr; }

		// Return the slot at the iterator location
		inline CFormationSlot* GetSlot() const { return *m_slotItr; }

		// Return the column iterator
		inline const ColumnItr& GetColumnItr() const { return m_columnItr; }

		// Return the slot iterator
		inline const SlotItr& GetSlotItr() const { return m_slotItr; }

		// Places the slot iterator onto the specified slot pointer, otherwise returns false if slot not found
		inline bool GoToSlot(IFormationSlot* pSlot) 
		{ 
			SlotItr slotItr = GetColumn()->FindSlot(pSlot); 
			if (slotItr != GetColumn()->GetEndSlot()) 
			{
				m_slotItr = slotItr;
				return true;
			} 
			return false;
		}
	private:
		ColumnItr m_columnItr;
		SlotItr m_slotItr;
	};
	using FormationItr = CFormationIterator;
public:
	CBattleFormation() = default;
	virtual ~CBattleFormation() = default;

	void Serialize(Serialization::IArchive& archive)
	{
		
	}

	static void ReflectType(Schematyc::CTypeDesc<CBattleFormation>& desc)
	{
		desc.SetGUID("{4354C586-96B8-4D5F-8410-B1E845995BF0}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Battle Formation");
		desc.SetDescription("Create an organized unit formation.");
		desc.SetComponentFlags(EntityComponentFlags(
			{
				EEntityComponentFlags::Singleton
			}
		));
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// IBattleFormation
	inline virtual Vec2 GetPos() const { return GetEntity()->GetWorldPos(); }
	inline virtual Quat GetRotation() const { return GetEntity()->GetWorldRotation(); }
	inline virtual IFormationSlot* GetSlot(uint columnIndex, uint index) const override { return m_formationColumns[columnIndex]->GetSlotAt(index); }
	inline virtual IFormationColumn* GetColumn(uint columnIndex) const override { return m_formationColumns[columnIndex]; }
	inline virtual uint GetSlotCount() const override;
	inline virtual uint GetColumnCount() const override { return m_formationColumns.size(); }
	inline virtual uint GetSlotCountInColumn(uint columnIndex) const override{ return m_formationColumns[columnIndex]->GetSlotCount(); }
	inline virtual Vec3 GetBattleLineDirection() const { return GetEntity()->GetRightDir() * GetBattleLineLength(); }
	inline virtual float GetBattleLineLength() const override;
	inline virtual bool IsEmpty() const { return m_formationColumns.empty(); }
	inline virtual bool IsColumnEmpty(uint columnIndex) const { return (*m_formationColumns[columnIndex]).IsEmpty(); }
	virtual IFormationSlot* InsertColumnAndUnit(uint columnIndex, IFormationUnit* pUnit, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual IFormationSlot* InsertUnitInColumn(uint columnIndex, uint depth, IFormationUnit* pUnit) override;
	virtual void RemoveColumn(uint columnIndex, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual bool RemoveColumn(IFormationColumn* pColumn, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual void RemoveSlot(uint columnIndex, uint index) override;
	virtual bool RemoveSlot(IFormationSlot* pSlot) override;
	virtual void ClearSlots() override;
	virtual Vec3 CreatePos(const Vec2& gridPos) override;
	inline virtual void AddBattleFormationListener(IBattleFormationListener* listener) { m_eventListeners.push_back(listener); }
	inline virtual void RemoveBattleFormationListener(IBattleFormationListener* listener) { m_eventListeners.erase(std::find(m_eventListeners.begin(), m_eventListeners.end(), listener)); }
	// ~IBattleFormation

	inline FormationItr GetBeginFormationItr() const { return FormationItr(m_formationColumns.begin(), (*m_formationColumns.begin())->GetStartSlot()); }
	inline FormationItr GetFormationItr(uint columnIndex, uint slotIndex) const 
	{ 
		CRY_ASSERT(columnIndex < GetColumnCount(), "Column index out of range!");
		CRY_ASSERT(slotIndex < (m_formationColumns[columnIndex])->GetSlotCount(), "Slot index out of range in Column!");
		ColumnItr columnItr = m_formationColumns.begin() + columnIndex;
		return FormationItr(columnItr, (*columnItr)->GetStartSlot() + slotIndex); 
	}

	// Remove the column at the specified column iterator
	void RemoveColumn(ColumnItr& columnItr, EColumnShiftType shiftType);

	// Remove the slot at the specified formation iterator 
	void RemoveSlot(const FormationItr& formationItr);

	// Return the formation properties
	SFormationProperties GetFormationProperties() const;
protected:
	// Inserts a new column in the formation
	ColumnItr InsertColumnAt(ColumnItr colItr, float columnWidth, const EColumnShiftType& shiftType);

	// Shift columns right and left on the horizontal axis in the formation
	void ShiftColumnsAt(const ColumnItr& colItr, float offset, const EColumnShiftType& shiftType = EColumnShiftType::Right);

	// Helper function that shifts all slots in the column range by the offset
	inline void ShiftColumnRange(ColumnItr startItr, const ColumnItr& endItr, float offset);

	// Update slot positions within the column range
	void UpdateSlotsInColumnRange(ColumnItr startItr, const ColumnItr& endItr);

	// Iterate over all slots in the formation
	void IterateSlots(std::function<void(CFormationSlot& slot)> func) const;

	// Update all slots to the new position
	void UpdateAllSlotPos();

	// Helper function to notify all listeners of the event
	inline void NotifyBattleFormationListeners(const SBattleFormationEvent& formationEvent) const { for (IBattleFormationListener* listener : m_eventListeners) { listener->OnBattleFormationEvent(formationEvent); } }
protected:
	ColumnCollection m_formationColumns;
	std::vector<IBattleFormationListener*> m_eventListeners;
};
