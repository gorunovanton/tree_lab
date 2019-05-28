#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "tree.h"


int main() {
	// data_type source_values[] = {1};
	// data_type source_values[] = {1, 3};
	data_type source_values[] = {1, 4, 3, 2, 5};
	// data_type source_values[] = {1, 4, 3, 2, 5, 6, 7, 8};
	// data_type source_values[] = {1, 4, 3, 2, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	tree_s tree = create_tree(source_values, sizeof(source_values) / sizeof(data_type));

	int exit = 0;
	while (!exit) {
		printf("Enter e to exit\n");
		printf("Enter p to print tree\n");
		printf("Enter r to print tree rank\n");
		printf("Enter a to add node to tree\n");
		printf("Enter d to delete node from tree\n");
		printf("Enter command: ");

		fseek(stdin, 0, SEEK_END);
		const int c = getchar();
		switch (c) {
			case 'a': {
				add_tree_node(&tree);
				break;
			}
			case 'd': {
				delete_node_from_tree(&tree);
				break;
			}
			case 'e': {
				exit = 1;
				break;
			}
			case 'p': {
				print_tree(tree);
				break;
			}
			case 'r': {
				const auto rank = get_tree_rank(tree);
				printf("Tree rank is %zu\n", rank);
				break;
			}
			default: {
				printf("Invalid command. Try again\n");
			}
		}
		printf("\n-------------------------------------\n");
	}

	delete_tree(&tree);

	return 0;
}
