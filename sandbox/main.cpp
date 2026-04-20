#include "Sandbox.h"
#include <GCGameEngine.h>

int main(){
    Sandbox* app = new Sandbox();
    app->run();
    delete app;
}