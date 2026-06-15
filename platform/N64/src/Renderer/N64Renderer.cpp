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

    debug_msg_queue = {};
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
    t3d_screen_clear_color(RGBA32(25, 25, 25, 0));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient.data()); // one global ambient light, always active
    t3d_light_set_count(1);
}

void E64::N64Renderer::sendModelMat4FP(T3DMat4FP* modelMat4FP){
    this->modelMatFP = modelMat4FP;
}

void E64::N64Renderer::draw(E64::Mesh* mesh){
    rspq_syncpoint_t point = rspq_syncpoint_new();
    if(*n64_window->getMode() == CONSOLE) return;

    if(!frame_buf || !z_buf){
        return;
    }

    fm_vec3_t norm_up = {{ 0, 0, 1}};
    uint16_t norm = t3d_vert_pack_normal(&norm_up);

    const float SCALE = 16.0f;
    const float COLOR_SCALE = 255.0f;

    auto flags = (T3DDrawFlags)(T3D_FLAG_DEPTH | T3D_FLAG_SHADED | T3D_FLAG_CULL_BACK);
    t3d_state_set_drawflags(flags);

    t3d_matrix_push(modelMatFP);
    std::vector<T3DVertPacked*> allocations;
    for(const MeshBatch& batch : mesh->batches){
        T3DVertPacked* vertices = (T3DVertPacked*)malloc_uncached(sizeof(T3DVertPacked) * (batch.vertices.size() + 1) / 2);
        allocations.push_back(vertices);
        int j = 0;
        for(size_t i = 0; i < batch.vertices.size(); i += 2){
            Vertex vertexA = batch.vertices.at(i);

            int16_t posA[3] = {(int16_t)(vertexA.pos.x * SCALE), (int16_t)(vertexA.pos.y * SCALE), (int16_t)(vertexA.pos.z * SCALE)};
            memcpy(vertices[j].posA, posA, sizeof(int16_t) * 3);
            uint32_t colorA = ((uint8_t)(vertexA.color.r * COLOR_SCALE) << 24) | ((uint8_t)(vertexA.color.g * COLOR_SCALE) << 16) | ((uint8_t)(vertexA.color.b * COLOR_SCALE) << 8) | 255;
            vertices[j].rgbaA = colorA;
            vertices[j].normA = norm;

            if(i + 1 == batch.vertices.size())
            {
                int16_t posB[3] = {0, 0, 0};
                memcpy(vertices[j].posB, posB, sizeof(int16_t) * 3);
                vertices[j].rgbaB = 0xFFFFFF'FF;
                vertices[j].normB = norm;
            }   
            else
            {
                Vertex vertexB = batch.vertices.at(i + 1);
                int16_t posB[3] = {(int16_t)(vertexB.pos.x * SCALE), (int16_t)(vertexB.pos.y * SCALE), (int16_t)(vertexB.pos.z * SCALE)};
                memcpy(vertices[j].posB, posB, sizeof(int16_t) * 3);
                uint32_t colorB = ((uint8_t)(vertexB.color.r * COLOR_SCALE) << 24) | ((uint8_t)(vertexB.color.g * COLOR_SCALE) << 16) | ((uint8_t)(vertexB.color.b * COLOR_SCALE) << 8) | 255;
                vertices[j].rgbaB = colorB;
                vertices[j].normB = norm;
            }
            j++;
        }

        t3d_vert_load(vertices, 0, batch.vertices.size());
        for(size_t i = 0; i + 2 < batch.indices.size(); i += 3){
            t3d_tri_draw(
                batch.indices[i],
                batch.indices[i + 1],
                batch.indices[i + 2]
            );
            t3d_tri_sync();
        }
    }

    for(T3DVertPacked* v : allocations){
        free_uncached(v);
    }

    t3d_matrix_pop(1);
    rspq_syncpoint_wait(point);
}

void E64::N64Renderer::drawText(){
    if(*n64_window->getMode() == CONSOLE) return;

    rdpq_text_print(NULL, 1, 30, 10, "Loaded Meshes: ");
    int i = 1;
    for(auto& [path, handle] : E64::Engine::ctx->asset_manager->getHandleRepository()){
        std::string msg = path + " : " + std::to_string(handle);
        rdpq_text_print(NULL, 1, 50, 20 + (i * 10), msg.c_str());
        i++;
    }

    std::string debug_light_color = "Color Ambient: R " + std::to_string(colorAmbient[0]) 
        + " G " + std::to_string(colorAmbient[1]) + " B " + std::to_string(colorAmbient[2]);
    rdpq_text_print(NULL, 1, 30, 100, debug_light_color.c_str());

    for(size_t i = 0; i < debug_msg_queue.size(); ++i){
        rdpq_text_print(NULL, 1, 500, (i + 1) * 10, debug_msg_queue[i].c_str());
    }

    E64::Scene* scene = E64::Engine::ctx->active_scene.get();
    std::string scene_debug = "Scene: " + scene->getName();
    rdpq_text_print(NULL, 1, 450, 10, scene_debug.c_str());
}

T3DViewport* E64::N64Renderer::getViewport(){
    return &this->viewport;
}

void E64::N64Renderer::submit(){
    if(*n64_window->getMode() == CONSOLE) return;
    rdpq_detach_show();
}

void E64::N64Renderer::toggleDebug(){
    this->debug_mode = !debug_mode;
}

bool E64::N64Renderer::isDebug(){
    return this->debug_mode;
}