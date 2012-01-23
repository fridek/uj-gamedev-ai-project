#include <iostream>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <slm/vec2.h>

#include "../communication/aigameclient.cpp"
#include "map.cpp"
#include "bot.cpp"
#include "../communication/boost/concept_check.hpp"

using namespace std;

#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60

AIGameClient* client;
vector<Bot*> bots;

float randf() {
    return float(rand()%1000)/1000;
}

// keyboard input
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
bool key[4] = { false, false, false, false };

Bot *player, *opponent;
AIMap AImap;
bool doexit, redraw;


void recievePosition(int player_id, float x, float y) {
    opponent->setPosition(slm::vec2(x,y));
    cout << "got a position of player " << player_id << ": [" << x << "," << y << "]" << endl;
    redraw = true;
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
}

int main(int argc, char **argv){

   client = AIGameClient::getInstance();
   client->registerRecievePositionHandler(&recievePosition);
   client->registerInitMapHandler(&recieveMap);
  
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
   
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   
   al_start_timer(timer);
   
   slm::vec2 p(20,20);    
   
   player = new Bot(p, al_map_rgb(0, 0, 255));
   bots.push_back(player);
   
   opponent = new Bot(p, al_map_rgb(255, 0, 0));
   bots.push_back(opponent);
   doexit = false, redraw = true;
   while(!doexit) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if(key[KEY_UP] && p.y >= 21.0) 		p.y -= 1.0;
         if(key[KEY_DOWN] && p.y <= SCREEN_H - 20.0) 	p.y += 1.0;
         if(key[KEY_LEFT] && p.x >= 21.0) 		p.x -= 1.0;
         if(key[KEY_RIGHT] && p.x <= SCREEN_W - 20.0) 	p.x += 1.0;
	 if(key[KEY_DOWN] || key[KEY_UP] || key[KEY_LEFT] || key[KEY_RIGHT]) {
	  bots[0]->setPosition(p);
	  client->sendPosition(p.x, p.y);
	  redraw = true;
	 }
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {break;}
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
             case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;
             case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;
             case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = true;
               break;
             case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
               break;
         }
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = false;
               break;
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;
            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }
 
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
 
	 al_clear_to_color(al_map_rgb(255,255,255));
	
	 for(int i = 0; i < bots.size(); i++) bots[i]->render();
	 
	 AImap.draw();
 
         al_flip_display();
      }     
   }
   
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
   return 0;
}