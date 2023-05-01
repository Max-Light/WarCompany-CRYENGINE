#pragma once


class CAnchorPoint 
{
public:
	CAnchorPoint() = default;
	virtual ~CAnchorPoint() = default;

	Vec3 GetPosition() const { return m_position; }
	void SetPos(const Vec3& position) { m_position = position; }
private:
	Vec3 m_position;
};