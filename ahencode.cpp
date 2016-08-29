#include <iostream>
#include <bitset>
#include <cstring>
#include <cstdlib>

#define NYT -129 // define NYT node symbol
#define internal_node -130 //define internal node symbol

typedef struct huffman_tree_node {
    int sym;
    int wgt; // times of symbol
    struct huffman_tree_node *lchild, *rchild, *parent; // tree structure
    struct huffman_tree_node *pre; // block structure
} h_node, *p_node; // tree structure

p_node initial_huffman_tree (); // initialise adaptive huffman tree
void des_huffman_tree (p_node huffman_tree); // destroy adaptive huffman tree
p_node find_symbol (p_node huffman_tree, int symbol); // find corresponding node in huffman tree by symbol
std::string coding_offset (p_node sym_node); // compute the offset in coding
p_node slide_and_increment (p_node inc_node) ; // slide node and increment the weight of the node
std::string update_huffman_tree (p_node huffman_tree, int symbol);// update whole huffman tree
p_node find_the_leader_in_block (p_node node); // find the first node in the block
void interchange_with_pre_node_pre_block (p_node node); // interchange the node with its previous node

int main(int argc, char *argv[]) {
	std::string str_input;
	while (std::getline(std::cin, str_input)) { // if EOF is received, end the program
        p_node huffman_tree = initial_huffman_tree(); 
        if (argc == 2) { // receive command line argument
            if (strcmp(argv[1], "-s") == 0){ // output with space
				int i;
                for (i = 0; i < str_input.length() - 1; i++) {
                    std::cout << update_huffman_tree(huffman_tree, (int) str_input[i]) <<" ";
                }
			std::cout << update_huffman_tree(huffman_tree, (int) str_input[i]);
            }
	    else { // output without space
			for (int i = 0; i < str_input.length(); i++) {
                    std::cout << update_huffman_tree(huffman_tree, (int) str_input[i]);
                }
	    }
                
        }
		else { // output without space
			for (int i = 0; i < str_input.length(); i++) {
                    std::cout << update_huffman_tree(huffman_tree, (int) str_input[i]);
                }
		}
        std::cout << std::endl;
        des_huffman_tree(huffman_tree); // destroy huffman tree
	}
    return 0;
}

p_node initial_huffman_tree () {
	p_node tree = (p_node) malloc (sizeof(h_node)); // allocate memory space for huffman tree
	tree -> lchild = NULL;
	tree -> rchild = NULL; 
	tree -> parent = NULL;
    tree -> pre = NULL;
    tree -> wgt = 0;
    tree -> sym = NYT; // initialise root node
    return tree; // return the root
}

void des_huffman_tree (p_node huffman_tree) {
    if (huffman_tree != NULL) { // if the node is not free
        des_huffman_tree(huffman_tree -> lchild); // destroy its left subtree
        des_huffman_tree(huffman_tree -> rchild); // destroy its right subtree
        free(huffman_tree); // free the node
    }
}

p_node find_symbol (p_node huffman_tree, int symbol) {
    if (huffman_tree != NULL) {
        if (huffman_tree -> sym == symbol)
            return huffman_tree;
        else {
            p_node tmp_node = find_symbol(huffman_tree -> lchild, symbol); // find in its left subtree
            if (tmp_node != NULL)
                return tmp_node;
            else {
                tmp_node = find_symbol(huffman_tree ->rchild, symbol); // find in its right subtree
                if (tmp_node != NULL)
                    return tmp_node;
                else
                    return NULL;
            }
        }
    }
    else
        return NULL;
}


std::string coding_offset (p_node sym_node) {
    if (sym_node != NULL) {
        if (sym_node -> parent != NULL) {
            p_node tmp = sym_node -> parent;
            if (tmp -> lchild == sym_node) { //if the node is its parent's lchild 
                return coding_offset(tmp) + "0"; // reutrn "0"
            }
            else
                return coding_offset(tmp) + "1"; // return "1"
        }
        else
            return "";
    }
    else
        return "";
}
p_node slide_and_increment (p_node inc_node) {
    p_node node_to_be_slided = inc_node;
    if (inc_node -> pre != NULL && inc_node -> pre -> parent != NULL) {
        p_node leader_node_in_pre_block = find_the_leader_in_block(inc_node->pre);
        if ((node_to_be_slided->sym != internal_node && leader_node_in_pre_block->sym == internal_node
             && node_to_be_slided->wgt == leader_node_in_pre_block->wgt)
            || (node_to_be_slided->sym == internal_node && leader_node_in_pre_block->sym != internal_node
                && node_to_be_slided->wgt == leader_node_in_pre_block->wgt - 1)) { // if the node is external node, slide it with the internal node in its previous block , or if the node is internal, slide it with the external node in its previous block
            do {
                interchange_with_pre_node_pre_block(node_to_be_slided);
                node_to_be_slided = node_to_be_slided->pre; // slide with previous node
            } while (leader_node_in_pre_block != node_to_be_slided);
        }
    }
    node_to_be_slided -> wgt += 1;
    if (node_to_be_slided -> sym != internal_node)
        return node_to_be_slided -> parent;
    else
        return inc_node -> parent;
}

std:: string update_huffman_tree (p_node huffman_tree, int symbol) {
    p_node corresponding_node = NULL;
    p_node leaf_to_increment = NULL;
    std::string sym_coded;
    if ((corresponding_node = find_symbol(huffman_tree, symbol))!= NULL) { // if the symbol has already been in the tree
        sym_coded = coding_offset(corresponding_node); // compute offset
        p_node leader_of_block = find_the_leader_in_block(corresponding_node); // find the leader
        int tmp_symbol = leader_of_block -> sym; //interchange corresponding node with the leader
        leader_of_block -> sym = corresponding_node -> sym;
        corresponding_node -> sym = tmp_symbol;
        int tmp_wgt = leader_of_block -> wgt;
        leader_of_block -> wgt = corresponding_node -> wgt;
        corresponding_node -> wgt = tmp_wgt;
        corresponding_node = leader_of_block;
        if (corresponding_node -> parent -> lchild ->sym == NYT) { // if the node is NYT's sibling
            leaf_to_increment = corresponding_node;
            corresponding_node = corresponding_node -> parent;
        }
    }
    else {
        corresponding_node = find_symbol(huffman_tree, NYT); // find NYT node
        std::bitset<8> b (symbol);
        sym_coded = coding_offset(corresponding_node) + b.to_string(); // compute offset
        corresponding_node -> sym = internal_node;
        corresponding_node -> rchild = (p_node)malloc(sizeof(h_node)); // create its two children
        corresponding_node -> lchild = (p_node)malloc(sizeof(h_node));
        corresponding_node -> rchild -> lchild = NULL;
        corresponding_node -> rchild -> rchild = NULL;
        corresponding_node -> lchild -> lchild = NULL;
        corresponding_node -> lchild -> rchild = NULL;
        corresponding_node -> rchild -> parent = corresponding_node;
        corresponding_node -> lchild -> parent = corresponding_node;
        corresponding_node -> rchild -> sym = symbol; // set new symbol node
        corresponding_node -> lchild -> sym = NYT; // set new NYT node
        corresponding_node -> rchild -> pre = corresponding_node;
        corresponding_node -> lchild -> pre = corresponding_node -> rchild;
        corresponding_node -> rchild -> wgt = 0;
        corresponding_node -> lchild -> wgt = 0;
        leaf_to_increment = corresponding_node -> rchild;
    }
    while (corresponding_node -> parent != NULL)
        corresponding_node = slide_and_increment (corresponding_node); // slide and increase the corresponding node
    corresponding_node -> wgt += 1;  // increase the weight of corresponding node
    if (leaf_to_increment != NULL)
        p_node tmp_node = slide_and_increment(leaf_to_increment); // slide and increase the external node which should be increased
    return sym_coded;
}

p_node find_the_leader_in_block (p_node node) {
    p_node tmp_node = node;
    int wgt = tmp_node -> wgt;
    int sym = tmp_node -> sym;
    if (sym == internal_node) {  // if it is internal node
        while (tmp_node -> pre != NULL && tmp_node -> pre -> sym == sym && tmp_node -> pre -> wgt == wgt && tmp_node -> pre -> parent != NULL) // find leader in the internal node block
            tmp_node = tmp_node -> pre;
    }
    else {
        while (tmp_node -> pre != NULL && tmp_node -> pre -> sym != internal_node && tmp_node -> pre -> wgt == wgt && tmp_node -> pre -> parent != NULL ) // find leader in the external node block
            tmp_node = tmp_node -> pre;
    }
    return tmp_node;
}

void interchange_with_pre_node_pre_block (p_node node) {
    p_node pre_node = node -> pre; // find its previous node
    int tmp_sym = node -> sym; // interchange symbol
    node -> sym = pre_node -> sym;
    pre_node -> sym = tmp_sym;
    int tmp_wgt = node -> wgt; // interchange weight
    node -> wgt = pre_node -> wgt;
    pre_node -> wgt = tmp_wgt;
    if (tmp_sym != internal_node) { // if it is external node
        node -> lchild = pre_node -> lchild; // interchange children
        pre_node -> lchild -> parent = node;
        pre_node -> rchild -> parent = node;
        node -> rchild = pre_node -> rchild; 
        pre_node -> lchild = NULL;
        pre_node -> rchild = NULL;
    }
    else {
        pre_node -> lchild = node -> lchild; // interchange children and children's parents
        pre_node -> rchild = node -> rchild;
        node -> lchild -> parent = pre_node;
        node -> rchild -> parent = pre_node;
        node -> lchild = NULL;
        node -> rchild = NULL;
    }
}
