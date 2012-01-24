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

#include "bot.h"

Bot::Bot() {
  position.x = 20;
  position.y = 20;
  
  color = al_map_rgb(255, 0, 0);
  
  currentPath = -1;
}

Bot::Bot(slm::vec2 p):position(p) {
  color = al_map_rgb(255, 0, 0);
  
  currentPath = -1;
}

Bot::Bot(slm::vec2 p, ALLEGRO_COLOR c):position(p),color(c) {
  currentPath = -1;
}

Bot::~Bot() {}

void Bot::render() {
  al_draw_filled_circle(position.x, position.y, 20, color);
  al_draw_filled_circle(nearestNode->position.x, nearestNode->position.y, 5, al_map_rgb(160, 160, 0));
  Pathfinding::getInstance().drawPath(currentPath);
}

void Bot::setMap(AIMap* m) {
  map = m;
}

void Bot::setPosition(slm::vec2 p) {
  position = p;
}

void Bot::findNearestNode() {
    float minDist = map->size.x*map->size.y;
    
    for(int i = 0; i < map->graphNodes.size(); i++) {
      float dist = slm::distance(position, map->graphNodes[i]->position);
      if(dist < minDist) {
	minDist = dist;
	nearestNode = map->graphNodes[i];
      }      
    }
}

void Bot::updatePosition() {
  updateNearestNode();
}

void Bot::updateNearestNode() {
  float minDist = slm::distance(position, nearestNode->position);
  
  if(minDist > 2*map->distance) {
    findNearestNode();
    return;
  }
  AIMap_Node* newNearest = nearestNode;
  for(int i = 0; i < nearestNode->neighbours.size(); i++) {
    float tmpDist = slm::distance(position, nearestNode->neighbours[i]->position);
    if(tmpDist < minDist) {
      minDist = tmpDist;
      newNearest = nearestNode->neighbours[i];
    }
  }
  nearestNode = newNearest;
}