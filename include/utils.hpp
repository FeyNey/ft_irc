#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <iostream>
#include <vector>
#include "RoomClass.hpp"

class Room;

bool		findOnVec(std::string toFind, std::vector<std::string> vec);
bool		findOnVec(std::string toFind, std::vector<Room*> vec);
std::string	getAndDel(std::vector<std::string> &vec);
Room*	findOnRoom(std::string toFind, std::vector<Room*> vec);
ClientSocket*	findOnClient(std::string toFind, std::vector<ClientSocket*> vec);

#endif
