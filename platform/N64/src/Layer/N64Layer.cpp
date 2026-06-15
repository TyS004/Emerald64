#include <Layer/N64Layer.h>
#include <Renderer/N64Renderer.h>
#include <Input/N64Input.h>

#include <libdragon.h>

E64::N64Layer::N64Layer(){
    ECS::ComponentRegistry<ECS::TransformComponent>::registerComponent("Transform");
    ECS::ComponentRegistry<ECS::MeshComponent>::registerComponent("Mesh");
    ECS::ComponentRegistry<ECS::CameraComponent>::registerComponent("Camera");

    E64::Engine::ctx->asset_manager = std::make_unique<E64::AssetManager>();

    dfs_init(DFS_DEFAULT_LOCATION);
    std::filesystem::path scene_path = E64::Engine::ctx->root_dir.string() + "scenes/scene.json";
    E64::SceneSerializer serializer;
    E64::Scene* scene = serializer.deserialize(scene_path);
    if(!scene){
        E64::Log::info(scene_path.string());
        E64::Log::info(E64::Engine::ctx->root_dir.string());
        exit(0);
    }
    static_assert(sizeof(Vertex) == 36, "Vertex struct has unexpected padding");

    modelMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP) * scene->getEntites().size());
    E64::Engine::ctx->active_scene = std::make_unique<E64::Scene>(std::move(*scene));
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
    Scene* scene = E64::Engine::ctx->active_scene.get();
    E64::N64Renderer* renderer = dynamic_cast<E64::N64Renderer*>(E64::Engine::ctx->renderer);
    E64::AssetManager* assetmanager = E64::Engine::ctx->asset_manager.get();

    const float SCALE = 16.0f;

    int i = 0;
    for(ECS::Entity e : scene->getEntites()){
        if(ECS::ComponentManager::hasComponent<ECS::MeshComponent>(e) && ECS::ComponentManager::hasComponent<ECS::TransformComponent>(e)){
            ECS::MeshComponent* mesh_comp = ECS::ComponentManager::getComponent<ECS::MeshComponent>(e);
            ECS::TransformComponent* transform = ECS::ComponentManager::getComponent<ECS::TransformComponent>(e);
            Mesh* mesh = assetmanager->getMeshAsset(mesh_comp->mesh_handle);
            E64::Log::info(std::to_string(mesh_comp->mesh_handle));
            if(!mesh){
                mesh = assetmanager->getMeshAsset(0);
                E64::Log::error("Missing Mesh Defaulted to Cube");
            }

            fm_vec3_t translation_vec = {{transform->position.x * SCALE, transform->position.y * SCALE, transform->position.z * SCALE}};
            float rotation_vec[3] = {transform->euler.x, transform->euler.y, transform->euler.z};
            fm_vec3_t scale_vec = {{transform->scale.x, transform->scale.y, transform->scale.z}};

            fm_mat4_t translation, rotation, scale;
            fm_mat4_t model;

            fm_mat4_identity(&translation);
            fm_mat4_identity(&rotation);
            fm_mat4_from_srt_euler(&model, &scale_vec, rotation_vec, &translation_vec);
            t3d_mat4_to_fixed(&modelMatFP[i], &model);

            renderer->sendModelMat4FP(&modelMatFP[i]);
            // if(mesh_comp->mesh_handle == 1){
                renderer->draw(mesh);
            // }
            i++;
        }
    }

    if(renderer->isDebug()){
        renderer->drawText();
    }
}