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

#include <slm/vec2.h>
#include "renderable.cpp"

class Bot : public Renderable
{
private:
    slm::vec2 position;
    ALLEGRO_COLOR color;
public:
    Bot();
    Bot(slm::vec2 p);
    Bot(slm::vec2 p, ALLEGRO_COLOR c);
    virtual ~Bot();
    
    void render();
    
    void setPosition(slm::vec2 p);
};

#endif
