
#include "CDatabase.h"
#include "CPOI.h"
#include "CRoute.h"
#include <list>

using namespace std;

CRoute::CRoute()
{
		m_pPoiDatabase = NULL;
		m_pWpDatabase = NULL;
}

CRoute::CRoute(const CRoute& origin)
{
	m_pPoiDatabase = origin.m_pPoiDatabase;
	m_pWpDatabase = origin.m_pWpDatabase;

	list<CWaypoint*>::const_iterator origin_itr = origin.m_pRoute.end();

	origin_itr = origin.m_pRoute.begin();
	for(;origin_itr!=origin.m_pRoute.end();++origin_itr)
	{
		m_pRoute.push_back(*origin_itr);
	}
}

CRoute::~CRoute()
{
}

bool CRoute::connectToPoiDatabase(CPoiDatabase* pPoiDB)
{
	if (pPoiDB != 0)
	{
		m_pPoiDatabase = pPoiDB;
		return true;
	}
	else
	{	// not required as this situation will not occur
		cout << "ERROR:POI DATABASE DOES NOT EXIST" << endl;
		return false;
	}
}

bool CRoute::connectToWaypointDatabase(CWpDatabase* pWpDB)
{
	if (pWpDB != 0)
	{
		m_pWpDatabase = pWpDB;
		return true;
	}
	else
	{	// not required as this situation will not occur
		cout << "ERROR:WP DATABASE DOES NOT EXIST" << endl;
		return false;
	}
}

bool CRoute::addWaypoint(string nameWp)
{
	if(m_pWpDatabase==NULL)
	{
		cout<<"ERROR:NOT CONNECTED TO WAYPOINTDATABASE"<<endl;
		return false;
	}

	if(m_pWpDatabase->getPointerToWaypoint(nameWp)!=NULL)
	{
		m_pRoute.push_back(m_pWpDatabase->getPointerToWaypoint(nameWp));
		return true;
	}

	cout<<"ERROR:WAYPOINT WITH NAME \""<<nameWp<<"\" NOT FOUND IN THE DATABASE"<<endl;
	return false;

}

bool CRoute::addPoi(string namePoi,string afterWp)
{
	//checking whether connected to poi database
	if(m_pPoiDatabase==NULL)
	{
		cout<<"ERROR:NOT CONNECTED TO POIDATABASE"<<endl;
		return false;
	}

	if(m_pPoiDatabase->getPointerToPoi(namePoi)==NULL)
	{
		cout<<"ERROR:POI WITH NAME "<<namePoi<<" NOT FOUND IN DATABASE"<<endl;
		return false;
	}

	list<CWaypoint*>::reverse_iterator r_itr;
	bool afterWpFound=false;

	//finding position of afterWp in the list
	for(r_itr=m_pRoute.rbegin();
			r_itr!=m_pRoute.rend();
			++r_itr)
	{
		if(!(*r_itr)->getName().compare(afterWp))
		{
			if(dynamic_cast<CPOI*>(*r_itr))
			{
				continue;
			}
			afterWpFound=true;
			break;
		}
	}

	if(!afterWpFound)
	{
		cout<<"ERROR:WAYPOINT WITH NAME \""<<afterWp<<"\" NOT FOUND IN THE ROUTE LIST"<<endl;
		return false;
	}

	m_pRoute.insert(r_itr.base(),m_pPoiDatabase->getPointerToPoi(namePoi));
	return true;
}

void CRoute::print()
{
	list<CWaypoint*>::iterator itr;

	for(itr=m_pRoute.begin();itr!=m_pRoute.end();++itr)
	{
		if(dynamic_cast<CPOI*>(*itr))
		{
			cout<<*((CPOI*)(*itr))<<endl;
		}
		else
		{
			cout<<*(*itr)<<endl;
		}
	}

	/*
	 * As the compiler does not provide a V-table entry for the overloaded operator<< all objects
	 * in the list are considered to be CWaypoint objects. thus the overloaded operator<< of
	 * CWaypoint is been called.
	 *
	 * whereas when we the print() method, then the appropriate print method are called from the
	 * v table.
	 *
	 * so when overloaded operator<< is being used to print the list, we will need to use dynamic casting
	 * to check the type of the object and then cast to the appropriate type before printing.
	 *
	 */
}

CRoute& CRoute::operator =(const CRoute& origin)
{
	m_pPoiDatabase = origin.m_pPoiDatabase;
	m_pWpDatabase = origin.m_pWpDatabase;

	m_pRoute.clear();

	list<CWaypoint*>::const_iterator origin_itr;

	for(origin_itr=origin.m_pRoute.begin();
			origin_itr!=origin.m_pRoute.end();
			++origin_itr)
	{
		m_pRoute.push_back(*origin_itr);
	}
	return *this;
}

CRoute& CRoute::operator +=(const std::string& name)
{
	bool poiOrWpadded = false;
//	bool waypointExist=false;
	bool databaseNotConnected = false;

	if(m_pWpDatabase!=NULL)
	{
		if(m_pWpDatabase->getPointerToWaypoint(name)!=NULL)
		{
			m_pRoute.push_back(m_pWpDatabase->getPointerToWaypoint(name));
			poiOrWpadded = true;
		}

	}
	else
	{
		databaseNotConnected = true;
		cout<<"ERROR:NOT CONNECTED TO WP DATABASE"<<endl;
	}

	if(m_pRoute.size()==0)
	{
		cout<<"ERROR:ADD WAYPOINT BEFORE ADDING POI TO THE ROUTE"<<endl;
		return *this;
	}
//	//Checking if the Route is Empty
//	if(!poiOrWpadded)
//	{
//		if(m_pRoute.size()!=0)
//		{
//			list<CWaypoint*>::const_iterator itr;
//
//			for(itr=m_pRoute.begin();itr!=m_pRoute.end();++itr)
//			{
//				if(dynamic_cast<CPOI*>(*itr))
//				{
//					continue;
//				}
//				waypointExist=true;
//				break;
//			}
//		}
//
//		if(!waypointExist)
//		{
//			cout<<"ERROR:ADD WAYPOINT BEFORE ADDING POI TO THE ROUTE"<<endl;
//			return *this;
//		}
//	}


	if(m_pPoiDatabase!=NULL)
	{
		if(m_pPoiDatabase->getPointerToPoi(name)!=NULL)
		{
			m_pRoute.push_back(m_pPoiDatabase->getPointerToPoi(name));
			poiOrWpadded = true;
		}
	}
	else
	{
		databaseNotConnected = true;
		cout<<"ERROR:NOT CONNECTED TO POI DATABASE"<<endl;
	}

	if(!poiOrWpadded && !databaseNotConnected)
	{
		cout<<"ERROR:\""<<name<<"\" COULD NOT BE FOUND IN EITHER DATABASES"<<endl;
	}

	return *this;
}

CRoute CRoute::operator +(const CRoute& route2)
{
	list<CWaypoint*>::iterator route1_itr;
	list<CWaypoint*>::const_iterator route2_itr;

	CRoute route;
	if((m_pPoiDatabase==route2.m_pPoiDatabase)
		&&(m_pWpDatabase==route2.m_pWpDatabase))
	{

		route.m_pPoiDatabase = m_pPoiDatabase;
		route.m_pWpDatabase = m_pWpDatabase;

		for(route1_itr=m_pRoute.begin();
				route1_itr!=m_pRoute.end();
				++route1_itr)
		{
			route.m_pRoute.push_back(*route1_itr);
		}

		for(route2_itr=route2.m_pRoute.begin();
				route2_itr!=route2.m_pRoute.end();
				++route2_itr)
		{
			route.m_pRoute.push_back(*route2_itr);
		}

		return route;
	}

	cout<<"ERROR:Cannot concanate"<<endl;
	route.m_pRoute.clear();
	return route;
}

const vector<const CWaypoint*> CRoute::getRoute()
{
	vector<const CWaypoint*> route_vector;

	list<CWaypoint*>::iterator itr = m_pRoute.begin();

	for(;itr!=m_pRoute.end();++itr)
	{
		route_vector.push_back(*itr);
	}

	return route_vector;
}
