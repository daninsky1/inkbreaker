#include "tree.h"


using namespace Tree;

size_t Node::size()
{
    return m_childs.size();
}

void Node::link(Node *node)
{
    m_childs.push_back(node);
}

void Node::parent(Node *node) {
    assert(node != this);
    m_parent = node;
    m_parent->add_child(this);
}

void Node::add_child(Node *node)
{
    m_childs.push_back(node);
}

vector<Node*> Node::nodes()
{
    return m_childs;
}