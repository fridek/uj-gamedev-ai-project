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

#ifndef BOT_H
#define BOT_H

#include "renderable.cpp"

class Bot : public Renderable, public Followable
{
private:
    ALLEGRO_COLOR color;
    AIMap* map;
    
    Sterring *sterring;
    void getSterring();
    
    void init();

protected:
    bool AI;
    AIMap_Node* nearestNode;     
public:
    Followable* followed;
  
    slm::vec2 position;
    float orientation;
    slm::vec2 velocity;    
    float rotation;
  
    Bot();
    Bot(slm::vec2 p);
    Bot(slm::vec2 p, ALLEGRO_COLOR c);
    virtual ~Bot();

    AIMap_Node* getNearestNode();
    
    int currentPath;
    
    void setMap(AIMap* m);
    
    void render();
    
    void setPosition(slm::vec2 p);
    
    void setSterring(Sterring *s);
    
    void setFollow(Followable *f);
    
    void findNearestNode();
    void updatePosition(float time);
    void updateNearestNode();
};

#endif
