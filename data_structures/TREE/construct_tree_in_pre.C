#include<iostream>

using namespace std;

struct node {
 int data;
 struct node *left;
 struct node *right;
};

int in_order[]  = {19, 3, 20, 4, 11, 10, 12, 6, 5};
int pre_order[] = {10, 4, 3, 19, 20, 11, 6, 12, 5};

int search_root_idx (int in[], int root, int start, int end)
{
  int i;
  for ( i=start; i<=end; i++) {
      if (in[i] == root)
          return i;
  }
  return -1;
}

node *construct_tree(int in[], int pre[], int in_start, int in_end)
{
  static int pre_idx = 0;
  if (in_start > in_end)
      return NULL;
  node *newnode  = new node; 
  newnode->data  = pre[pre_idx++];
  newnode->left  = NULL;
  newnode->right = NULL;
  if (in_start == in_end )
      return newnode;

  int root_idx = search_root_idx(in, newnode->data, in_start, in_end );

  newnode->left  = construct_tree(in, pre, in_start, root_idx-1); 
  newnode->right = construct_tree(in, pre, root_idx+1, in_end);

  return newnode;
}
void print_inorder(node *node)
{
    if (node==NULL)
        return;
    print_inorder(node->left);
    cout<<" "<<node->data;
    print_inorder(node->right);
}

int main()
{
 node *root;
 root = construct_tree(in_order, pre_order, 0, 8); 
 print_inorder(root);
return 0;
}
