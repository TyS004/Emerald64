#include <Layer/N64Layer.h>
#include <Renderer/N64Renderer.h>
#include <Input/N64Input.h>

#include <libdragon.h>

E64::N64Layer::N64Layer(){
    E64::Engine::ctx->asset_manager = std::make_unique<E64::AssetManager>();

    E64::Scene scene;
    scene.createDefaultScene();
    scene.printScene();

    modelMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP) * scene.getEntites().size());
    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(scene));

    camera = new N64Camera{};
    camera->transform = {{0.0f, 0.0f, -60.0f}};
    camera->euler = {{0.0f, 0.0f, 0.0f}};
    camera->target    = {{0.0f, 0.0f, 0.0f}};
    camera->velocity = {{2.0f, 2.0f, 2.0f}};

    camera->rot_angle = 0.0f;
    camera->rot_axis = {{1.0f, -2.3f, 1.0f}};

    fm_vec3_norm(&camera->rot_axis, &camera->rot_axis);
}

E64::N64Layer::~N64Layer(){

}

void E64::N64Layer::OnUpdate(float dt){
    E64::N64Renderer* renderer = dynamic_cast<E64::N64Renderer*>(E64::Engine::ctx->renderer);
    T3DViewport* viewport = renderer->getViewport();
    const float SCALE = 16.0f;

    E64::N64Input* input = dynamic_cast<E64::N64Input*>(E64::Engine::ctx->input);

    fm_vec3_t direction;
    direction.x = cos(T3D_DEG_TO_RAD(camera->euler.x)) * cos(T3D_DEG_TO_RAD(camera->euler.y));
    direction.y = sin(T3D_DEG_TO_RAD(camera->euler.y));
    direction.z = sin(T3D_DEG_TO_RAD(camera->euler.x)) * cos(T3D_DEG_TO_RAD(camera->euler.y));
    fm_vec3_norm(&camera->target, &direction);

    fm_vec3_t world_up = {{ 0, 1, 0}};
    fm_vec3_t camera_right;
    fm_vec3_t camera_up;

    fm_vec3_t out;
    fm_vec3_cross(&out, &camera->target, &world_up);
    fm_vec3_norm(&camera_right, &out);

    fm_vec3_t cross;
    fm_vec3_cross(&cross, &camera_right, &camera->target);
    fm_vec3_norm(&camera_up, &cross);

    fm_vec3_t camera_front = camera->target + camera->transform;
    t3d_viewport_set_projection(&*viewport, T3D_DEG_TO_RAD(85.0f), 0.1f, 100.0f * SCALE);
    t3d_viewport_look_at(&*viewport, &camera->transform, &camera_front, &camera_up);

    if(input->getInputs() != nullptr)
    {
        if(input->getInputs()->stick_y > 0) {
            fm_vec3_t move;
            fm_vec3_mul(&move, &camera->velocity, &camera->target);
            camera->transform += move;
        }
        if(input->getInputs()->stick_y < 0) {
            fm_vec3_t move;
            fm_vec3_mul(&move, &camera->velocity, &camera->target);
            camera->transform -= move;
        }
        if(input->getInputs()->stick_x > 0) {
            fm_vec3_t cross;
            fm_vec3_t norm;
            fm_vec3_t move;
            fm_vec3_cross(&cross, &camera->target, &camera_up);
            fm_vec3_norm(&norm, &cross);
            fm_vec3_mul(&move, &camera->velocity, &norm);
            camera->transform += move;
        }
        if(input->getInputs()->stick_x < 0) {
            fm_vec3_t cross;
            fm_vec3_t norm;
            fm_vec3_t move;
            fm_vec3_cross(&cross, &camera->target, &camera_up);
            fm_vec3_norm(&norm, &cross);
            fm_vec3_mul(&move, &camera->velocity, &norm);
            camera->transform -= move;
        }

        if(input->getInputs()->cstick_x > 0) camera->euler.x += 2.0f;
        if(input->getInputs()->cstick_x < 0) camera->euler.x -= 2.0f;
        if(input->getInputs()->cstick_y > 0) camera->euler.y -= 2.0f;
        if(input->getInputs()->cstick_y < 0) camera->euler.y += 2.0f;
    }
}

void E64::N64Layer::OnRender(){
    this->scene = E64::Engine::ctx->active_scene.get();
    E64::N64Renderer* renderer = dynamic_cast<E64::N64Renderer*>(E64::Engine::ctx->renderer);
    E64::AssetManager* assetmanager = E64::Engine::ctx->asset_manager.get();

    const float SCALE = 16.0f;

    int i = 0;
    for(ECS::Entity e : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(e) && ECS::ComponentManager::hasComponent<ECS::TransformComponent>(e)){
            ECS::MeshComponent* mesh_comp = ECS::ComponentManager::getComponent<ECS::MeshComponent>(e);
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);
            ECS::Mesh* mesh = assetmanager->getMesh(mesh_comp->mesh_handle);
            if(!mesh){
                mesh = assetmanager->getMesh({0, "default"});
            }

            fm_vec3_t trans = {{transform->position.x * SCALE, transform->position.y * SCALE, transform->position.z * SCALE}};
            fm_mat4_t translateMat, rotMat;
            fm_mat4_t finalMat;
            fm_mat4_identity(&translateMat);
            fm_mat4_identity(&rotMat);
            fm_mat4_from_translation(&translateMat, &trans);
            //fm_mat4_from_axis_angle(&rotMat, &camera->rot_axis, camera->rot_angle);
            fm_mat4_mul(&finalMat, &translateMat, &rotMat);
            t3d_mat4_to_fixed(&modelMatFP[i], &finalMat);

            E64::N64Window* window = dynamic_cast<E64::N64Window*>(E64::Engine::ctx->window);
            renderer->sendModelMat4FP(&modelMatFP[i]);
            renderer->draw(mesh);
            i++;
        }
    }
    renderer->drawText();
}