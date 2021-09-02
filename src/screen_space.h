#pragma once

#include <iostream>
#include <sstream>
#include <cmath>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>

#include "point.h"

// LEFT MOUSE BUTTON MODE
enum class mode {
	default,
	pan,
	zoom,
	draw
};

// DRAW MODE
enum class draw {
	line
};

// THE SCREEN CONTEXT
class ScreenSpace : public Fl_Box {
public:
	ScreenSpace(int wdx, int wdy, int wdw, int wdh, Fl_Double_Window* win);
private:
	void draw();
	int handle(int evt);
	void world_to_scr(float worldx, float worldy, int& scrx, int& screeny);
	void scr_to_world(int scrx, int screeny, float& worldx, float& worldy);
	void set_cursor();
	void pan();		// change offset values
	void zoom();	// change scale factor
	// The window that ScreenSpace are
	Fl_Double_Window* m_win;
	// ScreenSpace
	Fl_Offscreen m_screen_buffer;	// ScreenSpace buffer
	int m_sspx, m_sspy;				// ScreenSpace origin
	int m_sspw, m_ssph;				// ScreanSpace size
	float m_bg_grid_sz;				// background grid size
	float m_buffer_scale;			// m_screen_buffer scale, this is an FLTK implementation detail
	Vector m_off;					// world offset
	float m_scale;					// world scale
	float m_scale_sens;				// scale sensitivite
	// drag
	bool m_drag_constraint;
	float m_drag_sx, m_drag_sy;		// drag start position
	bool m_drag_state;
	mode m_lm_state;				// left mouse button state
	std::string m_md_scr_msg;
	double m_grid_sp;				// grid space
};