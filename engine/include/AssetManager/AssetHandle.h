#ifndef ASSET_HANDLE_H
#define ASSET_HANDLE_H

#include <string>
#include <cstdint>

namespace E64{
    struct AssetHandle{
        uint32_t id;
        std::string path;
    };
}

#endif