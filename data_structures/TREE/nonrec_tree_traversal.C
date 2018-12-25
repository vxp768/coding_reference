#include<iostream>
#include<stack>
#include<queue>

using namespace std;

struct node {
 int data;
 struct node *left;
 struct node *right;
};

//level_order traversal recursively
void insert_rec(node **root_c, int data)
{
}

void binary_tree_insert (node *root, int data)
{
  node *temp;
  node *newnode = NULL;
  queue<node *> Q;    
  if (root == NULL)
      return;
  Q.push(root);
  newnode = new node;
  newnode->left  = NULL;
  newnode->right = NULL;
  newnode->data  = data;
  while(Q.empty() == false) {
      temp = Q.front();
      Q.pop();
      if (temp->left) {
          Q.push(temp->left);
      } 
      if (temp->right) {
          Q.push(temp->right);
      } 
      if (temp->left == NULL) {
          temp->left = newnode; 
          break;
      }
      if (temp->right == NULL) {
          temp->right = newnode; 
          break;
      }
  }
}

void postorder_no_recursive(node *root)
{
  stack<node *> S;
  node *temp = root;
  if (temp == NULL)
      return;
  do {
    while(temp) {
      if(temp->right) {
         S.push(temp->right);
      }
      S.push(temp);
      temp = temp->left;
    }
    temp = S.top();
    S.pop();
    if (temp->right != NULL && !S.empty() && S.top() == temp->right) {
        S.pop();
        S.push(temp);
        temp = temp->right;  
    } else {
        cout<<" "<<temp->data;
        temp = NULL;
    }
  } while(!S.empty());
  cout<<endl;
}

void preorder_non_recursive(node *root)
{
    stack <node *> S;
    node *temp = root;
    cout<<"preorder non recur: "<<endl;
    while(1) {
      while(temp){
	cout<<" "<<temp->data;
        S.push(temp);
	temp = temp->left;
      }
      if (S.empty() == true)
	  break;
      temp = S.top();
      S.pop();
      temp = temp->right;
   }
   cout<<endl;
}
void inorder_non_recursive(node *root)
{
    stack<node *> S;
    node *temp = root;
    cout<<"inorder non recur: "<<endl;
    while(1) {
      while(temp) {
        S.push(temp);
        temp = temp->left;
      }
      if (S.empty() == true) 
	  break;
      temp = S.top();
      S.pop();
      cout<<" "<<temp->data;
      temp = temp->right;
    }
    cout<<endl;
}
int main()
{
  int data[10] = {10,4,6,3,11,12,5,19,20};
  stack<node *> S;
  node *root = new node;
  root->data = 10;
  root->left = NULL;
  root->right = NULL;
  for (int i=1; i< 9; i++) {
      binary_tree_insert(root,data[i]);
  }
  inorder_non_recursive(root);
  preorder_non_recursive(root);
  cout<<"postorder non recur: "<<endl;
  postorder_no_recursive(root);
}
