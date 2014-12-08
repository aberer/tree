#ifndef TREE_H
#define TREE_H

#define SHOW(x)  #x << "=" << x << "\t"


#include <iostream>
#include <cassert>
#include <map> 
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <set>
#include <stack>
#include <exception>


using std::stack; 
using std::map; 
using std::list; 
using std::unordered_map;
using std::cout ; 
using std::string;
using std::ostream; 
using std::vector; 
using std::unique_ptr;
using std::cin ;
using std::endl ;
using std::set;
using std::reference_wrapper; 
using std::exception; 



template<class DATA>
class Tree
{
  friend class iterator;
  using id_t = int;
  class iterator; 
  
public:
  Tree();

  iterator begin() ;
  iterator end() ;
  /** 
      @brief gets the number of elements inside the container 
   */ 
  size_t size() const ;
  /** 
      @brief gets the internal capacity before reallocation is necessary  
   */ 
  size_t capacity() const {return _nodes.size(); } 
  /** 
      @brief inserts a data point as a child of a the element pointed to by the iterator 
   */ 
  iterator insert( iterator iter, DATA data );
  /** 
      @brief inserts an element as root of the tree  
   */ 
  iterator insert( DATA data); 
  // just debug 
  void dumplinks();
  /** 
      @brief erases the element represented by iterator and returns an
      iterator pointing to the next element
   */ 
  iterator erase(iterator iterator); 
  
private:                        // METHODS
  /** 
      @brief gets the id of an unused node    
  */ 
  id_t allocateNode(); 
  
private:
  vector<DATA> _nodes;
  vector< set<id_t> > _children;
  vector<id_t> _parent;
  stack<id_t> _unused; 
  id_t _root; 
}; 


// auxilliary functions 
// template<class DATA> typename Tree<DATA>::iterator begin(Tree<DATA> &t) {  return t.begin(); }
// template<class DATA> typename Tree<DATA>::iterator end( Tree<DATA> &t) {  return t.end(); }


template<class DATA>
class Tree<DATA>::iterator
{
public: 
  friend class Tree<DATA>;
    
  iterator(Tree<DATA>& tree, id_t id) : _tree(tree) , _id{id} { }
  DATA& operator*()  { return _tree.get()._nodes.at(_id); }
  /** 
      @brief gets an iterator pointing to the next element in a DFS traversal  
   */ 
  iterator next() ;
  iterator& operator++();
  /** 
      @brief gets an iterator pointing to the first child of the current element
   */ 
  iterator child();
  /** 
      @brief gets an iterator pointing to the next sibling of the current element 
   */ 
  iterator sibling() ;
  /** 
      @brief gets an iterator pointing to the parent of the current element 
   */ 
  iterator parent(); 
  /**
     debug only 
   */ 
  id_t getId() const  {return _id; }
  
  friend bool operator==(iterator const& lhs, iterator const& rhs) { return lhs._id == rhs._id; }
  friend bool operator!=(iterator const& lhs, iterator const& rhs){ return not(lhs == rhs); }

private:
  reference_wrapper< Tree<DATA> > _tree;
  id_t _id; 
};


#include "Tree.tpp"

#endif /* TREE_H */

