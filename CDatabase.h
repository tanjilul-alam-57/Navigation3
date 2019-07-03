

#ifndef CDATABASE_H_
#define CDATABASE_H_

#include <map>
#include<string>
#include <iostream>

using namespace std;

template <typename V,class T>
class CDatabase
{
private:
	std::map<V , T> m_locationMap;
public:
	/*
	 * Constructor
	 */
	CDatabase();
	/*
	 * Destructor
	 */
	~CDatabase();
	/*
	 * add location to the the database
	 * @param the location to be added to the database
	 * @return true if added to the database, false otherwise
	 */
	bool addLocation(T const& location);
	/*
	 * return the pointer to the location with the given name
	 * @param the name of the location for which the pointer is returned
	 * @return the pointer to the location
	 */
	T* getPointerToLocation(std::string name);
	/*
	 * prints all the locations in the database
	 */
	void print();
	/*
	 * clears the Database, i.e all the locations are removed from the database
	 */
	void clear();
	/*
	 *@return the map contain the locations
	 */
	std::map<V,T> getLocationMap() const;
};

template <typename V,class T>
inline CDatabase<V,T>::CDatabase()
{
}

template <typename V,class T>
inline bool CDatabase<V,T>::addLocation(T const& location)
{
	if(m_locationMap.count(location.getName())==0)
	{
		m_locationMap.insert(std::pair<V,T> (location.getName(),location));
		return true;
	}
	std::cout<<"ERROR: Location with name \""<<location.getName()<<"\" already exist in the Database"<<std::endl;
	return false;
}

template <typename V,class T>
inline T* CDatabase<V,T>::getPointerToLocation(std::string name)
{
	if(m_locationMap.count(name)==1)
	{
		return &m_locationMap[name];
	}
	return NULL;
}

template <typename V,class T>
inline void CDatabase<V,T>::print()
{
	typename std::map<V,T>::iterator itr; // important to use typename keyword
	for(itr = m_locationMap.begin();itr!=m_locationMap.end();++itr)
	{
		std::cout<<itr->second<<endl;
	}
}

template <typename V,class T>
inline void CDatabase<V,T>::clear()
{
	m_locationMap.clear();
}

template<typename V, class T>
inline CDatabase<V, T>::~CDatabase()
{
}

template <typename V,class T>
inline std::map<V, T> CDatabase<V,T>::getLocationMap() const
{
	std::map<V,T> locationMapCopy(m_locationMap);
	return locationMapCopy;
}

#endif /* CDATABASE_H_ */
