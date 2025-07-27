#pragma once

#include <cstdint>
#include <string>
#include <format>
#include <vector>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include <include/core/SkCanvas.h>

#include "events.h"
#include "layout.h"

namespace ui
{
class Widget
{
public:
    uint32_t getWidth() const { return _size.width; }
    uint32_t getHeight() const { return _size.height; }
    void setPosition(const Position&position );
    void setSize(const Size&size );
                        
    Position getPosition() const { return { _position }; }
    Size getSize() const { return { _size }; }
    Size normalize(const BoxConstraint& constraint) {
        // Normalize the widget's size based on the given constraints
        _size.width = std::clamp(_size.width, constraint.minWidth, constraint.maxWidth);
        _size.height = std::clamp(_size.height, constraint.minHeight, constraint.maxHeight);
        return _size;
    }
    
    /**
     * Sets the widget constraint and apply layout
     * @param constraint The constraints to measure against.
     * @return The measured size of the widget.
     */
    virtual Size layout(const BoxConstraint& constraint) {
        return _size;
    }
    
    virtual Event& eventHandler(Event& event) {
        // Default event handling   , can be overridden by subclasses
        return event; // Return 0 to indicate the event was not handled
    }
    
    /*
     * Default rendering logic can be overridden by subclasses
     * This could include drawing the widget's background, borders, etc.
    */
    virtual void render(SkCanvas*canvas,uint32_t offsetX,uint32_t offsetY) {

    }
    
    /*
     * Default rendering logic can be overridden by subclasses
     * This could include drawing the widget's background, borders, etc.
     * This method should be called by the root widget to start the rendering process
     * in the widget tree.
     * The root widget should be responsible for setting up the canvas.
    */
    virtual void render() {
        
    }
    
    virtual std::string toString() const;
    virtual std::string getRuntimeType();
    Widget* getParent() const { return _parent; }
    /**
     * Recommended to override this method in subclasses to handle specific
     * widget updates.
     * Call this base method to ensure setting parent and the dirty flag.
     */
    virtual void setParent(Widget* parent) {
        _parent = parent;
        _needsUpdate = true; // Mark the widget as dirty when the parent is set
    }
    
    void markNeedsUpdate() { _needsUpdate = true; }
    
    
    // static bool canUpdate(Widget oldWidget, Widget newWidget) {
    //     return oldWidget._runtimeType == newWidget._runtimeType &&
    //         oldWidget._key == newWidget._key;
    // }
    
    // core::Element* createElement(uint64_t key) {
        
    //     return new core::Element(this);
    // }
protected:
    Widget() { }
    virtual ~Widget() = default;
    
    bool _focused = false;                      // Indicates if the widget is focused
    bool _visible = true;                       // Indicates if the widget is visible
    bool _active = true;                        // Indicates if the widget is active
    Position _position = Position{0, 0};  // Position of the widget, from the top-left corner to the bottom-right corner
    Size _size = Size{0, 0};        // Size of the widget, used for layout calculations
    BoxConstraint _boxConstraint;               // Constraints for the widget's size and position
    Widget* _parent = nullptr;
    uint32_t _depth = 0;                        // Depth in the widget tree, used for rendering order
    uint64_t _key = 0;
    std::string _runtimeType;
    bool _needsUpdate = true;                   // Indicates if the widget needs to be updated and redrawn
};

class LeafWidget : public Widget
{
public:
private:
};


/**
 * SingleChildWidget is a widget that can have only one child widget.
 */
class SingleChildWidget : public Widget
{
public:
    SingleChildWidget& setChild(Widget& child);
    Widget* getChild();
protected:
    virtual ~SingleChildWidget() = default;
    Widget* _child = nullptr; // Pointer to the single child widget
};


/**
 * MultiChildWidget is a widget that can have multiple child widgets.
 */
class MultiChildWidget : public Widget
{
public:
    MultiChildWidget& setChildren(const std::vector<Widget*>& children) {
        _needsUpdate = true;
        _children = children;
        for (auto& child : _children) {
            child->setParent(this);
        }
        return *this;
    }
    MultiChildWidget& setChildren(Widget* const children[], size_t count) {
        _needsUpdate = true;
        _children.assign(children, children + count);
        for (auto& child : _children) {
            child->setParent(this);
        }
        return *this;
    }

protected:
    std::vector<Widget*> _children; // Vector to hold multiple child widgets
};

struct UniqueKey
{
    UniqueKey() { }
    bool operator==(const UniqueKey& other) const {
        return this == &other;
    }

    bool operator!=(const UniqueKey& other) const {
        return !(*this == other);
    }
};

};  // ui