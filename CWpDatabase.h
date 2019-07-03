


#ifndef CWPDATABASE_H_
#define CWPDATABASE_H_

#include "CWaypoint.h"
#include "CDatabase.h"
#include <map>

class CWpDatabase : public CDatabase<std::string,CWaypoint>
{

public:
	/*
	 * Constructor
	 */
	CWpDatabase();
	/*
	 * adds POIs to the map m_waypoint
	 * @parm wp the waypoint that is needed to be added to the database
	 */
	bool addWaypoint( const CWaypoint& wp);
	/*
	 * returns the pointer to the Waypoint object with name 'name' from the map
	 * @param name name of the waypoint
	 * @return returns CWaypoint* of the Waypoint object with name matching the parameter name
	 * @return returns null if the POI is not found in the map
	 */
	CWaypoint* getPointerToWaypoint(std::string name);
	/*
	 * prints all the CWaypoint objects in the map
	 */
	void print();
	/*
	 * empties the objects stored in the map
	 */
	void clear();
	/*
	 * overloading of operator<<
	 * @parm out of type ostream
	 * @parm wpDb the waypoint database
	 * @return returns ostream
	 */
	std::map<std::string,CWaypoint> getWaypointMap() const;
	friend std::ostream& operator <<(std::ostream& out, CWpDatabase wpDb);
};

#endif /* CWPDATABASE_H_ */
