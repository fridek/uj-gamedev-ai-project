/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  <copyright holder> <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "bot.h"
#include "pathfinding_queue.cpp"
#include "waypoint.cpp"
#include "path.cpp"

class Pathfinding
{
    Pathfinding();
    vector<Path*> paths;
    AIMap *map;
    
    Path* dijkstra(AIMap_Node *start, AIMap_Node *end);
public:

    static Pathfinding& getInstance() {
	static Pathfinding instance;
	return instance;
    }
    
    void setMap(AIMap *m);
    
    int follow(Bot* following, Bot* followed);
    slm::vec2 getDirection(int pathID);
    
    void drawPath(int pathID);
};

#endif // PATHFINDING_H
