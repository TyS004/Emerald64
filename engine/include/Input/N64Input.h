#ifndef N64_INPUT_H
#define N64_INPUT_H

#include "Input/Input.h"

namespace E64{
    class N64Input : public Input{
        public:
            N64Input();
            ~N64Input();

            void poll();
    };
}

#endif