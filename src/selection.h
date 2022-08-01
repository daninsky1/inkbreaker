#pragma once

#include <vector>

#include "shape.h"


class Selection {
public:
    Selection(const Selection &) = delete;
    void operator=(const Selection &) = delete;
    
    static Selection *selection();
    std::vector<Shape*> shapes;
protected:
    Selection();
private:
    static Selection *m_instance;
};