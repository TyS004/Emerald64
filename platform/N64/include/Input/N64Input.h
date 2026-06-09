#ifndef N64_INPUT_H
#define N64_INPUT_H

#include <E64.h>
#include <libdragon.h>

namespace E64{
    class N64Input : public Input{
        public:
            N64Input();
            ~N64Input();

            void poll();

            void OnButtonPress(joypad_buttons_t);

            joypad_inputs_t* getInputs();
        private:
            joypad_inputs_t inputs;

            int r_axis;
            int u_axis;
    };
}

#endif