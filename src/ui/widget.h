#pragma once

#include <string>
#include <format>
#include <vector>

#include "events.h"
#include "layout.h"
#include "graphics/renderer.h"

namespace ui
{
/**
 * @class Widget
 * @brief Base class representing a UI widget with box model layout capabilities.
 *
 * This class defines the fundamental properties and behaviors of a UI widget,
 * including size management, event handling, layout constraints, and rendering.
 * It serves as the base for all concrete widget implementations.
 *
 * The widget maintains its size, parent relationship, and layout constraints.
 * It provides an interface for event handling, layout calculation, and rendering,
 * which must be implemented by subclasses.
 *
 * @note Subclasses must override the pure virtual methods:
 *       - layout()
 *       - render()
 */
class Widget
{
public:
    uint32_t getWidth() const { return _size.width; }
    uint32_t getHeight() const { return _size.height; }
    Size getSize() const { return _size; }
    void setSize(Size size) { _size = size; };
    Widget* getParent() const { return _parent; }
    virtual void setParent(Widget* parent) { _parent = parent; }

    /**
     * Normalize the widget's size based on the given constraints
     * @param constraint
     * @return
     */
    Size normalize(const BoxConstraints& constraint) ;
    virtual std::string toString() const;
    virtual std::string getRuntimeType();

    void markNeedsUpdate() { _needsUpdate = true; }

    /**
     * Handles an event for the widget.
     *
     * This method should be overridden by concrete subclasses to implement
     * custom event handling logic.
     *
     * If a parent class provides a concrete implementation of this method,
     * subclasses should call the base implementation
     * (e.g., Widget::eventHandler(event))
     * before or after their own logic, depending on the desired event
     * propagation behavior.
     *
     * To indicate that an event was handled, set `event.handled = true` within
     * the method. Unhandled events should leave this flag as `false`.
     *
     * @param event The event to process.
     * @return A reference to the same event, possibly modified.
     */
    virtual Event& eventHandler(Event& event) { return event; };

    /**
     * Sets the widget's constraints and performs layout calculation.
     *
     * This method should be overridden by concrete subclasses to measure
     * the widget's size based on the given constraints and to perform
     * any necessary layout logic.
     *
     * @param boxConstraints The constraints that define the minimum and maximum
     *                   allowable size for this widget.
     * @return The measured size of the widget after applying the constraints
     *         and performing layout.
     */
    virtual Size layout(const BoxConstraints& boxConstraints) = 0;

    /**
     * Renders the widget onto the given Skia canvas.
     *
     * This method should be overridden by concrete subclasses to perform
     * custom drawing logic using the provided `SkCanvas` instance.
     *
     * The `offsetX` and `offsetY` parameters represent the absolute position
     * offset of the widget relative to its parent or the root canvas. These
     * offsets should be applied to ensure correct placement of the widget in
     * the render tree.
     *
     * @param renderer  The Skia canvas to draw onto.
     * @param offset The offset to apply during rendering.
     */
    virtual void render(gfx::Renderer* renderer, Position offset) = 0;
protected:
    Widget() { }
    virtual ~Widget() = default;

    Size _size = Size{0, 0};        // Size of the widget, used for layout calculations
    BoxConstraints _boxConstraints;               // Box constraints for the widget's size and position
    Widget* _parent = nullptr;                  // The parent widget, if is nullptr assumes it's the root widget

    // NOTE(Daniel S): Not used bellow
    bool _focused = false;                      // Indicates if the widget is focused
    bool _visible = true;                       // Indicates if the widget is visible
    bool _active = true;                        // Indicates if the widget is active
    uint32_t _depth = 0;                        // Depth in the widget tree, used for rendering order
    uint64_t _key = 0;                          // Unique key of the Widgeet
    std::string _runtimeType;                   // The widget type name
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
    Position _childPosition = Position{0, 0};
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