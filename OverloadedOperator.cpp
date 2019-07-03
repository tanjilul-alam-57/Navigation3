

#include "OverloadedOperator.h"
#include <iostream>
#include<stdlib.h>
using namespace std;

ostream& operator<<(ostream &out, CWaypoint wp)
{
	int deg,mm; double ss;

	wp.transformLatitude2degmmss(deg, mm, ss);
	out << wp.getName() << " on Latitude = " << deg << "deg " << mm << "min " << ss
			<< "s ";
	wp.transformLongitude2degmmss(deg, mm, ss);
	out << "and Longitude = " << deg << "deg " << mm << "min " << ss << "s";

	return out;
}

std::ostream& operator<<(std::ostream &out, CPOI poi)
{
//	out<<"Point of Interest"<<endl;
//	out<<"=================="<<endl;
//	out<<"of type "<<t_aPoi[poi.m_type]<<" : "<<poi.m_description<<endl;
	out<<"POI :"<<t_aPoi[poi.m_type]<<", "<<"Desc:\""<<poi.m_description<<"\", ";
	out<<CWaypoint(poi.getName(),poi.getLatitude(),poi.getLongitude());

	return out;
}
