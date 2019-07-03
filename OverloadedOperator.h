
#ifndef OVERLOADEDOPERATOR_H_
#define OVERLOADEDOPERATOR_H_

#include <iostream>
#include "CWaypoint.h"
#include "CPOI.h"

/*
 * operator << overloaded for class CWaypoint
 */
std::ostream& operator<<(std::ostream &out, CWaypoint wp);

/*
 * operator << overloaded for class CPOI
 */
std::ostream& operator<<(std::ostream &out, CPOI poi);

#endif /* OVERLOADEDOPERATOR_H_ */
