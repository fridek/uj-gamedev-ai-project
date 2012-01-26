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
#include "../communication/boost/concept_check.hpp"

Bot::Bot() {
  position.x = 20;
  position.y = 20;
  
  color = al_map_rgb(255, 0, 0);
  AI = true;
  init();
}

Bot::Bot(slm::vec2 p):position(p) {
  color = al_map_rgb(255, 0, 0);
  AI = true;
  init();
}

Bot::Bot(slm::vec2 p, ALLEGRO_COLOR c):position(p),color(c) {
  AI = true;
  init();
}

void Bot::init() {
  currentPath = -1;

  orientation = 0;
  velocity = slm::vec2(0,0);
  rotation = 0;
  
  sterring = new Sterring(slm::vec2(0,0), 0);
  
  health = 100;
  ammo = 50;
  
  dead = false;
}

Bot::~Bot() {}

void Bot::render() {
  ostringstream ss;
  
  if(health > 0) {
    ss << "h:" << health << " a:" << ammo;
    
    al_draw_filled_circle(position.x, position.y, 20, color);
    al_draw_filled_circle(nearestNode->position.x, nearestNode->position.y, 5, al_map_rgb(160, 160, 0));
    al_draw_text(font, al_map_rgb(0, 0, 0), position.x, position.y - 30, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());
    Pathfinding::getInstance().drawPath(currentPath);
  } else {
    ss << "DEAD";
    
    al_draw_filled_circle(position.x, position.y, 20, al_map_rgb(150,150,150));
    al_draw_text(font, al_map_rgb(0, 0, 0), position.x, position.y - 30, ALLEGRO_ALIGN_CENTRE, ss.str().c_str());    
  }
}

void Bot::setMap(AIMap* m) {
  map = m;
}

void Bot::setPosition(slm::vec2 p) {
  position = p;
}

void Bot::setSterring(Sterring *s)
{
  sterring = s;
}


void Bot::findNearestNode() {
  if(dead) return;
    float minDist = map->size.x*map->size.y;
    
    for(int i = 0; i < map->graphNodes.size(); i++) {
      float dist = slm::distance(position, map->graphNodes[i]->position);
      if(dist < minDist) {
	minDist = dist;
	nearestNode = map->graphNodes[i];
      }      
    }
}

void Bot::getSterring()
{
  sterring = Pathfinding::getInstance().getSterring(position, currentPath);
}


void Bot::updatePosition(float time) {
  if(dead) return;
  if(AI) getSterring();
  
  position += velocity * time;
  orientation += rotation * time;
  
  velocity += sterring->linear * time;
  // let player be faster
  if(AI) velocity = slm::clamp(velocity, slm::vec2(-MAX_SPEED,-MAX_SPEED), slm::vec2(MAX_SPEED,MAX_SPEED));
  else  velocity = slm::clamp(velocity, slm::vec2(-2*MAX_SPEED,-2*MAX_SPEED), slm::vec2(2*MAX_SPEED,2*MAX_SPEED));
  rotation += sterring->angular * time;
  
  if(slm::length(sterring->linear) == 0) {
    velocity *= (1.0 - FRICTION*time);
    if(slm::length(velocity) < (MAX_SPEED*0.5)) velocity = slm::vec2(0,0);
  }
  
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

AIMap_Node* Bot::getNearestNode()
{
  return nearestNode;
}


void Bot::setFollow(Followable* f)
{
  followed = f;
}
