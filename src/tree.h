#pragma once

#include <vector>
#include <string>

#include <assert.h>


using namespace std;

namespace Tree {    
    class Node {
    public:
        string name;
        string user_def_name;
        size_t size();
        void link(Node *node);
        void parent(Node *node);
        void add_child(Node *node);
        vector<Node*> nodes();
    private:
        vector<Node*> m_childs;
        Node *m_parent;
    };

    // ObjectLeaf are tree leafs only, Node cannot point to ObjectLeaf
    class ObjectLeaf : public Node {
        // ObjectLeaf();
        size_t size() = delete;
        void link(Node) = delete;
    };
}
