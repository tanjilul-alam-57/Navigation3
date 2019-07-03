

#ifndef READWRITEUTILS_H_
#define READWRITEUTILS_H_

#include <string>
#include "CPOI.h"

	typedef enum {
		WAYPOINT,
		POI,
	}enum_type ;

	typedef struct
	{
		bool type;
		bool name;
		bool description;
		bool latitude;
		bool longitude;
	}fieldBool_struct;

	/*
	 * struct containing the attribute names of the CWaypoint & CPOI objects
	 */
	typedef struct
	{
		t_poi type;
		std::string name;
		std::string description;
		double latitude;
		double longitude;
		double invalid_num;
	}fields_struct;
	/*
	 * checks whether the given string is a valid poi type and converts it to enum
	 * @parm type the type of the poi separated from the read poi line
	 * @parm poitype return the poi type as refrence parameter
	 * @return false if the String does not represent a valid enum, true otherwise
	 */
	bool checkEnum(std::string type,t_poi& poitype);
	/*
	 * removes trailing and leading spaces, result passed back as reference parameter
	 * @parm readField the read line from the text file
	 */
	std::string removeSpaces(std::string readField);
	/**
	 *checks whether the coordinates are valid coordinates
	 * @parm latitiude
	 * @parm longitude
	 * @return true if the coordinates are valid
	 */
	bool checkCoordinates(double latitue,double longitude);
	/*
	 * changes the given string to uppercase
	 * @param the string to be converted to uppercase
	 * @return the string in uppercase
	 */
	std::string toUpperCase(std::string str);
	/*
	 * converts number to string
	 * @param num, the number to be converted to string
	 * @return returns the string converted from number
	 */
	std::string numberToString(double num);

#endif /* READWRITEUTILS_H_ */
