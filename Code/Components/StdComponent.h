#pragma once

#include <CryEntitySystem/IEntityComponent.h>


class CStdComponent : public IEntityComponent
{
public:
	CStdComponent() = default;
	~CStdComponent() = default;

	static void ReflectType(Schematyc::CTypeDesc<CStdComponent>& desc)
	{
		desc.SetGUID("{27970F89-CF35-4757-9035-9A9A93602964}"_cry_guid);
	}


	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent
};