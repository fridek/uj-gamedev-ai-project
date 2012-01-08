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
}

Bot::Bot(slm::vec2 p):position(p) {
  color = al_map_rgb(255, 0, 0);
}

Bot::Bot(slm::vec2 p, ALLEGRO_COLOR c):position(p),color(c) {}

Bot::~Bot() {}

void Bot::render() {
  al_draw_filled_circle(position.x, position.y, 20, color);
}

void Bot::setPosition(slm::vec2 p) {
  position = p;
}