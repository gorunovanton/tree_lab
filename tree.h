#pragma once

typedef char data_type;

struct node_s;

struct tree_s {
	node_s* root;
	size_t size;
};

tree_s create_tree(data_type* source, size_t size);
void delete_tree(tree_s* tree);

void add_tree_node(tree_s* tree);
void delete_node_from_tree(tree_s* tree);

size_t get_tree_rank(tree_s tree);
void print_tree(tree_s tree);