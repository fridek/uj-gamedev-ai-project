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

#include "rapidjson/document.h"
#include "aigameclient.h"

void AIGameClient::handleMessage(char* a) {
  rapidjson::Document json;
  
  if (json.ParseInsitu<0>(a).HasParseError()) {
    cout << "oopsie, JSON parsing error" << endl;
    cout << "message with error: " << a << endl;
  } else {
    string type = json["type"].GetString();
    cout << "incoming message with type: " << type << endl;
    
    if(type.compare("position") == 0) {
	const rapidjson::Value& position = json["position"];
	AIGameClient::getInstance()->recievePositionHandler(json["player_id"].GetInt(), position[rapidjson::SizeType(0)].GetDouble(), position[rapidjson::SizeType(1)].GetDouble());
    } else if(type.compare("heartbeat") == 0) {
        cout << "heartbeat form server, player_id: " << json["player_id"].GetInt() << " timestamp: " << json["timestamp"].GetInt64() << endl; 
    } else if(type.compare("init_map") == 0) {
	const rapidjson::Value& position = json["position"];
	const rapidjson::Value& map_size = json["map_size"];
	const rapidjson::Value& ammo = json["ammo"];
	const rapidjson::Value& map = json["map"];

	
	vector<AIGameClient_Obstacle> obstacles;
	
	for (rapidjson::SizeType i = 0; i < map.Size(); i++) {
	    const rapidjson::Value& obstacle = map[rapidjson::SizeType(i)];

	    AIGameClient_Obstacle obst;
	    obst.type = obstacle["type"].GetString();
	    for (rapidjson::SizeType j = 0; j < obstacle["vertices"].Size(); j++) {
	      const rapidjson::Value& vertex = obstacle["vertices"][rapidjson::SizeType(j)];
	      obst.vertex.push_back(std::pair<float,float>(
		vertex[rapidjson::SizeType(0)].GetDouble(),
		vertex[rapidjson::SizeType(1)].GetDouble()
	      ));
	    }
	    obstacles.push_back(obst);
	}
	
	AIGameClient::getInstance()->recieveInitMap(
	  map_size[rapidjson::SizeType(0)].GetInt(), map_size[rapidjson::SizeType(1)].GetInt(),
	  obstacles,
	  position[rapidjson::SizeType(0)].GetDouble(), position[rapidjson::SizeType(1)].GetDouble(),
	  ammo[rapidjson::SizeType(0)].GetInt(), ammo[rapidjson::SizeType(1)].GetInt(),
	  json["current_timestamp"].GetInt64(),
	  json["start_timestamp"].GetInt64()
	);
   
    } else if(type.compare("collectable") == 0) {
	AIGameClient::getInstance()->recieveCollectableHandler(json["subtype"].GetInt(), json["amount"].GetInt(), json["x"].GetInt(), json["y"].GetInt());
    }
  }
};

void AIGameClient::registerInitMapHandler(handlerInitMapType func) {
    recieveInitMap = func;
}

void AIGameClient::registerRecievePositionHandler(handlerRecievePositionType func) {
    recievePositionHandler = func;
}

void AIGameClient::registerRecieveCollectableHandler(handlerRecieveCollectableType func)
{
  recieveCollectableHandler = func;
}


AIGameClient::AIGameClient()
{
    client = new NodejsClient("127.0.0.1", "1338", "1337");
    client->registerMessageHandler(&AIGameClient::handleMessage);
    client->run();
}

AIGameClient::~AIGameClient()
{
    client->closeClient();
}

AIGameClient* AIGameClient::getInstance()
{
  static AIGameClient *instance;
  if(instance == NULL) instance = new AIGameClient();
  return instance;
}

void AIGameClient::sendPosition(float x, float y)
{
  std::ostringstream o;
  o << "{";
  o << "\"type\": \"position\",";
  o << "\"position\": [" << x << "," << y << "]";
  o << "}";
  client->sendMessage(o.str());
}

