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


#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard
{
private:
    Keyboard();
public:
    static Keyboard& getInstance() {
      static Keyboard instance;
      return instance;
    }

    bool up();
    bool down();
    bool left();
    bool right();
    bool anyArrow();
    
    void downKey(int keyCode);
    void upKey(int keyCode);
    
    // keyboard input
    enum MYKEYS {
      KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8
    };
    bool key[12];  
    
};

#endif // KEYBOARD_H
