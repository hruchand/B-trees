#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_ORDER 4

typedef struct record{
	int value;
}record;



typedef struct node {
	void ** pointers;
		int * keys;
		struct node * parent;
		bool is_leaf;
		int num_keys;
		struct node * next;
} node;

int order = DEFAULT_ORDER;

node * queue = NULL;
node * root;

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
node * insert(node *root, int key, int value);
node * start_new_tree(int key, record * pointer);


//PRINT PROTOTYPES

void print_tree( node * root );
void enqueue( node * new_node );
node * dequeue( void );
int path_to_root( node * root, node * child );

//DELETE PROTOTYPES
node* delete_entry(node*root , node*n, int key, void * pointer);
node*delete(node*root,int key);
node *remove_entry_from_node(node *n, int key, node* pointer);
node* adjust_root(node*root);




void main()
{
	char operation;
	int input;

	int exit_flag=0;


	printf ("press i for insertion;\npress d for deletion;\npress l for lookup;\npress p to print tree;\n ");

	while(exit_flag != 1)
	{
		scanf("%c", &operation);
		switch(operation)
		{

		case 'i':
			scanf("%d", &input);
			root = insert(root, input, input);
				print_tree(root);
			printf("the number inserted is %d ",input);
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

		default:
			//printf("there is no such option!!");
			break;
		}

	}

}


//INSERT FUNCTIONS

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


node * make_node( void ) {
	node * new_node;
	new_node = malloc(sizeof(node));
	new_node->keys = malloc( (order - 1) * sizeof(int) );
	new_node->pointers = malloc( order * sizeof(void *) );

	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;
	return new_node;
}

node * make_leaf( void ) {
	node * leaf = make_node();
	leaf->is_leaf = true;
	return leaf;
}


node * start_new_tree(int key, record * pointer)
{

	node * root = make_leaf();
	root->keys[0] = key;
	root->pointers[0] = pointer;
	root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->num_keys++;
	return root;
}

node * insert(node *root, int key, int value)
{
	record* pointer;
	node* leaf;

	pointer = make_record(value);

	if (root == NULL)
		return start_new_tree(key, pointer);

	leaf = find_leaf(root, key);

	if (leaf->num_keys < order - 1) {
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}


	return insert_into_leaf_after_splitting(root, leaf, key, pointer);


}

node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer)
{
	node * new_leaf;
	int * temp_keys;
	void ** temp_pointers;
	int insertion_index, split, new_key, i, j;

	new_leaf = make_leaf();

	temp_keys = malloc( order * sizeof(int) );

	temp_pointers = malloc( order * sizeof(void *) );

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



node * insert_into_parent(node * root, node * left, int key, node * right) {

	int left_index;
	node * parent;
	node * temp;

	parent = left->parent;

	/* Case: new root. */

	if (parent == NULL)
		return temp =  insert_into_new_root(left, key, right);


}

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

//PRINT FUNCTIONS

//Printing functions


void enqueue( node * new_node )
{
    node * c;
    if (queue == NULL)
    {
        queue = new_node;
        queue->next = NULL;
    }
    else
    {
        c = queue;
        while(c->next != NULL)
        {
            c = c->next;
        }
        c->next = new_node;
        new_node->next = NULL;
    }
}


node * dequeue( void )
{
    node * n = queue;
    queue = queue->next;
    n->next = NULL;
    return n;
}


void print_tree(node  * root)
{
    node * n = NULL;
    int rank =0;
    int i=0;
    int new_rank=0;

    if(root == NULL)
    {
        printf("The tree is empty\n");
        return;
    }

    queue = NULL;
    enqueue(root);
    while (queue!=NULL)
    {
        n= dequeue();
        if(n->parent != NULL && n == n->parent->pointers[0])
            new_rank = path_to_root(root,n);
        if(new_rank != rank)
        {
            rank=new_rank;
            printf("\n");
        }

        for(i=0; i < n->num_keys; i++)
        {
            printf(" %d", n->keys[i]);
        }

        if (!n ->is_leaf)
            for (i=0; i <= n->num_keys; i++)
{

               enqueue(n->pointers[i]);
}
printf("|");
    }
    printf("\n");
}

int path_to_root(node*root, node*child)
{
    int length = 0;
    node *c = child;
    while(c!=root)
    {
        c= c->parent;
        length++;
    }
    return length;
}




//FIND

int cut( int length ) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}


record * find(node*root, int key){
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

node * find_leaf( node * root, int key ) {
	int i = 0;
	node * c = root;
	if (c == NULL)
		return c;

	while (!c->is_leaf) {
		i = 0;
		while (i < c->num_keys) {
			if (key >= c->keys[i]) i++;
			else break;
		}
		c =(node*) c->pointers[i];
	}

	return c;
}

void find_and_print(node * root, int key) {
	record * r = find(root, key);
	if (r == NULL)
		printf("Record not found under key %d.\n", key);
	else
		printf("Record at %lx -- key %d, value %d.\n",
				(unsigned long)r, key, r->value);
}




node*delete(node*root,int key)
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

node* delete_entry(node*root , node*n, int key, void * pointer)
{
int min_keys;
node * adj;
int adj_idx;
int k_prime_index, k_prime;
int cap;

n =remove_entry_from_node(n, key, pointer);

if (n==root)
return adjust_root(root);

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
for(i;i<tot_pointers;i++)
n-> pointers[i] = n-> pointers[i+1];

n-> num_keys--;

if (n->is_leaf)

for(i= n-> num_keys; i< order-1; i++)
n-> pointers[i] = NULL;

else
for(i = n-> num_keys+1;i<order;i++)
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


