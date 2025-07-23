#pragma once

#include <cstdint>
#include <string>
#include <format>
#include <vector>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include <include/core/SkCanvas.h>

#include "events.h"

namespace ui
{
class Widget
{
public:
    uint32_t getX() const { return _x; }
    uint32_t getY() const { return _y; }
    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    void setPosition(uint32_t x, uint32_t y) { _x = x; _y = y; }
    void setSize(uint32_t width, uint32_t height) { _width = width; _height = height; }
    
    virtual Event& eventHandler(Event& event) {
        // Default event handling, can be overridden by subclasses
        return event; // Return 0 to indicate the event was not handled
    }
    
    /*
     * Default rendering logic can be overridden by subclasses
     * This could include drawing the widget's background, borders, etc.
    */ 
    virtual void render(SkCanvas* canvas,  uint32_t offsetX = 0, uint32_t offsetY = 0) {  
        SDL_Log("Rendering widget at (%d, %d) with size (%d, %d)", _x + offsetX, _y + offsetY, _width, _height);
        
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
    
    
    // static bool canUpdate(Widget oldWidget, Widget newWidget) {
    //     return oldWidget._runtimeType == newWidget._runtimeType &&
    //         oldWidget._key == newWidget._key;
    // }
    
    // core::Element* createElement(uint64_t key) {
        
    //     return new core::Element(this);
    // }
protected:
    Widget(const uint64_t key) : _key(key) { }
    Widget()                   : _key(0)   { }
    virtual ~Widget() = default;
    
    bool _focused = false;  // Indicates if the widget is focused
    bool _visible = true;   // Indicates if the widget is visible
    bool _active = true;    // Indicates if the widget is active
    uint32_t _x = 0, _y = 0;          // Position of the widget, from the top-left corner to the bottom-right corner
    uint32_t _width, _height; // Size of the widget
private:
    Widget* _parent = nullptr;
    uint32_t _depth = 0;    // Depth in the widget tree, used for rendering order
    const uint64_t _key;
    std::string _runtimeType;
};

class LeafWidget : public Widget
{
public:
    LeafWidget(const uint64_t key) : Widget(key) { }
    LeafWidget()                   : Widget(0)   { }
private:
};

class SingleChildWidget : public Widget
{
public:
    SingleChildWidget& setChild(Widget& child);
    Widget* getChild();
protected:
    SingleChildWidget(const uint64_t key) : Widget(key) { }
    SingleChildWidget()                   : Widget(0)   { }
    virtual ~SingleChildWidget() = default;
    Widget* _child = nullptr; // Pointer to the single child widget
};

class MultiChildWidget : public Widget
{
public:
protected:
    MultiChildWidget(const uint64_t key) : Widget(key) { }
    MultiChildWidget()                   : Widget(0)   { }
    std::vector<Widget*> _children; // Vector to hold multiple child widgets
};

};  // ui