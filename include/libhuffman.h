/*  libhuffman.c */

#ifndef COM_PLUS_MEVANSPN_LIBHUFFMAN
#define COM_PLUS_MEVANSPN_LIBHUFFMAN

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _Node {
    unsigned char value;
    struct _Node * left_node, * right_node, * parent_node;
    size_t occurances;
    size_t code;
    size_t code_length;
} Node;

// Public methods.
u_char * Huffman_Encode(u_char * source_data, size_t source_data_length, size_t * encoded_data_length_ptr);

// Private methods used by the public methods above.  Do not use unless you know what you're doing!
Node * _createLeafNode(unsigned char value, size_t occurances);
Node * _createBranchNode(Node * left_node, Node * right_node);
void _freeNode(Node * node);
void _freeNodes(Node ** nodes);
void _sortNodes(Node ** nodes, size_t total_leaf_nodes);
void _mergeLowestOccuranceNodes(Node ** nodes, size_t nodes_count);
bool _isLeafNode(Node * n);
void _setNodeCode(Node * n, Node * leaf_node);
Node ** _createDataNodes(unsigned char * source_data, size_t source_data_length, size_t * data_node_count_ptr);

#endif