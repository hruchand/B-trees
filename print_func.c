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
            printf("%d", n->keys[i]);
        }

        if (!n ->is_leaf)
            for (i=0; i <= n->num_keys; i++)
                enqueue(n->pointers[i]);
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


