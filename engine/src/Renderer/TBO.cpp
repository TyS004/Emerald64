#include "Renderer/TBO.h"

E64::TBO::TBO(){
    stbi_set_flip_vertically_on_load(true);
    img_data = stbi_load("../assets/textures/test.png", &width, &height, &channels, STBI_rgb_alpha);
    if(!img_data){
        E64::Log::error("Could Not Load Image Data");
    }
    img_size = width * height * 4;
}

E64::TBO::TBO(std::string path){
    stbi_set_flip_vertically_on_load(true);
    img_data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if(!img_data){
        E64::Log::error("Could Not Load Image Data");
    }
    img_size = width * height * 4;
}

E64::TBO::~TBO(){

}

void E64::TBO::upload(){
    SDL_Window* window = E64::Window::getWindow();
    SDL_GPUDevice* device = E64::Window::getDevice();

    SDL_GPUTextureCreateInfo info = {};
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    info.width  = width;
    info.height = height;
    info.type   = SDL_GPU_TEXTURETYPE_2D;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    texture = SDL_CreateGPUTexture(device, &info);

    // E64::Log::debug("TBO pointer: " + std::to_string((uintptr_t)texture));
    // E64::Log::debug("Image size: " + std::to_string(img_size));
    // E64::Log::debug("Width: " + std::to_string(width) + " Height: " + std::to_string(height));

    SDL_GPUSamplerCreateInfo sampler_info{};
    sampler_info.min_filter = SDL_GPU_FILTER_LINEAR;         // For minification
    sampler_info.mag_filter = SDL_GPU_FILTER_LINEAR;          // For magnification
    sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;

    sampler = SDL_CreateGPUSampler(device, &sampler_info);

    SDL_GPUTransferBufferCreateInfo transferInfo{};
    transferInfo.size = img_size;
    transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

    SDL_GPUTextureTransferInfo textureTransferInfo{};
    textureTransferInfo.offset = 0;
    textureTransferInfo.pixels_per_row = width;
    textureTransferInfo.rows_per_layer = height;
    textureTransferInfo.transfer_buffer = transfer_buffer;

    void* data = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
    memcpy(data, img_data, img_size);
    SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

    SDL_GPUCommandBuffer* cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd_buffer);

    SDL_GPUTextureRegion region{};
    region.texture = texture;
    region.w = width;
    region.h = height;
    region.d = 1;
    region.x = 0;
    region.y = 0;
    region.z = 0;
    
    SDL_UploadToGPUTexture(copy_pass, &textureTransferInfo, &region, true);
    E64::Log::debug("Sent TBO to GPU");

    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(cmd_buffer);
    SDL_WaitForGPUIdle(device);

    if (!texture)         { E64::Log::error(SDL_GetError()); }
    if (!sampler)         { E64::Log::error(SDL_GetError()); }
    if (!transfer_buffer) { E64::Log::error(SDL_GetError()); }
}


SDL_GPUTexture* E64::TBO::getTexture(){
    return texture;
}

SDL_GPUSampler* E64::TBO::getSampler(){
    return sampler;
}