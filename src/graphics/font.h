//
// Created by daniel on 16/08/2025.
//

#pragma once

#include <string>

namespace gfx {
class Typeface
{
public:
    Typeface() = default;
    virtual ~Typeface() = default;

    // FACTORY
    static std::shared_ptr<Typeface> createFromFile(std::string filepath);
};

class Font
{
public:
    Font() = default;
    virtual ~Font() = default;
    static std::shared_ptr<Font> createFromTypeface(std::shared_ptr<Typeface> typeface, float size);
};
}
