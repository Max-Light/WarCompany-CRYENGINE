#pragma once

class CTroopUnit
{
public:
	CTroopUnit() = default;
	virtual ~CTroopUnit() = default;

	uint GetHealth() const { return m_health; }
	void SetHealth(uint health) { m_health = health; }
	void TakeDamage(uint damage);
	void Heal(uint health);
protected:
	uint m_health;
};