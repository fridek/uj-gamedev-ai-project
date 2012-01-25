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


#include "path.h"

Path::Path(list<Waypoint*> w, AIMap_Node *s, AIMap_Node *e)
{	
  start = s;
  end = e;
  waypoints = w;
}

void Path::draw()
{
  if(waypoints.empty()) return;
  
  Waypoint* prev = NULL;
  for (list<Waypoint*>::iterator it = waypoints.begin(); it != waypoints.end(); ++it) {
    
    if(prev) {
      al_draw_line(prev->node->position.x, prev->node->position.y, 
		  (*it)->node->position.x, (*it)->node->position.y,
		  al_map_rgb(255,0,0), 3.0f);
    }
    prev = *it;
  }  
}

slm::vec2 Path::getPosition(int fromStart)
{
  if(waypoints.empty()) return slm::vec2(0,0);
  
  Waypoint* prev = NULL;
  for (list<Waypoint*>::iterator it = waypoints.begin();; ++it) {
    if(it == waypoints.end()) return prev->node->position;
    if(--fromStart <= 0) return (*it)->node->position;
    prev = *it;
  }
}
