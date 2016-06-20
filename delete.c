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
return root_new;

}



