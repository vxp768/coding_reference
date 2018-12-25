#include "tree.h"

Tree *Tree::allocate(int data) {
   Tree *node = new Tree;
   node->data = data;
   node->left = NULL;
   node->right = NULL;
return node;
}

Tree *Tree::insert(Tree *root, int data)
{
   if(root == NULL) { //has reached end, allocate new node
      return allocate(data);
   }
   if (data < root->data) {
      root->left  = Tree::insert(root->left, data); 
   } else {
      root->right = Tree::insert(root->right, data); 
   }
   return root;
}

void Traversal::inorder(Tree *root)
{
   if (root == NULL) {
       return;
   }
   inorder(root->left);
   cout<<root->data<<" "<<endl;
   inorder(root->right);
   
}

void Traversal::preorder(Tree *root)
{
   if(root == NULL)
      return;
   cout<<root->data<<endl;
   preorder(root->left);
   preorder(root->right);
}

void Traversal::postorder(Tree *root)
{
   if(root == NULL)
      return;
   cout<<root->data<<endl;
   postorder(root->left);
   postorder(root->right);
}

Traversal_norecur::inorder_norecur(Tree *root)
{

 stack <int> n_stack;
 n_stack.push(root);


}

int main()
{
 Tree *root = NULL;
 root = root->insert(root, 10);
 root->insert(root,30);
 root->insert(root,33);
 root->insert(root,38);
 root->insert(root,20);
 root->insert(root,5);

 cout<<"Inorder"<<endl;
 root->inorder(root);
 cout<<"Preorder"<<endl;
 root->preorder(root);
 cout<<"Postorder"<<endl;
 root->postorder(root);
return 0;
}
