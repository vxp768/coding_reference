#ifndef __TREE_HEADER__
#define __TREE_HEADER__

#include <iostream>

using namespace std;

class Traversal {

 public:
   void inorder(Tree *root);
   void preorder(Tree *root);
   void postorder(Tree *root);

}

class Traversal_norecur {

 public:
   void inorder(Tree *root);
   void preorder(Tree *root);
   void postorder(Tree *root);

}

class Tree : public Traversal {
 private:
   int data;
   Tree *left;
   Tree *right;
 public:
   Tree() {};
   Tree *allocate(int data);
   Tree *insert(Tree *root, int data);
/*   delete();
   find_data();
   find_min();
   find_max();

   level_order();
*/
};


#endif //__TREE_HEADER__
