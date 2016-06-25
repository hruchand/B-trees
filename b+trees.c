#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_ORDER 4

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
} node;

int order = DEFAULT_ORDER;

node * Q1 = NULL;
node* Q2 = NULL;
node * root;
node*left=NULL;
node*right= NULL;
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

        default:
            //printf("there is no such option!!");
            break;
        }

    }

}


//INSERT FUNCTIONS

node * insert_into_leaf( node * leaf, int key, record * pointer )
{

    int i, insertion_point;

    insertion_point = 0;
    while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
        insertion_point++;

    for (i = leaf->num_keys; i > insertion_point; i--)
    {
        leaf->keys[i] = leaf->keys[i - 1];
        leaf->pointers[i] = leaf->pointers[i - 1];
    }
    leaf->keys[insertion_point] = key;
    leaf->pointers[insertion_point] = pointer;
    leaf->num_keys++;
    return leaf;
}


node * make_node( void )
{
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

node * make_leaf( void )
{
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

    if (leaf->num_keys < order - 1)
    {
        leaf = insert_into_leaf(leaf, key, pointer);
        return root;
    }


    return insert_into_leaf_after_splitting(root, leaf, key, pointer);


}

node * insert_into_leaf_after_splitting(node * root, node * leaf, int key, record * pointer)
{
    node * new_leaf;
    int temp_keys[order];
    int temp_pointers[order+1];

    int insertion_index, split, new_key, i, j;

    new_leaf = make_leaf();

//find index in leaf where to insert before splitting
    insertion_index = 0;
    while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
        insertion_index++;

//    printf("\insertion_index= %d\n",insertion_index);

//    printf("\leaf->num_keys= %d\n",leaf->num_keys);

//make space for key to insert in temp array.
    for (i = 0, j = 0; i < leaf->num_keys; i++, j++)
    {
        if (j == insertion_index)
        {
            j++;
        }
        temp_keys[j] = leaf->keys[i];
        temp_pointers[j] = leaf->pointers[i];
    }

//insert passed key in the temp
    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = pointer;

//reset leaf
    leaf->num_keys = 0;

    split = cut(order - 1);

//fill leaf
    for (i = 0; i < split; i++)
    {
        leaf->pointers[i] = temp_pointers[i];
        leaf->keys[i] = temp_keys[i];
        leaf->num_keys++;
    }

//fill new_leaf
    for (i = split, j = 0; i < order; i++, j++)
    {
        new_leaf->pointers[j] = temp_pointers[i];
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->num_keys++;
    }

//leaf's last pointer points to new_leaf first
    new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
    leaf->pointers[order - 1] = new_leaf;

//clear middle pointers
    for (i = leaf->num_keys; i < order - 1; i++)
        leaf->pointers[i] = NULL;
    for (i = new_leaf->num_keys; i < order - 1; i++)
        new_leaf->pointers[i] = NULL;

//assign same parent to both leafs
    new_leaf->parent = leaf->parent;
//assign key to the parent before inserting
    new_key = new_leaf->keys[0];

    return insert_into_parent(root, leaf, new_key, new_leaf);
}

int get_left_index(node * parent, node * left) {

	int left_index = 0;
	while (left_index <= parent->num_keys &&
			parent->pointers[left_index] != left)
		left_index++;

//	printf("\nget_left_index= %d\n",left_index);
	return left_index;
}


node * insert_into_node(node * root, node * n,
                        int left_index, int key, node * right)
{
    int i;

//make space for inserting key in parent and adjust pointers
    for (i = n->num_keys; i > left_index; i--)
    {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }

    n->pointers[left_index + 1] = right;    //point to new leaf
    n->keys[left_index] = key;              //insert key in parent
    n->num_keys++;
    return root;
}



node * insert_into_parent(node * root, node * left, int key, node * right)
{

    int left_index;
    node * parent;
    node * temp;

    parent = left->parent;

//easy case when no parent
    if (parent == NULL)
        return temp =  insert_into_new_root(left, key, right);

//get the index in parent where we have to insert
	left_index = get_left_index(parent, left);

//case when parent has a space left for insertion
	if (parent->num_keys < order - 1)
		return insert_into_node(root, parent, left_index, key, right);

//no space in parent, need to split
	return insert_into_node_after_splitting(root, parent, left_index, key, right);


}

node * insert_into_node_after_splitting(node * root, node * old_node, int left_index,
                                        int key, node * right)
{

    int i, j, split, k;
    node * new_node, * child;
    int temp_keys[order];
    int temp_pointers[order+1];

//    printf("\nold_node->num_keys= %d\n",old_node->num_keys);

//make space for split key in temp to insert
    for (i = 0, j = 0; i < old_node->num_keys + 1; i++, j++)
    {
        if (j == left_index + 1)
        {
            j++;
        }
        temp_pointers[j] = old_node->pointers[i];
    }

    for (i = 0, j = 0; i < old_node->num_keys; i++, j++)
    {
        if (j == left_index)
        {
            j++;
        }
        temp_keys[j] = old_node->keys[i];
    }
//insert key in temp
    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;


    split = cut(order);
    new_node = make_node();
//reset old node
    old_node->num_keys = 0;

//fill old node
    for (i = 0; i < split - 1; i++)
    {
        old_node->pointers[i] = temp_pointers[i];
        old_node->keys[i] = temp_keys[i];
        old_node->num_keys++;
    }
    old_node->pointers[i] = temp_pointers[i];
    k = temp_keys[split - 1]; //contains key which we send up

//fill new node
    for (++i, j = 0; i < order; i++, j++)
    {
        new_node->pointers[j] = temp_pointers[i];
        new_node->keys[j] = temp_keys[i];
        new_node->num_keys++;
    }

    new_node->pointers[j] = temp_pointers[i];
    new_node->parent = old_node->parent;
/*

    for (i = 0; i <= new_node->num_keys; i++)
    {
        child = new_node->pointers[i];
        child->parent = new_node;
    }
*/

    return insert_into_parent(root, old_node, k, new_node);
}



node * insert_into_new_root(node * left, int key, node * right)
{

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

record * make_record(int value)
{
    record * new_record = (record *)malloc(sizeof(record));
    if (new_record == NULL)
    {
        perror("Record creation.");
        exit(EXIT_FAILURE);
    }
    else
    {
        new_record->value = value;
    }
    return new_record;
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

void print_tree(node  * root)
{
    int i=0;
    node * current = NULL;
    node * x = NULL;



    if(root == NULL)
    {
        printf("The tree is empty\n");
        return;
    }


    enqueue_1(root);
    while (Q1!=NULL || Q2!= NULL)
    {
        while (Q1!=NULL)
        {

            current = dequeue_1();
            for(i=0; i<current->num_keys; i++)
            {
                printf("%d", current->keys[i]);
                printf("  ");
            }
            printf("|");
            if (current->is_leaf == false)
            {
                for(i=0; i<=current->num_keys; i++)
                    enqueue_2(current->pointers[i]);
            }

        }
        printf("\n");
        while(Q2!= NULL)
        {
            current= dequeue_2();
            for(i=0; i<current->num_keys; i++)
            {
                printf("%d", current->keys[i]);
                printf("  ");
            }
            printf("|");
            if (current->is_leaf == false)
            {
                for(i=0; i<=current->num_keys; i++)
                    enqueue_1(current->pointers[i]);
            }

        }
        printf("\n");
    }
}





//FIND

int cut( int length )
{
    if (length % 2 == 0)
        return length/2;
    else
        return length/2 + 1;
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


