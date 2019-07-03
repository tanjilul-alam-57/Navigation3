

#include "CFileIO.h"

#include <string>
#include <sstream>
#include <stdlib.h> //atof
#include <vector>
#include "ReadWriteUtils.h"

/*
 * file extension of text file containing waypoints
 */
#define POSTFIXWP "-wp.txt"
/*
 * file extension of text file containing pois
 */
#define POSTFIXPOI "-poi.txt"


using namespace std;

CFileIO::CFileIO() { }

std::string location_type_array[2] = {"CWAYPOINT","POI"};

void CFileIO::setMediaName(std::string name)
{
	m_mediaName = name;
}

bool CFileIO::writeData(const CWpDatabase& waypointDb,
		const CPoiDatabase& poiDb)
{
	fstream m_wpfile,m_poifile;
	if(!m_wpfile.is_open())
	{
		m_wpfile.open((m_mediaName+POSTFIXWP).c_str(),fstream::out);
		m_wpfile.seekp(0, ios::beg);
	}

	if(!m_poifile.is_open())
	{
		m_poifile.open((m_mediaName+POSTFIXPOI).c_str(),fstream::out);
		m_poifile.seekp(0, ios::beg);
	}

//	if (!(m_wpfile.is_open()&&m_poifile.is_open()))
//	{
//		cout<<"ERROR: COULD NOT OPEN FILE(s)"<<endl;
//		return false;
//	}

	cout<<endl<<"Writing to File "<<m_mediaName+POSTFIXWP<<"............"<<endl<<endl;
	map<string,CWaypoint> wpMap(waypointDb.getWaypointMap());
	map<string,CWaypoint>::iterator wpItr;
	for(wpItr = wpMap.begin();wpItr!=wpMap.end();++wpItr)
	{
		string name;
		double latitude,longitude;
		wpItr->second.getAllDataByReference(name,latitude,longitude);
		m_wpfile<<name<<";"<<latitude<<";"<<longitude<<endl;
	}

	cout<<endl<<"Writing to File "<<m_mediaName+POSTFIXPOI<<"............"<<endl<<endl;
	map<string,CPOI> poiMap(poiDb.getPoiMap());
	map<string,CPOI>::iterator poiItr;
	for(poiItr = poiMap.begin();poiItr!=poiMap.end();++poiItr)
	{
		string name,desc;
		double latitude,longitude;
		t_poi type;
		poiItr->second.getAllDataByReference(name,latitude,longitude,type,desc);
		m_poifile<<t_aPoi[type]<<";"<<name<<";"<<desc<<";"<<latitude<<";"<<longitude<<endl;
	}

	m_wpfile.close();
	m_poifile.close();
	return true;
}

bool CFileIO::readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
		MergeMode mode)
{
	fstream m_stream;
	if(mode==REPLACE){
		waypointDb.clear();
		poiDb.clear();
	}
	unsigned int count=0; string fileName;
	enum_type type;
	while(count<2){
		switch (count){
		case 0:m_stream.open((m_mediaName+POSTFIXWP).c_str(),fstream::in);type=WAYPOINT; fileName = m_mediaName+POSTFIXWP; break;
		case 1:m_stream.open((m_mediaName+POSTFIXPOI).c_str(),fstream::in);type=POI; fileName = m_mediaName+POSTFIXPOI; break;
		}
		if(!m_stream.is_open()){
			cout<<"ERROR:Database file "<<fileName<<" could not be accessed"<<endl;
			return false;
		}
		m_stream.seekg(0, ios::beg);
		string fileName;
		string line;
		unsigned int lineCount=0;
		fields_struct field;
		while(m_stream){
			lineCount++;
			getline(m_stream,line);
			error_enum error  = CheckErrorsInLine(line,type,field);
			if(error==EMPTY_LINE)
			{
				line.clear();
				continue;
			}
			else if(error!=NO_ERROR)
			{
				cout<<"ERROR:"<<error_types[error]<<":in line("<<lineCount<<"):"<<line<<endl;
				line.clear();
				continue;
			}
			switch (type)
			{
			case WAYPOINT: waypointDb.addWaypoint(CWaypoint(field.name,field.latitude,field.longitude));break;
			case POI:poiDb.addPoi(CPOI(field.type,field.name,field.description,field.latitude,field.longitude));break;
			}
			line.clear();
		}
		count++;
		m_stream.close();
	}

	return true;
}

error_enum CFileIO::CheckErrorsInLine(std::string line, enum_type type, fields_struct& myType)
{
	unsigned int nameIndex,latitudeIndex,longitudeIndex,typeIndex,descriptionIndex,numberOfFields;
	if(type==POI)
	{
		typeIndex=0;
		nameIndex=1;
		descriptionIndex=2;
		latitudeIndex=3;
		longitudeIndex=4;
		numberOfFields =5;

	}
	else if(type==WAYPOINT)
	{
		nameIndex=0;
		latitudeIndex=1;
		longitudeIndex=2;
		numberOfFields =3;
	}

	line = removeSpaces(line);
	if(line.empty())
	{
		return EMPTY_LINE;
	}
	line = replaceDelimiter(line);

	if(countDelimiters(line)!=numberOfFields-1)
	{
		return INVALID_FORMAT;
	}
	vector<string> vFields(getFields(line,numberOfFields));
	if(vFields.size()!=numberOfFields)
	{
		return MISSING_FIELDS;
	}
	if(!checkName(vFields[nameIndex]))
	{
		return INVALID_NAME;
	}
	else
	{
		myType.name = vFields[nameIndex];
	}
	if(type==POI)
	{
		if(!checkEnum(vFields[typeIndex],myType.type)) //dont do this check for waypoint
		{
			return INVALID_POITYPE;
		}
	}

	if(!checkDigit(vFields[latitudeIndex]))
	{
		return INVALID_LATITUDE;
	}
	if(!checkDigit(vFields[longitudeIndex]))
	{
		return INVALID_LONGITUDE;
	}
	if(!checkCoordinates(myType.latitude =atof(vFields[latitudeIndex].c_str()),myType.longitude=atof(vFields[longitudeIndex].c_str())))
	{
		return INVALID_LATITUDE_LONGITUDE;
	}
	if(type==POI)
	{
		myType.description = vFields[descriptionIndex];
	}
	return NO_ERROR;
}

CFileIO::~CFileIO() { }

unsigned int CFileIO::countDelimiters (std::string readLine)
{
	unsigned int countDelimiter=0;
	for(unsigned int i=0;i<readLine.length();i++)
	{
		if(readLine[i]==';')
		{
			countDelimiter++;
		}
	}
	return countDelimiter;
}

bool CFileIO::checkName(std::string name)
{

	unsigned int count=0;
	for(unsigned int i=0;i<name.length();i++)
	{
		if(isdigit(name[i]) || name[i]==' ')
		{
			count++;
		}
	}

	//checking for name with only digits
	if(count==name.length())
	{
		return false;
	}

	for(unsigned int i=0;i<name.length();i++)
	{
		if(isalnum(name[i]) || name[i]==' ')
		{
			continue;
		}
		return false;
	}

	return true;
}

bool CFileIO::checkDigit(string num)
{
	unsigned int noOfDots=0;
	//Counting number of decimal points
	for(unsigned int i=0;i<num.length();i++)
	{
		if(num[i]=='.')
		{
			noOfDots++;
		}
	}
	//Checking the number of decimal points
	if(noOfDots>1)
	{
		return false;
	}
	//Checking if all the characters are numbers or dots or -/+ sign
	for(unsigned int i=0;i<num.length();i++)
	{
		if(isdigit(num[i]) || num[i]=='.' || (num[i]=='-' && i==0) || (num[i]=='+' && i==0))
		{
			continue;
		}
		return false;
	}

	return true;
}

//bool CFileIO::checkEnum(string s,t_poi& poitype)
//{
//	//Changing the characters of the string to upper case
//	for(unsigned int i=0;i<s.length();i++)
//	{
//		s[i]=toupper(s[i]);
//	}
//
//	//comparing string with predefined poi types and
//	//returning corresponding enum
//	if(s=="RESTAURANT")
//	{
//		poitype =  RESTAURANT;
//		return true;
//	}
//	else if(!s.compare("TOURISTIC"))
//	{
//		poitype =  TOURISTIC;
//		return true;
//	}
//	else if(!s.compare("GASSTATION"))
//	{
//		poitype =  GASSTATION;
//		return true;
//	}
//	else if(!s.compare("UNIVERSITY"))
//	{
//		poitype =  UNIVERSITY;
//		return true;
//	}
//	else
//	{
//		poitype =  NOTYPE;
//		return false;
//	}
//}

string CFileIO::replaceDelimiter(string const& readLine)
{
	string replacedString="";
	replacedString=readLine;
	for(unsigned int i=0;i<readLine.length();i++)
	{
		if(replacedString[i] == ',')
		{
			replacedString[i]=';';
		}
	}
	return replacedString;
}

//bool CFileIO::getWaypointFields(string& waypointLine,string& name,string& lat,string& lon)
//{
//	string waypointString[3];
//
//	istringstream wpStream (waypointLine);
//
//	getline(wpStream,waypointString[0],';');
//	getline(wpStream,waypointString[1],';');
//	getline(wpStream,waypointString[2]);
//
//	name = waypointString[0];
//	lat = waypointString[1];
//	lon = waypointString[2];
//
//	return true;
//}

vector<string> CFileIO::getFields(string readLine,unsigned int numberOfFields)
{
	vector<string> vFields;

	istringstream lineStream (readLine);

	for(unsigned int i=0;i<numberOfFields;++i)
	{
		string field;
		getline(lineStream,field,';');
		if(!(removeSpaces(field)).empty())
		{
			vFields.push_back(removeSpaces(field));
		}
	}
//	cout<<"FIELDS:"<<vFields[0]<<":"<<vFields[1]<<":"<<vFields[2]<<endl;
	return vFields;
}

//bool CFileIO::getPoiFields(string& poiLine,string& type,
//		string& name,string& description,string& lat,string& lon)
//{
//	string poiString[5];
//
//	istringstream poiStream (poiLine);
//
//	getline(poiStream,poiString[0],';');
//	getline(poiStream,poiString[1],';');
//	getline(poiStream,poiString[2],';');
//	getline(poiStream,poiString[3],';');
//	getline(poiStream,poiString[4]);
//
//	type=poiString[0];
//	name=poiString[1];
//	description=poiString[2];
//	lat=poiString[3];
//	lon=poiString[4];
//
//	return true;
//}

std::string error_types[9] = {
		"<NO ERROR>",
		"<INVALID FORMAT, CHECK NUMBER OF DELIMITERS>",
		"<MISSING FIELD(s)>",
		"<INVALID NAME>",
		"<INVALID LATITUDE>",
		"<INVALID LONGITUDE>",
		"<INVALID LATITUDE/LONGITUDE VALUE>",
		"<UNDEFINED POI TYPE>",
		"<EMPTY LINE>"
};

//string CFileIO::removeSpaces(std::string readField)
//{
//	readField.erase(0,readField.find_first_not_of(" "));
//	readField.erase(readField.find_last_not_of(" ")+1,string::npos);
//
//	return readField;
//}
