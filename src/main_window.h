#pragma once

#include <iostream>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>

#include "screen_space.h"

class MainWindow : public Fl_Double_Window {
public:
    MainWindow(int sspw, int ssph, const char* l = "");
    ~MainWindow();
private:
    Fl_Menu_Bar m_mbar;
    ScreenSpace* m_ssp;
};
