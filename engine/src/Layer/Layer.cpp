#include "Layer/Layer.h"

namespace E64 {
    std::vector<Layer*> Layer::layers;  // single definition, lives in the DLL

    void Layer::Attach(Layer* layer) {
        layers.push_back(layer);
        layer->OnAttach();
    }
}