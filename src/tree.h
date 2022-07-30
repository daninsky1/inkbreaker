#pragma once

#include <vector>
#include <string>

#include <assert.h>


using namespace std;

class Node {
public:
    string name;
    string user_def_name;
    size_t size() { return m_childs.size(); }
    void link(Node *node) { m_childs.push_back(node); }
    void parent(Node *node) {
        assert(node != this);
        m_parent = node;
        m_parent->add_child(this);
    }
    void add_child(Node *node) { m_childs.push_back(node); }
    vector<Node*> nodes() { return m_childs; }
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
