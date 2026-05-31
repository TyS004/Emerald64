#include "N64Renderer.h"

#include <libdragon.h>

N64Renderer::N64Renderer(){
    display_init( RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE );
    
}

N64Renderer::~N64Renderer(){

}

