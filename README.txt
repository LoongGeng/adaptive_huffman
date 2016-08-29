Adaptive huffman coding and decoding software (version 1.0) written by C.Geng.

1.description of algorithm

In this software, Vitter's adaptive huffman algorithm is used. In his algorithm, the huffman tree is constructed dynamicly. When a symbol is being coded, the tree will be traversed to find the symbol. If the symbol has already been in the tree, the node which contains the symbol should be slided and the weight of the symbol should be increased, which is called slide and increment. 

If the symbol is not in the tree, find the NYT node which has 0 weight. Then, the new NYT node will be created as the old NYT node's left child, and the new symbol node will also be created as the old NYT node's right child. The old NYT node becomes internal node. Then do slide and increment steps on the new symbol node and new internoal node.

The slide and increment step is described blow.

In the first, if the node is leaf node, interchange the node with the leader of its block (the leaf node which has the same weight but the highest number). Then, slide the node with nodes in its previous block (the class of internodes which have the same weight as the node to be slided). In the next, increase its weight and slide its new parent node. If the node is internal node, it should be slided in its previous block(the class of leaf nodes which have one more weight than the new parent node). Then, increase the node's weight and slide its former parent node. If the node is the root, stop slide and increament step.

2.implementation mechanism

The node structure in adaptive huffman tree is modified from ordinary binary tree. two pointers are used to point its previous node in the block and its parent node. The symbol and its weight is also stored in the node as descirbed blow.

typedef struct huffman_tree_node {
    int sym;
    int wgt; // times of symbol
    struct huffman_tree_node *lchild, *rchild, *parent; // tree structure
    struct huffman_tree_node *pre; // block structure
}

When a new symbol node and a new NYT node are created, the pointers of the new symbol will both point to its parent, however, in the new NYT node, a pointer will point to the new symbol node, another pointer will point to its parent.

When interchanging two nodes position, a simple way is used. If the two nodes are both leaf nodes, just interchange their symbol and weight. If they are an internal node and a leaf node, interchange not only their symbol and weight but also their parent nodes and their children nodes.