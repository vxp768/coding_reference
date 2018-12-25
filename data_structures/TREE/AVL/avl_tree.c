#include<stdio.h>
#include<stdlib.h>

typedef struct avl_node_ {
    int key;
    struct avl_node_ *right;
    struct avl_node_ *left;
    int height;
}Node;

int node_height(Node *node)
{
    if(node == NULL)
       return 0;
    return node->height;
}

int balance_factor(Node *node)
{
    if (node == NULL)
        return 0;
    return node_height(node->left) - node_height(node->right);
} 

int max_height(int a, int b)
{
    return (a > b)? a:b ;
}

void print_tree_preorder(Node *root)
{
    if(root==NULL)
        return;
    printf("%d ",root->key);
    print_tree_preorder(root->left);
    print_tree_preorder(root->right);
}

void print_tree_inorder(Node *root)
{
    if(root==NULL)
        return;
    print_tree_inorder(root->left);
    printf("%d ",root->key);
    print_tree_inorder(root->right);
}

Node *left_rotate(Node *x)
{
    Node *y = x->right;
    Node *sub_T = y->left;
    y->left = x;
    x->right = sub_T;
    /*Update heights*/
    x->height = max_height(node_height(x->left), node_height(x->right))+1;
    y->height = max_height(node_height(y->left), node_height(y->right))+1;
    return y;
}

Node *right_rotate(Node *x)
{
    Node *y = x->left;
    Node *sub_T = y->right;
    y->right = x;
    x->left = sub_T;
    /*Update heights*/
    x->height = max_height(node_height(x->left), node_height(x->right))+1;
    y->height = max_height(node_height(y->left), node_height(y->right))+1;
    return y;
}

Node *delete_node(Node *node, int key)
{
    Node *temp=NULL;
    if(key < node->key){
       node->left  = delete_node(node->left,key);
    } else if (key > node->key) {
       node->right = delete_node(node->right,key);
    } else {

       /* This node can have one or two or no child */

       /*One or no child case are easier to handle*/
       if (node->left == NULL || node->right == NULL){
	   temp = node->left ? node->left:node->right;
           if (temp == NULL) { /*no child case*/
               temp = node;
	       node = NULL; 
           } else {
               /* One child case */
               *node = *temp
           }
           free(temp);
       } else { /* Two child case */
           /* Find inorder successor of node */
           temp = inorder_successor(node->right);
           node->key = temp->key;
           node->right = delete_node(node->right,temp->key);
       }
    }
    if (node == NULL)
       return; //No child so no need of any update

    /*Update the height of current node */
    node->height = max_height(node_height(node->left),node_height(node->right))+1;
    int balance = balance_factor(node);

    if (balance < 1 &&  
}

Node *insert_node(Node *node, int key)
{
    if (node==NULL){
        node= (Node*)malloc(sizeof(Node));
        node->key = key;
        node->height = 1; /*leaf height as 1*/
        node->left  = NULL;
        node->right = NULL;
        return node;
    } 
    
    if (key < node->key) {
        node->left  = insert_node(node->left, key);
    } else {
        node->right = insert_node(node->right, key);
    }
    /* Update height of each node */
    //plus 1 for new node added
    node->height = max_height(node_height(node->left), node_height(node->right)) + 1;

    /* Check balance factor of parent */
    int balance = balance_factor(node);

    /*Rotation required if height is off by more than 1
      Leaf node height is considered 1 and if no child then height 
      of subtree is 0*/

    /*Four cases to handle */

    /*Left Left case */
    if (balance > 1 && key < node->left->key) {
        return right_rotate(node);
    }

    /*Left Right case */
    if (balance > 1 && key > node->left->key) {
      /*need two rotations--left then right*/
      node->left = left_rotate(node->left);
      return right_rotate(node);
    }

    /*Right Left case */
    if (balance < -1 && key < node->right->key){
        /*need two rotations--right then left*/
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    /*Right Right case */
    if (balance < -1 && key > node->right->key){
        return left_rotate(node);
    }

    /*If root does not change*/
    return node;
}

int main()
{
    Node *root = NULL;

    /* root can change on every insert because of rotation */
    root = insert_node(root,1);
    root = insert_node(root,2);
    root = insert_node(root,3);
    root = insert_node(root,4);
    root = insert_node(root,5);
    root = insert_node(root,6);
    root = insert_node(root,7);
    root = insert_node(root,8);
    root = insert_node(root,9);

    printf("InOrder Traversal: ");
    print_tree_inorder(root);
    printf("\n");
    printf("PreOrder Traversal: ");
    print_tree_preorder(root);
    printf("\n");
    printf("Tree height:%d key:%d\n",root->height,root->key);
return 0;
}
