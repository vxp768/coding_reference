#include<iostream>
#include<queue>

using namespace std;

typedef struct node_ {
 int data;
 struct node_ *left;
 struct node_ *right;
} node;

void insert (node **in_node, int data)
{
  node *temp;
  node *root = *in_node;
  if (root == NULL) {
      temp = new node;
      temp->left  = NULL;
      temp->right = NULL;
      temp->data  = data;
      *in_node = temp;
      return;
  }
  //in case data is equal don't do anything and return
  if (data < root->data) {
      insert(&(root->left), data);
  } else if (data > root->data) {
      insert(&(root->right), data);
  }
}
node *search (node *root, int data)
{
  if (root == NULL) {
      cout<<"NOT found data "<<data<<endl;
      return NULL;
  }
  if (root->data == data) {
      cout<<"Found data "<<data<<endl;
      return root;
  }
  if (data < root->data) {
      search(root->left, data);
  } else {
      search(root->right, data);
  }
}
node *delete_node (node *root, int data)
{
  node *temp, *successor, *prev;
  if (root == NULL) {
      cout<<"Not found data"<<endl;
      return NULL;
  }
  if (data == root->data) {
      if (root->left == NULL) {
          temp = root->right;
          delete root;
          return temp;
      } else if (root->right == NULL){
          temp = root->left;
          delete root;
          return temp;
      } 
      //no child is NULL
      prev = root;
      successor = root->right;
      while (successor->left) {
         prev = successor;
         successor = successor->left;
      }
      //copy the successor data
      root->data = successor->data;
      if (prev->left == successor) {
          //case where successor has right subtree
          prev->left = successor->right;
      } else {
          //case where successor is right child of root
          prev->right = successor->right;
      }
      delete successor;
      return root;
  }
  if (data < root->data) {
      root->left  = delete_node(root->left, data);      
  } else if (data > root->data) {
      root->right = delete_node(root->right, data);
  }
  return root;
}
void in_order (node *root)
{
  if (root == NULL)
      return;

  in_order(root->left);
  cout<<" "<<root->data;
  in_order(root->right);
}
void level_order (node *root)
{
  node *temp = NULL;
  queue <node *> Q;
  if (root == NULL)
      return;
  Q.push(root);
  Q.push(NULL);
  while (!Q.empty()) {
    temp = Q.front();
    Q.pop();
    if (temp == NULL && !Q.empty()) {
        Q.push(NULL);
        cout<<endl;
    } 
    if (temp != NULL) {
        cout<<" "<<temp->data;
        if (temp->left)
            Q.push(temp->left);
        if (temp->right)
	    Q.push(temp->right);
    }
  }
  cout<<endl;
}
void bst_succ_pred (node *root, int key, int *succ, int *pred)
{
    if (root == NULL) {
        return;
    }
    if (key < root->data) {
        *succ = root->data;
        bst_succ_pred(root->left, key, succ, pred);
    } else if (key > root->data) {
        *pred = root->data;
        bst_succ_pred(root->right, key, succ, pred);
    } else {
	//root data equals key
        if (root->right) {
            node *temp = root->right;
            while (temp->left)
 		temp = temp->left;
            *succ = temp->data;
        }
        if (root->left) {
            node *temp = root->left;
            while (temp->right)
                temp = temp->right;
            *pred = temp->data;
        }
    }    
}
int check_bst (node *root, int min, int max)
{
  if (root == NULL) {
      return 1;
  }
  if (root->data < min || root->data > max) 
      return 0;
  return (check_bst (root->left, min, root->data) &&  //to allow disinct values use root->data-1
          check_bst (root->right, root->data, max));  //root->data+1
}
node *bst_least_common (node *root, int a, int b)
{
  if (root == NULL)
      return root;
  if ( a < root->data && b < root->data ) {
      return bst_least_common(root->left, a, b);
  } else if ( a > root->data && b > root->data ) {
      return bst_least_common(root->right, a, b);
  }
  return root;

  //iterative solution
  /* while(root!=NULL) {
    if ( a < root->data && b < root->data )
         root = root->left;
    else if ( a > root->data && b > root->data )
	 root = root->right;
    else
         break;
  }
  return root; */
}

int main ()
{
  int i, succ, pred;
  node *root = NULL;
  int arr_data[] = {50, 30, 20, 40, 70, 60, 56, 58, 57, 59, 80, 61, 79, 78, 81, 82, 83, 84};
  int size = sizeof(arr_data)/sizeof(arr_data[0]);

  for (i=0; i<size; i++) {
      insert(&root, arr_data[i]);
  }
  search(root, 20);
  search(root, 10);
  in_order(root);
  cout<<endl;
  bst_succ_pred(root, 50, &succ, &pred);
  cout<<"node:50 succ: "<<succ<<" pred: "<<pred<<endl;
  bst_succ_pred(root, 20, &succ, &pred);
  cout<<"node:20 succ: "<<succ<<" pred: "<<pred<<endl;
  cout<<"level order\n";
  level_order(root);
  node *temp = bst_least_common(root, 57, 61);
  cout<<"LCA of 57 and 61 is "<<temp->data<<endl;

  delete_node(root, 60);
  cout<<"after deleting 60"<<endl;
  in_order(root);
  cout<<endl;
  cout<<"level order\n";
  level_order(root);
  cout<<endl;
return 0;
}
