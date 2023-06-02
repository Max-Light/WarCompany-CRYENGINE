#pragma once

#include <CryEntitySystem/IEntityComponent.h>

class CStdComponent : public IEntityComponent
{
public:
	CStdComponent() = default;
	~CStdComponent() = default;

	void Serialize(Serialization::IArchive& archive)
	{
		archive(m_nums, "Nums", "Nums");
		archive(m_single, "Single", "Single");
		archive(m_floatNum, "Single", "Single");
	}

	static void ReflectType(Schematyc::CTypeDesc<CStdComponent>& desc)
	{
		desc.SetGUID("{27970F89-CF35-4757-9035-9A9A93602964}"_cry_guid);
		desc.AddMember(&CStdComponent::m_nums, 'nums', "Nums", "Nums", nullptr, Schematyc::CArray<int>());
		desc.AddMember(&CStdComponent::m_single, 'sing', "Single", "Single", nullptr, 1);
		desc.AddMember(&CStdComponent::m_floatNum, 'flot', "FloatNum", "Float Num", nullptr, 1.5f);
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent
private:
	Schematyc::CArray<int> m_nums;
	int m_single;
	float m_floatNum;
};