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


#include "pathfinding_queue.h"

Pathfinding_Queue::Pathfinding_Queue()
{

}

bool Pathfinding_Queue::empty()
{
  return storage.empty();
}

void Pathfinding_Queue::insert(AIMap_Node* node)
{
  storage.push_back(node);
}

AIMap_Node* Pathfinding_Queue::extractMin(float heuristicsWeight, slm::vec2 target)
{
  if(storage.empty()) return NULL;
  
  int min = 0;
  for(int i = 1; i < storage.size(); i++) {
      if((storage[i]->cost + slm::distance(storage[i]->position, target) * heuristicsWeight)
	< storage[min]->cost + slm::distance(storage[min]->position, target) * heuristicsWeight) {
	min = i;
      }
  }
  AIMap_Node* minNode = storage[min];
  storage.erase(storage.begin() + min);
  return minNode;
}
