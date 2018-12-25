#include <iostream>

using namespace std;
#define MAX_ALPHABETS 256

class trieNode {
    trieNode *child[MAX_ALPHABETS];
    int end_word;
  public:
    trieNode();
    int  insert (trieNode*, string);
    int  search (trieNode*, string);
    int  delete_key (trieNode*, string, int);
    void get_all_keys (trieNode*, char*, int);
    void longest_prefix_match (trieNode*, string);
    //void match_key_wildcard (trieNode*, string);
};

trieNode::trieNode () 
{
    for (int i=0; i<MAX_ALPHABETS; i++) {
        child[i] = NULL;
    }
    end_word = false;
}

int trieNode::insert (trieNode *t, string key)
{
  int i=0;
  for (i=0; i<key.length(); i++) {
    if (t->child[key[i]] == NULL) {
        t->child[key[i]] = new trieNode;
    }
    t = t->child[key[i]];
  }
  t->end_word = true;
}

int trieNode::search (trieNode *t, string key)
{
  int i=0;
  for (i=0; i<key.length(); i++) {
      if (t->child[key[i]] == NULL) {
          return false;
      } else {
         t = t->child[key[i]];
      }
  }
  /*t could become NULL...e.g. if only last char differs */
  if (t != NULL && t->end_word) {
      return true;
  } else {
      return false;
  }
}

void trieNode::get_all_keys(trieNode* t, char *allkeys, int idx)
{
   int i=0;
   int key_index = idx;
   if (t->end_word){
       allkeys[idx] = '\0';
       cout<<"Str: "<<allkeys<<endl;
   }
   for (i=0; i<MAX_ALPHABETS; i++) {
	/* check all child at any level */
       key_index = idx;
       if (t->child[i]) {
           allkeys[key_index++] = i;
           get_all_keys(t->child[i], allkeys, key_index);
       }
   }
}
int trieNode::delete_key (trieNode *t, string key, int key_idx)
{
   int i = 0;
   if (t == NULL || key_idx >= key.length())
       return 0;
   for (i = 0; i < MAX_ALPHABETS; i++) {
       if (i == key[key_idx]) {
	   if (delete_key(t->child[i], key, ++key_idx)) {
               for (i=0; i<MAX_ALPHABETS; i++) {
		    if (t->child[i] != NULL) {
			break;
		    } 
               }
	       if( i == MAX_ALPHABETS  ) {
	           t->child[i]->end_word = true;
	       }
           }
	   break;	
        }
   }
   if (i < MAX_ALPHABETS && t->child[i]->end_word == true) {
	t->child[i]->end_word = false;
	int j;
        for (j=0; j<MAX_ALPHABETS; j++) {
	    if(t->child[j] != NULL) {
		break;
	    }
	}
        if (j == MAX_ALPHABETS) {
            cout<<" deleting"<<endl;
	    delete(t->child[i]);
            t->child[i] = NULL;
	    return 1;
        }
   }
   return 0;
}
void trieNode::longest_prefix_match (trieNode *t, string key)
{
    int i = 0;
    char allkeys[256];
    for (i=0; i< key.length(); i++) {
         if (t->child[key[i]] != NULL) {
             allkeys[i] = key[i];
	     t = t->child[key[i]];
	 } else {
	     allkeys[i] = '\0';
             cout<<"longest prefix match "<<allkeys<<endl;
	     break;
	 }
    }
}

/*
void match_key_wildcard (trieNode *t, string key)
{
   int i=0;

   for (i=0; i<key.length(); i++) {
        if (key[i] == '?') {

	} else {
	    if (t->child[key[i]] != NULL) {

	    } else {

	    }
	}
   }
}*/

int main()
{
    string keys[] = {"she", "sells", "sea", "shells", "by", "the",
                     "sea", "shore", "010101"};
    //string allkeys;
    char allkeys[256];
    int key_idx = 0;
    trieNode *trieST = new trieNode;
    for(int i=0; i<9; i++) {
        trieST->insert(trieST, keys[i]);
    }
    trieST->search(trieST, "shells")?cout<<"found\n":cout<<"not found\n";
    trieST->search(trieST, "shoren")?cout<<"found\n":cout<<"not found\n";
    trieST->get_all_keys(trieST, allkeys, key_idx);
    trieST->longest_prefix_match(trieST, "shellsppppp");
    trieST->longest_prefix_match(trieST, "tpp");
    trieST->delete_key(trieST, "she", 0);
    cout<<"after deleting she"<<endl;
    trieST->get_all_keys(trieST, allkeys, key_idx);
return 0;
}
