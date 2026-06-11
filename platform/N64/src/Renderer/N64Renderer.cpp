#include "Renderer/N64Renderer.h"

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

E64::N64Renderer::N64Renderer(){
    n64_window = dynamic_cast<E64::N64Window*>(E64::Engine::ctx->window);

    colorAmbient = {200, 50, 170, 0xFF};
    colorDir    = {0xFF, 0xFF, 0xFF, 0xFF};

    lightDirVec = {{0.0f, 0.0f, 1.0f}};
    fm_vec3_norm(&lightDirVec, &lightDirVec);

    viewport = t3d_viewport_create();

    dplDraw = NULL;
}

E64::N64Renderer::~N64Renderer(){
    
}

void E64::N64Renderer::startFrame(){
    if(*n64_window->getMode() == CONSOLE) return;

    frame_buf = display_get();
    z_buf = display_get_zbuf();
    if(!frame_buf || !z_buf){
        return;
    }
    rdpq_attach(frame_buf, z_buf); // set the target to draw to
    t3d_frame_start(); // call this once per frame at the beginning of your draw function

    t3d_viewport_attach(&viewport); // now use the viewport, this applies proj/view matrices and sets scissoring

    rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    // this cleans the entire screen (even if out viewport is smaller)
    t3d_screen_clear_color(RGBA32(0, 0, 0, 0));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient.data()); // one global ambient light, always active
    t3d_light_set_count(1);
}

void E64::N64Renderer::sendModelMat4FP(T3DMat4FP* modelMat4FP){
    this->modelMatFP = modelMat4FP;
}

void E64::N64Renderer::draw(E64::Mesh* mesh){
    if(*n64_window->getMode() == CONSOLE) return;

    if(!frame_buf || !z_buf){
        return;
    }

    fm_vec3_t norm_up = {{ 0, 0, 1}};
    uint16_t norm = t3d_vert_pack_normal(&norm_up);

    const float SCALE = 16.0f;

    vertices = (T3DVertPacked*)malloc_uncached(sizeof(T3DVertPacked) * mesh->vertices.size() / 2);
    int j = 0;
    for(size_t i = 0; i < mesh->vertices.size(); i += 2){
        if(i + 1 == mesh->vertices.size())
        {
            int16_t posA[3] = {(int16_t)(mesh->vertices[i].pos.x * SCALE), (int16_t)(mesh->vertices[i].pos.y * SCALE), (int16_t)(mesh->vertices[i].pos.z * SCALE)};
            int16_t posB[3] = {0, 0, 0};
            memcpy(vertices[j].posA, posA, sizeof(int16_t) * 3);
            memcpy(vertices[j].posB, posB, sizeof(int16_t) * 3);
            vertices[j].rgbaA = 0xFF00FF'FF;
            vertices[j].rgbaB = 0x000000'FF;
            vertices[j].normA = norm;
            vertices[j].normB = norm;
        }
        else{
            int16_t posA[3] = {(int16_t)(mesh->vertices[i].pos.x * SCALE), (int16_t)(mesh->vertices[i].pos.y * SCALE), (int16_t)(mesh->vertices[i].pos.z * SCALE)};
            int16_t posB[3] = {(int16_t)(mesh->vertices[i + 1].pos.x * SCALE), (int16_t)(mesh->vertices[i + 1].pos.y * SCALE), (int16_t)(mesh->vertices[i + 1].pos.z * SCALE)};
            memcpy(vertices[j].posA, posA, sizeof(int16_t) * 3);
            memcpy(vertices[j].posB, posB, sizeof(int16_t) * 3);
            vertices[j].rgbaA = 0x0000FF'FF;
            vertices[j].rgbaB = 0xFF00FF'FF;
            vertices[j].normA = norm;
            vertices[j].normB = norm;
        }
        j++;
    }

    // ======== Draw (3D) ======== //
    auto flags = (T3DDrawFlags)(T3D_FLAG_DEPTH | T3D_FLAG_SHADED | T3D_FLAG_CULL_BACK);
    t3d_state_set_drawflags(flags);

    t3d_matrix_push(modelMatFP);
    t3d_vert_load(vertices, 0, mesh->vertices.size());
    t3d_matrix_pop(1); 
    for(size_t i = 0; i < mesh->indices.size(); i += 3) {
        t3d_tri_draw(
            mesh->indices[i],
            mesh->indices[i + 1],
            mesh->indices[i + 2]
        );
    }
    t3d_tri_sync(); 
    free_uncached(vertices);
}

void E64::N64Renderer::drawText(){
    if(*n64_window->getMode() == CONSOLE) return;

    rdpq_text_print(NULL, 1, 30, 20, "Loaded Meshes: ");
    for(size_t i = 0; i < E64::Engine::ctx->asset_manager->getMeshes().size(); ++i)
    {
        std::string msg = E64::Engine::ctx->asset_manager->getMeshes()[i]->obj_path;
        rdpq_text_print(NULL, 1, 50, i + 30, msg.c_str());
    }

    std::string debug_light_color = "Color Ambient: R " + std::to_string(colorAmbient[0]) 
        + " G " + std::to_string(colorAmbient[1]) + " B " + std::to_string(colorAmbient[2]);
    rdpq_text_print(NULL, 1, 30, 100, debug_light_color.c_str());

    for(size_t i = 0; i < debug_msg_queue.size(); ++i){
        rdpq_text_print(NULL, 1, 50, i * 10, debug_msg_queue[i].c_str());
    }
}

T3DViewport* E64::N64Renderer::getViewport(){
    return &this->viewport;
}

void E64::N64Renderer::submit(){
    if(*n64_window->getMode() == CONSOLE) return;
    rdpq_detach_show();
}