#define _CRT_SECURE_NO_WARNINGS
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif


struct node_s {
	data_type value;
	node_s* left;
	node_s* right;
};


int compare(const void* const a, const void* const b) {
	return *(data_type*)a - *(data_type*)b;
}


node_s* create_node(const data_type value) {
	node_s* const node = (node_s*)malloc(sizeof(node_s));

	if (!node) {
		printf("Fatal error: failed to allocate memory for noe");
	}

	assert(node);

	node->value = value;
	node->left = NULL;
	node->right = NULL;

	return node;
}


node_s* create_tree_nodes(const data_type* const source, const size_t size) {
	if (size == 0) {
		return NULL;
	}

	const size_t middle_index = size / 2;
	const data_type mean_value = source[middle_index];

	node_s* const root_node = create_node(mean_value);
	root_node->left = create_tree_nodes(source, middle_index);
	root_node->right = create_tree_nodes(source + middle_index + 1, size - middle_index - 1);
	return root_node;
}


void delete_tree_node(node_s* const node) {
	assert(node);
	if (node->left) {
		delete_tree_node(node->left);
		node->left = NULL;
	}

	if (node->right) {
		delete_tree_node(node->right);
		node->right = NULL;
	}

	free(node);
}


void print_node(const node_s* const node, const size_t level) {
	if (node->right) {
		print_node(node->right, level + 1);
	}

	for (size_t i = level; i != 0; --i) {
		printf("  ");
	}

	printf("%d\n", node->value);

	if (node->left) {
		print_node(node->left, level + 1);
	}
}


void write_node_values_recursive(const node_s* const node, data_type* const destination, size_t* const current_index) {
	if (!node) {
		return;
	}
	write_node_values_recursive(node->left, destination, current_index);
	destination[*current_index] = node->value;
	++(*current_index);

	write_node_values_recursive(node->right, destination, current_index);
}


void delete_node_from_tree(tree_s* const tree) {
	if (tree->size == 0) {
		printf("Unable to delete element from tree: tree is already empty\n");
		return;
	}

	size_t write_index = 0;
	data_type* const previous_values = (data_type*)malloc(sizeof(data_type) * (tree->size));

	if (!previous_values) {
		printf("Unable to allocate memory");
		return;
	}

	write_node_values_recursive(tree->root, previous_values, &write_index);

	data_type value_to_delete;

	while (true) {
		printf("You can delete one of this values from tree:\n");
		for (size_t i = 0; i < tree->size; ++i) {
			printf("%d ", previous_values[i]);
		}

		printf("\n\nEnter value to remove from tree: \n");

		assert(sizeof(char) == sizeof(data_type)); // checks below are valid only if data_type is char

		int read_value = 0;
		fseek(stdin, 0, SEEK_END);
		if (!scanf("%d", &read_value)) {
			printf("Invalid input. Please enter valid number in range 0-255\n");
			continue;
		}

		if (read_value < 0 || read_value > 255) {
			printf("Entered value is outside range 0-255. Please enter another one in range 0-255\n");
			continue;
		}
		value_to_delete = (data_type)read_value;

		bool found = false;
		for (size_t i = 0; i < tree->size; ++i) {
			found |= previous_values[i] == value_to_delete;
		}

		if (found) {
			break;
		}

		printf("unable to delete node because it is not exists in tree. Try again\n");
	}

	const size_t new_size = tree->size - 1;
	data_type* const new_values = (data_type*)malloc(sizeof(data_type) * (new_size));

	if (!new_values) {
		printf("Unable to allocate memory");
		free(previous_values);
		return;
	}

	size_t new_values_write_index = 0;
	for (size_t i = 0; i < tree->size; ++i) {
		if (previous_values[i] == value_to_delete) {
			continue;
		}

		// can be optimized by using 2 memcpy for all data instead from per-element array assigment
		new_values[new_values_write_index] = previous_values[i];
		++new_values_write_index;
	}

	// no need to use qsort here because element already perfectly sorted
	node_s* const new_tree = create_tree_nodes(new_values, new_size);

	if (tree->root) {
		delete_tree_node(tree->root);
	}

	tree->root = new_tree;
	tree->size = new_size;

	free(previous_values);
	free(new_values);
}


void add_tree_node(tree_s* const tree) {
	const size_t previous_size = tree->size;
	const size_t new_size = previous_size + 1;
	data_type* values = (data_type*)malloc(sizeof(data_type) * (new_size));

	if (!values) {
		printf("Unable to allocate memory");
		return;
	}

	size_t current_index = 0;
	write_node_values_recursive(tree->root, values, &current_index);

	data_type new_value;

	while (true) {
		printf("Enter value to add in range 0-255: \n");

		assert(sizeof(char) == sizeof(data_type)); // checks below are valid only if data_type is char

		fseek(stdin, 0, SEEK_END);
		int read_value = 0;
		if (!scanf("%d", &read_value)) {
			printf("Invalid input. Please enter valid number in range 0-255");
			continue;
		}

		if (read_value < 0 || read_value > 255) {
			printf("Entered value is outside range 0-255. Please enter another one in range 0-255");
			continue;
		}
		new_value = (data_type)read_value;

		bool already_added = false;
		for (size_t i = 0; i < previous_size; ++i) {
			already_added |= values[i] == new_value;
		}

		if (already_added) {
			printf("This value already present in tree. Please enter another value\n");
			continue;
		}
		break;
	}

	values[new_size - 1] = (data_type)new_value;

	qsort(values, new_size, sizeof(data_type), compare);

	node_s* const new_tree = create_tree_nodes(values, new_size);

	if (tree->root) {
		delete_tree_node(tree->root);
	}

	tree->root = new_tree;
	tree->size = new_size;

	free(values);
}


tree_s create_tree(data_type* const source, const size_t size) {
	assert(source);

	tree_s tree;
	tree.root = NULL;
	tree.size = size;

	if (size == 0) {
		return tree;
	}

	qsort(source, size, sizeof(data_type), compare);

	tree.root = create_tree_nodes(source, size);
	return tree;
}


size_t get_node_rank_recursive(const node_s* const node) {
	if (!node) {
		return 0;
	}

	const size_t current_node_rank = (size_t)(node->left != NULL) + (size_t)(node->right != NULL);
	const size_t max_child_rank = max(get_node_rank_recursive(node->left), get_node_rank_recursive(node->right));

	return max(max_child_rank, current_node_rank);
}


size_t get_tree_rank(const tree_s tree) {
	if (!tree.root) {
		return 0;
	}

	return get_node_rank_recursive(tree.root);
}


void delete_tree(tree_s* const tree) {
	if (!tree->root) {
		return;
	}
	delete_tree_node(tree->root);
	tree->root = NULL;
	tree->size = 0;
}

void print_tree(const tree_s tree) {
	if (!tree.root) {
		return;
	}
	print_node(tree.root, 0);
}
