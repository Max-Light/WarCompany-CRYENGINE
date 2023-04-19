#pragma once

#include "IVertexPoint.h"

class CBattleLineSpline;

class CVertexPoint : public IVertexPoint
{
public:
	// relative position the vertex is pointing to on the formation
	struct SFormationRef
	{
		uint columnIndex;
		float normalizedValue;

		bool operator<(const SFormationRef& other) { return columnIndex < other.columnIndex && normalizedValue < other.normalizedValue; }
		bool operator>(const SFormationRef& other) { return columnIndex > other.columnIndex && normalizedValue > other.normalizedValue; }
		bool operator<=(const SFormationRef& other) { return columnIndex <= other.columnIndex && normalizedValue <= other.normalizedValue; }
		bool operator>=(const SFormationRef& other) { return columnIndex >= other.columnIndex && normalizedValue >= other.normalizedValue; }
	};
public:
	CVertexPoint() = default;
	virtual ~CVertexPoint() override = default;

	// IVertexPoint
	virtual void Initialize() override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	virtual Vec3 GetPosition() const override { return m_position; }
	virtual void MoveToPosition(const Vec3& pos) override;
	// ~IVertexPoint

	void SetPosition(const Vec3& pos) { m_position = pos; }

	// return the relative formation position 
	SFormationRef GetFormationReference() const { return m_formationRef; }
private:
	SFormationRef m_formationRef;
	Vec3 m_position;
};