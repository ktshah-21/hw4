#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <queue>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void rightRotation(AVLNode<Key, Value>* n1);
    virtual void leftRotation(AVLNode<Key, Value>* n1);
    virtual AVLNode<Key, Value>* internalFind(const Key& key) const ;
    virtual void insert_fix (AVLNode<Key, Value>* p, AVLNode<Key,Value>* n);
    virtual void remove_fix(AVLNode<Key, Value>* n, int diff);
    virtual AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  if(this->root_==nullptr){
      AVLNode<Key, Value>* new_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      new_->setBalance(0);
      this->root_ = new_;
    }else{
      // bool check_ = true;
      AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); 
      AVLNode<Key, Value>* parent = nullptr;  
      int side = 0;
      while(curr!=nullptr){
        if(new_item.first > curr->getKey()){
          parent = curr;
          curr = curr->getRight();
          side=0;
        }else if(new_item.first == curr->getKey()){
          curr->setValue(new_item.second);
          side=10;
          break;
        }else{
          parent = curr;
          curr = curr->getLeft();
          side=1;
        }
      }
      if(side<10){
        curr = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
        if(side==0){
          parent->setRight(curr);
          curr->setBalance(0);
          int bal = parent->getBalance();
          if(bal==0){
            parent->setBalance(1);
            insert_fix(parent, curr);
          }else{
            parent->setBalance(0);
          }
        }else{
          parent->setLeft(curr);
          //->updateBalance(1);
          //rebalance(parent);
          curr->setBalance(0);
          int bal = parent->getBalance();
          if(bal==0){
            parent->setBalance(-1);
            insert_fix(parent, curr);
          }else{
            parent->setBalance(0);
          }
        }
      }      
    }
    // BinarySearchTree<Key, Value>::printRoot(this->root_);
}


template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix (AVLNode<Key, Value>* p, AVLNode<Key,Value>* n){
  if(p!=nullptr && p->getParent()!=nullptr){
    AVLNode<Key, Value>* g = p->getParent();
    if(g->getLeft()==p){
      g->updateBalance(-1);
      int bal = g->getBalance();
      if(bal==0){
        return;
      }else if(bal==-1){
        insert_fix(g,p);
      }else{
        if(p->getBalance()<0){
          rightRotation(g);
          g->setBalance(0);
          p->setBalance(0);
        }else{
          leftRotation(p);
          rightRotation(g);
          int new_bal = n->getBalance();
          if(new_bal==-1){
            p->setBalance(0);
            g->setBalance(1);
            n->setBalance(0);
          }else if(new_bal==0){
            p->setBalance(0);
            g->setBalance(0);
            n->setBalance(0);
          }else{
            p->setBalance(-1);
            g->setBalance(0);
            n->setBalance(0);
          }
        }
        return;
      }
    }else if(g->getRight()==p){
      // std::cout<<"11"<<std::endl;
      g->updateBalance(1);
      int bal = g->getBalance();
      if(bal==0){
        // std::cout<<"sdsa"<<std::endl;
        return;
      }else if(bal==1){
        // std::cout<<"Sdfdsz"<<std::endl;
        insert_fix(g,p);
      }else{
        if(p->getBalance()>0){
          // std::cout<<"22"<<std::endl;
          leftRotation(g);
          g->setBalance(0);
          p->setBalance(0);
        }else{
          // std::cout<<"223"<<std::endl;
          rightRotation(p);
          leftRotation(g);
          int new_bal = n->getBalance();
          if(new_bal==1){
            p->setBalance(0);
            g->setBalance(-1);
            n->setBalance(0);
          }else if(new_bal==0){
            p->setBalance(0);
            g->setBalance(0);
            n->setBalance(0);
          }else{
            p->setBalance(1);
            g->setBalance(0);
            n->setBalance(0);
          }
        }
        return;
      }
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* curr = internalFind(key);
  if(curr==nullptr){
    return;
  }else{
    if(curr->getLeft() && curr->getRight()) {
        AVLNode<Key, Value>* pred = predecessor(curr->getLeft());
        nodeSwap(curr, pred);
    }
    AVLNode<Key, Value>* parent = curr->getParent();

    AVLNode<Key, Value>* child = nullptr;
    // AVLNode<Key, Value>* parent = nullptr;
    if(curr->getLeft()){
      child = curr->getLeft();
      parent = curr->getParent();
    }else{
      child = curr->getRight();
      parent = curr->getParent();
    }
    if(child){
      child->setParent(parent);
    }
    int diff=0;
    if(parent){
      if(parent->getLeft()==curr){
        parent->setLeft(child);
        diff=1;
        //parent->updateBalance(1);
      }else{
        parent->setRight(child);
        diff=-1;
        //parent->updateBalance(-1);
      }
    }else{
      this->root_ = child;
    }
    delete curr;
    remove_fix(parent, diff);


  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::remove_fix(AVLNode<Key, Value>* n, int diff)
{
  if(n==nullptr){
    return;
  }else{
    AVLNode<Key, Value>* p = n->getParent();
    int n_diff =0;
    if(p!=nullptr){
      if(p->getLeft()==n){
        n_diff = 1;
      }else{
        n_diff = -1;
      }
    }
    if(diff==-1){
      if(n->getBalance() + diff==-2){
        AVLNode<Key,Value>* c = n->getLeft();
        if(c->getBalance()==-1){
          rightRotation(n);
          n->setBalance(0);
          c->setBalance(0);
          remove_fix(p, n_diff);
        }else if(c->getBalance()==0){
          rightRotation(n);
          n->setBalance(-1);
          c->setBalance(1);
          return; 
        }else{
          AVLNode<Key, Value>* g = c->getRight();
          leftRotation(c);
          rightRotation(n);
          if(g->getBalance()==1){
            n->setBalance(0);
            c->setBalance(-1);
            g->setBalance(0);
          }else if(g->getBalance()==0){
            n->setBalance(0);
            c->setBalance(0);
            g->setBalance(0);
          }else{
            n->setBalance(1);
            c->setBalance(0);
            g->setBalance(0);
          }
          remove_fix(p, n_diff);
        }
      }else if(n->getBalance() + diff == -1){
        n->setBalance(-1);
      }else{
        n->setBalance(0);
        remove_fix(p, n_diff);
      }
    }else{
      if(n->getBalance() + diff==2){
        AVLNode<Key,Value>* c = n->getRight();
        if(c->getBalance()==1){
          leftRotation(n);
          n->setBalance(0);
          c->setBalance(0);
          remove_fix(p, n_diff);
        }else if(c->getBalance()==0){
          leftRotation(n);
          n->setBalance(1);
          c->setBalance(-1);
          return; 
        }else{
          AVLNode<Key, Value>* g = c->getLeft();
          rightRotation(c);
          leftRotation(n);
          if(g->getBalance()==-1){
            n->setBalance(0);
            c->setBalance(1);
            g->setBalance(0);
          }else if(g->getBalance()==0){
            n->setBalance(0);
            c->setBalance(0);
            g->setBalance(0);
          }else{
            n->setBalance(-1);
            c->setBalance(0);
            g->setBalance(0);
          }
          remove_fix(p, n_diff);
        }
      }else if(n->getBalance() + diff == 1){
        n->setBalance(1);
      }else{
        n->setBalance(0);
        remove_fix(p, n_diff);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation( AVLNode<Key,Value>* n1)
{
    
    if(!n1 || !(n1->getLeft())) return;
    // std::cout<<n1->getValue()<<std::endl;
    AVLNode<Key, Value>* l = n1->getLeft();
    // AVLNode<Key, Value>* r = n1->getRight();
    AVLNode<Key, Value>* gp = n1->getParent();
    AVLNode<Key, Value>* preserve = nullptr;
    if(l){
      preserve = l->getRight();
    }
    l->setRight(n1);
    // std::cout<<l->getRight()->getKey()<<std::endl;
    n1->setParent(l);
    n1->setLeft(preserve);
    if(preserve){
      preserve->setParent(n1);
    }
    // std::cout<<333<<std::endl;
    // std::cout<<l->getRight()->getKey()<<std::endl;
    
    l->setParent(gp);
    if(gp){
      if(gp->getRight()==n1){
        gp->setRight(l);
      }else{
        gp->setLeft(l);
      }
    }else{
      this->root_ = l;
    }

    // n2 = l;


}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation( AVLNode<Key,Value>* n1)
{
    if(!n1 || !(n1->getRight())){return;}
    // AVLNode<Key, Value>* l = n1->getLeft();
    AVLNode<Key, Value>* r = n1->getRight();
    AVLNode<Key, Value>* preserve = r->getLeft();
    AVLNode<Key, Value>* gp = n1->getParent();
    r->setLeft(n1);
    n1->setParent(r);
    n1->setRight(preserve);
    if(preserve){
      preserve->setParent(n1);
    }

    //AVLNode<Key, Value>* gp = n1->getParent();
    r->setParent(gp);
    if(gp){
      if(gp->getRight()==n1){
        gp->setRight(r);
      }else{
        gp->setLeft(r);
      }
    }else{
      this->root_ = r;
    }
   
    // n2=r;
}
 


template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& key) const  
{
  AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
  //Node<Key, Value>* parent = nullptr;
  while(curr!=nullptr){
    if(key > curr->getKey()){
      //parent = curr;
      curr = curr->getRight();
      //side=0;
    }else if(key == curr->getKey()){
      return curr;
    }else{
      //parent = curr;
      curr = curr->getLeft();
      //side=1;
    }
  }
  return nullptr;
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
    if(current->getRight() == nullptr){
      return current;
    }else{
      return predecessor(current->getRight());
    }
}

#endif
