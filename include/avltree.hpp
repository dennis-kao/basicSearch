#ifndef __AVL_TREE_HPP__ 
#define __AVL_TREE_HPP__ 

/*
 * C++ program to Implement AVL Tree
 * Taken from: https://www.sanfoundry.com/cpp-program-implement-avl-trees/
 */
#include<iostream>
#include<cstdio>
#include<sstream>
#include<algorithm>
#include<string.h>
#include<stack>
#include<fstream>

#define pow2(n) (1 << (n))
using namespace std;

extern string dict, //   output files
    post, 
    doc,
    in,  //  input files
    test,
    fullTestPath;

struct docDetails {
    string docID;
    unsigned int titleOffset;
    unsigned int bodyOffset;
};

struct documentNode
{
    int docNo;
    unsigned int freq;
};
 
/*
 * Term Declaration
 */
struct avl_node
{
    string data;
    stack<documentNode> docList;
    struct avl_node *left;
    struct avl_node *right;
};
 
/*
 * Class Declaration
 */
class avlTree
{
    public:
        int height(avl_node *);
        int diff(avl_node *);
        avl_node *rr_rotation(avl_node *);
        avl_node *ll_rotation(avl_node *);
        avl_node *lr_rotation(avl_node *);
        avl_node *rl_rotation(avl_node *);
        avl_node* balance(avl_node *);
        avl_node* insert(avl_node *, string, int);
        void display(avl_node *, avl_node *, int);
        void inorder(avl_node *);
        void preorder(avl_node *);
        void postorder(avl_node *);
        int size(avl_node *, int);
		
		/* My methods begin here */
        void inorderDictionaryWrite(avl_node *, ofstream &);
        void generateDictionary(avl_node *);
        int totalStackSize(avl_node *, int);
        void inorderPostingsWrite(avl_node *, ofstream &);
        void generatePostings(avl_node *tree);
};

#endif