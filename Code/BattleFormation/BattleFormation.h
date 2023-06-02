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

	using SlotItr = FormationColumn::SlotItr;
	using ColumnItr = ColumnCollection::iterator;
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
	virtual IFormationSlot* InsertColumnAndUnit(uint columnIndex, IFormationUnit* pUnit, EColumnShiftType shiftType = EColumnShiftType::Right) override;
	virtual IFormationSlot* InsertUnitInColumn(uint columnIndex, uint depth, IFormationUnit* pUnit) override;
	virtual void RemoveColumn(uint columnIndex) override;
	virtual bool RemoveColumn(IFormationColumn* pColumn) override;
	virtual void RemoveSlot(uint columnIndex, uint index) override;
	virtual bool RemoveSlot(IFormationSlot* pSlot) override;
	virtual void ClearSlots() override;
	virtual Vec3 CreatePos(const Vec2& gridPos) override;
	inline virtual void AddBattleFormationListener(IBattleFormationListener* listener) { m_eventListeners.push_back(listener); }
	inline virtual void RemoveBattleFormationListener(IBattleFormationListener* listener) { m_eventListeners.erase(std::find(m_eventListeners.begin(), m_eventListeners.end(), listener)); }
	// ~IBattleFormation

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
