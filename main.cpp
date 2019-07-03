#include <QCoreApplication>
#include <iostream>		// Header f�r die Standard-IO-Objekte (z.B. cout, cin)
#include <stdlib.h>


#include "CJsonPersistence.h"
#include "CNavigationSystem.h"
#include "CFileIO.h"
#include "CRoute.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CNavigationSystem n;
    n.run();

    CPoiDatabase poiDb;
    CWpDatabase wpDb;

    cout<<"============Testing CJsonPersistent================="<<endl;
    CJsonPersistence j;
    j.setMediaName("json2");
    j.readData(wpDb,poiDb,CJsonPersistence::MERGE);
    poiDb.print();
    wpDb.print();
    j.setMediaName("json3");
    j.writeData(wpDb,poiDb);

    //testing getroute
    cout<<"============Testing getRoute()================="<<endl;
    CRoute route,route2;
    route.connectToWaypointDatabase(&wpDb);
    route.connectToPoiDatabase(&poiDb);
    route.addWaypoint("Amsterdam");
    route.addWaypoint("Berlin");
    route.addPoi("Mensa HDA","Amsterdam");
    route.addPoi("Dieburg","Amsterdam");

    route2.connectToWaypointDatabase(&wpDb);
    route2.connectToPoiDatabase(&poiDb);
    route2.addWaypoint("Amsterdam");
    route2.addWaypoint("Berlin");
    route2.addPoi("Dieburg","Amsterdam");
    route2.addPoi("Mensa HDA","Amsterdam");


    vector<const CWaypoint*> vect;
    vect = route.getRoute();

    vector<const CWaypoint*>::const_iterator itr = vect.begin();

    for(;itr!=vect.end();++itr)
    {
        if(dynamic_cast<const CPOI*>(*itr))
        {
            cout<<*((CPOI*)*itr)<<endl;
        }
        else
        {
            cout<<*(*itr)<<endl;
        }
    }

    //testing CFileIo

    cout<<"============Testing CFiloIO================="<<endl;
    CFileIO c;
    c.setMediaName("userInput");
    c.readData(wpDb,poiDb,CFileIO::REPLACE);
    wpDb.print();
    poiDb.print();
    c.setMediaName("userOutput");
    c.writeData(wpDb,poiDb);

    return a.exec();
}
