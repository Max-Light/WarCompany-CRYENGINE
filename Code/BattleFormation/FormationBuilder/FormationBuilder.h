#pragma once

#include <StdAfx.cpp>
#include <CryPhysics/IPhysics.h>
#include <CryString/HashedString.h>

#include "BattleFormation/BlockFormation.h"


//class CFormationGroup
//{
//public:
//	CFormationGroup() = default;
//	virtual ~CFormationGroup() = default;
//
//	std::vector<IFormationUnit*> GetLine(uint line) const;
//
//	// add multiple units to the formation group
//	void AssignUnits(const std::vector<IFormationUnit*>& units);
//	// extends the first line by one unit and formats each line behind less than or equal to the first line's length
//	void ExtendLine();
//	// reduces the first line by one unit and formats each line behind less than or equal to the first line's length
//	void ReduceLine();
//protected:
//	// format all back lines to be equal or less than to the first line
//	void FormatBackLines();
//private:
//	
//	std::vector<IFormationUnit*> m_units;
//};
//
//
//class CUnitFilter
//{
//public:
//	IFormationUnit::TUnitClassType UnitType() const { return m_unitType; }
//	IFormationUnit::TArmorClassType ArmorType() const { return m_armorType; }
//	// returns the number of matches found in the filter and the formation unit
//	int GetFilterMatchScore(const IFormationUnit& unit);
//	bool const operator==(const CUnitFilter& filter) const;
//private:
//	IFormationUnit::TUnitClassType m_unitType = -1;
//	IFormationUnit::TArmorClassType m_armorType = -1;
//};
//
//namespace std
//{
//	template<>
//	struct hash<CUnitFilter>
//	{
//		size_t operator()(const CUnitFilter& key) const
//		{
//			return key.UnitType() ^ key.ArmorType();
//		}
//	};
//}
//
//
//class CGroupFilter
//{
//public:
//	CGroupFilter() = default;
//	virtual ~CGroupFilter() = default;
//
//	CUnitFilter UnitFilter() const { return m_filter; }
//	int GetUnitPriority() const { return m_unitPriority; }
//	int GetReservedAmount() const { return m_reservedUnits; }
//	bool HasReservedAmount() const { return m_reservedUnits != 0; }
//private:
//	int m_reservedUnits = 0;
//	int m_unitPriority = 0;
//	CUnitFilter m_filter = CUnitFilter();
//};
//
//
//class CUnitFormationSelector
//{
//public:
//	const std::vector<IFormationUnit*>& Units() const;
//	void SelectUnit(IFormationUnit* unit);
//	void DeselectUnit(IFormationUnit* unit);
//private:
//	std::vector<IFormationUnit*> m_units;
//};


#include "BattleFormation/FormationBuilder/FormationBuildGroup.h"


//class CFormationBuilder
//{
//protected:
//	struct SUniqueDividerFilterGroup
//	{
//		std::vector<IFormationUnit*> units;
//		std::vector<CFormationBuildGroup*> groups;
//	};
//
//	struct SFormationMatchResult
//	{
//		std::vector<SUniqueDividerFilterGroup> uniqueFilterGroups;
//		std::vector<IFormationUnit*> matchlessUnits;
//	};
//
//	using TBuildGroupGrid = typename std::vector<std::vector<CFormationBuildGroup*>>;
//	using TRowIterator = typename TBuildGroupGrid::iterator;
//	using TColIterator = typename TBuildGroupGrid::iterator::value_type::iterator;
//
//	struct SGroupRowRange
//	{
//		TColIterator start;
//		TColIterator end;
//	};
//public:
//	CFormationBuilder(const Vec3& origin, const Vec3& dir, const std::vector<IFormationUnit*>& units);
//	virtual ~CFormationBuilder() = default;
//
//	float GetGridGroupWidth() const;
//	// get total amount of groups
//	size_t BuildGroupCount() const;
//	// return all build groups in a signle vector
//	std::vector<CFormationBuildGroup*> GetGroups();
//	// insert a formation group based on its location 
//	void InsertGroup(CFormationBuildGroup& group);
//	// remove the group from the formation and push up all reinforcing groups
//	bool RemoveGroup(CFormationBuildGroup& group);
//
//protected:
//	// checks to see the group horizontal axis range is within x [start, end)
//	bool WithinHorizontalRange(const CFormationBuildGroup& group, float x) const;
//	// returns a range of view of the row within the horizontal range
//	SGroupRowRange GetRangeInRow(const TRowIterator& row, float start, float width) const;
//	// pushes all groups back a row starting from the grid origin that are within its range
//	void InsertAndPushBackGridGroups(TRowIterator& row, TColIterator& col, CFormationBuildGroup& group);
//	// assorts and maps units into groups having a uniquely common filter
//	SFormationMatchResult DivideUnitsIntoUniqueFilteredGroups(const std::vector<IFormationUnit*>& units) const;
//	// sort each group in unique groups based on priority
//	void SortGroupsByPriority(std::vector<CFormationBuildGroup*>& groups);
//	// assign units to the formation group(s)
//	void AssignUnitsToAllGroups(const std::vector<IFormationUnit*>& units);
//	// raycast down onto the physical world to locate a available position for formation units
//	int FormationRayCast(const Vec3& origin, const Vec3& direction, ray_hit& hit);
//protected:
//	TBuildGroupGrid m_buildGroupGrid;
//	Vec3 m_startPoint = ZERO;
//	Vec3 m_endPoint = ZERO;
//	std::vector<IFormationUnit*> m_units;
//};