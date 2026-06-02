#include "Input/N64Input.h"

E64::N64Input::N64Input(){
    joypad_init();
    E64::Log::info("N64 JOYPAD INITAILIZED");

    if(joypad_is_connected(JOYPAD_PORT_1)){
        E64::Log::info("PORT 1 CONTROLLER CONNECTED");
    }
    if(joypad_is_connected(JOYPAD_PORT_2)){
        E64::Log::info("PORT 2 CONTROLLER CONNECTED");
    }
    if(joypad_is_connected(JOYPAD_PORT_3)){
        E64::Log::info("PORT 3 CONTROLLER CONNECTED");
    }
    if(joypad_is_connected(JOYPAD_PORT_4)){
        E64::Log::info("PORT 4 CONTROLLER CONNECTED");
    }
}

E64::N64Input::~N64Input(){
    joypad_close();
}

void E64::N64Input::poll(){
    joypad_poll();
    joypad_buttons_t button = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    if(button.a != 0){
        E64::Log::info("A Button Pressed");
        
    }
}