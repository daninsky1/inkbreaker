#include "tree.h"

#include <vector>
#include <string>


using namespace std;

class Node;
class ObjectLeaf {
    string name;
    Node *parent;
};

class Node : public ObjectLeaf {
public:
    size_t size() { return nodes_and_leafs.size(); }
    void link(Node *node) { nodes_and_leafs.push_back(node); }
private:
    vector<ObjectLeaf*> nodes_and_leafs;
};

// ObjectLeaf are tree leafs only, Node cannot point to ObjectLeaf
