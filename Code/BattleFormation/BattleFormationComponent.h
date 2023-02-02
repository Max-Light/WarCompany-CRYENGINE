#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <BattleFormation/BlockFormation.h>


class CBattleFormationComponent : public IEntityComponent
{
public:
	CBattleFormationComponent() = default;
	~CBattleFormationComponent() = default;

	static void ReflectType(Schematyc::CTypeDesc<CBattleFormationComponent>& desc)
	{
		desc.SetGUID("{1945996D-7839-483E-8A81-9C302D0A7371}"_cry_guid);
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent
private:
	CBlockFormation blockFormation = CBlockFormation();
};