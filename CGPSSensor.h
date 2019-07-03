

#ifndef CGPSSENSOR_H_
#define CGPSSENSOR_H_

#include "CWaypoint.h"

class CGPSSensor
{
public:
	/*
	 * Constructor
	 */
	CGPSSensor();
	/*
	 *gets the current position from the user
	 *@returns the current position of the user as CWaypoint
	 */
	CWaypoint getCurrentPosition();
};

#endif /* CGPSSENSOR_H_ */
