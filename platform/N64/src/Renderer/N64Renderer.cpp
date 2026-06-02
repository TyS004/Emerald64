#include "Renderer/N64Renderer.h"

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

E64::N64Renderer::N64Renderer(){
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);
    console_init();
    rdpq_init();
    t3d_init(T3DInitParams());

    viewport = t3d_viewport_create_buffered(2);
}

E64::N64Renderer::~N64Renderer(){
    
}

void E64::N64Renderer::sendUniforms(glm::mat4 mvp){
    T3DMat4FP* modelMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP) * 2); 
}

void E64::N64Renderer::draw(E64::ECS::Mesh* mesh){
    
}

void E64::N64Renderer::submit(){

}