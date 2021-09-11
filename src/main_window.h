#pragma once

#include <iostream>

#include <FL/fl_ask.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>

#include "shapes.h"
#include "screen_space.h"

namespace ib {
	enum class mode {
		default,
		pan,
		zoom,
		draw
	};

	// DRAW MODE
	enum class draw {
		line,
		rect
	};
}

class MainWindow : public Fl_Double_Window {
public:
    MainWindow(int sspw, int ssph, const char* l = "");
    ~MainWindow();
	void set_mode();
	void set_draw_state();

	/* Widgets */
    Fl_Menu_Item* menutable;
    Fl_Menu_Bar menubar;
	ScreenSpace* screensp;
private:
	/* States*/
	ib::mode m_mode_state = ib::mode::default;
	ib::draw m_draw_state = ib::draw::line;

	std::vector<sShape*> m_shapes;

};
