#include <utils.hpp>
#include "RoomClass.hpp"

bool	findOnVec(std::string toFind, std::vector<std::string> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
		if (toFind.compare(vec[i]) == 0)
			return (true);
	return (false);
}

bool	findOnVec(std::string toFind, std::vector<Room*> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
		if (toFind.compare(vec[i]->getName()) == 0)
			return (true);
	return (false);
}

Room*	findOnRoom(std::string toFind, std::vector<Room*> vec)
{
	for (std::vector<Room*>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if ((*it)->getName() == toFind)
			return((*it));
	}
	return nullptr;
}

ClientSocket*	findOnClient(std::string toFind, std::vector<ClientSocket*> vec)
{
	for (std::vector<ClientSocket*>::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if ((*it)->getnick() == toFind)
			return((*it));
	}
	return nullptr;
}

std::string getAndDel(std::vector<std::string> &vec)
{

	std::string tmp(vec[0]);
	vec.erase(vec.begin());
	return (tmp);
}
