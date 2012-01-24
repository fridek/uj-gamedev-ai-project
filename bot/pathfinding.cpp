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


#include "pathfinding.h"
#include "../communication/boost/container/detail/tree.hpp"

Pathfinding::Pathfinding()
{

}

void Pathfinding::setMap(AIMap* m)
{
  map = m;
  
}

Path* Pathfinding::dijkstra(AIMap_Node* start, AIMap_Node* end)
{
  map->initCosts(INF);
  Pathfinding_Queue queue;

  start->cost = 0;
  queue.insert(start);

  while(!queue.empty()) {
    AIMap_Node* current = queue.extractMin(0, end->position);
    if(current->position == end->position) break;
    
    for(int i = 0; i < current->neighbours.size(); i++) {
      if(current->neighbours[i]->cost > current->cost + 1) {
	current->neighbours[i]->cost = current->cost + 1;
	current->neighbours[i]->prev = current;
	queue.insert(current->neighbours[i]);
      }
    }
  }

  Waypoint* current = new Waypoint(end);
  list<Waypoint*> reverse_path;
  
  while(true) {
    reverse_path.push_back(current);
    if(!current->node->prev) break;
    current = new Waypoint(current->node->prev);
  }
  reverse_path.reverse();
  
  Path *p = new Path(reverse_path, start, end);
  
  return p;
}

int Pathfinding::follow(Bot* following, Bot* followed)
{
  if(following->currentPath != -1 && 
    paths[following->currentPath]->start->position == following->nearestNode->position && 
    paths[following->currentPath]->end->position == followed->nearestNode->position)
    return following->currentPath;

  Path* foundPath = dijkstra(following->nearestNode, followed->nearestNode);
  paths.push_back(foundPath);
  return paths.size() - 1;
}

void Pathfinding::drawPath(int pathID)
{
  if(pathID == -1 || !paths[pathID]) return;
  paths[pathID]->draw();
}

