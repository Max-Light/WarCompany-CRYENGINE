#pragma once

class CAnchorPoint : public IControlPoint
{
public:
	friend class CBattleLineSpline;
public:
	CAnchorPoint(CVertexPoint& vertexPoint) 
		: m_vertexPoint(&vertexPoint)
	{
	}
	~CAnchorPoint() = default;
	CVertexPoint* GetVertexPoint() const { return m_vertexPoint; }

	//////////////////////////////////////////////////////////
	// IControlPoint implementation
	virtual Vec3 GetPosition() const override { return m_position; }
	virtual void SetPosition(Vec3 position) override;
	//////////////////////////////////////////////////////////
protected:
	CVertexPoint* const m_vertexPoint = nullptr;
	Vec3 m_position;
};
