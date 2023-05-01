#pragma once

#include "BattleFormation/IFormationColumn.h"

struct IVertexPoint : public IEntityComponent
{
	static void ReflectType(Schematyc::CTypeDesc<IVertexPoint>& desc)
	{
		desc.SetGUID("{D01691A4-23BF-4B7F-B71D-FB14BFD7BE81}"_cry_guid);
	}

	virtual Vec3 GetPosition() const = 0;
	virtual IFormationColumn* GetColumn() const = 0;
	virtual void MoveToPosition(const Vec3& gridPos) = 0;
};