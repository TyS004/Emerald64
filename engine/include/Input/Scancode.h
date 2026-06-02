#pragma once

namespace E64 {
    enum class Scancode {
        Unknown = 0,

        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4,
        Num5, Num6, Num7, Num8, Num9,

        F1,  F2,  F3,  F4,  F5,  F6,
        F7,  F8,  F9,  F10, F11, F12,
        F13, F14, F15, F16, F17, F18,
        F19, F20, F21, F22, F23, F24,

        Up, Down, Left, Right,

        LShift, RShift,
        LCtrl,  RCtrl,
        LAlt,   RAlt,
        LGui,   RGui,

        CapsLock, NumLock, ScrollLock,

        Return, Escape, Backspace, Tab, Space,
        Insert, Delete, Home, End,
        PageUp, PageDown,

        Minus,       
        Equals,       
        LeftBracket,  
        RightBracket, 
        Backslash,    
        Semicolon,    
        Apostrophe,  
        Grave,        
        Comma,        
        Period,       
        Slash,       

        KP0, KP1, KP2, KP3, KP4,
        KP5, KP6, KP7, KP8, KP9,
        KPDecimal,
        KPDivide,
        KPMultiply,
        KPMinus,
        KPPlus,
        KPEnter,
        KPEquals,

        PrintScreen,
        Pause,
        Application,
        Power,

        MediaStop,
        MediaPrev,
        MediaNext,
        MediaPlayPause,
        MediaMute,
        MediaVolumeUp,
        MediaVolumeDown,

        Count
    };
}