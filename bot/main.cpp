#include <iostream>
#include <vector>
#include <list>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <slm/vec2.h>

#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60

#define NUMBER_OF_BOTS 5
#define MAX_SPEED 20.0
#define MAX_STERRING 100.0
#define FRICTION 2.0

#define INF 4294967295

ALLEGRO_FONT *font;

#include "../communication/aigameclient.cpp"
#include "sterring.cpp"
#include "keyboard.cpp"
#include "aimap_node.cpp"

#include "followable.h"
#include "map.cpp"

#include "renderable.cpp"
#include "collectable.cpp"

#include "pathfinding.cpp"
#include "bot.cpp"
#include "player.cpp"

using namespace std;

AIGameClient* client;

float randf() {
    return float(rand()%1000)/1000;
}

vector<Bot*> bots;
vector<Bot*> AIbots;
Player *player;

vector<Collectable*> collectables;
AIMap AImap;
int pathID;
   
bool doexit, redraw, initialized;


void recievePosition(int player_id, float x, float y) {
    //opponent->setPosition(slm::vec2(x,y));
    //cout << "got a position of player " << player_id << ": [" << x << "," << y << "]" << endl;
    //redraw = true;
}

void recieveCollectable(int type, int amount, float x, float y) {
  cout << "====================================================================================================" << endl;
  cout << "collectable: " << type << "(" << amount << ") on (" << x << "," << y << ")" << endl;
  cout << "====================================================================================================" << endl;
  
  Collectable* c = new Collectable(type, amount, slm::vec2(x,y), &AImap);
  collectables.push_back(c);
}

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
void recieveMap(int map_size_x, int map_size_y, vector<AIGameClient_Obstacle> &obstacles, 
		float player_position_x, float player_position_y, int ammo_pistol, int ammo_rpg,
		long long current_timestamp, long long start_timestamp) {
  cout << "got a map: " << endl;
  cout << "size: [" << map_size_x << "," << map_size_y << "]" << endl;
  cout << "player_position: [" << player_position_x << "," << player_position_y << "]" << endl;
  cout << "ammo: pistol: " << ammo_pistol << " rpg: " << ammo_rpg << endl;
  cout << "current timestamp: " << current_timestamp << " start timestamp: " << start_timestamp << endl;
  
  AImap.size = slm::vec2(map_size_x, map_size_y);
  
  for(int i = 0; i < obstacles.size(); i++) {
      cout << "obstacle " << obstacles[i].type << ": ";
      for(int j = 0; j < obstacles[i].vertex.size(); j++) {
	  cout << "[" << obstacles[i].vertex[j].first << "," << obstacles[i].vertex[j].second << "]";
      }
      cout << endl;
  }
  
  AImap.map_obstacles = obstacles;
  AImap.createGraph(20);
  
  Pathfinding::getInstance().setMap(&AImap);
  
  player->setMap(&AImap);
  player->findNearestNode();
  
   for(int i = 0; i < NUMBER_OF_BOTS; i++) {
      slm::vec2 p(rand()%(int)AImap.size.x,rand()%(int)AImap.size.y);    
      Bot* bot = new Bot(p, al_map_rgb(0, 0, 255));
      bot->setMap(&AImap);
      bot->setFollow(player);      
      bot->findNearestNode();
      bots.push_back(bot);
      AIbots.push_back(bot);
   }  
 
  initialized = true;
}

void checkCollected(Bot* bot) {
  for(int i = 0; i < collectables.size(); i++) {
    if(collectables[i]->nearestNode->position == bot->nearestNode->position) {
      if(collectables[i]->type == Collectable::MEDKIT) {
	bot->health += collectables[i]->amount;
      } else if(collectables[i]->type == Collectable::WEAPON) {
	bot->ammo += collectables[i]->amount;
      }
      collectables.erase(collectables.begin() + i);
    }
  }
}

void chooseBehaviour(Bot* bot) {
  if(bot->health == 0) {
    //DEAD
      bot->setFollow(NULL);
      return;    
  }
  
  if(bot->health < 30) {
    for(int i = 0; i < collectables.size(); i++) {
      if(collectables[i]->type == Collectable::MEDKIT) {
	bot->setFollow(collectables[i]);
	return;
      }
    }      
  }
  
  if(bot->ammo == 0) {
    for(int i = 0; i < collectables.size(); i++) {
      if(collectables[i]->type == Collectable::WEAPON) {
	AIbots[i]->setFollow(collectables[i]);
	return;
      }
    }
  }
  
  if(slm::distance(player->position, bot->position) < 100) {
    if(bot->ammo > 0 && player->health > 0) {
      bot->ammo--;
      player->health--;
      
      bot->setFollow(NULL);
      return;
    }
    
    if(player->ammo > 0 && bot->health > 0) {
      player->ammo--;
      bot->health--;      
    }
  }
  bot->setFollow(player);  
}

int main(int argc, char **argv){
   initialized = false;
  
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   
   if(!al_init()) {
      cout << "failed to initialize allegro!" << endl;
      return -1;
   }
 
   display = al_create_display(SCREEN_W, SCREEN_H);
   timer = al_create_timer(1.0 / FPS);
   event_queue = al_create_event_queue();
   
   al_init_primitives_addon();
   al_install_keyboard();
   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon
   
   font = al_load_ttf_font("pirulen.ttf",20,0 );
   
   
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   
   al_start_timer(timer);

   player = new Player(slm::vec2(20,20));
   player->health = 500;
   player->ammo = 200;
   bots.push_back(player);    
   
   //opponent = new Bot(p, al_map_rgb(255, 0, 0));
   //bots.push_back(opponent);

   client = AIGameClient::getInstance();
   client->registerRecievePositionHandler(&recievePosition);
   client->registerRecieveCollectableHandler(&recieveCollectable);
   client->registerInitMapHandler(&recieveMap);   
   
   doexit = false, redraw = true;
   int serverCounter = 0;
   
   while(!doexit) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER && initialized) {
	 slm::vec2 p(0,0);
	 float rotation = 0;
	
         if(Keyboard::getInstance().up() && player->position.y >= 21.0) 		p.y -= MAX_STERRING;
         if(Keyboard::getInstance().down() && player->position.y <= SCREEN_H - 20.0) 	p.y += MAX_STERRING;
         if(Keyboard::getInstance().left() && player->position.x >= 21.0) 		p.x -= MAX_STERRING;
         if(Keyboard::getInstance().right() && player->position.x <= SCREEN_W - 20.0) 	p.x += MAX_STERRING;

	 if(Keyboard::getInstance().key[Keyboard::KEY_1]) {
	   for(int i = 0; i < bots.size(); i++) bots[i]->health--;
	 }
	 if(Keyboard::getInstance().key[Keyboard::KEY_2]) {
	   for(int i = 0; i < bots.size(); i++) bots[i]->ammo--;
	 }
	 
	 /*
	  * Behaviours
	  */
	 player->setSterring(new Sterring(p, rotation));
	 
	 for(int i = 0; i < AIbots.size(); i++) {
	   checkCollected(AIbots[i]);
	   chooseBehaviour(AIbots[i]);
	 }
	  for(int i = 0; i < AIbots.size(); i++) {
	    if(AIbots[i]->followed != NULL)
	      AIbots[i]->currentPath = Pathfinding::getInstance().follow(AIbots[i], AIbots[i]->followed);
	    else
	      AIbots[i]->currentPath = -1;
	  }
	 
	 for(int i = 0; i < bots.size(); i++) {
	    bots[i]->updatePosition(1.0 / FPS);
	 }

	 if(++serverCounter % FPS == 0) client->sendPosition(player->position.x, player->position.y);

	  
	 redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {break;}
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
	Keyboard::getInstance().downKey(ev.keyboard.keycode);
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
	  Keyboard::getInstance().upKey(ev.keyboard.keycode);
	  if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) doexit = true;
      }
 
      if(redraw && al_is_event_queue_empty(event_queue) && initialized) {
         redraw = false;
 
	 al_clear_to_color(al_map_rgb(255,255,255));
	 AImap.draw();
	 
	 for(int i = 0; i < bots.size(); i++) {
	   bots[i]->render();
	 }
	 
	 for(int i = 0; i < collectables.size(); i++) {
	   collectables[i]->render();
	 }
 
         al_flip_display();
      }     
   }
   
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   return 0;
}