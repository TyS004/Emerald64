#include "N64Renderer.h"

#include <libdragon.h>
#include <t3d/t3d.h>

N64Renderer::N64Renderer(){
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);
    console_init();
    rdpq_init();
    t3d_init((T3DInitParams){});
}

void N64Renderer::draw(E64::ECS::Mesh* mesh){

}   

void N64Renderer::submit(){

    display_context_t frame_buffer{};
    rdpq_attach(frame_buffer, NULL);

    rdpq_detach_show();
}

N64Renderer::~N64Renderer(){

}