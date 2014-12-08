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
typename Tree<DATA>::iterator Tree<DATA>::begin()
{
  if( size() == 0) 
    return end();
  else
    return iterator(*this, _root);
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::end()
{
  return iterator(*this, _nodes.size());
}


template<class DATA>
size_t Tree<DATA>::size() const
{
  return _nodes.size() - _unused.size() ;
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::insert( iterator iter, DATA data )
{
  auto result = iter; 
  
  if( size() == 0  )
    {
      result =  insert(data);
    }
  else 
    {
      auto id = allocateNode();
      _nodes.at(id)= data ; 

      _parent[id] = iter._id;
      _children[_parent[id]].insert(id);

      auto siz = size();
      auto unused = _unused.size(); 
      assert(siz == _nodes.size() - unused );
      assert(siz == _children.size() - unused );
      assert(siz == _parent.size() - unused ); 
    
      result =  iterator(*this, id);
    }
  
  return result ; 
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::erase(typename Tree<DATA>::iterator iterator)
{
  auto result = iterator;
  auto targetNode = iterator; 
  if( iterator.child() != end()  ) // has children 
    {
      auto childNode = iterator.child();
      std::swap( *targetNode, *childNode );

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
      if(par != end() && par != iterator)
        _children.at(par._id).erase(iterator._id);
    }

  return result; 
}


template<class DATA>
typename Tree<DATA>::id_t Tree<DATA>::allocateNode()
{
  auto result = 0;
  
  if(_nodes.size() == size())
    {
      result = id_t(_nodes.size());
      
      _nodes.resize(_nodes.size() + 1 );
      _children.push_back( set<id_t>{} );
      _parent.push_back( result );
    }
  else
    {
      result = _unused.top();
      _unused.pop(); 
    }
  
  return result; 
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::insert( DATA data )
{
  auto wasEmpty = size() == 0 ;  
  auto id = allocateNode();
  _nodes.at(id) = data; 
 
  if(not wasEmpty)
    {
      _children.at(id).insert(_root);
      _parent.at(_root) = id;
    }

  _root = id;
  return iterator(*this, id);
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
typename Tree<DATA>::iterator& Tree<DATA>::iterator::operator++() {
  *this = next();
  return *this;
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::iterator::next()
{
  auto end = _tree.get().end();
  auto result = *this;
  auto c = child();

  if( c != end )
    {
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
              if( maybeResult == end ) // at root 
                {
                  maybeResult = end; 
                  break; 
                }
            }
          else
            {
              maybeResult = maybeResult.sibling();
              break; 
            }
        }

      result = maybeResult; 
    }
      
  return result; 
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::iterator::child()
{
  auto result = _tree.get().end();
  if( not  _tree.get()._children.at(_id).empty() )
    {
      auto iter = _tree.get()._children.at(_id).begin(); 
      result = iterator(_tree,  *iter);
    }
  return result; 
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::iterator::parent()
{
  auto result = _tree.get().end();
  if( _tree.get()._root != _id)
    result = iterator(_tree,_tree.get()._parent.at(_id));
  return result; 
}


template<class DATA>
typename Tree<DATA>::iterator Tree<DATA>::iterator::sibling()
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
        result = iterator(_tree, *iter);
    }
  
  return result; 
}
