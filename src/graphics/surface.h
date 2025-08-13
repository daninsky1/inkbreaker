//
// Created by daniel on 10/08/2025.
//

#pragma once

#include <memory>

#include "image_info.h"

#include <string>

namespace gfx {

class Surface
{
public:
    virtual ~Surface() = default;

    virtual void* getData() const = 0;
    virtual void setData(ImageInfo textureInfo, void* data) = 0;
    virtual int32_t getWidth() const = 0;
    virtual int32_t getHeight() const = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual void* getHandle() const = 0;
    virtual void writeToFile(std::string fileName) = 0;

    static std::shared_ptr<Surface> create(ImageInfo textureInfo);
};

} // gfx
