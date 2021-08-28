#pragma once

#include <iostream>

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include "world_space.h"

#define DEFAULT_MODE 0
#define PANNING_MODE 1
#define ZOOM_MODE 2

// THE SCREEN CONTEXT
class ScreenSpace : public Fl_Box {
public:
	ScreenSpace(int wdx, int wdy, int wdw, int wdh, Fl_Window* win_wd);
private:
	void draw();
	int handle(int evt);
	void world_to_scr(float worldx, float worldy, int& scrx, int& screeny);
	void scr_to_world(int scrx, int screeny, float& worldx, float& worldy);
	void pan();		// change offset values
	void zoom();	// change scale factor
	// The window that ScreenSpace are
	Fl_Widget* m_win;
	// ScreenSpace
	Fl_Offscreen m_screen_buffer;	// ScreenSpace buffer
	int m_sspx, m_sspy;				// ScreenSpace origin
	int m_sspw, m_ssph;				// ScreanSpace size
	float m_buffer_scale;			// m_screen_buffer scale, this is an FLTK implementation detail
	float m_xoff, m_yoff;			// world offset
	float m_scale;					// world scale
	float m_scale_sens;				// scale sensitivite
	// drag
	bool m_drag_constraint;
	float m_drag_sx, m_drag_sy;		// drag start position
	bool m_drag_state;
	
	int m_ssp_state;
};