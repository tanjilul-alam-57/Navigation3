

#include "CJsonScanner.h"
#include "CJsonToken.h"
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "CDatabase.h"
#include "CJsonPersistence.h"
#include "ReadWriteUtils.h"
#include "InvalidCharacterException.h"


using namespace std;
using namespace APT;


string space[3] = {"  ","    ","      "};
string tag[5] = {"name","latitude","longitude","type","description"};
string objectType[2]={"\"waypoints\"","\"pois\""};
#define BEGINOBJECT			"{"
#define BEGINARRAY			"["
#define ENDOBJECT			"}"
#define ENDARRAY			"]"
#define NAMESEPARATOR		":"
#define VALUSEPARATOR		","
#define QUOTE				"\""

typedef enum{
	IDLE,
	WAIT_FOR_DBNAME,
	GOT_DBNAME,
	WAIT_FOR_ARRAY,
	WAIT_FOR_OBJECT,
	WAIT_FOR_ATTRIBUTE,
	GOT_NAME,
	WAIT_FOR_VALUE,
	GOT_VALUE,
	WAIT_FOR_NEXT_DBNAME
}enum_state;



CJsonPersistence::CJsonPersistence(){}
CJsonPersistence::~CJsonPersistence(){}

void CJsonPersistence::setMediaName(std::string name)
{
	m_mediaName = name;
}

bool CJsonPersistence::writeData (const CWpDatabase& waypointDb,const CPoiDatabase& poiDb)
{
	fstream stream;
	stream.open((m_mediaName+POSTFIX).c_str(),fstream::out);
	if (!stream.is_open())
	{
		cout<<"ERROR: COULD NOT OPEN FILE"<<endl;
		return false;
	}
	vector<string> serializedDatabase = serializeDatabase(waypointDb,poiDb);
	writeDatabsetoStream(stream,serializedDatabase);

	return true;
}

bool CJsonPersistence::readData(CWpDatabase& waypointDb, CPoiDatabase& poiDb,
		MergeMode mode)
{
	fstream stream;
	stream.open((m_mediaName+POSTFIX).c_str(),fstream::in);
	if(!stream.is_open()){
		cout<<"ERROR:Database file "<<m_mediaName+POSTFIX<<" could not be accessed"<<endl;
		return false;
	}
	if(mode==REPLACE)
	{
		waypointDb.clear();
		poiDb.clear();
	}
	CJsonScanner scanner(stream);
	CJsonToken *currentToken;
	enum_type type;
	unsigned int status=0,status1=0;
	bool objectEnded = true,wantNumber = false,wantString = false,objectCurrupted=false,curruptionWarningIssued=false;
	enum_state state=IDLE,nextState;
	vector<error_cjson_enum> error_vector,warning_vector;
	fields_struct attributes;
	fieldBool_struct attributeBool;
	setAttributeBool(attributeBool,true);
	while(1){
		try{
			currentToken = scanner.nextToken();
			if(currentToken==0)	{ break; }
			if((*currentToken).getType()==CJsonToken::JSON_NULL) { break; }

			CJsonToken::TokenType token = (*currentToken).getType();

			switch(state)
			{
				case IDLE:{
					if(token==CJsonToken::BEGIN_OBJECT){
						nextState = WAIT_FOR_DBNAME;
						break;
					}
					else{
						error_vector.push_back(SYNTAX_ERROR);
						break;
					}
				}
				case WAIT_FOR_DBNAME:{
					if(token==CJsonToken::STRING){
						if(!(((CJsonStringToken*)currentToken)->getValue().compare("waypoints"))){
							type=WAYPOINT;
							nextState = GOT_DBNAME;
						}
						else if(!(((CJsonStringToken*)currentToken)->getValue().compare("pois"))){
							type=POI;
							nextState = GOT_DBNAME;
						}
						else{
							error_vector.push_back(INVALID_OBJECT_TYPE);
						}
					}
					else{
						error_vector.push_back(INVALID_NAME_TAG);
					}
					break;
				}
				case GOT_DBNAME:{
					if(token==CJsonToken::NAME_SEPARATOR){
						nextState = WAIT_FOR_ARRAY;
					}
					else{
						error_vector.push_back(SYNTAX_ERROR);
					}
					break;
				}
				case WAIT_FOR_ARRAY:{
					if(token==CJsonToken::BEGIN_ARRAY){
						nextState = WAIT_FOR_OBJECT;
					}
					else{
						error_vector.push_back(SYNTAX_ERROR);
					}
					break;
				}
				case WAIT_FOR_OBJECT:{
					if(status!=0){
						objectEnded=true;
						if(!objectCurrupted){
							error_cjson_enum ERROR = addToDatabase(attributes,type,waypointDb,poiDb,status);
							if(ERROR!=NO_ERROR_CJSON){
								warning_vector.push_back(ERROR);
							}
						}
						setAttributeBool(attributeBool,true);
						status=0;
					}
					if(token==CJsonToken::BEGIN_OBJECT){
						objectEnded=false;
						objectCurrupted=false;
						curruptionWarningIssued=false;
						nextState = WAIT_FOR_ATTRIBUTE;
					}
					else if(token==CJsonToken::VALUE_SEPARATOR && !objectCurrupted){
						nextState = WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::END_ARRAY && !objectCurrupted){
						nextState = WAIT_FOR_NEXT_DBNAME;
					}
					else if(token==CJsonToken::END_OBJECT && objectCurrupted){
						objectCurrupted=false;
						objectEnded=true;
						curruptionWarningIssued=false;
						nextState = WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::END_ARRAY || token==CJsonToken::END_OBJECT){
						error_vector.push_back(SYNTAX_ERROR);
					}
					else{
						if(objectCurrupted){
							warning_vector.push_back(OBJECT_CURRUPTION);
						}
						else{
							error_vector.push_back(SYNTAX_ERROR);
						}
					}
					break;
				}
				case WAIT_FOR_ATTRIBUTE:{
					if(token==CJsonToken::STRING){
						if(!(((CJsonStringToken*)currentToken)->getValue()).compare("name")){status1=1;}
						else if(!(((CJsonStringToken*)currentToken)->getValue()).compare("latitude")){status1=2;}
						else if(!(((CJsonStringToken*)currentToken)->getValue()).compare("longitude")){status1=4;}
						else if(!(((CJsonStringToken*)currentToken)->getValue()).compare("description")){status1=8;}
						else if(!(((CJsonStringToken*)currentToken)->getValue()).compare("type")){status1=16;}
						if(((status1&1)==1) || ((status1&8)==8) || ((status1&16)==16)){
							wantString=true;
						}
						else if(((status1&2)==2) || ((status1&4)==4)){
							wantNumber=true;
						}
						status=status1|status;
						if(!(wantString || wantNumber)){
							warning_vector.push_back(INVALID_ATTRIBUTE);
							objectCurrupted=true;
							break;
						}
						nextState = GOT_NAME;

					}
					else if(token==CJsonToken::VALUE_SEPARATOR){
						nextState=WAIT_FOR_ATTRIBUTE;
					}
					else if(token==CJsonToken::NAME_SEPARATOR){
						warning_vector.push_back(INVALID_NAME_TAG);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::BEGIN_OBJECT){
						if(!objectEnded){
							warning_vector.push_back(OBJECT_CURRUPTION);
							setAttributeBool(attributeBool,true); //clearing last object details
							status=0;
							objectCurrupted=false;
							objectEnded=false;
							nextState=WAIT_FOR_ATTRIBUTE;
						}
					}
					else if(token==CJsonToken::NUMBER){
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::END_OBJECT){
						nextState=WAIT_FOR_OBJECT;
					}
					break;
				}
				case GOT_NAME:{
					if(token==CJsonToken::NAME_SEPARATOR){
						nextState = WAIT_FOR_VALUE;
					}
					else if(token==CJsonToken::VALUE_SEPARATOR){
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::STRING){
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					else if(token==CJsonToken::NUMBER){
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					else{
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					break;
				}
				case WAIT_FOR_VALUE:{
					if(token==CJsonToken::STRING && wantString){
						if(((status&1)==1) && attributeBool.name){
							attributes.name = removeSpaces(((CJsonStringToken*)currentToken)->getValue());
							attributeBool.name=false;
						}
						else if(((status&8)==8) && attributeBool.description&&(type==POI)){
							attributes.description = removeSpaces(((CJsonStringToken*)currentToken)->getValue());
							attributeBool.description=false;
						}
						else if(((status&16)==16)&&attributeBool.type&&(type==POI)){
							string poiType = removeSpaces(((CJsonStringToken*)currentToken)->getValue());
							checkEnum(poiType,attributes.type);
							attributeBool.type=false;
						}
						else if(((status&1)==1) || ((status&8)==8) || ((status&16)==16)){
							warning_vector.push_back(INSUFFICIENT_ATTRIBUTES);
							objectCurrupted=true;
							nextState=WAIT_FOR_OBJECT;
							break;
						}
						else{
							warning_vector.push_back(OBJECT_CURRUPTION);
							objectCurrupted=true;
							break;
						}
						nextState = GOT_VALUE;
					}
					else if(token==CJsonToken::NUMBER && wantNumber){
						if(((status&2)==2)&&attributeBool.latitude){
							attributes.latitude = ((CJsonNumberToken*)currentToken)->getValue();
							attributeBool.latitude=false;
						}
						else if(((status&4)==4)&&attributeBool.longitude){
							attributes.longitude = ((CJsonNumberToken*)currentToken)->getValue();
							attributeBool.longitude=false;
						}
						else if(((status&2)==2) || ((status&4)==4)){
							warning_vector.push_back(INSUFFICIENT_ATTRIBUTES);
							objectCurrupted=true;
							nextState=WAIT_FOR_OBJECT;
							break;
						}
						else{
							warning_vector.push_back(OBJECT_CURRUPTION);
							objectCurrupted=true;
							break;
						}
						nextState = GOT_VALUE;
					}
					else{
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					break;
				}
				case GOT_VALUE:{
					wantNumber=false;
					wantString=false;
					if(token==CJsonToken::VALUE_SEPARATOR){
						nextState = WAIT_FOR_ATTRIBUTE;
					}
					else if(token==CJsonToken::END_OBJECT){
						nextState = WAIT_FOR_OBJECT;
					}
					else{
						warning_vector.push_back(OBJECT_CURRUPTION);
						objectCurrupted=true;
						nextState=WAIT_FOR_OBJECT;
					}
					break;
				}
				case WAIT_FOR_NEXT_DBNAME:{
					if(token==CJsonToken::VALUE_SEPARATOR){
						nextState = WAIT_FOR_DBNAME;
					}
					else if(token==CJsonToken::END_OBJECT){
						nextState=IDLE;
					}
					else{
						error_vector.push_back(SYNTAX_ERROR);
					}
					break;
				}
			} // end of switch
		}
		catch(InvalidCharacterException& e){
			cout<<e.what()<<endl;
		}
		if(!handleWarningError(scanner.scannedLine(),warning_vector,1,curruptionWarningIssued)&&objectCurrupted){
			curruptionWarningIssued=true;
		}
		if(!handleWarningError(scanner.scannedLine(),error_vector,2,false)){
			break;
		}
		state = nextState;
	}
	stream.close();
	return true;
} // end of readData

bool CJsonPersistence::handleWarningError(unsigned int lineNo,vector<error_cjson_enum> &error,unsigned int n,bool block)//1=warning,2=error
{
	string s;
	if(n==1){s="WARNING";}
	else if(n==2){s="ERROR";}
	if(error.size()!=0)
	{
		if(block)
		{
			error.clear();
			return false;
		}
		cout<<s<<"<"<<lineNo<<">";
		for (unsigned int i = 0; i < error.size(); ++i)
		{
			cout<<error_cjson_types[error[i]];

			if(i<error.size()-1)
			{
				cout<<"or";
			}
		}
		cout<<endl;
		if(n==2){cout<<"!! FILE MIGHT BE CURRUPTED !! PARSING STOPPED !!"<<endl;}
		error.clear();
		return false;
	}
	return true;
}

error_cjson_enum CJsonPersistence::addToDatabase(fields_struct attributes,enum_type type,CWpDatabase& waypointDb, CPoiDatabase& poiDb,unsigned int status)
{
	if(!checkCoordinates(attributes.latitude,attributes.longitude))
	{
		return INVALID_LATITUDE_LONGITUDE_CJSON;
	}
	if((type==WAYPOINT) && ((status&7)==7))
	{
		waypointDb.addWaypoint(CWaypoint(attributes.name,attributes.latitude,attributes.longitude));
	}
	else if((type==POI) && ((status&31)==31))
	{
		poiDb.addPoi(CPOI(attributes.type,attributes.name,attributes.description,attributes.latitude,attributes.longitude));
	}
	else
	{
		//insufficient attributes;
		return INSUFFICIENT_ATTRIBUTES;
	}
	return NO_ERROR_CJSON;
}

void CJsonPersistence::setAttributeBool(fieldBool_struct& field, bool state)
{
	field.name=state;
	field.latitude=state;
	field.longitude=state;
	field.description=state;
	field.type=state;
}

void CJsonPersistence::writeDatabsetoStream(ostream& stream,vector<string>& serializedDatabse)
{
	for(unsigned int i=0;i<serializedDatabse.size();++i)
	{
		stream<<serializedDatabse[i];
	}
}

vector<string> CJsonPersistence::serializeDatabase(CWpDatabase wpDb, CPoiDatabase poiDb)
{
	map<string,CWaypoint> wpMap = wpDb.getWaypointMap();
	map<string,CPOI> poiMap = poiDb.getPoiMap();
	vector<string> write_vector;
	unsigned int objectTypeCount = 2;
	unsigned int level = 0;
	if(wpMap.size()+poiMap.size()>0)
	{
		write_vector.push_back(BEGINOBJECT);
		write_vector.push_back("\n");
		for(unsigned int i=0;i<objectTypeCount;++i)
		{
			if(wpMap.size()==0 && i==0)
			{
				continue;
			}
			if(poiMap.size()==0 && i==1)
			{
				continue;
			}
			write_vector.push_back(space[level]);
			write_vector.push_back(objectType[i]);
			write_vector.push_back(NAMESEPARATOR);
			write_vector.push_back(BEGINARRAY);
			write_vector.push_back("\n");

			switch(i)
			{
			case 0:{serializeMap<CWaypoint>(write_vector,wpMap);break;}
			case 1:{serializeMap<CPOI>(write_vector,poiMap);break;}
			}

			write_vector.push_back(space[level]);
			write_vector.push_back(ENDARRAY);
			if(i!=objectTypeCount-1){write_vector.push_back(VALUSEPARATOR);}
			write_vector.push_back("\n");
		}
		write_vector.push_back(ENDOBJECT);
	}

	return write_vector;
}

template<typename T>
inline void CJsonPersistence::serializeMap(vector<string>& write_vector,map<string,T>& dbMap)
{
	unsigned int objectCount =(unsigned int)dbMap.size();
	typename map<string,T>::iterator itr = dbMap.begin();

	for(unsigned int i=0;i<objectCount;++i)
	{
		serializeObject<T>(itr->second,write_vector);
		if(i!=objectCount-1)
		{
			write_vector.push_back(VALUSEPARATOR);
		}
		write_vector.push_back("\n");
		++itr;
	}
}

template<typename T>
inline void CJsonPersistence::serializeObject(T& object,vector<std::string>& write_vector)
{
	unsigned int level = 1;
	vector<string> object_vector = objectasVector<T>(&object);
	write_vector.push_back(space[level]);
	write_vector.push_back(BEGINOBJECT);
	write_vector.push_back("\n");
	for(unsigned int i=0;i<object_vector.size();++i)
	{
		write_vector.push_back(space[2]);
		write_vector.push_back(QUOTE);
		write_vector.push_back(tag[i]);
		write_vector.push_back(QUOTE);
		write_vector.push_back(NAMESEPARATOR);
		if(!(i==1 || i==2)){write_vector.push_back(QUOTE);}
		write_vector.push_back(object_vector[i]);
		if(!(i==1 || i==2)){write_vector.push_back(QUOTE);}
		if(i!=object_vector.size()-1){write_vector.push_back(VALUSEPARATOR);}
		write_vector.push_back("\n");
	}
	write_vector.push_back(space[level]);
	write_vector.push_back(ENDOBJECT);
}

template<typename T>
inline std::vector<string> CJsonPersistence::objectasVector(T* object)
{
	vector<string> object_vector;
	object_vector.push_back(object->getName());
	object_vector.push_back(numberToString(object->getLatitude()));
	object_vector.push_back(numberToString(object->getLongitude()));
	if(dynamic_cast<CPOI*>(object))
	{
		object_vector.push_back(t_aPoi[((CPOI*)object)->getType()]);
		object_vector.push_back(((CPOI*)object)->getDescription());
	}
	return object_vector;
}

string error_cjson_types[NO_ERROR_CJSON+1] = {
		"<File is currupted by excess/missing/unwanted token>",
		"<Invalid Object Type>",
		"<Invalid/Missing Name tag>",
		"<Invalid/Missing value>",
		"<Syntax error,missing/unwanted token>",
		"<Excess/Missing/unwanted attribute(s)>",
		"<Undefined Attribute Tag>",
		"<Invalid Latitutde/Longitude Value(s)>",
		"<No Error>"
		};


