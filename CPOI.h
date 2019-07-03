

#ifndef CPOI_H_
#define CPOI_H_

#include "CWaypoint.h"

typedef enum
{
	RESTAURANT,
	TOURISTIC,
	GASSTATION,
	UNIVERSITY,
	NOTYPE
}t_poi;

// Array t_aPoi prints the Type of the POI using the enum as index
extern std::string t_aPoi[5];

class CPOI : public CWaypoint
{
private:
	t_poi m_type;
	std::string m_description;
public:
	/*
	 *initializing attributes with the parameter values
	 */
	CPOI(t_poi type=NOTYPE, std::string name="NoLocation", std::string description="No_Description", double latitude=0, double longitude=0);
	/*
	 *prints the poi in the specified format
	 *@param format, either MMSS or DEGREE
	 */
	void print(int format);
	/*
	 *return the type of the poi
	 */
	t_poi getType() const;
	/*
	 * returns the description of the poi
	 */
	std::string getDescription() const;
	/*
	 * returns all the parameters as reference
	 */
	void getAllDataByReference(std::string& name, double& latitude, double& longitude,t_poi& type, std::string& description) const;
	/*
	 * operator<< overloading
	 */
	friend std::ostream& operator<<(std::ostream &out, CPOI poi);
};



#endif /* CPOI_H_ */
