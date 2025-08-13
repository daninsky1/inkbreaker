//
// Created by daniel on 07/08/2025.
//

#pragma once

#include <iostream>
#include <format>
#include <memory>
#include <utility>
#include <vector>

#include <include/core/SkImageInfo.h>
#include <include/core/SkSurface.h>
#include <include/core/SkColor.h>

#include "geometry.h"

namespace  gfx
{

/* External Polymosrphism */
struct ShapeConcept
{
    virtual ~ShapeConcept() = default;

    // virtual void serialize() const = 0;
    virtual void draw() const = 0;
    [[nodiscard]] virtual std::unique_ptr<ShapeConcept> clone() const = 0;
};

template<typename ShapeT, typename RenderStrategy>
struct ShapeModel final : ShapeConcept
{
    explicit ShapeModel(ShapeT shape, RenderStrategy renderStrategy)
        : shape(std::move(shape)), render(std::move(renderStrategy))
    { }
    // template<typename U>
    // explicit ShapeModel(U&& value)
    //     : shape{ std::forward<U>(value) }
    // { }

    void draw() const override
    {
        render.draw(shape);
    }

    [[nodiscard]] std::unique_ptr<ShapeConcept> clone() const override
    {
        return std::make_unique<ShapeModel>(*this);
    }

    ShapeT shape;
    RenderStrategy render;
};

class Shape
{
public:
    template<typename ShapeT, typename RenderStrategy>
    explicit Shape(ShapeT shape, RenderStrategy renderStrategy)
    {
        using Model = ShapeModel<ShapeT, RenderStrategy>;
        _pimpl = std::make_unique<Model>(std::move(shape), std::move(renderStrategy));
    }

    /* Special member functions */
    Shape(const Shape& shape)
        : _pimpl{shape._pimpl->clone()}
    { }
    Shape& operator=(const Shape& shape);
    Shape(Shape&& shape) = default;
    Shape& operator=(Shape&& shape) = default;
private:

    friend void serialize(const Shape& shape)
    {
        // shape._pimpl->serialize();
    }

    friend void draw(const Shape& shape)
    {
        shape._pimpl->draw();
    }

    std::unique_ptr<ShapeConcept> _pimpl;
};

void serialize(const Circle& circle);
void serialize(const Rect& rectangle);

inline void draw(const Rect& rectangle)
{
    auto msg = std::format(
        "Drawing rectangle: x: {}, y: {}, width: {}, height: {}",
        rectangle.x, rectangle.y,
        rectangle.width, rectangle.height
    );

    std::cout << msg << std::endl;
}

inline void drawAllShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    for (auto const& shape : shapes) {
        // draw(*shape);
    }
}



class Render
{
public:
    virtual ~Render() {}
    virtual void draw(const Line& line) const = 0;
    virtual void draw(const Circle& circle) const = 0;
};

}