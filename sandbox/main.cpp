#include "Sandbox.h"
#include <E64.h>

int main(){
    Sandbox* app = new Sandbox();
    app->run();
    delete app;
}