/* This draw and edit by mouse input for now */
#pragma once

#include "assert.h"

#include <FL/Enumerations.H>

#include "../main_window.h"
#include "../enumerators.h"
#include "../coordinates.h"
#include "../tree.h"


/* The EditTool is a singleton that store one EditTool subclass at the time is
 * used by View2D to draw inside it.
* First it set the singleton it's passed by the subclass to the EditTool
* constructor.
* Than the subclass can start listening to View2D events to draw.
* The subclass need to call begin_operation() to register a temporary shape it's
* being drawn to screen.
* The end_operation() it's to be called by the subclass in a normal usage, but
* the end_operation() can be accessed outside.
* To change the EditTool in use, simply instantiate another subclass, keep in
* mind the EditTool constructor will delete the last subclass instance, so do
* not keep a direct pointer of an subclass. The only interface secure to use to
* check the current subclass in use is the EditTool singleton.
*/

class EditTool {
public:
    static EditTool *edit_tool() { return m_active_tool; }
    static void free_edit_tool();
    static EditTool *active_tool();
    static bool is_active();
    static Shape *temp_shape();
    /* Check if is in the middle of a unfinished drawing */
    static void begin_operation(Shape *temp_shape);
    static void end_operation();
    static bool is_in_operation();
    virtual int create_main_handle(int evt) = 0;
    virtual int edit_main_handle(int evt) = 0;
protected:
    EditTool(MainWindow *mw, EditTool *et);

    // virtual void register_shape() = 0;
    MainWindow *m_mw;    
private:
    EditTool(EditTool *) = delete;
    void operator=(const EditTool &) = delete;
    
    static EditTool *m_active_tool;
    static Shape *m_temp_shape;
    /* NOTE(daniel): Check if EditTool is in the middle of an operation */
};
