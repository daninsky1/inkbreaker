//
// Created by daniel on 09/08/2025.
//

#pragma once

#include "../image_info.h"
#include "../surface.h"

#include <blend2d.h>
#include <string>

namespace gfx {

/**
 * Allocates a surface on RAM
 */
class Blend2dSurface final : public Surface {
public:
    explicit Blend2dSurface(ImageInfo imageInfo);
    ~Blend2dSurface() override { };
    void setData(ImageInfo textureInfo, void* data) override;
    void* getData() const override;
    void bind() override {}
    void unbind() override {}
    void* getHandle() const override { return nullptr; }
    void writeToFile(std::string fileName);
    BLImage& getBLImage();
private:
    BLFormat _getBlFormat() const;
    ImageInfo _imageInfo;
    BLImage _blImage;
    BLImageData _blImageData;
};

} // gfx
