#include<iostream>

#include "CPOI.h"

using namespace std;

std::string t_aPoi[5]={"RESTAURANT","TOURISTIC","GASSTATION","UNIVERSITY","NOTYPE"};

CPOI::CPOI(t_poi type, string name, string description, double latitude,
		double longitude) : CWaypoint(name,latitude,longitude)
{
	m_type=type;
	m_description=description;
}

void CPOI::print(int format)
{
	cout<<"Point of Interest"<<endl;
	cout<<"=================="<<endl;
	cout<<"of type "<<t_aPoi[m_type]<<" : "<<m_description<<endl;
	CWaypoint::print(format);
}

void CPOI::getAllDataByReference(string& name, double& latitude,
		double& longitude, t_poi& type, string& description) const
{
	name = m_name;
	latitude = m_latitude;
	longitude = m_longitude;
	type = m_type;
	description = m_description;
}

t_poi CPOI::getType() const
{
	return m_type;
}

std::string CPOI::getDescription() const
{
	return m_description;
}
