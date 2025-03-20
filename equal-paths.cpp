#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


int numChildren(Node* root){
  if(root==nullptr){
    return 0;
  }else{
    return 1 + numChildren(root->right) + numChildren(root->left);
  }
}

int depth(Node* root){
  if(root==nullptr){
    return 0;
  }else{
    int ldepth = depth(root->left);
    int rdepth = depth(root->right);
    if(ldepth>rdepth){
      return 1 + ldepth;
    }else{
      return 1 + rdepth;
    }
  }
}

bool equalPaths(Node * root)
{
    // Add your code below
  if(root==nullptr){
    return true;
  }
  int left_depth = depth(root->left);
  int right_depth = depth(root->right);
  if(left_depth==right_depth || left_depth==0 || right_depth == 0){
    return (equalPaths(root->right) && equalPaths(root->left));
  }else{
    return false;
  }

}

