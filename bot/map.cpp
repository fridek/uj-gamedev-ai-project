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


#include "map.h"
#include "include/slm/quat.h"

AIMap::AIMap()
{

}

AIMap::~AIMap()
{

}

void AIMap::drawObstacle(AIGameClient_Obstacle obstacle) {
/*
 float v[obstacle.vertex.size()*2];
 for(int i = 0; i < obstacle.vertex.size(); i++) {
   v[2*i] = obstacle.vertex[i].first;
   v[2*i+1] = obstacle.vertex[i].second;
 }
 al_draw_filled_polygon(v, obstacle.vertex.size(), al_map_rgb(50, 50, 50));
*/
  ALLEGRO_VERTEX v[obstacle.vertex.size()];
  for(int i = 0; i < obstacle.vertex.size(); i++) {
    ALLEGRO_VERTEX tmp = {obstacle.vertex[i].first, obstacle.vertex[i].second, 0, 0, 0, al_map_rgb(0,0,0)};
    v[i] = tmp;
  }
  al_draw_prim(v, NULL, 0, 0, obstacle.vertex.size(), ALLEGRO_PRIM_LINE_LOOP);
};

void AIMap::draw() {
  for(int i = 0; i < map_obstacles.size(); i++) {
    drawObstacle(map_obstacles[i]);
  }
  
  for(int i = 0; i < graphNodes.size(); i++) {
    al_draw_circle(graphNodes[i].x, graphNodes[i].y, 1, al_map_rgb(0,255,0), 1);
  }
}

/**
 * @TODO: liczyc przeciecia odcinkow z kolkiem o promieniu aktora i z odcinkami miedzy nodami
 */
bool AIMap::insideObstacle(float x, float y) {
  bool ret2 = false;
  for(int i = 0; i < map_obstacles.size(); i++) {
    bool ret = true;
    std::vector<std::pair<float, float> > vertices = map_obstacles[i].vertex;
      
      slm::vec2 v01(vertices.back().first - vertices.front().first,vertices.back().second - vertices.front().second);
      slm::vec2 v02(vertices.back().first - x,vertices.back().second - y);
      
      if(slm::dot(v01,v02) < 0) ret = false;   
      
      for(int j = 1; j < vertices.size(); j++) {
	slm::vec2 v1(vertices[j-1].first - vertices[j].first,vertices[j-1].second - vertices[j].second);
	slm::vec2 v2(vertices[j-1].first - x,vertices[j-1].second - y);
	
	if(slm::dot(v1,v2) < 0) ret = false;
      }
      if(ret) ret2 = true;
    }

  return ret2;
}

/**
 * @TODO: change to flooding
 */
void AIMap::createGraph(int distance)
{
  for(float x = 0; x <= size.x; x+=distance) {
    for(float y = 0; y <= size.y; y+=distance) {
      if(!insideObstacle(x,y)) {
	graphNodes.push_back(slm::vec2(x,y));
	// cout << "added node (" << x << "," << y << ")" << endl;
      }
    }
  }
}

