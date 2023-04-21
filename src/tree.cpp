#include "tree.h"


using namespace Tree;

Node::Node() :
    m_children{ new vector<Node*>() }
{
    name = "I'm Pure Node!\n";
}

Node::Node(Shape *shape) {
    m_value = shape;
    name = "I'm Leaf Node!\n";
}

void Node::traverse_tree()
{
    if (!m_value) {
        for (size_t i = 0; i < m_children->size(); i++) {
            (*m_children)[i]->traverse_tree();
        }
    }
    printf("%s", name.c_str());
}

void Node::get_shapes_dfs(vector<Shape*> &shapes)
{
    if (m_value) {
        shapes.push_back(m_value);
        return;
    }
    for (size_t i = 0; i < m_children->size(); i++) {
        (*m_children)[i]->get_shapes_dfs(shapes);
    }
}

void Node::draw_shapes_dfs()
{
    if (m_value) {
        m_value->draw();
        return;
    }
    for (size_t i = 0; i < m_children->size(); i++) {
        (*m_children)[i]->draw_shapes_dfs();
    }
}

void Node::parent(Node *parent) {
    assert(parent != this);
    m_parent = parent;
}

void Node::add_child(Node *child)
{
    m_children->push_back(child);
    child->parent(this);
}

vector<Node*> Node::nodes()
{
    return *m_children;
}

size_t Node::size()
{
    printf("%lu\n", m_children->size());
    return m_children->size();
}
