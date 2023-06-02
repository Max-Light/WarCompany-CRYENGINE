#pragma once

#include "IBattleFormation.h"

class CDebugBattleFormation : public IEntityComponent
{
public:
	struct SSlotParams
	{
		inline bool operator==(const SSlotParams& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SSlotParams& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		void Serialize(Serialization::IArchive& archive)
		{
			archive(unitId, "UnitEntity", "Unit Entity");
		}
		
		static void ReflectType(Schematyc::CTypeDesc<SSlotParams>& desc)
		{
			desc.SetGUID("{C5A24DAA-49D8-46E1-A72C-11769C843720}"_cry_guid);
			desc.AddMember(&SSlotParams::unitId, 'unit', "UnitEntity", "Unit Entity", nullptr, Schematyc::ExplicitEntityId::Invalid);
		}

		IFormationSlot* pSlot = nullptr;
		EntityId unitId = 0;
	};

	struct SColumnParams
	{
		inline bool operator==(const SColumnParams& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SColumnParams& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		void Serialize(Serialization::IArchive& archive)
		{
			archive(slotParams, "Slots", "Slots");
		}
		
		static void ReflectType(Schematyc::CTypeDesc<SColumnParams>& desc)
		{
			desc.SetGUID("{C3BB6A5D-77BB-4EA4-9EB7-22AA31870F15}"_cry_guid);
			desc.SetLabel("Slots");
			desc.AddMember(&SColumnParams::slotParams, 'slot', "Slots", "Slots", "Add/Remove Formation Slots", Schematyc::CArray<SSlotParams>());
		}

		IFormationColumn* pColumn = nullptr;
		Schematyc::CArray<SSlotParams> slotParams;
	};

	struct SFormationParams
	{
		inline bool operator==(const SFormationParams& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SFormationParams& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }
		
		void Serialize(Serialization::IArchive& archive)
		{
			if (archive.isOutput())
			{
				Schematyc::CArray<SColumnParams> columnParamsTemp;
				for (uint columnIndex = 0; columnIndex < columnParams.Size(); ++columnIndex)
				{
					if (columnParams.At(columnIndex).slotParams.Size() != 0)
					{
						columnParamsTemp.PushBack(columnParams.At(columnIndex));
					}
				}
				columnParams = columnParamsTemp;
			}
			archive(columnParams, "Columns", "Columns");
		}

		static void ReflectType(Schematyc::CTypeDesc<SFormationParams>& desc)
		{
			desc.SetGUID("{65D90924-959D-4C76-85C8-BDF00FEF72D8}"_cry_guid);
			desc.SetLabel("Columns");
			desc.AddMember(&SFormationParams::columnParams, 'cols', "Columns", "Columns", "Add/Remove Formation Columns", Schematyc::CArray<SColumnParams>());
		}

		Schematyc::CArray<SColumnParams> columnParams;
	};
public:
	CDebugBattleFormation() = default;
	virtual ~CDebugBattleFormation() = default;

	static void ReflectType(Schematyc::CTypeDesc<CDebugBattleFormation>& desc)
	{
		desc.SetGUID("{992793A8-ACDD-4E9A-BA9C-B654DA6057E4}"_cry_guid);
		desc.SetEditorCategory("Battle Formation");
		desc.SetLabel("Debug Battle Formation");
		desc.SetDescription("Create an organized unit formation.");
		desc.SetComponentFlags(EntityComponentFlags(
			{
				EEntityComponentFlags::Singleton
			}
		));
		desc.AddMember(&CDebugBattleFormation::m_formationParams, 'form', "Formation", "Formation", nullptr, SFormationParams());
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent
private:
	void UpdateFormation();
	inline void InsertNewColumnAndUnit(uint columnIndex);
	inline void InsertNewUnitInColumn(uint columnIndex, uint slotIndex);
	inline void RemoveColumn(uint columnIndex);
	inline void RemoveSlotInColumn(uint columnIndex, uint slotIndex);
private:
	IBattleFormation* m_formation = nullptr;
	SFormationParams m_formationParams;
};