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


#ifndef AIGAMECLIENT_H
#define AIGAMECLIENT_H

struct AIGameClient_Obstacle {
  std::string type;
  std::vector<std::pair<float, float> > vertex;
};

#include "NodejsClient.cpp"

typedef void (*handlerRecievePositionType)(int, float, float);
typedef void (*handlerRecieveCollectableType)(string, int, float, float);
/**
 * @param map_size_x int
 * @param map_size_y int
 * @param obstacles vector<AIGameClient_Obstacle>
 * @param player_position_x float
 * @param player_position_y float
 * @param ammo_pistol int
 * @param ammo_rpg int
 * @param current_timestamp long long
 * @param start_timestamp long long
 */
typedef void (*handlerInitMapType)(int, int, vector<AIGameClient_Obstacle>&, float, float, int, int, long long, long long);

class AIGameClient
{
private:
    NodejsClient *client;
    AIGameClient();
    virtual ~AIGameClient();
   
public:
      static void handleMessage(char* a);
      static AIGameClient* getInstance();
      
      handlerRecievePositionType recievePositionHandler;
      handlerRecieveCollectableType recieveCollectableHandler;
      handlerInitMapType recieveInitMap;
      
    void sendPosition(float x, float y);
    void registerRecievePositionHandler(handlerRecievePositionType func);
    void registerRecieveCollectableHandler(handlerRecieveCollectableType func);
    void registerInitMapHandler(handlerInitMapType func);
};

#endif // AIGAMECLIENT_H
