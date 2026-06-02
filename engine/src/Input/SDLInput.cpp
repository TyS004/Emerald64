#include "Input/SDLInput.h"
#include "Layer/PhysicsLayer.h"

E64::SDLInput::SDLInput() { 
    buildKeyMap(); 
}

void E64::SDLInput::poll() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        for(E64::Layer* layer : E64::Layer::layers){
            layer->OnEvent(&e);
        }
        processEvent(e);
    }
}

void E64::SDLInput::processEvent(SDL_Event& e) {
    switch (e.type) {
        case SDL_EVENT_KEY_DOWN:
            OnKeyPressed(key_map[e.key.scancode]);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            xrel = e.motion.xrel;
            yrel = e.motion.yrel;
            OnMouseMove(xrel, yrel);
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            OnWindowResize(e.window.data1, e.window.data2);
            break;
        case SDL_EVENT_QUIT:
            running = false;
            break;
        default: 
            break;
    }
}

void E64::SDLInput::OnKeyPressed(Scancode key) {
    if(key == E64::Scancode::Escape) running = false;
}

void E64::SDLInput::buildKeyMap() {
    key_map[SDL_SCANCODE_A] = E64::Scancode::A;
    key_map[SDL_SCANCODE_B] = E64::Scancode::B;
    key_map[SDL_SCANCODE_C] = E64::Scancode::C;
    key_map[SDL_SCANCODE_D] = E64::Scancode::D;
    key_map[SDL_SCANCODE_E] = E64::Scancode::E;
    key_map[SDL_SCANCODE_F] = E64::Scancode::F;
    key_map[SDL_SCANCODE_G] = E64::Scancode::G;
    key_map[SDL_SCANCODE_H] = E64::Scancode::H;
    key_map[SDL_SCANCODE_I] = E64::Scancode::I;
    key_map[SDL_SCANCODE_J] = E64::Scancode::J;
    key_map[SDL_SCANCODE_K] = E64::Scancode::K;
    key_map[SDL_SCANCODE_L] = E64::Scancode::L;
    key_map[SDL_SCANCODE_M] = E64::Scancode::M;
    key_map[SDL_SCANCODE_N] = E64::Scancode::N;
    key_map[SDL_SCANCODE_O] = E64::Scancode::O;
    key_map[SDL_SCANCODE_P] = E64::Scancode::P;
    key_map[SDL_SCANCODE_Q] = E64::Scancode::Q;
    key_map[SDL_SCANCODE_R] = E64::Scancode::R;
    key_map[SDL_SCANCODE_S] = E64::Scancode::S;
    key_map[SDL_SCANCODE_T] = E64::Scancode::T;
    key_map[SDL_SCANCODE_U] = E64::Scancode::U;
    key_map[SDL_SCANCODE_V] = E64::Scancode::V;
    key_map[SDL_SCANCODE_W] = E64::Scancode::W;
    key_map[SDL_SCANCODE_X] = E64::Scancode::X;
    key_map[SDL_SCANCODE_Y] = E64::Scancode::Y;
    key_map[SDL_SCANCODE_Z] = E64::Scancode::Z;

    key_map[SDL_SCANCODE_0] = E64::Scancode::Num0;
    key_map[SDL_SCANCODE_1] = E64::Scancode::Num1;
    key_map[SDL_SCANCODE_2] = E64::Scancode::Num2;
    key_map[SDL_SCANCODE_3] = E64::Scancode::Num3;
    key_map[SDL_SCANCODE_4] = E64::Scancode::Num4;
    key_map[SDL_SCANCODE_5] = E64::Scancode::Num5;
    key_map[SDL_SCANCODE_6] = E64::Scancode::Num6;
    key_map[SDL_SCANCODE_7] = E64::Scancode::Num7;
    key_map[SDL_SCANCODE_8] = E64::Scancode::Num8;
    key_map[SDL_SCANCODE_9] = E64::Scancode::Num9;

    key_map[SDL_SCANCODE_F1]  = E64::Scancode::F1;
    key_map[SDL_SCANCODE_F2]  = E64::Scancode::F2;
    key_map[SDL_SCANCODE_F3]  = E64::Scancode::F3;
    key_map[SDL_SCANCODE_F4]  = E64::Scancode::F4;
    key_map[SDL_SCANCODE_F5]  = E64::Scancode::F5;
    key_map[SDL_SCANCODE_F6]  = E64::Scancode::F6;
    key_map[SDL_SCANCODE_F7]  = E64::Scancode::F7;
    key_map[SDL_SCANCODE_F8]  = E64::Scancode::F8;
    key_map[SDL_SCANCODE_F9]  = E64::Scancode::F9;
    key_map[SDL_SCANCODE_F10] = E64::Scancode::F10;
    key_map[SDL_SCANCODE_F11] = E64::Scancode::F11;
    key_map[SDL_SCANCODE_F12] = E64::Scancode::F12;
    key_map[SDL_SCANCODE_F13] = E64::Scancode::F13;
    key_map[SDL_SCANCODE_F14] = E64::Scancode::F14;
    key_map[SDL_SCANCODE_F15] = E64::Scancode::F15;
    key_map[SDL_SCANCODE_F16] = E64::Scancode::F16;
    key_map[SDL_SCANCODE_F17] = E64::Scancode::F17;
    key_map[SDL_SCANCODE_F18] = E64::Scancode::F18;
    key_map[SDL_SCANCODE_F19] = E64::Scancode::F19;
    key_map[SDL_SCANCODE_F20] = E64::Scancode::F20;
    key_map[SDL_SCANCODE_F21] = E64::Scancode::F21;
    key_map[SDL_SCANCODE_F22] = E64::Scancode::F22;
    key_map[SDL_SCANCODE_F23] = E64::Scancode::F23;
    key_map[SDL_SCANCODE_F24] = E64::Scancode::F24;

    key_map[SDL_SCANCODE_UP]    = E64::Scancode::Up;
    key_map[SDL_SCANCODE_DOWN]  = E64::Scancode::Down;
    key_map[SDL_SCANCODE_LEFT]  = E64::Scancode::Left;
    key_map[SDL_SCANCODE_RIGHT] = E64::Scancode::Right;

    key_map[SDL_SCANCODE_LSHIFT] = E64::Scancode::LShift;
    key_map[SDL_SCANCODE_RSHIFT] = E64::Scancode::RShift;
    key_map[SDL_SCANCODE_LCTRL]  = E64::Scancode::LCtrl;
    key_map[SDL_SCANCODE_RCTRL]  = E64::Scancode::RCtrl;
    key_map[SDL_SCANCODE_LALT]   = E64::Scancode::LAlt;
    key_map[SDL_SCANCODE_RALT]   = E64::Scancode::RAlt;
    key_map[SDL_SCANCODE_LGUI]   = E64::Scancode::LGui;
    key_map[SDL_SCANCODE_RGUI]   = E64::Scancode::RGui;

    key_map[SDL_SCANCODE_CAPSLOCK]   = E64::Scancode::CapsLock;
    key_map[SDL_SCANCODE_NUMLOCKCLEAR] = E64::Scancode::NumLock;
    key_map[SDL_SCANCODE_SCROLLLOCK] = E64::Scancode::ScrollLock;

    key_map[SDL_SCANCODE_RETURN]    = E64::Scancode::Return;
    key_map[SDL_SCANCODE_ESCAPE]    = E64::Scancode::Escape;
    key_map[SDL_SCANCODE_BACKSPACE] = E64::Scancode::Backspace;
    key_map[SDL_SCANCODE_TAB]       = E64::Scancode::Tab;
    key_map[SDL_SCANCODE_SPACE]     = E64::Scancode::Space;
    key_map[SDL_SCANCODE_INSERT]    = E64::Scancode::Insert;
    key_map[SDL_SCANCODE_DELETE]    = E64::Scancode::Delete;
    key_map[SDL_SCANCODE_HOME]      = E64::Scancode::Home;
    key_map[SDL_SCANCODE_END]       = E64::Scancode::End;
    key_map[SDL_SCANCODE_PAGEUP]    = E64::Scancode::PageUp;
    key_map[SDL_SCANCODE_PAGEDOWN]  = E64::Scancode::PageDown;

    key_map[SDL_SCANCODE_MINUS]        = E64::Scancode::Minus;
    key_map[SDL_SCANCODE_EQUALS]       = E64::Scancode::Equals;
    key_map[SDL_SCANCODE_LEFTBRACKET]  = E64::Scancode::LeftBracket;
    key_map[SDL_SCANCODE_RIGHTBRACKET] = E64::Scancode::RightBracket;
    key_map[SDL_SCANCODE_BACKSLASH]    = E64::Scancode::Backslash;
    key_map[SDL_SCANCODE_SEMICOLON]    = E64::Scancode::Semicolon;
    key_map[SDL_SCANCODE_APOSTROPHE]   = E64::Scancode::Apostrophe;
    key_map[SDL_SCANCODE_GRAVE]        = E64::Scancode::Grave;
    key_map[SDL_SCANCODE_COMMA]        = E64::Scancode::Comma;
    key_map[SDL_SCANCODE_PERIOD]       = E64::Scancode::Period;
    key_map[SDL_SCANCODE_SLASH]        = E64::Scancode::Slash;

    key_map[SDL_SCANCODE_KP_0]        = E64::Scancode::KP0;
    key_map[SDL_SCANCODE_KP_1]        = E64::Scancode::KP1;
    key_map[SDL_SCANCODE_KP_2]        = E64::Scancode::KP2;
    key_map[SDL_SCANCODE_KP_3]        = E64::Scancode::KP3;
    key_map[SDL_SCANCODE_KP_4]        = E64::Scancode::KP4;
    key_map[SDL_SCANCODE_KP_5]        = E64::Scancode::KP5;
    key_map[SDL_SCANCODE_KP_6]        = E64::Scancode::KP6;
    key_map[SDL_SCANCODE_KP_7]        = E64::Scancode::KP7;
    key_map[SDL_SCANCODE_KP_8]        = E64::Scancode::KP8;
    key_map[SDL_SCANCODE_KP_9]        = E64::Scancode::KP9;
    key_map[SDL_SCANCODE_KP_DECIMAL]  = E64::Scancode::KPDecimal;
    key_map[SDL_SCANCODE_KP_DIVIDE]   = E64::Scancode::KPDivide;
    key_map[SDL_SCANCODE_KP_MULTIPLY] = E64::Scancode::KPMultiply;
    key_map[SDL_SCANCODE_KP_MINUS]    = E64::Scancode::KPMinus;
    key_map[SDL_SCANCODE_KP_PLUS]     = E64::Scancode::KPPlus;
    key_map[SDL_SCANCODE_KP_ENTER]    = E64::Scancode::KPEnter;
    key_map[SDL_SCANCODE_KP_EQUALS]   = E64::Scancode::KPEquals;

    key_map[SDL_SCANCODE_PRINTSCREEN] = E64::Scancode::PrintScreen;
    key_map[SDL_SCANCODE_PAUSE]       = E64::Scancode::Pause;
    key_map[SDL_SCANCODE_APPLICATION] = E64::Scancode::Application;
    key_map[SDL_SCANCODE_POWER]       = E64::Scancode::Power;

    key_map[SDL_SCANCODE_MEDIA_STOP]           = E64::Scancode::MediaStop;
    key_map[SDL_SCANCODE_MEDIA_PREVIOUS_TRACK] = E64::Scancode::MediaPrev;
    key_map[SDL_SCANCODE_MEDIA_NEXT_TRACK]     = E64::Scancode::MediaNext;
    key_map[SDL_SCANCODE_MEDIA_PLAY]           = E64::Scancode::MediaPlayPause;
    key_map[SDL_SCANCODE_MUTE]                 = E64::Scancode::MediaMute;
    key_map[SDL_SCANCODE_VOLUMEUP]             = E64::Scancode::MediaVolumeUp;
    key_map[SDL_SCANCODE_VOLUMEDOWN]           = E64::Scancode::MediaVolumeDown;
}

bool E64::SDLInput::isKeyPressed(E64::Scancode key) {
    const bool* state = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
        if (key_map[i] == key && state[i]) return true;
    return false;
}

bool E64::SDLInput::isKeyDown(E64::Scancode key) {
    const bool* state = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
        if (key_map[i] == key && state[i]) return true;
    return false;
}