
#include <numeric>

#include "BattleFormation/FormationBuilder/FormationBuilder.h"


//int CUnitFilter::GetFilterMatchScore(const IFormationUnit& unit)
//{
//	int matchScore = 0;
//
//	if (m_unitType == -1 || m_unitType == unit.UnitClassType())
//	{
//		++matchScore;
//	}
//	if (m_armorType == -1 || m_armorType == unit.ArmorClassType())
//	{
//		++matchScore;
//	}
//
//	return matchScore;
//}
//
//bool const CUnitFilter::operator==(const CUnitFilter& filter) const
//{
//	return m_unitType == filter.m_unitType && m_armorType == filter.m_armorType;
//}

//CFormationBuilder::CFormationBuilder(const Vec3& start, const Vec3& end, const std::vector<IFormationUnit*>& units)
//{
//	m_units = units;
//	m_startPoint = start;
//	m_endPoint = end;
//}
//
//float CFormationBuilder::GetGridGroupWidth() const
//{
//	if (m_buildGroupGrid.begin()->empty())
//	{
//		return 0;
//	}
//	CFormationBuildGroup* group = *m_buildGroupGrid.begin()->end();
//	return group->GetRect().x + group->GetRect().w;
//}
//
//size_t CFormationBuilder::BuildGroupCount() const
//{
//	auto accumulatorFunc = [](int accumulator, const TBuildGroupGrid::iterator::value_type& val)
//	{
//		return accumulator + val.size();
//	};
//	return std::accumulate(m_buildGroupGrid.begin(), m_buildGroupGrid.end(), 0, accumulatorFunc);
//}
//
//std::vector<CFormationBuildGroup*> CFormationBuilder::GetGroups()
//{
//	std::vector<CFormationBuildGroup*> buildGroups = std::vector<CFormationBuildGroup*>(BuildGroupCount());
//	for (TBuildGroupGrid::iterator rowItr = m_buildGroupGrid.begin(); rowItr != m_buildGroupGrid.end(); ++rowItr)
//	{
//		buildGroups.insert(buildGroups.end(), rowItr->begin(), rowItr->end());
//	}
//	return buildGroups;
//}
//
//void CFormationBuilder::InsertGroup(CFormationBuildGroup& group)
//{
//	if (m_buildGroupGrid.empty())
//	{
//		m_buildGroupGrid.begin()->push_back(&group);
//		return;
//	}
//	CFormationBuildGroup* pFrontGroup = nullptr;
//	for (TRowIterator rowItr = m_buildGroupGrid.begin(); rowItr != m_buildGroupGrid.end(); ++rowItr)
//	{
//		if (group.GetRect().x < (*rowItr->begin())->GetRect().x)
//		{
//			// insert group at beginning of row
//			rowItr->insert(rowItr->begin(), &group);
//			return;
//		}
//		else if (group.GetRect().x >= (*rowItr->end())->GetRect().x + (*rowItr->end())->GetRect().w)
//		{
//			// insert group at end of row
//			rowItr->push_back(&group);
//			return;
//		}
//
//		for (TColIterator colItr = rowItr->begin(); colItr != rowItr->end(); ++colItr)
//		{
//			CFormationBuildGroup* pGridGroup = (*colItr);
//			
//			if (WithinHorizontalRange(*pGridGroup, group.GetRect().x))
//			{
//				if (pGridGroup->GetRect().y >= group.GetRect().y)
//				{
//					// insert at position and push back all other groups within the range
//					InsertAndPushBackGridGroups(rowItr, colItr, group);
//					return;
//				}
//				else
//				{
//					pFrontGroup = pGridGroup;
//					break;
//				}
//			}
//			else if (colItr + 1 != rowItr->end() && group.GetRect().x < (*colItr + 1)->GetRect().x)
//			{
//				// insert at position (between the two groups)
//				rowItr->insert(colItr + 1, &group);
//				return;
//			}
//		}
//	}
//	// insert new row if no available insertion slot was found
//	m_buildGroupGrid.push_back({ &group });
//}
//
//bool CFormationBuilder::RemoveGroup(CFormationBuildGroup& group)
//{
//	for (TRowIterator rowItr = m_buildGroupGrid.begin(); rowItr != m_buildGroupGrid.end(); ++rowItr)
//	{
//		std::vector<CFormationBuildGroup*>::iterator groupItr = std::find(rowItr->begin(), rowItr->end(), group);
//		if (groupItr != rowItr->end())
//		{
//			rowItr->erase(groupItr);
//			return true;
//		}
//	}
//	return false;
//}
//
//bool CFormationBuilder::WithinHorizontalRange(const CFormationBuildGroup& group, float x) const
//{
//	return x >= group.GetRect().x && x < group.GetRect().x + group.GetRect().w;
//}
//
//CFormationBuilder::SGroupRowRange CFormationBuilder::GetRangeInRow(const TRowIterator& row, float start, float width) const
//{
//	TColIterator firstColItr;
//	TColIterator lastColItr = row->end();
//	for (firstColItr = row->begin(); firstColItr != row->end() && (*firstColItr)->GetRect().x < start; ++firstColItr);
//	if (firstColItr == lastColItr)
//	{
//		return SGroupRowRange
//		{
//			firstColItr,
//			lastColItr
//		};
//	}
//	for (lastColItr = firstColItr + 1; lastColItr != row->end() && (*lastColItr)->GetRect().x < start + width; ++lastColItr);
//	return SGroupRowRange
//	{
//		firstColItr,
//		lastColItr
//	};
//}
//
//void CFormationBuilder::InsertAndPushBackGridGroups(TRowIterator& row, TColIterator& col, CFormationBuildGroup& group)
//{
//	TRowIterator rowItr = m_buildGroupGrid.end() - 1;
//	SGroupRowRange range = GetRangeInRow(rowItr, (*col)->GetRect().x, (*col)->GetRect().w);
//	std::vector<CFormationBuildGroup*> groups = std::vector<CFormationBuildGroup*>(range.start, range.end);
//	if (!groups.empty())
//	{
//		m_buildGroupGrid.push_back(groups);
//	}
//	if (rowItr != row)
//	{
//		for (--rowItr; rowItr != row; --rowItr)
//		{
//			SGroupRowRange frontRange = GetRangeInRow(rowItr, (*col)->GetRect().x, (*col)->GetRect().w);
//			groups = std::vector<CFormationBuildGroup*>(frontRange.start, frontRange.end);
//			if (!groups.empty())
//			{
//				int backRangeLength = range.end - range.start;
//				int frontRangeLength = frontRange.end - frontRange.end;
//				int rangeDifference = backRangeLength - frontRangeLength;
//				if (rangeDifference != 0)
//				{
//					(rowItr + 1)->erase(range.end + 1, range.end + rangeDifference + 1);
//					size_t backRowIndexOffset = range.start - (rowItr + 1)->begin();
//					for (size_t groupIndex = 0; groupIndex < groups.size(); ++groupIndex)
//					{
//						(*(rowItr + 1))[groupIndex + backRowIndexOffset] = groups[groupIndex];
//					}
//				}
//				range = frontRange;
//			}
//		}
//	}
//	*col = &group;
//}


//CFormationBuilder::SFormationMatchResult CFormationBuilder::DivideUnitsIntoUniqueFilteredGroups(const std::vector<IFormationUnit*>& units) const
//{
//	SFormationMatchResult result = SFormationMatchResult();
//	result.uniqueFilterGroups = std::vector<SUniqueDividerFilterGroup>(BuildGroupCount());
//	result.matchlessUnits = std::vector<IFormationUnit*>();
//	std::unordered_map<CUnitFilter, int> uniqueFilterIndexMap = std::unordered_map<CUnitFilter, int>();
//
//	for (TBuildGroupGrid::const_iterator rowItr = m_buildGroupGrid.begin(); rowItr != m_buildGroupGrid.end(); ++rowItr)
//	{
//		for (CFormationBuildGroup* group : *rowItr)
//		{
//			if (uniqueFilterIndexMap.count(group->Filter().UnitFilter()) == 0)
//			{
//				SUniqueDividerFilterGroup uniqueFilterGroup = SUniqueDividerFilterGroup();
//				uniqueFilterGroup.groups.push_back(group);
//
//				uniqueFilterIndexMap.insert({ group->Filter().UnitFilter(), uniqueFilterIndexMap.size() });
//				result.uniqueFilterGroups.push_back(uniqueFilterGroup);
//			}
//			else
//			{
//				result.uniqueFilterGroups[uniqueFilterIndexMap[group->Filter().UnitFilter()]].groups.push_back(group);
//			}
//		}
//	}
//	uniqueFilterIndexMap.clear();
//	result.uniqueFilterGroups.shrink_to_fit();
//
//	for (IFormationUnit* pUnit : units)
//	{
//		SUniqueDividerFilterGroup* pMaxScoringFilterGroup = nullptr;
//		int maxScore = 0;
//		for (SUniqueDividerFilterGroup uniqueFilter : result.uniqueFilterGroups)
//		{
//			if (uniqueFilter.groups[0]->Filter().UnitFilter().GetFilterMatchScore(*pUnit) > maxScore)
//			{
//				pMaxScoringFilterGroup = &uniqueFilter;
//			}
//		}
//		if (pMaxScoringFilterGroup != nullptr)
//		{
//			pMaxScoringFilterGroup->units.push_back(pUnit);
//		}
//		else
//		{
//			result.matchlessUnits.push_back(pUnit);
//		}
//	}
//	return result;
//}
//
//void CFormationBuilder::SortGroupsByPriority(std::vector<CFormationBuildGroup*>& groups)
//{
//	auto prioritySortFunc = [](CFormationBuildGroup* left, CFormationBuildGroup* right)
//	{
//		return left->Filter().GetUnitPriority() < right->Filter().GetUnitPriority();
//	};
//	std::sort(groups.begin(), groups.end(), prioritySortFunc);
//}
//
//void CFormationBuilder::AssignUnitsToAllGroups(const std::vector<IFormationUnit*>& units)
//{
//	SFormationMatchResult formationMatchResult = DivideUnitsIntoUniqueFilteredGroups(units);
//	
//	for (std::vector<SUniqueDividerFilterGroup>::iterator uniqueGroupItr = formationMatchResult.uniqueFilterGroups.begin(); uniqueGroupItr != formationMatchResult.uniqueFilterGroups.end(); ++uniqueGroupItr)
//	{
//		SortGroupsByPriority(uniqueGroupItr->groups);
//
//		uint totalReserved = 0;
//		uint numReservedGroups = 0;
//		uint defaultSplitAmount;
//		uint remainderAmount;
//		for (std::vector<CFormationBuildGroup*>::iterator groupItr = uniqueGroupItr->groups.begin(); groupItr != uniqueGroupItr->groups.end(); ++groupItr)
//		{
//			if ((*groupItr)->Filter().HasReservedAmount())
//			{
//				totalReserved += (*groupItr)->Filter().GetReservedAmount();
//				++numReservedGroups;
//			}
//		}
//		if (numReservedGroups != 0)
//		{
//			defaultSplitAmount = uniqueGroupItr->units.size() - totalReserved / numReservedGroups;
//			remainderAmount = uniqueGroupItr->units.size() - totalReserved % numReservedGroups;
//		}
//
//		std::vector<IFormationUnit*>::iterator unitItr = uniqueGroupItr->units.begin();
//		for (std::vector<CFormationBuildGroup*>::iterator groupItr = uniqueGroupItr->groups.begin(); groupItr != uniqueGroupItr->groups.end(); ++groupItr)
//		{
//			int unitAmount = 0;
//			if ((*groupItr)->Filter().HasReservedAmount())
//			{
//				unitAmount = defaultSplitAmount;
//				if (remainderAmount != 0)
//				{
//					--remainderAmount;
//					unitAmount++;
//				}
//			}
//			else
//			{
//				unitAmount = (*groupItr)->Filter().GetReservedAmount();
//			}
//
//			if (unitAmount != 0)
//			{
//				(*groupItr)->FormationGroup().AssignUnits(std::vector<IFormationUnit*>(unitItr, unitItr + unitAmount));
//			}
//			unitItr += unitAmount;
//		}
//	}
//}

//int CFormationBuilder::FormationRayCast(const Vec3& origin, const Vec3& direction, ray_hit& hit)
//{
//	static IPhysicalEntity* pSkippedEntities = nullptr;
//	return gEnv->pPhysicalWorld->RayWorldIntersection(
//		origin, 
//		direction, 
//		ent_all, 
//		rwi_colltype_any | rwi_stop_at_pierceable, 
//		&hit, 
//		1, 
//		pSkippedEntities);
//}

