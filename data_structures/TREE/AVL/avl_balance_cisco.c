#include<stdio.h>
#include<stdlib.h>

typedef enum avl_balance_type_ {
    LEFT_HEAVY  = -1;
    BALANCED    =  0;
    RIGHT_HEAVY =  1;
} avl_balance_type;

typedef struct avl_node_ {
    struct avl_node_ *left;
    struct avl_node_ *right;
    int balance;
    int key;
} avl_node;

void *avl_insert(avl_node **root, int key)
{
    if (*root == NULL) {
        avl_node *new = (avl_node*)malloc(sizeof(avl_node));    
        new->left  = NULL;
        new->right = NULL;
        new->balance = BALANCED;
        *root = new;
        return new;
    }

    if (key < (*root)->key) {
        avl_insert(&((*root)->left), key);
        switch((*root)->balance) {
            case LEFT_HEAVY:
                /* was left heavy and node added in left so need balancing */
                left = (*root)->left;
                if (left->balance == LEFT_HEAVY) {

                }
            break;
            case BALANCED:
                root->balance = LEFT_HEAVY;
            break;
            case RIGHT_HEAVY:
                /* was right heavy and node added in left so it becomes balanced */
                root->balance = BALANCED;
            break;
        }
    } else if (key > (*root)->key) {
        avl_insert(&(*(root)->right), key);
    }else {
        printf("Node already in tree\n");
    }
}
