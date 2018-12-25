#include<stdio.h>
#include<stdlib.h>

typedef struct tree_ {
  int data;
  struct tree_ *right, *left;
}Node; 

void binary_insert(Node **tree_root, Node *item)
{
    Node *root;
    if (!(*tree_root)) {
        *tree_root = item;
        return;
    } 
    root = *tree_root;
    if (item->data < root->data) {
        binary_insert(&root->left,item);
    } else {
        binary_insert(&root->right,item);
    }
}

void height_of_tree(Node *root)
{
}

int findmax(Node *root)
{
    int root_val, left_val, right_val, max=-1;
    if(root!=NULL) {
       root_val = root->data;
       left_val = findmax(root->left);
       right_val = findmax(root->right);

       if(left_val < right_val)
          max = right_val;
       else
          max = left_val;
       if (root_val > max)
           max = root_val;
    }

    return max;
}

void print_preorder(Node *node)
{
    if(node==NULL)
       return;
    printf("%d\t",node->data);
    print_preorder(node->left);
    print_preorder(node->right);
}

void print_inorder(Node *node)
{
    if (node==NULL)
	return;
    print_inorder(node->left);
    printf("%d \t",node->data);
    print_inorder(node->right);
}

void print_postorder(Node *node)
{
    if(node==NULL)
       return;
    print_postorder(node->left);
    print_postorder(node->right);
    printf("%d\t",node->data);
}

int main()
{

  Node *root=NULL, *curr;
  root = NULL;
  int i=0;
  int data[10] = {10,4,6,3,11,12,5,19,20};
  for(i=0;i<10;i++) {
     curr = (Node*)malloc(sizeof(Node));
     curr->data=data[i];
     printf(" %d\t",data[i]);
     curr->left = curr->right = NULL; 
     binary_insert(&root,curr);
  }
 printf("\nPreOrder\n");
 print_preorder(root);
 printf("\nInOrder\n");
 print_inorder(root);
 printf("\nPostOrder\n");
 print_postorder(root);
 printf("\n");
 printf("Max is %d\n",findmax(root));
return 0;
}
