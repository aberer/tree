template<class DATA>
Tree<DATA>::Tree()
  : _nodes{ }
  , _children{}
  , _parent{}
  , _unused{}
  , _root{0}
{
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::insert( dfs_iterator iter, DATA data )
{
  bool isRoot = _nodes.empty(); 
  auto id = extend( data);

  if(not isRoot)
    {
      _parent[id] = iter._id;
      _children[_parent[id]].insert(id);
    }

  auto siz = size();
  auto unused = _unused.size(); 
  assert(siz == _nodes.size() - unused );
  assert(siz == _children.size() - unused );
  assert(siz == _parent.size() - unused ); 
    
  return dfs_iterator(*this, id);
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::erase(typename Tree<DATA>::dfs_iterator iterator)
{
  std::cout << "erasing " << *iterator << std::endl; 
  
  auto result = iterator;
  auto targetNode = iterator; 
  if( iterator.child() != end()  ) // has children 
    {
      std::cout << "REPL with child " << *(iterator.child()) << std::endl; 
      
      auto childNode = iterator.child();
      std::swap( *targetNode, *childNode );

      std::cout << "swapped"<< std::endl; 

      // parentnode  adopts children; children recognize new parent 
      auto &children = _children.at(childNode._id);
      _children.at(targetNode._id).insert(children.begin(),children.end());
      _children.at(targetNode._id).erase(childNode._id);
      for(auto &v : children)
        _parent.at(v) = targetNode._id;
      children.clear();
      
      // push to unused stack
      _unused.push(childNode._id);
    }
  else
    {
      result = iterator.next();
      _unused.push(iterator._id);

      auto par = iterator.parent();
      if(par != iterator)
        _children.at(par._id).erase(iterator._id);
    }


  
  
  std::cout << "everything alright! returning " << *result << std::endl; 

  return result; 
}


template<class DATA>
typename Tree<DATA>::id_t Tree<DATA>::extend( DATA data )
{
  // replace with "get next node" and use a stack
  auto result = id_t(_nodes.size());
  
  _nodes.push_back( data );
  _children.push_back( set<id_t>{} );
  _parent.push_back( result );
  
  return result; 
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::insert( DATA data )
{
  auto wasEmpty = size() == 0 ;  
  auto id = extend(data);
 
  if(not wasEmpty)
    {
      _children.at(id).insert(_root);
      _parent.at(_root) = id;
    }

  _root = id;
  return dfs_iterator(*this, id);
}



template<class DATA>
void Tree<DATA>::dumplinks()
{
  std::cout << "PARENTS:" << std::endl;
  int ctr = 0; 
  for(auto &v : _parent)
    std::cout << ctr++ << "\t" << v << std::endl;

  std::cout << "CHILDREN: " << std::endl;
  ctr = 0; 
  for(auto &children : _children)
    {
      std::cout <<  ctr++ << "\t"; 
      for(auto &v : children)
        std::cout <<  v << "," ;
      std::cout << std::endl; 
    }
      
}


// ================================================================
// iterator functions
// ================================================================


template<class DATA>
typename Tree<DATA>::dfs_iterator& Tree<DATA>::dfs_iterator::operator++() {
  *this = next();
  return *this;
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::dfs_iterator::next()
{
  auto end = _tree.get().end();
  auto begin = _tree.get().begin();
  auto result = *this;
  auto c = child();

  // cout << "hello?" << std::endl; 
  
  if( c != end )
    {
      cout << "has children!"  << std::endl; 
      result = c;
    }
  else
    {
      auto maybeResult = *this;
      while( true )             // efficiency
        {
          if(maybeResult.sibling() == end )
            {
              maybeResult = maybeResult.parent();
              if( maybeResult == begin) // at root 
                {
                  cout << "is root!"  << std::endl; 
                  maybeResult = end; 
                  break; 
                }
            }
          else
            {
              cout << "has sibling!"  << std::endl; 
              maybeResult = maybeResult.sibling();
              break; 
            }
        }

      result = maybeResult; 
    }
      
  return result; 
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::dfs_iterator::child()
{
  auto result = _tree.get().end();
  if( not  _tree.get()._children.at(_id).empty() )
    {
      auto iter = _tree.get()._children.at(_id).begin(); 
      result = dfs_iterator(_tree,  *iter);
    }
  return result; 
}


template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::dfs_iterator::parent()
{
  auto result = _tree.get().end();
  if( _tree.get()._root != _id)
    result = dfs_iterator(_tree,_tree.get()._parent.at(_id));
  return result; 
}



template<class DATA>
typename Tree<DATA>::dfs_iterator Tree<DATA>::dfs_iterator::sibling()
{
  auto result = _tree.get().end();
  
  if(*this != _tree.get().begin()) // not root
    {
      auto parentId = _tree.get()._parent.at(_id);
      auto iter = _tree.get()._children.at(parentId).find(_id);
      auto endElem = _tree.get()._children.at(parentId).end(); 
      assert(iter != endElem); // must be there
      ++iter;
      if(iter != endElem)       // has another sibling  
        result = dfs_iterator(_tree, *iter);
    }
  
  return result; 
}
