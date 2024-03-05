/*  libhuffman.c */

#include "libhuffman.h"

Node * _createLeafNode(unsigned char value, size_t occurances)
{
    Node * n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->value = value;
        n->occurances = occurances;
        n->left_node = n->right_node = n->parent_node = NULL;
        n->code = 0;
        n->code_length = 0;
    }
    return n;
}

Node * _createBranchNode(Node * left_node, Node * right_node)
{
    Node * n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->value = 0;
        n->left_node = left_node;
        n->right_node = right_node;
        if (left_node) left_node->parent_node = n;
        if (right_node) right_node->parent_node = n;
        n->occurances = (left_node ? left_node->occurances : 0) + (right_node ? right_node->occurances : 0);
        n->code = 0;
        n->code_length = 0;
        n->parent_node = NULL;
    }
    return n;
}

void _freeNode(Node * node)
{
    if (!node) return;
    if (node->left_node) _freeNode(node->left_node);
    if (node->right_node) _freeNode(node->right_node);
    node->value = 0;
    node->code = 0;
    node->code_length = 0;
    node->occurances = 0;
    node->parent_node = NULL;
    node->right_node = NULL;
    node->left_node = NULL;
    free(node);
}

void _freeNodes(Node ** nodes) 
{
    if (!nodes) return;
    for (size_t i = 0; i < 256; i++) {
        if (nodes[i]) {
            _freeNode(nodes[i]);
            nodes[i] = NULL;
        }
    }
    free(nodes);
}

void _sortNodes(Node ** nodes, size_t total_leaf_nodes) 
{
    if (!nodes) return;

    for (size_t i = 0; i < total_leaf_nodes; i++) {
        for (size_t j = total_leaf_nodes - 1; j > i; j--) {
            if (nodes[j]->occurances < nodes[i]->occurances) {
                Node * temp_node = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp_node;
            }
        }
    }
}

void _mergeLowestOccuranceNodes(Node ** nodes, size_t nodes_count) 
{
    if (!nodes || nodes_count < 2) return;
    nodes[0] = _createBranchNode(nodes[0],nodes[1]);
    for (size_t i = 1; i < nodes_count - 1; i++) {
        nodes[i] = nodes[i + 1];
    }
    nodes[nodes_count - 1] = NULL;
}

bool _isLeafNode(Node * n)
{
    return n && n->parent_node && n->left_node == NULL && n->right_node == NULL;
}

void _setNodeCode(Node * n, Node * leaf_node)
{
    if (!n) return;
    if (n->parent_node == NULL) return;
    if (n == n->parent_node->right_node) leaf_node->code += 1 << leaf_node->code_length;
    leaf_node->code_length++;
    _setNodeCode(n->parent_node, leaf_node);
}

Node ** _createDataNodes(unsigned char * source_data, size_t source_data_length, size_t * data_node_count_ptr)
{
    // Make sure the calling routine has passed valid parameter values.  If not exit returning no data.
    if (!source_data || !data_node_count_ptr || source_data_length == 0) return NULL;

    // Initialise the user provided data node count variable.
    *data_node_count_ptr = 0;

    // Create a node pointer array which will hold a pointer to a data node for each possible byte value.
    Node ** nodes = (Node **) malloc(sizeof(Node *) * 256);

    // If we can't create the node pointer array, exit returning no data.
    if (!nodes) return NULL;
    else {
        // Otherwise, try to create node data structures for each byte value and store the pointers from them in the array.
        for (size_t i = 0; i < 256; i++) {
            nodes[i] = (Node *) malloc(sizeof(Node));
            if (!nodes[i]) {
                // If we can't create a node data structure for a certain byte value, we need to free any previously
                // allocated node data structures, then the node pointer array and return no data.
                for (size_t j = 0; j < i; j++) {
                    free(nodes[j]);
                    nodes[j] = NULL;
                }
                free(nodes);
                return NULL;
            } else {
                // If we can create a node data structure, we need to initialise it.
                nodes[i]->value = i;
                nodes[i]->left_node = nodes[i]->right_node = nodes[i]->parent_node = NULL;
                nodes[i]->occurances = 0;
                nodes[i]->code = 0;
                nodes[i]->code_length = 0;
            }
        }
    }

    // Now we need to increment the occurance member of each byte value's data structure each time that value is found in the
    // source data.
    for (size_t i = 0; i < source_data_length; i++) {
        nodes[source_data[i]]->occurances++;
    }

    // We then sort the node data structures in ascending order of occurance.
    _sortNodes(nodes, 256);

    // We can get rid of any node data structures where the byte value has never occurred in the source data.
    size_t ni = 0;
    while (nodes[ni]->occurances == 0) {
        _freeNode(nodes[ni]);
        nodes[ni] = NULL;
        ni++;
    }

    // We also need to move any node data structures for bytes values that have occurred in the source data so that they appear
    // at the start of the nodes pointer array.
    while (ni < 256) nodes[*data_node_count_ptr++] = nodes[ni++];

    // Return the nodes pointer array to the calling method.
    return nodes;
}

u_char * Huffman_Encode(u_char * source_data, size_t source_data_length, size_t * encoded_data_length_ptr)
{
    // Make sure the calling method has passed sensible parameter values.  If not, return no data.
    if (!source_data || !encoded_data_length_ptr || source_data_length == 0) return NULL;

    // Create the nodes pointer array from the source data.
    size_t nodes_count;
    Node ** nodes = _createDataNodes(source_data, source_data_length, &nodes_count);

    // Initialise the encoded_data_length_ptr variable's value.
    *encoded_data_length_ptr = 0;

    // If we couldn't create the nodes pointer array, return no data.
    if (!nodes) return NULL;

    // Make of copy of the node count as this will be the same as the number of leaf nodes in the data.
    size_t total_leaf_nodes = nodes_count;

    // Create an array of node data structure pointers.
    // Each entry in this array will point to a leaf node.
    Node ** leaf_nodes = (Node **) malloc(sizeof(Node *) * total_leaf_nodes);
    if (!leaf_nodes) {
        // If we can't create the leaf node pointer array, free any data allocated to the nodes and
        // return no data.
        _freeNodes(nodes);
        return NULL;
    } else {
        // Otherwise, make a copy of the pointers stored currently in the nodes array (as these will 
        // all be leaf nodes currently).
         for (size_t i = 0; i < total_leaf_nodes; i++) {
            leaf_nodes[i] = nodes[i];
        }
    }

    // We then need to create the binary tree, which we will use to generate the huffman codes.
    // To do this we join the two nodes with the lowest occurances using a branch node, then sort the 
    // nodes (including the newly created branch node) in ascending order of occurance.  These steps
    // are repeated until we are left with a single branch node, whose branches can be followed to 
    // generate the leaf node codes.
    while (nodes_count > 1) {
        _mergeLowestOccuranceNodes(nodes, nodes_count);
        nodes_count--;
        _sortNodes(nodes, nodes_count);
    }

    // Generate the leaf node codes.
    for (size_t i = 0; i < total_leaf_nodes; i++) {
        _setNodeCode(leaf_nodes[i], leaf_nodes[i]);
    }

    // TODO: Transfer the node byte values, code values and code lengths to a packed code lookup table.
    // TODO: Encode the source data using packed codes.
    // NOTE: The steps above will require the creation of a library to pack variable length binary values.
    // TODO: Return the encoded data and encoded data length.

    // Currently we're just freeing the created data structures and arrays and returning nothing.   
    free(leaf_nodes);
    _freeNodes(nodes);
    return NULL;
}