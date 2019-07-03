

#include "CPoiDatabase.h"
#include <string>


using namespace std;


CPoiDatabase::CPoiDatabase()
{
}

bool CPoiDatabase::addPoi(CPOI const& poi)
{
	return CDatabase<std::string,CPOI>::addLocation(poi);
}

CPOI* CPoiDatabase::getPointerToPoi(string name)
{
	return CDatabase<std::string,CPOI>::getPointerToLocation(name);
}

void CPoiDatabase::print()
{
	CDatabase<std::string,CPOI>::print();
}

void CPoiDatabase::clear()
{
	CDatabase<std::string,CPOI>::clear();
}

std::map<std::string, CPOI> CPoiDatabase::getPoiMap() const
{
	return CDatabase<std::string,CPOI>::getLocationMap();
}
