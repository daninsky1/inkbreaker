#pragma once

#include <vector>
#include <string>

#include <assert.h>

#include "objects/shape.h"


using namespace std;

/* NOTE(daniel): Nodes with a shape value CANNOT have children! In this case
* they are mere shape wrappers. So they are leaves only.
* If Node is contructed with a shape value *m_children becomes null, so no
* operation on m_children will be possible without breaking everything. However
* in the future make a better error handling for this case.
*/
namespace Tree {    
    class Node {
    public:
        string name;
        string user_def_name;
        
        Node();
        Node(Shape *shape);
        
        Shape *shape() { return m_value; }
        void traverse_tree();
        void get_shapes_dfs(vector<Shape*> &shapes);
        Node *parent();
        void add_child(Node *child);
        vector<Node*> nodes();
        size_t size();
    private:
        Node *m_parent = nullptr;
        vector<Node*> *m_children = nullptr;
        Shape *m_value = nullptr;
        void parent(Node *parent);
    };
}
