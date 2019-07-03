

#ifndef CPOIDATABASE_H_
#define CPOIDATABASE_H_

#include "CPOI.h"
#include "CDatabase.h"
#include<map>


class CPoiDatabase : public CDatabase<std::string,CPOI>
{
public:
	/*
	 * Constructor
	 */
	CPoiDatabase();
	/*
	 * adds POIs to the map m_poi
	 */
	bool addPoi(CPOI const& poi);
	/*
	 * returns the pointer to the poi object with name 'name' from the map
	 * @param name a string
	 * @return returns CPOI* of the POI object with name matching the parameter name
	 * @return returns null if the POI is not found in the map
	 */
	CPOI* getPointerToPoi(std::string name);
	/*
	 * prints all the CPOI objects in the map
	 */
	void print();
	/*
	 * empties the objects stored in the map
	 */
	void clear();
	/*
	 * overloading of operator<<
	 * @parm out of type ostream
	 * @parm poiDb of type CPoiDatabase
	 * @return returns ostream
	 */
	std::map<std::string,CPOI> getPoiMap() const;
};



#endif /* CPOIDATABASE_H_ */
