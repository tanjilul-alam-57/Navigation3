

#include "CNavigationSystem.h"

#include "CFileIO.h"
#include "CJsonPersistence.h"

using namespace std;


CNavigationSystem::CNavigationSystem()
{

}

void CNavigationSystem::run()
{
	//CONNECTING TO DATABASE
	m_Route.connectToWaypointDatabase(&m_WpDatabase);
	m_Route.connectToPoiDatabase(&m_PoiDatabase);

	enterRoute();
}

void CNavigationSystem::enterRoute()
{

#if 0
	cout<<"====================CJson Testing===================="<<endl;
	CJsonPersistence json;
	json.setMediaName("jsonInput");
	json.readData(m_WpDatabase,m_PoiDatabase,CFileIO::REPLACE);
	cout<<endl<<"================WAYPOINT DATABASE==============="<<endl;
	m_WpDatabase.print();
	cout<<endl<<"===================POI DATABASE================="<<endl;
	m_PoiDatabase.print();
	json.setMediaName("jsonOutput");
	json.writeData(m_WpDatabase,m_PoiDatabase);
#endif
	//CHECKING CONDITION WHEN DATABASE DOESNOT EXIST
//	m_Route.connectToWpDatabase(NULL);
//	m_Route.connectToPoiDatabase(NULL);

#if 0
	//Checking Read, and error handling
	CFileIO readWrite;
	readWrite.setMediaName("test");
	readWrite.readData(m_WpDatabase,m_PoiDatabase,CFileIO::REPLACE);
	cout<<endl<<"================INITIAL WAYPOINT DATABASE==============="<<endl;
	m_WpDatabase.print();
	cout<<endl<<"===================INITIAL POI DATABASE================="<<endl;
	m_PoiDatabase.print();

	cout<<endl<<"Adding POI and Waypoints to m_Route.........."<<endl<<endl;


	m_Route+="Berlin";

	m_Route+="Amsterdam";
	m_Route+="00FUA";
	m_Route.addPoi("TUD","nagaland"); // trying to add poi after a waypoint that doesnot exist in the route
	m_Route.addPoi("FUA","Berlin");

	cout<<endl<<"=========================m_Route==========================="<<endl;
	printRoute();

	readWrite.setMediaName("test2");
	readWrite.writeData(m_WpDatabase,m_PoiDatabase);
#endif

#if 0
//======================================================================================//
//						TESTING READ AND WRITE
//======================================================================================//
	CFileIO readWrite;
	//READ FROM FILE A-wp.txt & A-poi.txt
	readWrite.setMediaName("userInput");
	readWrite.readData(m_WpDatabase,m_PoiDatabase,CFileIO::REPLACE);

	cout<<endl<<"================INITIAL WAYPOINT DATABASE==============="<<endl;
	m_WpDatabase.print();
	cout<<endl<<"===================INITIAL POI DATABASE================="<<endl;
	m_PoiDatabase.print();

	//WRITE DATABASE TO FILE B-wp.txt & B-poi.txt
	readWrite.setMediaName("Database");
	readWrite.writeData(m_WpDatabase,m_PoiDatabase);

	//READ FROM FILE C-wp.txt & C-poi.txt
	readWrite.setMediaName("userInput2");
//	readWrite.readData(m_WpDatabase,m_PoiDatabase,CFileIO::REPLACE);
	readWrite.readData(m_WpDatabase,m_PoiDatabase,CFileIO::MERGE);

	cout<<endl<<"================MERGED/REPLACED WAYPOINT DATABASE==============="<<endl;
	m_WpDatabase.print();
	cout<<endl<<"===================MERGED/REPLACED POI DATABASE================="<<endl;
	m_PoiDatabase.print();

	cout<<endl<<"Adding POI and Waypoints to m_Route.........."<<endl<<endl;

	/*
//	m_Route.addWaypoint("Berlin");
//	m_Route.addWaypoint("Amsterdam");
//	m_Route.addWaypoint("Darmstadt");
//	m_Route.addWaypoint("Dieburg");
//	m_Route.addWaypoint("Tokiyo");
//
//	m_Route.addPoi("HeidelbergCastle","Berlin");
//	m_Route.addPoi("FUA","Darmstadt");
//	m_Route.addPoi("Jugendstilbad","Darmstadt");
//	m_Route.addPoi("TUD","Darmstadt");
//	m_Route.addPoi("FrankfurtZoo","Dieburg");

	*/

	m_Route+="Berlin";
	m_Route+="FUA";
	m_Route+="FrankfurtZoo"; //its a waypoint and POI
	m_Route+="Munich";  //waypoint does not exist


	cout<<endl<<"===========================m_Route LIST==========================="<<endl;
	m_Route.print();

	// TO WRITE THE CURRENT DATABASE TO THE FILE
	readWrite.setMediaName("Database");
	readWrite.writeData(m_WpDatabase,m_PoiDatabase);

#endif

}

void CNavigationSystem::printRoute()
{
	m_Route.print();
}

