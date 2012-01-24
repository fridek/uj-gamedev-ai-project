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


#include "aimap_node.h"

AIMap_Node::AIMap_Node(slm::vec2 pos)
{
  position = pos;
}

bool AIMap_Node::operator==(const AIMap_Node& other) const
{
  return (position.x == other.position.x && position.y == other.position.y);
}

bool AIMap_Node::operator==(AIMap_Node* other) const
{
  return (position.x == other->position.x && position.y == other->position.y);
}

void AIMap_Node::addNeighbour(AIMap_Node* n) {
  if(!n) return;
  neighbours.push_back(n);
}