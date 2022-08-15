#pragma once

#include <BattleFormation/BattleFormationSlot.h>

class CBattleLineSlot : public CBattleFormationSlot
{
public:
	friend class CBattleLineSpline;
public:
	CBattleLineSlot* GetLeftSlot() const { return m_leftSlot; }
	CBattleLineSlot* GetRightSlot() const { return m_rightSlot; }
	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);
protected:
	CBattleLineSlot* m_leftSlot = nullptr;
	CBattleLineSlot* m_rightSlot = nullptr;
	float m_radius;
};