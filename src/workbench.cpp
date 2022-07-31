// Workbench is a test program
// Run make test recipe
#include <stdio.h>
#include <type_traits>
#include <iostream>

#include "tree.h"
#include "objects/polygon.h"

using namespace std;
using namespace Tree;
int main()
{
    Node *root = new Node();
    root->name = "I'm the root node!\n";
    
    Polygon *poly1 = new Polygon();
    Node *wrapper1 = new Node(poly1);
    Polygon *poly2 = new Polygon();
    Node *wrapper2 = new Node(poly2);
    Node *node1 = new Node();
    Node *node2 = new Node();
    
    root->add_child(wrapper1);
    root->add_child(wrapper2);
    root->add_child(node1);
    root->add_child(node2);
    
    vector<Shape*> shapes;
    root->get_shapes_dfs(shapes);
    
    printf("%lu\n", shapes.size());
    printf("%d\n", shapes[0] == wrapper1->shape());
    printf("%d\n", shapes[1] == wrapper2->shape());
    assert(shapes[0] == wrapper1->shape());
    assert(shapes[1] == wrapper2->shape());
}
