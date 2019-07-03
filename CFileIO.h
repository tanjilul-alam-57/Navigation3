
#ifndef CFILEIO_H_
#define CFILEIO_H_

#include <fstream>
#include "CPersistentStorage.h"
#include <vector>
#include "ReadWriteUtils.h"
#include "CDatabase.h"
	/*
	 * enum containing the error types
	 */
enum error_enum
{
	NO_ERROR,
	INVALID_FORMAT,
	MISSING_FIELDS,
	INVALID_NAME,
	INVALID_LATITUDE,
	INVALID_LONGITUDE,
	INVALID_LATITUDE_LONGITUDE,
	INVALID_POITYPE,
	EMPTY_LINE
};

extern std::string error_types[9];

class CFileIO : public CPersistentStorage
{
private:
	/*
	 * the media path of the text files
	 */
	std::string m_mediaName;
public:
	/*
	 * Constructor
	 */
	CFileIO();
	/*
	 * Destructor
	 */
	~CFileIO();
	 /**
	 * Set the name of the media to be used for persistent storage.
	 * The exact interpretation of the name depends on the implementation
	 * of the component.
	 *
	 * @param name the media to be used
	 */
	void setMediaName(std::string name);
	 /**
	 * Write the data to the persistent storage.
	 *
	 * @param waypointDb the data base with way points
	 * @param poiDb the database with points of interest
	 * @return true if the data could be saved successfully
	 */
	bool writeData (const CWpDatabase& waypointDb,
						const CPoiDatabase& poiDb);
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
	bool readData (CWpDatabase& waypointDb, CPoiDatabase& poiDb,MergeMode mode);
private:
	/*
	 * count the number of delimiters(',' or ';')
	 * @parm readLine the line read for the text file
	 * @return the number of delimiters
	 */
	unsigned int countDelimiters (std::string readLine);
	/*
	 * checks the line for errors and return the error type, the attribute values are returned as reference in a struct of type fields_struct
	 * @param line, the line in which the error check need to be done
	 * @param type, the type of the object for which the values need to be derieved from the read line
	 * @param myType- of type fields_struct, the values obtained from the line are passes back as reference in a struct
	 * @return the enum of the error type detected in the line
	 */
	error_enum CheckErrorsInLine(std::string line,enum_type type,fields_struct& myType);
	/*
	 * checks if the string contains a valid number
	 * @parm num of type string
	 * @return true if its a valid number else false
	 */
	bool checkDigit (std::string num);
	/*
	 * checks if the string, name, is composed of only alphanumeric characters and spaces
	 * @parm name the name separated from the line
	 * @return true if its a valid name with alphanumeric characters and spaces, false otherwise
	 */
	bool checkName(std::string name);
	/*
	 * replaces the delimiter ',' in the read line with ';'
	 * @parm readLine the line read from the text file
	 * @return the string with the replaced delimiters
	 */
	std::string replaceDelimiter(std::string const& readLine);

	/*
	 * return the attribute values of the object from the read line
	 * @param line, the read line
	 * @param number of fields of the objects
	 */
	std::vector<std::string> getFields(std::string waypointLine,unsigned int numberOfFields);
};


#endif /* CFILEIO_H_ */
