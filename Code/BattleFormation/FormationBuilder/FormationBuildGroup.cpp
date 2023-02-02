#include "FormationBuildGroup.h"

bool CFormationBuildGroup::IntersectsWidth(float x) const
{
	return x >= m_rectangle.x && x <= m_rectangle.x + m_rectangle.w;
}
