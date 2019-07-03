

#ifndef CROUTE_H_
#define CROUTE_H_

#include "CPOI.h"
#include <list>
#include <vector>
#include "CPoiDatabase.h"
#include "CWpDatabase.h"

class CRoute
{
private:
	/*
	 * list that stores the pointers to the waypoints and pois of the route
	 */
	std::list<CWaypoint*> m_pRoute;
	/*
	 * used to connect to the PoiDatabase of the CNavigationSystem
	 */
	CPoiDatabase *m_pPoiDatabase;
	/*
	 * used to connect to the WpDatabase of the CNavigationSystem
	 */
	CWpDatabase *m_pWpDatabase;

public:
	/*
	 * Constructor
	 * Initializes all the attributes
	 */
	CRoute();
	/*
	 * Copy constructor
	 */
	CRoute(CRoute const& origin);
	/*
	 * Destructor
	 */
	~CRoute();
	/*
	 * this function is used to get the address of the CPoiDatabase object
	 * in order to access the POIs stored in the database
	 * @parm pPoiDb pointer to database storing poi
	 */
	bool connectToPoiDatabase(CPoiDatabase* pPoiDB);
	/*
	 * this function is used to get the address of the CWpDatabase object
	 * in oder to access the Waypoints stored in the database
	 * @parm pPoiDb pointer to database storing poi
	 */
	bool connectToWaypointDatabase(CWpDatabase* pWpDB);
	/*
	 * add Waypoints to the list m_pRoute
	 *@parm nameWp name the waypoint to be added to the route
	 *@return true if Waypoint was successfully added to the list, false otherwise
	 */
	bool addWaypoint(std::string nameWp);
	/*
	 * insert the specific POIs, from the PoiDatabase, to the list just after the waypoint with name afterWp
	 * adds poi, only if connected to POI
	 * @parm namePoi the name of the poi to be added to the route
	 * @parm afterWp the name of the waypoint after which poi has to be added
	 */
	bool addPoi(std::string namePoi,std::string afterWp);
	/*
	 * Prints all the Waypoints & POIs available in the List
	 */
	void print();
	/*
	 * operator= overloading
	 */
	CRoute& operator=(CRoute const& origin);
	/*
	 * operator+= overloading
	 * Adds waypoint and/or poi if such an object exist with the given name
	 * if both waypoin and poi are present, the waypoint is added first and then the poi
	 * @parm name name of the poi &/ waypoint to be added to the route from the database
	 */
	CRoute& operator+=(std::string const& name);
	/*
	 * operator+ overloading
	 * combines the list of both the operand routes only if their databases are same
	 * @return null if the operand routes databases are different
	 */
	CRoute operator+(CRoute const& route2);
	/*
	 * returns the list in route, containing the pointers to the locations as a vector
	 */
	const vector<const CWaypoint*> getRoute();
};



#endif /* CROUTE_H_ */
