#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/CoreAPI.h>

class CCompanyUnit final : public IEntityComponent
{
public:
	CCompanyUnit() = default;
	virtual ~CCompanyUnit() = default;

	static void ReflectType(Schematyc::CTypeDesc<CCompanyUnit>& desc)
	{
		desc.SetGUID("{26A8DDB5-7CEF-4984-82A0-2E1FD5AFD407}"_cry_guid);
		desc.SetEditorCategory("Company Unit");
		desc.SetLabel("Company Unit");
		desc.SetDescription("Base component for all company units.");
	}

	// IEntityComponent
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	//Vec2 GetTopDimension() const;
private:

};