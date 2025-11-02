//
// Created by daniel on 09/08/2025.
//

#include "blend2d_surface.h"

#include <cassert>

#include <blend2d.h>
#include <iostream>

#include "blend2d_utils.h"

namespace gfx {

Blend2DSurface::Blend2DSurface(ImageInfo imageInfo)
    : _imageInfo{imageInfo}
{
    BLFormat blFormat = _getBlFormat();
    auto dimension = imageInfo.dimension;
    _blImage = BLImage(dimension.width, dimension.height, blFormat);
    assert(_blImage);
    BLResult blResult = _blImage.getData(&_blImageData);
    if (blResult != BL_SUCCESS) {
        std::cout << "Blend2D Error: " << getBLError(blResult) << blResult << std::endl;
    }
}

void Blend2DSurface::setData(ImageInfo textureInfo, void* data)
{
    
}

void* Blend2DSurface::getData() const
{
    if (_blImage.empty()) return nullptr;
    return _blImageData.pixelData;
}

int32_t Blend2DSurface::getWidth() const
{
    return _blImage.width();
}

int32_t Blend2DSurface::getHeight() const
{
    return _blImage.height();
}

void Blend2DSurface::writeToFile(std::string fileName)
{
    _blImage.writeToFile(fileName.c_str());
}

BLImage& Blend2DSurface::getBLImage()
{
    return _blImage;
}

BLFormat Blend2DSurface::_getBlFormat() const
{
    switch (_imageInfo.colorType) {
    case ColorType::PREMUL_ARGB_8888: {
        return BL_FORMAT_PRGB32;
    }
    case ColorType::XRGB_8888: {
        return BL_FORMAT_XRGB32;
    }
    default:
        // NOT SUPPORTED
        assert(false);
        return BL_FORMAT_PRGB32;
    }
}



} // gfx