/*
 * C++ program to Implement AVL Tree
 * All AVL logic taken from: https://www.sanfoundry.com/cpp-program-implement-avl-trees/
 * Made modifications to insert document related info and view it
 */
#include "avltree.hpp"

string dict = "dictionary.txt", //   output files
    post = "postings.txt", 
    doc = "docids.txt",
    in = "inputFiles/a4documents.txt",  //  input files
    test = "inputFiles/test.txt",
    fullTestPath = "inputFiles/test.txt_1";
 
/*
 * Code example from the link mentioned above^^
 */
// int main()
// {
//     int choice, item;
//     avlTree avl;
//     while (1)
//     {
//         cout<<"\n---------------------"<<endl;
//         cout<<"AVL Tree Implementation"<<endl;
//         cout<<"\n---------------------"<<endl;
//         cout<<"1.Insert Element into the tree"<<endl;
//         cout<<"2.Display Balanced AVL Tree"<<endl;
//         cout<<"3.InOrder traversal"<<endl;
//         cout<<"4.PreOrder traversal"<<endl;
//         cout<<"5.PostOrder traversal"<<endl;
//         cout<<"6.Exit"<<endl;
//         cout<<"Enter your Choice: ";
//         cin>>choice;
//         switch(choice)
//         {
//         case 1:
//             cout<<"Enter value to be inserted: ";
//             cin>>item;
//             root = avl.insert(root, item);
//             break;
//         case 2:
//             if (root == NULL)
//             {
//                 cout<<"Tree is Empty"<<endl;
//                 continue;
//             }
//             cout<<"Balanced AVL Tree:"<<endl;
//             avl.display(root, 1);
//             break;
//         case 3:
//             cout<<"Inorder Traversal:"<<endl;
//             avl.inorder(root);
//             cout<<endl;
//             break;
//         case 4:
//             cout<<"Preorder Traversal:"<<endl;
//             avl.preorder(root);
//             cout<<endl;
//             break;
//         case 5:
//             cout<<"Postorder Traversal:"<<endl;
//             avl.postorder(root);    
//             cout<<endl;
//             break;
//         case 6:
//             exit(1);    
//             break;
//         default:
//             cout<<"Wrong Choice"<<endl;
//         }
//     }
//     return 0;
// }

void printStack(stack<documentNode> docList) {

    /* WARNING PERMANENTLY CHANGES AVL DS BY REMOVING
    DOCUMENTNODES FROM DOCLIST */
	
	struct documentNode newNode;
	
	while (! docList.empty()) {
		newNode = docList.top();
		docList.pop();
		
		cout << "docNo: " + to_string(newNode.docNo) + " freq: " + to_string(newNode.freq) <<endl;
	}
}
 
/*
 * Height of AVL Tree
 */
int avlTree::height(avl_node *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    return h;
}
 
/*
 * Height Difference 
 */
int avlTree::diff(avl_node *temp)
{
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;
    return b_factor;
}
 
/*
 * Right- Right Rotation
 */
avl_node *avlTree::rr_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}
/*
 * Left- Left Rotation
 */
avl_node *avlTree::ll_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}
 
/*
 * Left - Right Rotation
 */
avl_node *avlTree::lr_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = rr_rotation (temp);
    return ll_rotation (parent);
}
 
/*
 * Right- Left Rotation
 */
avl_node *avlTree::rl_rotation(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = ll_rotation (temp);
    return rr_rotation (parent);
}
 
/*
 * Balancing AVL Tree
 */
avl_node *avlTree::balance(avl_node *temp)
{
    int bal_factor = diff (temp);
    if (bal_factor > 1)
    {
        if (diff (temp->left) > 0)
            temp = ll_rotation (temp);
        else
            temp = lr_rotation (temp);
    }
    else if (bal_factor < -1)
    {
        if (diff (temp->right) > 0)
            temp = rl_rotation (temp);
        else
            temp = rr_rotation (temp);
    }
    return temp;
}
 
/*
 * Insert Element into the tree
 */
avl_node *avlTree::insert(avl_node *root, string value, int docNo)
{
    if (root == NULL)
    {
        root = new avl_node;
        root->data = value;
        root->left = NULL;
        root->right = NULL;
		
		struct documentNode newDoc;
        newDoc.docNo = docNo;
        newDoc.freq = 1;

        root->docList.push(newDoc);
		
        return root;
    }
    else if (value.compare(root->data) < 0)
    {
        root->left = insert(root->left, value, docNo);
        root = balance (root);
    }
    else if (value.compare(root->data) > 0){
        root->right = insert(root->right, value, docNo);
        root = balance (root);
    }
    else {	//	the keys are equal!  increment the count

        if (root->docList.empty() || root->docList.top().docNo != docNo) {  //  insert the new document under the term
            struct documentNode newDoc;
            newDoc.docNo = docNo;
            newDoc.freq = 1;

            root->docList.push(newDoc);
        }
        else {	//	other wise, we are still in the same document
			//	so add to that document's count
            root->docList.top().freq++;
        }
    }
    return root;
}
 
/*
 * Display AVL Tree
 */
void avlTree::display(avl_node *root, avl_node *ptr, int level)
{
    int i;
    if (ptr!=NULL)
    {
        display(root, ptr->right, level + 1);
        printf("\n");
        if (ptr == root)
        cout<<"Root -> ";
        for (i = 0; i < level && ptr != root; i++)
            cout<<"        ";
        cout<<ptr->data;
        display(root, ptr->left, level + 1);
    }
}
 
/*
 * Inorder Traversal of AVL Tree
 */
void avlTree::inorder(avl_node *tree)
{
    /* WARNING PERMANENTLY CHANGES AVL DS BY REMOVING
    DOCUMENTNODES FROM DOCLIST */

    if (tree == NULL)
        return;
    inorder (tree->left);
    cout<<tree->data<<endl;
	printStack(tree->docList); //  this is the function that removes elements from the stack
    inorder (tree->right);
}

/*
 * Preorder Traversal of AVL Tree
 */
void avlTree::preorder(avl_node *tree)
{
    if (tree == NULL)
        return;
    cout<<tree->data<<"  ";
    preorder (tree->left);
    preorder (tree->right);
 
}
 
/*
 * Postorder Traversal of AVL Tree
 */
void avlTree::postorder(avl_node *tree)
{
    if (tree == NULL)
        return;
    postorder ( tree ->left );
    postorder ( tree ->right );
    cout<<tree->data<<"  ";
}

int avlTree::size(avl_node *tree, int count) {
    //  traverses and counts each node

    if (tree == NULL)   return count;   //  do not increment, just return previous value
    count = size (tree->left, count);
    count++;    //  now count this node
    count = size (tree->right, count);
    return count;   //  return count after visiting all children and this node
}

void avlTree::inorderDictionaryWrite(avl_node *tree, ofstream &out) {
    if (tree == NULL)
        return;
    inorderDictionaryWrite (tree->left, out);

    //  format: data doc_freq
    out << tree->data + " " + to_string(tree->docList.size()) + "\n";

    inorderDictionaryWrite (tree->right, out);
}

void avlTree::generateDictionary(avl_node *tree) {
    int nTerms = size(tree, 0);    //  size() should always be performed on root
    ofstream out(dict);

    out << to_string(nTerms) + "\n";    //  header

    inorderDictionaryWrite(tree, out);  //  now write the terms
}

int avlTree::totalStackSize(avl_node *tree, int count) {
    //  traverses and counts each node

    if (tree == NULL)   return count;   //  do not increment, just return previous value
    count = totalStackSize (tree->left, count);
    count += tree->docList.size();    //  now count this node
    count = totalStackSize (tree->right, count);
    return count;   //  return count after visiting all children and this node
}

void avlTree::inorderPostingsWrite(avl_node *tree, ofstream &out) {
    /* WARNING PERMANENTLY CHANGES AVL DS BY REMOVING
        DOCUMENTNODES FROM DOCLIST */

    if (tree == NULL)
        return;
    inorderPostingsWrite (tree->left, out);

    //  format: data doc_freq
    while(!tree->docList.empty()) {
        struct documentNode doc = tree->docList.top();
        tree->docList.pop();

        //  write to file the details
        out << to_string(doc.docNo) + " " + to_string(doc.freq) + "\n";
    }

    inorderPostingsWrite (tree->right, out);
}

void avlTree::generatePostings(avl_node *tree) {
    /* WARNING PERMANENTLY CHANGES AVL DS BY REMOVING
        DOCUMENTNODES FROM DOCLIST */

    int nEntries = totalStackSize(tree, 0);

    ofstream out(post);

    out << to_string(nEntries) + "\n";

    inorderPostingsWrite(tree, out);
}