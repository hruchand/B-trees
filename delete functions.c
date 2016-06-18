// Deletion function prototype.

int get_neighbor_index( node * n );
node * adjust_root();
node * coalesce_nodes();
node * redistribute_nodes();
node * delete_entry();
node * delete();




node * remove_entry_from_node(node * n, int key, node * pointer) {

	int i, num_pointers;

	// Remove the key and shift other keys accordingly.
	i = 0;
	while (n->keys[i] != key) //search for the key
		i++;
	for (++i; i < n->num_keys; i++) //shift the key
		n->keys[i - 1] = n->keys[i];

	// Remove the pointer and shift other pointers accordingly.
	// First determine number of pointers.
	num_pointers = n->is_leaf ? n->num_keys : n->num_keys + 1;//get number of pointers
	i = 0;
	while (n->pointers[i] != pointer)//search for pointer
		i++;
	for (++i; i < num_pointers; i++)//shift the pointers
		n->pointers[i - 1] = n->pointers[i];


	// Update total no.of keys
	n->num_keys--;

	// Set the other pointers to NULL 
		if (n->is_leaf)
		for (i = n->num_keys; i < order - 1; i++)
			n->pointers[i] = NULL;
	else
		for (i = n->num_keys + 1; i < order; i++)
			n->pointers[i] = NULL;

	return n;
}


/* Gets the index of a node's nearest neighbor
 * to the left if one exists.  If not returns -1 to signify
 * this case.*/
 
int get_neighbor_index( node * n ) {

	int i;
	for (i = 0; i <= n->parent->num_keys; i++)  //Returns index of the key to the left of pointer
		if (n->parent->pointers[i] == n)
			return i - 1;//If n is the leftmost child returns -1

	// Error state.
	printf("Search for nonexistent pointer to node in parent \n");
	exit(EXIT_FAILURE);
}

