
#ifndef CWAYPOINT_H_
#define CWAYPOINT_H_

#include<iostream>

#define DEGREE 1
#define MMSS 2
#define R 6378.17
#define PI 3.14159


class CWaypoint
{
protected:
	std::string m_name;
	double m_latitude;
	double m_longitude;
	//TODO delete the below code
//	wp_type m_type;
public:

	/*
	 * Constructor
	 * Initializes all the parameters.
	 * Default values are assigned as "NoLocation",0 & 0 to name, latitude, longitude
	 */
	CWaypoint(std::string name="NoLocation", double latitude=0, double longitude=0);
	/*
	 * virtual Destructor
	 */
	virtual ~CWaypoint();
	/*
	 * set the value of m_latitude, m_longitude & name
	 * Error messages printed if the values are not within permissible range
	 * Values are set to default values in case of error
	 * @param name a string
	 * @param latitude a double
	 * @param longitude a double
	 */
	bool set(std::string name, double latitude, double longitude);
	/*
	 * Returns m_name
	 * @return the name
	 */
	std::string getName() const;
	/*
	 * Returns m_latitude
	 * @return the value of the latitude
	 */
	double getLatitude() const;
	/*
	 * Return m_longitude
	 * @return the value of the latitude
	 */
	double getLongitude() const;
	/*
	 * the method is used to get all the attribute values of the CWaypoint object
	 * the attribute values are passed by storing them in the reference variables passed in as function parameter
	 */
	void getAllDataByReference(std::string& name, double& latitude, double& longitude) const;
	/*
	 * method to calculate distance
	 * takes in another CWaypoint as parameter and returns the distance
	 */
	double calculateDistance(const CWaypoint& wp);
	/*
	 * prints the CWaypoint in Decimal Degree(when FORMAT is set as DEGREE(or 1) or Degree, Mins, Seconds (when FORMAT is set as MMSS(or 2)
	 * Default value of the parameter is set to DEGREE
	 */
	virtual void print(int=DEGREE);
	/*
	 * operator<< overloading
	 */
	friend std::ostream& operator<<(std::ostream &out, CWaypoint c);
private:
	/*
	 * converts Longitude from Decimal Degree to degree minutes seconds format
	 */
	void transformLongitude2degmmss(int& deg, int& mm, double& ss);
	/*
	 * converts Latitude from Decimal Degree to degree minutes seconds format
	 */
	void transformLatitude2degmmss(int& deg, int& mm, double& ss);
};



#endif /* CWAYPOINT_H_ */
