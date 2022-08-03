/* This draw and edit by mouse input for now */
#pragma once

#include "assert.h"

#include <FL/Enumerations.H>

#include "main_window.h"
#include "enumerators.h"
#include "coordinates.h"
#include "objects/polygon.h"


class EditTool {
public:
    void is_active(bool ia);
    bool is_active();
    EditTool *active_tool();
private:
    static bool m_is_active;
    static EditTool *m_active_tool;
    // Draw tool_type = Draw::polygon;
};

class PolygonTool : public EditTool {
public:
    PolygonTool(MainWindow *mw) {
        m_mw = mw;
        is_active(true);
    }
    int create_main_handle(int evt);
    int edit_main_handle(int evt);
    // void draw_nodes();
    // void draw_outline();
private:
    Polygon *active_polygon;
    MainWindow *m_mw;
};
