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

void find_and_print(node * root, int key);
node * find_leaf( node * root, int key);
record * find( node * root, int key );




void main(){

char operation;
int input;
node * root;
int exit_flag=0;


printf ("press i for insertion;\npress d for deletion;\npress l for lookup;\npress p to print tree;\n ");

while(exit_flag != 1){

scanf("%c", &operation);

switch(operation){

case 'i':

scanf("%d", &input);
	//root = insert(root, input, input);
		//	print_tree(root);
		printf("the number inserted is %d ",input);


        break;



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



case 'p':
//print_tree(root);
			break;
case 'e':
exit_flag = 1;
break;

//default:
//printf("there is no such option!!");
//break;

}

}




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
	if (c == NULL) {
		return c;
	}
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



