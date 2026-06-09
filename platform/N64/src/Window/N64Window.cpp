#include "Window/N64Window.h"

#include <t3d/t3d.h>

E64::N64Window::N64Window(N64DisplayMode mode, int16_t FB_COUNT){
    this->mode = mode;
    this->FB_COUNT = FB_COUNT;

    if(mode == RDP){
        display_init(RESOLUTION_640x480, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE, FILTERS_RESAMPLE);
        rdpq_init();
        t3d_init((T3DInitParams){});
        rdpq_font_t *fnt = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
        rdpq_text_register_font(1, fnt);
    }
    else{
        console_init();
        rdpq_init();
        t3d_init((T3DInitParams){});
    }
}

E64::N64Window::~N64Window(){
    if(mode == RDP)
    {
        display_close();
    }
    else {
        console_close();
    }   
}

void E64::N64Window::toggleDisplayMode(){
    if(mode == CONSOLE){
        console_close();
        display_init(RESOLUTION_640x480, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE, FILTERS_RESAMPLE);
        rdpq_init();
        rdpq_font_t *fnt = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
        rdpq_text_register_font(1, fnt);
        mode = RDP;
    }
    else if(mode == RDP){
        rdpq_close();
        console_init();
        mode = CONSOLE;
    }
}

E64::N64DisplayMode* E64::N64Window::getMode(){
    return &this->mode;
}

void E64::N64Window::setMode(N64DisplayMode mode){
    this->mode = mode;
}