#pragma once

#include "ITroopWeaponAttachment.h"

struct SWeaponParams
{
	uint minDamage = 0;
	uint maxDamage = 0;
	string name = "";
};

class CWeaponAttachment : public ITroopWeaponAttachment
{
public:
	CWeaponAttachment(const SWeaponParams& params);
	virtual ~CWeaponAttachment() = default;

	// ITroopWeaponAttachment
	virtual uint GetMinDamage() override { return m_minDamage; };
	virtual uint GetMaxDamage() override { return m_maxDamage; };
	virtual string GetName() override { return m_name; };
	// ~ITroopWeaponAttachment
protected:
	uint m_minDamage;
	uint m_maxDamage;
	string m_name;
};