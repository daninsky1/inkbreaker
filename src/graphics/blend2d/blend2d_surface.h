//
// Created by daniel on 09/08/2025.
//

#pragma once

#include "graphics/image_info.h"
#include "graphics/surface.h"

#include <blend2d.h>
#include <string>

namespace gfx {

/**
 * Allocates a surface on RAM
 */
class Blend2DSurface final : public Surface {
public:
    explicit Blend2DSurface(ImageInfo imageInfo);
    ~Blend2DSurface() override { };
    void setData(ImageInfo textureInfo, void* data) override;
    void* getData() const override;
    int32_t getWidth() const override;
    int32_t getHeight() const override;
    void bind() override {}
    void unbind() override {}
    void* getHandle() const override { return nullptr; }
    void writeToFile(std::string fileName) override;
    BLImage& getBLImage();
private:
    BLFormat _getBlFormat() const;public:
    ImageInfo _imageInfo;
    BLImage _blImage;
    BLImageData _blImageData;
};

} // gfx
