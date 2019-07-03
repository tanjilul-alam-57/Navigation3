#include "CGPSSensor.h"

using namespace std;

CGPSSensor::CGPSSensor()
{
	// TODO Auto-generated constructor stub
}

CWaypoint CGPSSensor::getCurrentPosition()
{
	double latitude,longitude;
	CWaypoint currentLocation;

	cout<<"GPS Sensor"<<endl;

	cout<<" Enter Latitude: ";
//	cin>>latitude;
	cout<<"23"<<endl;
	latitude = 23;
	//Checking if user has input characters instead of number
//	if(cin.fail())
//		{
//			latitude=-999;
//		}
//	cin.clear();
//	cin.ignore();

	cout<<" Enter Longitude: ";
//	cin>>longitude;
	cout<<"12"<<endl;
	longitude = 12;
	//Checking if user has input characters instead of number
//	if(cin.fail())
//		{
//			longitude=-999;//setting invalid value to trigger error in setting CWaypoint
//		}
//	cin.clear();
//	cin.ignore();

	currentLocation.set("CurrentLocation",latitude,longitude);

	return currentLocation;
}
