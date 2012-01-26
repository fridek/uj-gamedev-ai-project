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


#include "collectable.h"

Collectable::Collectable(int t, int a, slm::vec2 p, AIMap* map)
{
  type = t;
  amount = a;
  position = p;

  float minDist = map->size.x*map->size.y;
  
  for(int i = 0; i < map->graphNodes.size(); i++) {
    float dist = slm::distance(position, map->graphNodes[i]->position);
    if(dist < minDist) {
      minDist = dist;
      nearestNode = map->graphNodes[i];
    }      
  }
  position = nearestNode->position;
}

void Collectable::render()
{
  al_draw_filled_circle(position.x, position.y, 20, al_map_rgb(255,0,255));
  if(type == Collectable::MEDKIT) al_draw_text(font, al_map_rgb(0, 0, 0), position.x, position.y - 30, ALLEGRO_ALIGN_CENTRE, "MEDKIT");
  if(type == Collectable::WEAPON) al_draw_text(font, al_map_rgb(0, 0, 0), position.x, position.y - 30, ALLEGRO_ALIGN_CENTRE, "WEAPON");
}

AIMap_Node* Collectable::getNearestNode()
{
  return nearestNode;
}
