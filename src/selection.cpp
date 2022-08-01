#include "selection.h"


Selection::Selection()
{
}

Selection *Selection::selection()
{
    if(!m_instance) {
        m_instance = new Selection();
    }
    return m_instance;
}