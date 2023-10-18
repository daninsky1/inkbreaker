#include "selection.h"


Selection *Selection::m_instance;

Selection::Selection()
{
}

Selection *Selection::selection()
{
    if(!Selection::m_instance) {
        Selection::m_instance = new Selection();
    }
    return Selection::m_instance;
}