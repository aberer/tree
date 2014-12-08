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
  friend class dfs_iterator;
  using id_t = int;
  class dfs_iterator; 
  
public:
  Tree();

  dfs_iterator begin_dfs() { return dfs_iterator(*this, _root); }
  dfs_iterator end_dfs() { return dfs_iterator(*this, _nodes.size());  }
  dfs_iterator begin()  {return begin_dfs(); }
  dfs_iterator end()  { return end_dfs(); }
  size_t size() const {return _nodes.size();  } // TODO !!! 
  /** 
      @brief inserts a data point as a child of a the element pointed to by the iterator 
   */ 
  dfs_iterator insert( dfs_iterator iter, DATA data );
  /** 
      @brief inserts an element as root of the tree  
   */ 
  dfs_iterator insert( DATA data); 
  // just debug 
  void dumplinks();

  dfs_iterator erase(dfs_iterator iterator); 
  
private:                        // METHODS
  /** 
      @brief TODO  
   */ 
  id_t extend( DATA data ); 
  
private:
  vector<DATA> _nodes;
  vector< set<id_t> > _children;
  vector<id_t> _parent;
  stack<id_t> _unused; 
  id_t _root; 
}; 


// auxilliary functions 
// template<class DATA> typename Tree<DATA>::dfs_iterator begin(Tree<DATA> &t) {  return t.begin(); }
// template<class DATA> typename Tree<DATA>::dfs_iterator end( Tree<DATA> &t) {  return t.end(); }


template<class DATA>
class Tree<DATA>::dfs_iterator
{
public: 
  friend class Tree<DATA>;
    
  dfs_iterator(Tree<DATA>& tree, id_t id) : _tree(tree) , _id{id} { }
  DATA& operator*()  { return _tree.get()._nodes.at(_id); }
  /** 
      @brief gets an iterator pointing to the next element in a DFS traversal  
   */ 
  dfs_iterator next() ;
  dfs_iterator& operator++();
  /** 
      @brief gets an iterator pointing to the first child of the current element
   */ 
  dfs_iterator child();
  /** 
      @brief gets an iterator pointing to the next sibling of the current element 
   */ 
  dfs_iterator sibling() ;
  /** 
      @brief gets an iterator pointing to the parent of the current element 
   */ 
  dfs_iterator parent(); 
  /**
     debug only 
   */ 
  id_t getId() const  {return _id; }

  
  
  friend bool operator==(dfs_iterator const& lhs, dfs_iterator const& rhs) { return lhs._id == rhs._id; }
  friend bool operator!=(dfs_iterator const& lhs, dfs_iterator const& rhs){ return not(lhs == rhs); }

private:
  reference_wrapper< Tree<DATA> > _tree;
  id_t _id; 
};


#include "Tree.tpp"

#endif /* TREE_H */

