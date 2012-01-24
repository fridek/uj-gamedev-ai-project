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


#include "keyboard.h"

Keyboard::Keyboard()
{
  for(int i = 0; i < 12; i++) key[i] = false;
}

bool Keyboard::up()
{
  return key[KEY_UP];
}
bool Keyboard::down()
{
  return key[KEY_DOWN];
}
bool Keyboard::left()
{
  return key[KEY_LEFT];
}
bool Keyboard::right()
{
  return key[KEY_RIGHT];
}

bool Keyboard::anyArrow()
{
  return up() || down() || left() || right();
}

void Keyboard::downKey(int keyCode)
{
  switch(keyCode) {
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
      case ALLEGRO_KEY_1:
	key[KEY_1] = true;
	break;	
      case ALLEGRO_KEY_2:
	key[KEY_2] = true;
	break;	
      case ALLEGRO_KEY_3:
	key[KEY_3] = true;
	break;	
      case ALLEGRO_KEY_4:
	key[KEY_4] = true;
	break;	
      case ALLEGRO_KEY_5:
	key[KEY_5] = true;
	break;	
      case ALLEGRO_KEY_6:
	key[KEY_6] = true;
	break;	
      case ALLEGRO_KEY_7:
	key[KEY_7] = true;
	break;	       
      case ALLEGRO_KEY_8:
	key[KEY_8] = true;
	break;	
  }
}

void Keyboard::upKey(int keyCode)
{
  switch(keyCode) {
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
      case ALLEGRO_KEY_1:
	key[KEY_1] = false;
	break;	
      case ALLEGRO_KEY_2:
	key[KEY_2] = false;
	break;	
      case ALLEGRO_KEY_3:
	key[KEY_3] = false;
	break;	
      case ALLEGRO_KEY_4:
	key[KEY_4] = false;
	break;	
      case ALLEGRO_KEY_5:
	key[KEY_5] = false;
	break;	
      case ALLEGRO_KEY_6:
	key[KEY_6] = false;
	break;	
      case ALLEGRO_KEY_7:
	key[KEY_7] = false;
	break;	       
      case ALLEGRO_KEY_8:
	key[KEY_8] = false;
	break;	
  }
}