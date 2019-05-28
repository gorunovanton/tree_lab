#ifndef TREE_H
#define TREE_H
#include <stddef.h>

typedef char data_type;

typedef struct _node_s {
	data_type value;
	struct _node_s* left;
	struct _node_s* right;
} node_s;


typedef struct {
	node_s* root;
	size_t size;
} tree_s;

tree_s create_tree(data_type* source, const size_t size);
void delete_tree(tree_s* tree);

void add_tree_node(tree_s* tree);
void delete_node_from_tree(tree_s* tree);

size_t get_tree_rank(const tree_s tree);
void print_tree(const tree_s tree);

#endif
