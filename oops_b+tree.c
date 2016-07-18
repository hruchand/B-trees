#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_ORDER 4




typedef struct tree {
    struct   node * (*insert_in_tree)( struct node*, int, int);
  struct node * (*delete_from_tree)(struct node*,int);
  void (*find_from_tree)(struct node*, int);
  void (*print_the_tree)(struct node*);

}tree;

typedef struct tree *my_tree;

typedef struct record
{
    int value;
} record;



typedef struct node
{
    void ** pointers;
    int * keys;
    struct node * parent;
    bool is_leaf;
    int num_keys;
    struct node * next;
  //  struct  working tree1;

} node;
node * insert(node *root, int key, int value);



my_tree tree_init()
{
my_tree self = (my_tree)malloc(sizeof(struct tree));

 self->insert_in_tree = insert;
 return self;


}


//Function pointers










int order = DEFAULT_ORDER;

node * Q1 = NULL;
node* Q2 = NULL;
node * root;
FILE *fp_o;
node *queue =NULL;
bool test = false;
bool verbose = false;
//struct working *mt;


//FIND PROTOTYPES
void find_and_print(node * root, int key);
node * find_leaf( node * root, int key);
record * find( node * root, int key );
int cut( int length );
node * find_leaf( node * root, int key);
void find_and_print(node * root, int key);


//INSERT PROTOTYPES
record * make_record(int value);
node * make_node( void );
node * make_leaf( void );
int get_left_index(node * parent, node * left);
node * insert_into_leaf( node * leaf, int key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer);
node * insert_into_new_root(node * left, int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);

node * start_new_tree(int key, record * pointer);
node * insert_into_node(node * root, node * parent,
		int left_index, int key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent, int left_index,
		int key, node * right);


//PRINT PROTOTYPES

void print_tree( node * root );
void enqueue_1(node * new_node );
void enqueue_2(node * new_node );
node * dequeue_1( void );
node * dequeue_2( void );
int path_to_root( node * root, node * child );

//DELETE PROTOTYPES
node* delete_entry(node*root , node*n, int key, void * pointer);
node*delete(node*root,int key);
node *remove_entry_from_node(node *n, int key, node* pointer);
node* adjust_root(node*root);
int get_neighbor_index( node * n );
node * coalesce_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime);
node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index,
		int k_prime_index, int k_prime);

		//MISC

		void enqueue( node * new_node );
node * dequeue( void );
int height( node * root );
int path_to_root( node * root, node * child );






void main()
{
    char operation;
    int input;

    int exit_flag=0;
    int exit=0;
    char detail;
   // tree my_tree();

// my_tree tree;

//root->tree1 = my_tree();
tree tree1 = tree_init();

    printf ("press i for insertion;\npress d for deletion;\npress l for lookup;\npress p to print tree;\n ");

    while(exit_flag != 1)
    {


        scanf("%c", &operation);
        switch(operation)
        {

        case 'i':
            scanf("%d", &input);
     root->tree1.insert_in_tree(root,input,input);
            print_tree(root);
            //printf("the number inserted is %d ",input);
            break;

        case 'd':
            scanf("%d",& input);
            root = delete(root, input);
            print_tree(root);
            printf("the number deleted is %d ",input);
            break;

        case 'l':
            scanf("%d", &input);
            find_and_print(root, input);
            printf("the number lookedup for is %d ",input);
            break;

        case 'p':
            print_tree(root);
            break;

        case 'e':
            exit_flag = 1;
            break;
        case 't':

            test = true;
            printf("-- test mode is ON\n");
            printf("verbose?[y/n]\n");
            while (exit !=1){
            scanf(" %c",&detail);
            if(detail == 'y')
            {
            verbose=true;
            printf("verbose mode is ON\n");
            exit =1;

            }
            else
            verbose=false;
            exit =1;
}

break;
        }

    }

}

record * make_record(int value) {
	record * new_record = (record *)malloc(sizeof(record));
	if (new_record == NULL) {
		perror("Record creation.");
		exit(EXIT_FAILURE);
	}
	else {
		new_record->value = value;
	}
	return new_record;
}


/* Creates a new general node, which can be adapted
 * to serve as either a leaf or an internal node.
 */
node * make_node( void ) {
	node * new_node;
	new_node = malloc(sizeof(node));
	if (new_node == NULL) {
		perror("Node creation.");
		exit(EXIT_FAILURE);
	}
	new_node->keys = malloc( (order - 1) * sizeof(int) );
	if (new_node->keys == NULL) {
		perror("New node keys array.");
		exit(EXIT_FAILURE);
	}
	new_node->pointers = malloc( order * sizeof(void *) );
	if (new_node->pointers == NULL) {
		perror("New node pointers array.");
		exit(EXIT_FAILURE);
	}
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}

/* Creates a new leaf by creating a node
 * and then adapting it appropriately.
 */
node * make_leaf( void ) {
	node * leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}


/* Helper function used in insert_into_parent
 * to find the index of the parent's pointer to
 * the node to the left of the key to be inserted.
 */
int get_left_index(node * parent, node * left) {

	int left_index = 0;
	while (left_index <= parent->num_keys &&
			parent->pointers[left_index] != left)
		left_index++;
	return left_index;
}

/* Inserts a new pointer to a record and its corresponding
 * key into a leaf.
 * Returns the altered leaf.
 */
node * insert_into_leaf( node * leaf, int key, record * pointer ) {

	int i, insertion_point;

	insertion_point = 0;
	while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
		insertion_point++;

	for (i = leaf->num_keys; i > insertion_point; i--) {
		leaf->keys[i] = leaf->keys[i - 1];
		leaf->pointers[i] = leaf->pointers[i - 1];
	}
	leaf->keys[insertion_point] = key;
	leaf->pointers[insertion_point] = pointer;
	leaf->num_keys++;
	return leaf;
}


/* Inserts a new key and pointer
 * to a new record into a leaf so as to exceed
 * the tree's order, causing the leaf to be split
 * in half.
 */
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer) {

	node * new_leaf;
	int * temp_keys;
	void ** temp_pointers;
	int insertion_index, split, new_key, i, j;

	new_leaf = make_leaf();

	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array.");
		exit(EXIT_FAILURE);
	}

	temp_pointers = malloc( order * sizeof(void *) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array.");
		exit(EXIT_FAILURE);
	}

	insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
		insertion_index++;

	for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
		if (j == insertion_index) j++;
		temp_keys[j] = leaf->keys[i];
		temp_pointers[j] = leaf->pointers[i];
	}

	temp_keys[insertion_index] = key;
	temp_pointers[insertion_index] = pointer;

	leaf->num_keys = 0;

	split = cut(order - 1);

	for (i = 0; i < split; i++) {
		leaf->pointers[i] = temp_pointers[i];
		leaf->keys[i] = temp_keys[i];
		leaf->num_keys++;
	}

	for (i = split, j = 0; i < order; i++, j++) {
		new_leaf->pointers[j] = temp_pointers[i];
		new_leaf->keys[j] = temp_keys[i];
		new_leaf->num_keys++;
	}

	free(temp_pointers);
	free(temp_keys);

	new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
	leaf->pointers[order - 1] = new_leaf;

	for (i = leaf->num_keys; i < order - 1; i++)
		leaf->pointers[i] = NULL;
	for (i = new_leaf->num_keys; i < order - 1; i++)
		new_leaf->pointers[i] = NULL;

	new_leaf->parent = leaf->parent;
	new_key = new_leaf->keys[0];

	return insert_into_parent(root, leaf, new_key, new_leaf);
}


/* Inserts a new key and pointer to a node
 * into a node into which these can fit
 * without violating the B+ tree properties.
 */
node * insert_into_node(node * root, node * n,
		int left_index, int key, node * right) {
	int i;

	for (i = n->num_keys; i > left_index; i--) {
		n->pointers[i + 1] = n->pointers[i];
		n->keys[i] = n->keys[i - 1];
	}
	n->pointers[left_index + 1] = right;
	n->keys[left_index] = key;
	n->num_keys++;
	return root;
}


/* Inserts a new key and pointer to a node
 * into a node, causing the node's size to exceed
 * the order, and causing the node to split into two.
 */
node * insert_into_node_after_splitting(node * root, node * old_node, int left_index,
		int key, node * right) {

	int i, j, split, k_prime;
	node * new_node, * child;
	int * temp_keys;
	node ** temp_pointers;

	/* First create a temporary set of keys and pointers
	 * to hold everything in order, including
	 * the new key and pointer, inserted in their
	 * correct places.
	 * Then create a new node and copy half of the
	 * keys and pointers to the old node and
	 * the other half to the new.
	 */

	temp_pointers = malloc( (order + 1) * sizeof(node *) );
	if (temp_pointers == NULL) {
		perror("Temporary pointers array for splitting nodes.");
		exit(EXIT_FAILURE);
	}
	temp_keys = malloc( order * sizeof(int) );
	if (temp_keys == NULL) {
		perror("Temporary keys array for splitting nodes.");
		exit(EXIT_FAILURE);
	}

	for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++) {
		if (j == left_index + 1) j++;
		temp_pointers[j] = old_node->pointers[i];
	}

	for (i = 0, j = 0; i < old_node->num_keys; i++, j++) {
		if (j == left_index) j++;
		temp_keys[j] = old_node->keys[i];
	}

	temp_pointers[left_index + 1] = right;
	temp_keys[left_index] = key;

	/* Create the new node and copy
	 * half the keys and pointers to the
	 * old and half to the new.
	 */
	split = cut(order);
	new_node = make_node();
	old_node->num_keys = 0;
	for (i = 0; i < split - 1; i++) {
		old_node->pointers[i] = temp_pointers[i];
		old_node->keys[i] = temp_keys[i];
		old_node->num_keys++;
	}
	old_node->pointers[i] = temp_pointers[i];
	k_prime = temp_keys[split - 1];
	for (++i, j = 0; i < order; i++, j++) {
		new_node->pointers[j] = temp_pointers[i];
		new_node->keys[j] = temp_keys[i];
		new_node->num_keys++;
	}
	new_node->pointers[j] = temp_pointers[i];
	free(temp_pointers);
	free(temp_keys);
	new_node->parent = old_node->parent;
	for (i = 0; i <= new_node->num_keys; i++) {
		child = new_node->pointers[i];
		child->parent = new_node;
	}

	/* Insert a new key into the parent of the two
	 * nodes resulting from the split, with
	 * the old node to the left and the new to the right.
	 */

	return insert_into_parent(root, old_node, k_prime, new_node);
}



/* Inserts a new node (leaf or internal node) into the B+ tree.
 * Returns the root of the tree after insertion.
 */
node * insert_into_parent(node * root, node * left, int key, node * right) {

	int left_index;
	node * parent;

	parent = left->parent;

	/* Case: new root. */

	if (parent == NULL)
		return insert_into_new_root(left, key, right);

	/* Case: leaf or node. (Remainder of
	 * function body.)
	 */

	/* Find the parent's pointer to the left
	 * node.
	 */

	left_index = get_left_index(parent, left);


	/* Simple case: the new key fits into the node.
	 */

	if (parent->num_keys < order - 1)
		return insert_into_node(root, parent, left_index, key, right);

	/* Harder case:  split a node in order
	 * to preserve the B+ tree properties.
	 */

	return insert_into_node_after_splitting(root, parent, left_index, key, right);
}


/* Creates a new root for two subtrees
 * and inserts the appropriate key into
 * the new root.
 */
node * insert_into_new_root(node * left, int key, node * right) {

	node * root = make_node();
	root->keys[0] = key;
	root->pointers[0] = left;
	root->pointers[1] = right;
	root->num_keys++;
	root->parent = NULL;
	left->parent = root;
	right->parent = root;
	return root;
}



/* First insertion:
 * start a new tree.
 */
node * start_new_tree(int key, record * pointer) {
int i=0;
	node * root = make_leaf();
	root->keys[0] = key;
	root->pointers[0] = pointer;
	root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->num_keys++;
	if(test == true)
	{
	if(root->parent == NULL)
	printf("root pointer test\n");
	if (verbose == true)
	for (i = 0; i <= order-1; i++)
    printf("%d th pointer of node is %lx\n",i,(unsigned long)root->pointers[i]);
	printf("roots parent pointer points to --> %lx\n",(unsigned long)root->parent);
	printf("---test passed---\n");
	}
	return root;
}



/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */
node * insert( node * root, int key, int value ) {

	record * pointer;
	node * leaf;
	node * t1;
	int i=0;

	/* The current implementation ignores
	 * duplicates.
	 */

	if (find(root, key) != NULL)
		return root;

	/* Create a new record for the
	 * value.
	 */
	pointer = make_record(value);


	/* Case: the tree does not exist yet.
	 * Start a new tree.
	 */

	if (root == NULL)
		return start_new_tree(key, pointer);


	/* Case: the tree already exists.
	 * (Rest of function body.)
	 */

	leaf = find_leaf(root, key);
if(test == true){
	//{i
//	if(root->parent == NULL)
	//printf("root pointer test\n");
//	if (verbose == true)
for (i = 0; i <= order-1; i++)
				printf("%d th pointer of leaf is %lx\n",i,(unsigned long)leaf->pointers[i]);
	printf("leafs parent pointer points to --> %lx\n",(unsigned long)leaf->parent);

	}/* Case: leaf has room for key and pointer.
	 */

	if (leaf->num_keys < order - 1) {
	if(test == true){
	//{i
//	if(root->parent == NULL)
	//printf("root pointer test\n");
//	if (verbose == true)
for (i = 0; i <= order-1; i++)
				printf("%d th pointer of leaf is %lx\n",i,(unsigned long)leaf->pointers[i]);
	printf("leafs parent pointer points to --> %lx\n",(unsigned long)leaf->parent);
	//printf("---test passed---\n");
}
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}


	/* Case:  leaf must be split.
	 */

	t1= insert_into_leaf_after_splitting(root, leaf, key, pointer);
if(test == true){
	//{i
//	if(root->parent == NULL)
	//printf("root pointer test\n");
//	if (verbose == true)
for (i = 0; i <= order-1; i++)
				printf("%d th pointer of node is %lx\n",i,(unsigned long)t1->pointers[i]);
	printf("nodes parent pointer points to --> %lx\n",(unsigned long)t1->parent);
	//printf("---test passed---\n");
}
return t1;
}


//PRINT FUNCTIONS

//Printing functions


void enqueue_1(node * new_node )
{
    node * c;
    if (Q1 == NULL)
    {
        Q1 = new_node;
        Q1->next = NULL;
    }
    else
    {
        c = Q1;
        while(c->next != NULL)
        {
            c = c->next;
        }
        c->next = new_node;
        new_node->next = NULL;
    }
}

void enqueue_2(node * new_node )
{
    node * c;
    if (Q2 == NULL)
    {
        Q2 = new_node;
        Q2->next = NULL;
    }
    else
    {
        c = Q2;
        while(c->next != NULL)
        {
            c = c->next;
        }
        c->next = new_node;
        new_node->next = NULL;
    }
}





node * dequeue_1(void)
{
    node * n = Q1;
    Q1 = Q1->next;
    n->next = NULL;
    return n;
}
node * dequeue_2(void)
{
    node * n = Q2;
    Q2 = Q2->next;
    n->next = NULL;
    return n;
}

void enqueue( node * new_node ) {
	node * c;
	if (queue == NULL) {
		queue = new_node;
		queue->next = NULL;
	}
	else {
		c = queue;
		while(c->next != NULL) {
			c = c->next;
		}
		c->next = new_node;
		new_node->next = NULL;
	}
}


/* Helper function for printing the
 * tree out.  See print_tree.
 */
node * dequeue( void ) {
	node * n = queue;
	queue = queue->next;
	n->next = NULL;
	return n;
}


/* Prints the bottom row of keys
 * of the tree (with their respective
 * pointers, if the verbose_output flag is set.
 */
void print_leaves( node * root ) {
	int i;
	node * c = root;
	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	while (!c->is_leaf)
		c = c->pointers[0];
	while (true) {
		for (i = 0; i < c->num_keys; i++) {
			//if (verbose_output)
				//printf("%lx ", (unsigned long)c->pointers[i]);
			printf("%d ", c->keys[i]);
		}
		//if (verbose_output)
			//printf("%lx ", (unsigned long)c->pointers[order - 1]);
		if (c->pointers[order - 1] != NULL) {
			printf(" | ");
			c = c->pointers[order - 1];
		}
		else
			break;
	}
	printf("\n");
}


/* Utility function to give the height
 * of the tree, which length in number of edges
 * of the path from the root to any leaf.
 */
int height( node * root ) {
	int h = 0;
	node * c = root;
	while (!c->is_leaf) {
		c = c->pointers[0];
		h++;
	}
	return h;
}


/* Utility function to give the length in edges
 * of the path from any node to the root.
 */
int path_to_root( node * root, node * child ) {
	int length = 0;
	node * c = child;
	while (c != root) {
		c = c->parent;
		length++;
	}
	return length;
}


/* Prints the B+ tree in the command
 * line in level (rank) order, with the
 * keys in each node and the '|' symbol
 * to separate nodes.
 * With the verbose_output flag set.
 * the values of the pointers corresponding
 * to the keys also appear next to their respective
 * keys, in hexadecimal notation.
 */
void print_tree( node * root ) {

	node * n = NULL;
	int i = 0;
	int rank = 0;
	int new_rank = 0;

	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}
	queue = NULL;
	enqueue(root);
	while( queue != NULL ) {
		n = dequeue();
		if (n->parent != NULL && n == n->parent->pointers[0]) {
			new_rank = path_to_root( root, n );
			if (new_rank != rank) {
				rank = new_rank;
				printf("\n");
			}
		}
		//if (verbose_output)
			//printf("(%lx)", (unsigned long)n);
		for (i = 0; i < n->num_keys; i++) {
			//if (verbose_output)
				//printf("%lx ", (unsigned long)n->pointers[i]);
			printf("%d ", n->keys[i]);
		}
		if (!n->is_leaf)
			for (i = 0; i <= n->num_keys; i++)
				enqueue(n->pointers[i]);
	//	if (verbose_output) {
		//	if (n->is_leaf)
			//	printf("%lx ", (unsigned long)n->pointers[order - 1]);
			//else
				//printf("%lx ", (unsigned long)n->pointers[n->num_keys]);
		//}
		printf("| ");
	}
	printf("\n");
}



record * find(node*root, int key)
{
    int i = 0;
    node * c = find_leaf( root, key);
    if (c == NULL) return NULL;
    for (i = 0; i < c->num_keys; i++)
        if (c->keys[i] == key) break;
    if (i == c->num_keys)
        return NULL;
    else
        return (record *)c->pointers[i];
}

node * find_leaf( node * root, int key )
{
    int i = 0;
    node * c = root;
    if (c == NULL)
        return c;

    while (!c->is_leaf)
    {
        i = 0;
        while (i < c->num_keys)
        {
            if (key >= c->keys[i]) i++;
            else break;
        }
        c =(node*) c->pointers[i];
    }

    return c;
}

void find_and_print(node * root, int key)
{
    record * r = find(root, key);
    if (r == NULL)
        printf("Record not found under key %d.\n", key);
    else
        printf("Record at %lx -- key %d, value %d.\n",
               (unsigned long)r, key, r->value);
}




node *delete(node*root,int key)
{
    node* curr_leaf;
    record* curr_record;

    curr_leaf = find_leaf(root,key);
    curr_record =  find(root, key);

    if (curr_record != NULL && curr_leaf != NULL)
    {
        root = delete_entry(root,curr_leaf, key, curr_record);
        free(curr_record);
    }

    return root;
}
int cut( int length )
{
    if (length % 2 == 0)
        return length/2;
    else
        return length/2 + 1;
}

node* delete_entry(node*root , node*n, int key, void * pointer)
{
    int min_keys;
    node * adj;
    int adj_idx;
    int k_prime_index, k_prime;
    int neighbor_index;
    node * neighbor;
    int capacity;

    n =remove_entry_from_node(n, key, pointer);

    if (n==root)
        return adjust_root(root);

    min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;

	if (n->num_keys >= min_keys)
		return root;


	neighbor_index = get_neighbor_index( n );
	k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
	k_prime = n->parent->keys[k_prime_index];
	neighbor = neighbor_index == -1 ? n->parent->pointers[1] :
		n->parent->pointers[neighbor_index];

	capacity = n->is_leaf ? order : order - 1;


	if (neighbor->num_keys + n->num_keys < capacity)
		return coalesce_nodes(root, n, neighbor, neighbor_index, k_prime);


	else
		return redistribute_nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}


int get_neighbor_index( node * n ) {

	int i;

	for (i = 0; i <= n->parent->num_keys; i++)
		if (n->parent->pointers[i] == n)
			return i - 1;

	// Error state.
	printf("Search for nonexistent pointer to node in parent.\n");
	printf("Node:  %#lx\n", (unsigned long)n);
	exit(EXIT_FAILURE);
}

node * coalesce_nodes(node * root, node * n, node * neighbor, int neighbor_index, int k_prime) {

	int i, j, neighbor_insertion_index, n_end;
	node * tmp;



	if (neighbor_index == -1) {
		tmp = n;
		n = neighbor;
		neighbor = tmp;
	}



	neighbor_insertion_index = neighbor->num_keys;


	if (!n->is_leaf) {

		neighbor->keys[neighbor_insertion_index] = k_prime;
		neighbor->num_keys++;


		n_end = n->num_keys;

		for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->num_keys++;
			n->num_keys--;
		}

		neighbor->pointers[i] = n->pointers[j];

		for (i = 0; i < neighbor->num_keys + 1; i++) {
			tmp = (node *)neighbor->pointers[i];
			tmp->parent = neighbor;
		}
	}

	else {
		for (i = neighbor_insertion_index, j = 0; j < n->num_keys; i++, j++) {
			neighbor->keys[i] = n->keys[j];
			neighbor->pointers[i] = n->pointers[j];
			neighbor->num_keys++;
		}
		neighbor->pointers[order - 1] = n->pointers[order - 1];
	}

	root = delete_entry(root, n->parent, k_prime, n);
	free(n->keys);
	free(n->pointers);
	free(n);
	return root;
}

node * redistribute_nodes(node * root, node * n, node * neighbor, int neighbor_index,
		int k_prime_index, int k_prime) {

	int i;
	node * tmp;

	if (neighbor_index != -1) {
		if (!n->is_leaf)
			n->pointers[n->num_keys + 1] = n->pointers[n->num_keys];
		for (i = n->num_keys; i > 0; i--) {
			n->keys[i] = n->keys[i - 1];
			n->pointers[i] = n->pointers[i - 1];
		}
		if (!n->is_leaf) {
			n->pointers[0] = neighbor->pointers[neighbor->num_keys];
			tmp = (node *)n->pointers[0];
			tmp->parent = n;
			neighbor->pointers[neighbor->num_keys] = NULL;
			n->keys[0] = k_prime;
			n->parent->keys[k_prime_index] = neighbor->keys[neighbor->num_keys - 1];
		}
		else {
			n->pointers[0] = neighbor->pointers[neighbor->num_keys - 1];
			neighbor->pointers[neighbor->num_keys - 1] = NULL;
			n->keys[0] = neighbor->keys[neighbor->num_keys - 1];
			n->parent->keys[k_prime_index] = n->keys[0];
		}
	}


	else {
		if (n->is_leaf) {
			n->keys[n->num_keys] = neighbor->keys[0];
			n->pointers[n->num_keys] = neighbor->pointers[0];
			n->parent->keys[k_prime_index] = neighbor->keys[1];
		}
		else {
			n->keys[n->num_keys] = k_prime;
			n->pointers[n->num_keys + 1] = neighbor->pointers[0];
			tmp = (node *)n->pointers[n->num_keys + 1];
			tmp->parent = n;
			n->parent->keys[k_prime_index] = neighbor->keys[0];
		}
		for (i = 0; i < neighbor->num_keys - 1; i++) {
			neighbor->keys[i] = neighbor->keys[i + 1];
			neighbor->pointers[i] = neighbor->pointers[i + 1];
		}
		if (!n->is_leaf)
			neighbor->pointers[i] = neighbor->pointers[i + 1];
	}
	n->num_keys++;
	neighbor->num_keys--;

	return root;
}


node *remove_entry_from_node(node *n, int key, node* pointer)
{
    int i, tot_pointers;

    i=0;
    while (n-> keys[i] != key)
        i++;
    for(i; i< n-> num_keys; i++)
        n-> keys[i] = n-> keys[i+1];

    tot_pointers = n-> is_leaf ? n-> num_keys: n-> num_keys+1;
    i=0;
    while(n->pointers[i] != pointer)
        i++;
    for(i; i<tot_pointers; i++)
        n-> pointers[i] = n-> pointers[i+1];

    n-> num_keys--;

    if (n->is_leaf)

        for(i= n-> num_keys; i< order-1; i++)
            n-> pointers[i] = NULL;

    else
        for(i = n-> num_keys+1; i<order; i++)
            n-> pointers[i] = NULL;

    return n;
}

node* adjust_root(node*root)
{
    node *root_new;

    if (root->num_keys > 0)
        return root;

    if(root-> is_leaf == false)
    {
        root_new = root->pointers[0];
        root_new-> parent = NULL;
    }

    else
        root_new = NULL;
    free(root-> keys);
    free(root->pointers);
    free(root);

    return root_new;



}



