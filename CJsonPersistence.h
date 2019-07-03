
#ifndef CJSONPERSISTENCE_H_
#define CJSONPERSISTENCE_H_


#include "CPersistentStorage.h"
#include "ReadWriteUtils.h"
#include "CPOI.h"
#include "CDatabase.h"
#include <vector>


#define POSTFIX ".txt"


enum error_cjson_enum
{
	SYNTAX_ERROR,
	INVALID_OBJECT_TYPE,
	INVALID_VALUE,
	INVALID_NAME_TAG,
	OBJECT_CURRUPTION,
	INSUFFICIENT_ATTRIBUTES,
	INVALID_ATTRIBUTE,
	INVALID_LATITUDE_LONGITUDE_CJSON,
	NO_ERROR_CJSON
};
extern std::string error_cjson_types[NO_ERROR_CJSON+1];

class CJsonPersistence:public CPersistentStorage
{
private:
	std::string m_mediaName;
public:
	CJsonPersistence();
	~CJsonPersistence();
	 /**
	 * Set the name of the media to be used for persistent storage.
	 * The exact interpretation of the name depends on the implementation
	 * of the component.
	 *
	 * @param name the media to be used
	 */
	 virtual void setMediaName(std::string name);

	 /**
	 * Write the data to the persistent storage.
	 *
	 * @param waypointDb the data base with way points
	 * @param poiDb the database with points of interest
	 * @return true if the data could be saved successfully
	 */
	 bool writeData (const CWpDatabase& waypointDb,const CPoiDatabase& poiDb);

	 /**
	 * Fill the databases with the data from persistent storage. If
	 * merge mode is MERGE, the content in the persistent storage
	 * will be merged with any content already existing in the data
	 * bases. If merge mode is REPLACE, already existing content
	 * will be removed before inserting the content from the persistent
	 * storage.
	 *
	 * @param waypointDb the the data base with way points
	 * @param poiDb the database with points of interest
	 * @param mode the merge mode
	 * @return true if the data could be read successfully
	 */
	 bool readData (CWpDatabase& waypointDb, CPoiDatabase& poiDb,
	 MergeMode mode);

private:
	 /*
	  * adds the given waypoint or poi to the respective database
	  * @param attributes, the field values of the object are stored in this struct
	  * @param type, the type of the object
	  * @param the waypoint database to store the waypoints
	  * @param the poi database to store the pois
	  * @param status, the in value that keep tracks if all the required attributes for the object is attained
	  * status = 0b0000 0111 for Waypoints
	  * status = 0b0001 1111 for POIs
	  * @return returns the error enum for the error detected
	  */
	 error_cjson_enum addToDatabase(fields_struct attributes, enum_type type,
			 CWpDatabase& waypointDb, CPoiDatabase& poiDb, unsigned int status);
	 /*
	  * clear the bool values that keep track of the obtained attributes from the Json file
	  */
	 void setAttributeBool(fieldBool_struct& field,bool state);
	 /*
	  * prints the warning and errors in the given line
	  * @param lineNo the number of the line
	  * @param error, a vector containing the errors / warnings in a single line
	  * @param n, used to distinguish between error and warnings n=1 for warning & n=2 for error
	  * @param block, used to block the error/warning from being printed
	  */
	 bool handleWarningError(unsigned int lineNo,vector<error_cjson_enum> &error,unsigned int n,bool block);
	 /*
	  * the serialized database in the form of a vector is written on to the stream
	  */
	 void writeDatabsetoStream(ostream& stream, vector<string>& serializedDatabse);
	 /*
	  * the database is serialized and stored in a vector
	  */
	 std::vector<std::string> serializeDatabase(CWpDatabase wpDb, CPoiDatabase poiDb);
	 /*
	  * the Map is serialized and stored in a vector
	  */
	 template <typename T>
	 void serializeMap(vector<string>& write_vector,std::map<std::string,T>& dbMap);
	 /*
	  * the Object is serialized and stored in a vector
	  */
	 template<typename T>
	 void serializeObject(T& object,std::vector<std::string>& write_vector);

	 template<typename T>
	 std::vector<string> objectasVector(T* object);

};



#endif /* CJSONPERSISTENCE_H_ */
