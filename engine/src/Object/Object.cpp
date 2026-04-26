#include "Object/Object.h"

GCGameEngine::Object::Object(){
    mesh = { new VBO(3) };
    transform = { glm::vec3(0.0f, 0.0f, 0.0f) };
    mvp = glm::mat4(1.0f);
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.transform);

    glm::mat4 view = glm::lookAt(
        glm::vec3(1, 1, 1),  // Camera Pos
        glm::vec3(0, 0, 0),  // Looking at Origin
        glm::vec3(0, 1, 0)   // Up Vector 
    );

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), // FOV
        800.0f / 600.0f,     // Aspect Ratio
        0.1f,                // Near plane
        100.0f               // Far Plane
    );

    mvp = proj * view * model;
}

GCGameEngine::Mesh GCGameEngine::Object::getMesh(){
    return this->mesh;
}

glm::mat4 GCGameEngine::Object::getMVP(){
    return this->mvp;
}