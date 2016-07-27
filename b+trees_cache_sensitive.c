#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_ORDER 2
#define MAX_CHILDREN 3
#define CSBPT_ELEM_SIZE (sizeof(int) + sizeof(void *))


typedef struct record{
	int value;
}record;



typedef struct node {
	void * first_pointer;
		int * keys;
		struct node * parent;
		//bool is_leaf;
		int num_keys;
		struct node * queue_next;
}node;

typedef struct leaf_group
{
struct leaf_group *prev;
struct leaf_group *next;
struct leaf_group * parent;
int no_of_elements;
int *keys;

}leaf_group;

int order = DEFAULT_ORDER;

node * queue = NULL;
leaf_group * root = NULL;
node *parent = NULL;
int num_of_leaf_groups;
int count;
leaf_group **leaf;

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
leaf_group * insert_into_leaf( leaf_group * leaf, int key, record * pointer );
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer);
node * insert_into_new_root(node * left, int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);
leaf_group * insert(leaf_group *root, int key, int value);
leaf_group * new_leaf_group(int key, record * pointer);

//PRINT PROTOTYPES

void print_tree( node * root );
void enqueue( node * new_node );
node * dequeue( void );
int path_to_root( node * root, node * child );

//new function
node* new_parent(void);
node* make_parent(void);
leaf_group * create_new_leaf_group(leaf_group *leaf,int key, int pointer);


void main()
{
	char operation;
	int input;

	int exit_flag=0;
	num_of_leaf_groups = 0;
count =0;


	printf ("press i for insertion;\npress d for deletion;\npress l for lookup;\npress p to print tree;\n ");

	while(exit_flag != 1)
	{

		scanf("%c", &operation);
        switch(operation)
		{

		case 'i':
			scanf("%d", &input);
			count++;
			root = insert(root, input, input);
			//	print_tree(root);
			//printf("the number inserted is %d ",root->keys[0]);
		        break;
/*
		case 'd':
			scanf("%d",& input);
			//root = delete(root, input);
			//		print_tree(root);
			printf("the number deleted is %d ",input);
			break;

		case 'l':
			scanf("%d", &input);
			find_and_print(root, input);
			printf("the number lookedup for is %d ",input);
			break;
*/
		case 'p':
		printf("the keys are %d\n",*leaf[0]->keys);
			//print_tree(root);
			break;
/*
		case 'e':
			exit_flag = 1;
			break;

		default:
			//printf("there is no such option!!");
			break;
	*/
		}

	}

}


//INSERT FUNCTIONS
/*

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

/*
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
*/


//INSERT FUNCTIONS


leaf_group * insert_into_leaf( leaf_group * leaf, int key, record * pointer ) {


	//count++;
	return leaf;
}

/*
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
*/
leaf_group * make_leaf_group(void)
{
leaf_group *new_leaf_group;
new_leaf_group = malloc(sizeof(leaf_group));
new_leaf_group->keys = malloc((order * (order+1)) * sizeof(int));
new_leaf_group->prev =  malloc(1 * sizeof(void*));
new_leaf_group->next =  malloc(1 * sizeof(void*));
return new_leaf_group;
}
/*
node * make_leaf( void ) {
	node * leaf = make_leaf_group();
	leaf->is_leaf = true;
	return leaf;
}
*/

leaf_group * new_leaf_group(int key, record * pointer)
{

	leaf_group * root = make_leaf_group();
	root->keys[0] = key;
	//count++;
	//root->pointers[0] = pointer;
	//root->pointers[order - 1] = NULL;
	root->parent = NULL;
	root->prev = NULL;
	root->next =  NULL;
	root->no_of_elements++;
	return root;
}
/*
node* new_parent(void)
{
node * parent ;
parent=make_parent();

return parent;


}
*/

node* make_parent(void)
{
node *new_node;
//new_node = malloc(sizeof(node));
new_node->keys = malloc(order * sizeof(int));
new_node->first_pointer = malloc(1 * sizeof(void*));
new_node->parent = malloc(1 * sizeof(void*));
return new_node;

}



leaf_group * insert(leaf_group *root, int key, int value)
{
	record* pointer;
	//leaf_group **leaf;
	node *parent;
	node *new_root;
	int num_leaves;
	int new_keys[18] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
	node *tree_root;

	int max_children = MAX_CHILDREN;

	int i=0;
	int arr_idx=0;;


    tree_root = calloc(1, sizeof(struct node));
    leaf = calloc(5, sizeof(struct leaf_group*));
	parent = calloc(5, sizeof(struct node));
	pointer = make_record(value);

	while(i<3){
	int x = 1;
	int y = 0;

	leaf[i] = calloc(1, sizeof(struct leaf_group) + max_children * CSBPT_ELEM_SIZE);
	leaf[i]->keys = malloc((order * (order+1)) * sizeof(int));
parent[i].keys = malloc(sizeof(int));
	parent[i].first_pointer = leaf[i];


	if (i>0)
	{
	leaf[i]->prev = leaf[i - 1];
    leaf[i - 1]->next = leaf[i];
    }

   // if (leaf[i] == NULL)
    //{
    //leaf[i]->keys = key;

    //}

    //while (leaf[i]->no_of_elements < (order*(order+1)))
    //{
        for(int j =0 ;j<(order*(order+1));j++)
        {
        leaf[i]->keys[j] = new_keys[arr_idx];
        arr_idx++;
        leaf[i]->no_of_elements++;
        printf("the keys are %d\n",leaf[i]->keys[j]);
        }
        for( x, y;y<order,x<order+2;y++,x=x+2)
    {
    parent[i].keys[y] = leaf[i]->keys[x];
    printf("%d\n",parent[i].keys[y]);
    }
        printf("leaf group no is %d \n ", i);
        if(leaf[i]->no_of_elements == (order*(order+1)) )
        i++;
}
tree_root->first_pointer = parent;
tree_root->keys = malloc(sizeof(int));
tree_root->keys[0] = leaf[0]->keys[(order*(order+1))-1];
printf("root keys are:%d",tree_root->keys[0]);
print_parent(tree_root);
return leaf[i];
}

void print_parent(node*temp_root)
{
node *temp_parent = temp_root->first_pointer;
leaf_group * temp_leaf = temp_parent->first_pointer;
leaf_group * temp_next_leaf = temp_leaf->next;

printf("parent keys are: %d\n",temp_parent[0].keys[1]);
printf("leaf keys are : %d\n", temp_leaf[0].keys[5]);
printf("next leaf keys are:%d\n",temp_next_leaf[0].keys[5]);


}



//}


leaf_group * create_new_leaf_group(leaf_group *leaf,int key, int pointer)
{
leaf_group * temp_leaf;
leaf_group * prev_leaf;
int insertion_point;

temp_leaf = make_leaf_group();
temp_leaf->prev = prev_leaf;
temp_leaf->next = NULL;

int insertion_index = 0;
	while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
		insertion_index++;

for (int i = temp_leaf->no_of_elements; i > insertion_point; i--) {
		temp_leaf->keys[i] = temp_leaf->keys[i - 1];
		//leaf->pointers[i] = leaf->pointers[i - 1];
	}
	temp_leaf->keys[insertion_point] = key;
	//leaf->pointers[insertion_point] = pointer;
	temp_leaf->no_of_elements++;

	for(int i =0; i<temp_leaf->no_of_elements;i++)
    printf("%d\t",temp_leaf->keys[i]);
	//count++;
	return temp_leaf;
}
/*
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

	parent = left->parent;

	/* Case: new root.

	if (parent == NULL)
		return insert_into_new_root(left, key, right);


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
*/
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

/*
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

leaf_group * find_leaf( leaf_group * root, int key ) {
	int i = 0;
	leaf_group * c = root;
	if (c == NULL)
		return c;

	//while (!c->is_leaf) {
		//i = 0;
		while (i < c->no_of_elements) {
			if (key >= c->keys[i]) i++;
			else break;
		}
		c = (leaf_group *)c->[i];
	//}

	return c;
}
/*
void find_and_print(node * root, int key) {
	record * r = find(root, key);
	if (r == NULL)
		printf("Record not found under key %d.\n", key);
	else
		printf("Record at %lx -- key %d, value %d.\n",
				(unsigned long)r, key, r->value);
}

*/








/*
	leaf = find_leaf(root, key);

	if (leaf->num_keys < order - 1) {
		leaf = insert_into_leaf(leaf, key, pointer);
		return root;
	}


	return insert_into_leaf_after_splitting(root, leaf, key, pointer);

*/

/*
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

	parent = left->parent;

	/* Case: new root.

	if (parent == NULL)
		return insert_into_new_root(left, key, right);


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
*/



//PRINT FUNCTIONS

//Printing functions

/*
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
		c = (node *)c->pointers[i];
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

*/



