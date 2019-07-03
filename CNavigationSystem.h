#ifndef CNAVIGATIONSYSTEM_H_
#define CNAVIGATIONSYSTEM_H_

#include "CGPSSensor.h"
#include "CRoute.h"
#include "CDatabase.h"

class CNavigationSystem
{
private:
	/*
	 * give the current position of the user
	 */
	CGPSSensor m_GPSSensor;
	/*
	 * used to store the waypoints and pois of the route
	 */
	CRoute m_Route;
	/*
	 * Poi database that holds the POI objects
	 */
	CPoiDatabase m_PoiDatabase;
	/*
	 * Waypoint database that holds the Waypoint objects
	 */
	CWpDatabase m_WpDatabase;
public:
	/*
	 * Constructor
	 *
	 */
	CNavigationSystem();
	/*
	 * calls enterRoute method
	 * calls printRoute method
	 */
	void run();
private:
	/*
	 * reads poi and Wp text files and populates the m_PoiDatabase & m_WpDatabase with Poi & Waypoint objects
	 */
	void enterRoute();
	/*
	 * prints Waypoints & POIs in m_Route
	 */
	void printRoute();

};

#endif /* CNAVIGATIONSYSTEM_H_ */
