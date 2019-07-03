

#include "CWpDatabase.h"

#include <string>

using namespace std;

CWpDatabase::CWpDatabase()
{
}

bool CWpDatabase::addWaypoint( const CWaypoint& wp)
{
	return CDatabase<std::string,CWaypoint>::addLocation(wp);
}

CWaypoint* CWpDatabase::getPointerToWaypoint(std::string name)
{
	return CDatabase<std::string,CWaypoint>::getPointerToLocation(name);
}

void CWpDatabase::print()
{
	CDatabase<std::string,CWaypoint>::print();
}

void CWpDatabase::clear()
{
	CDatabase<std::string,CWaypoint>::clear();
}

std::map<std::string, CWaypoint> CWpDatabase::getWaypointMap() const
{
	return CDatabase<std::string,CWaypoint>::getLocationMap();
}
