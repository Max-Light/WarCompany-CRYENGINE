#include "WeaponAttachment.h"

CWeaponAttachment::CWeaponAttachment(const SWeaponParams& params)
{
	m_minDamage = params.minDamage;
	m_maxDamage = params.maxDamage;
	m_name = params.name;
}
