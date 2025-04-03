#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<int, double> bst;
	bst.insert(std::make_pair(2, 5.0));
	bst.insert(std::make_pair(1, 3.0));
	bst.insert(std::make_pair(3, 2.0));
    //bst.printRoot();
    // for(BinarySearchTree<int,double>::iterator it = bst.begin(); it != bst.end(); ++it) {
    //     // cout<<"hhh"<<endl;
    //     cout << it->first << " " << it->second << endl;
    // }
    bst.remove(2);
    for(BinarySearchTree<int,double>::iterator it = bst.begin(); it != bst.end(); ++it) {
        // cout<<"hhh"<<endl;
        cout << it->first << " " << it->second << endl;
    }
    // if(bt.find('b') != bt.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // bt.remove('b');

    // AVL Tree Tests
    // cout<<"1"<<endl;
    // AVLTree<char,int> at;
    // cout<<"2"<<endl;
    // at.insert(std::make_pair('a',1));
    // cout<<"3"<<endl;
    // at.insert(std::make_pair('b',2));
    // cout<<"4"<<endl;
    // cout << "\nAVLTree contents:" << endl;
    // for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }
    // if(at.find('b') != at.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // at.remove('b');
    AVLTree<uint16_t, uint16_t> testTree;

	testTree.insert(std::make_pair(6, 8));
	testTree.insert(std::make_pair(3, 159));
	testTree.insert(std::make_pair(1, 9));
	testTree.insert(std::make_pair(10, 0x5a));
	testTree.insert(std::make_pair(5, 0xb));
	testTree.insert(std::make_pair(2, 4));
	testTree.insert(std::make_pair(4, 159));
      return 0;
}
