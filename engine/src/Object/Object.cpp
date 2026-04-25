#include "Object/Object.h"

GCGameEngine::Object::Object(){
    Mesh mesh = { new VBO(3) };
    this->mesh = mesh;
}

GCGameEngine::Mesh GCGameEngine::Object::getMesh(){
    return this->mesh;
}