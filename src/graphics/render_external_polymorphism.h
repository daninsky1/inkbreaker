//
// Created by daniel on 07/08/2025.
//

#pragma once

#include <iostream>
#include <format>
#include <memory>
#include <utility>
#include <vector>

#include "geometry.h"

namespace  gfx
{

class Shape
{
public:
    template<typename  T>
    explicit Shape(T const& value)
        : _pimpl{new ShapeModel<T>(value)}
    {}

    /* Special member functions */
    Shape(const Shape& shape)
    {
        _pimpl = shape._pimpl->clone();
    }
    Shape& operator=(const Shape& shape);
    Shape(Shape&& shape) = default;
    Shape& operator=(Shape&& shape) = default;
private:
    /* External Polymosrphism */
    struct ShapeConcept
    {
        virtual ~ShapeConcept() = default;

        virtual void serialize() const = 0;
        virtual void draw() const = 0;
        [[nodiscard]] virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template<typename T>
    struct ShapeModel final : ShapeConcept
    {
        explicit ShapeModel(T&& value)
            : shape{ std::forward<T>(value) }
        { }

        void serialize() const override
        {
            serialize(shape);
        }

        void draw() const override
        {
            draw(shape);
        }

        [[nodiscard]] std::unique_ptr<ShapeConcept> clone() const override
        {
            return std::make_unique<ShapeModel>(*this);
        }

        T shape;
    };

    friend void serialize(const Shape& shape)
    {
        shape._pimpl->serialize();
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
inline void draw(const Circle& circle)
{
    auto msg = std::format(
        "Drawing rectangle: x: {}, y: {}, radius: {}",
        circle.x, circle.y, circle.radius
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
    virtual void draw(Line line) const = 0;
};

class AggRender : public Render
{

};

class Blend2DRender : public Render
{

};

class SkiaRender : public Render
{

};

class OpenGLRender : public Render
{

};



}