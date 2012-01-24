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


#ifndef MAP_H
#define MAP_H

class AIMap
{
private:
    void drawObstacle(AIGameClient_Obstacle obstacle);
    bool insideObstacle(float x, float y);
    AIMap_Node* addNode(float x, float y);
public:
    int distance;
    vector<AIMap_Node*> graphNodes;
    vector<AIGameClient_Obstacle> map_obstacles;
    slm::vec2 size;
  
    AIMap();
    virtual ~AIMap();
    void draw();
    
    void createGraph(int distance);
    void initCosts(float cost);
};

#endif // MAP_H
