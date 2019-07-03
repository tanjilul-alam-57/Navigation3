#include "CWaypoint.h"

#include<string>
#include<stdlib.h>

#include<math.h>

using namespace std;

#define SHOWADDRESS 0

CWaypoint::CWaypoint(string name, double latitude, double longitude)
{
	set(name, latitude, longitude);
#if SHOWADDRESS
	cout << "Constructor: " << "\t" << "\t" << this << endl << "m_name & Addr:"
			<< "\t" << "\t" << m_name << " : " << &m_name << endl
			<< "m_latitude Addr:" << "\t" << m_latitude << " : " << &m_latitude
			<< endl << "m_longitude Addr:" << "\t" << m_longitude << " : "
			<< &m_longitude << endl;
	/*
	 * The address of the CWaypoint Object matches with address of the first
	 	 attribute defined(in my case "m_name").
	 * String occupies 2bytes of data
	 * double occupies 1byte of data
	 *
	 */
#endif
}

bool CWaypoint::set(string name, double latitude, double longitude)
{
	if ((latitude < -90 || latitude > 90) || (longitude < -180
			|| longitude > 180))
	{
		m_name = "NoLocation";
		m_latitude = 0;
		m_longitude = 0;
		return false;
	}
		m_name = name;
		m_latitude = latitude;
		m_longitude = longitude;
		return true;
}

string CWaypoint::getName() const
{
	return m_name;
}

double CWaypoint::getLatitude() const
{
	return m_latitude;
}

double CWaypoint::getLongitude() const
{
	return m_longitude;
}

void CWaypoint::getAllDataByReference(string& name, double& latitude,
		double& longitude) const
{
	name = m_name;
	latitude = m_latitude;
	longitude = m_longitude;

#if SHOWADDRESS
	cout<<"Parameter Addr.:"<<"\t"<<&name<<"\t"<<&latitude<<"\t"<<&longitude<<endl;
	cout<<"Parameter Values :"<<"\t"<<name<<"\t"<<"\t"<<latitude<<"\t"<<"\t"<<longitude<<"\t"<<endl;
#endif
}



double CWaypoint::calculateDistance(const CWaypoint& wp)
{
	double dist = 0;
	dist = R* acos(sin(m_latitude * PI /180) * sin(wp.m_latitude * PI / 180)
			+cos(m_latitude * PI / 180)* cos(wp.m_latitude * PI / 180)
			* cos(wp.m_longitude * PI / 180- m_longitude * PI / 180));
	return dist;
}

void CWaypoint::print(int format)
{
	int deg, mm;
	double ss;
	switch (format)
	{
	//DEGREE
	case 1:
	{
		cout << m_name << " on Latitude = " << m_latitude << " and Longitude = "
				<< m_longitude << endl;
		break;
	}
	//MMSS
	case 2:
	{
		transformLatitude2degmmss(deg, mm, ss);
		cout << m_name << " on Latitude " << deg << "deg " << mm << "min " << ss
				<< "s ";
		transformLongitude2degmmss(deg, mm, ss);
		cout << "and Longitude = " << deg << "deg " << mm << "min " << ss << "s"
				<< endl;
		break;
	}
	}
}

void CWaypoint::transformLongitude2degmmss(int& deg, int& mm, double& ss)
{
	deg = (int) m_longitude;
	mm = (int) (((float) m_longitude - (int) m_longitude) * 60);
	ss = (float)(((float) m_longitude - (int) m_longitude) - (float) mm / 60)
			* 3600;
}

void CWaypoint::transformLatitude2degmmss(int& deg, int& mm, double& ss)
{
	deg = (int) m_latitude;
	mm = (int) (((float) m_latitude - (int) m_latitude) * 60);
	ss = (float)(((float) m_latitude - (int) m_latitude) - (float) mm / 60)
			* 3600;
}

CWaypoint::~CWaypoint()
{
}
