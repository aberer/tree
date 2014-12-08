#include "Tree.hpp"

#include <gtest/gtest.h>


TEST(iter, children)
{
  auto t = Tree<int>();
  auto ptr = t.insert(3);
  t.insert(ptr, 4);
  ptr = t.insert(ptr, 5);

  ASSERT_TRUE( *(t.begin().child() ) == 4 );
  ASSERT_TRUE(  ptr.child() == t.end()); 
}

TEST(iter, sibling)
{
  auto t = Tree<int>();
  auto ptr = t.insert(3);
  auto ptr2 = t.insert(ptr, 4);
  t.insert(ptr, 5);
  
  ASSERT_TRUE(t.begin().sibling() == t.end());
  ASSERT_EQ( *(ptr2.sibling()) , 5  ); 
}


TEST(iter, sibling2)
{
  auto t = Tree<int>();
  auto ptr = t.insert(3);
  
  auto ptr2 = t.insert(ptr, 4); 
  t.insert(ptr, 5);
  auto ptr4 = t.insert(ptr, 6);
    
  auto ptr5 = t.insert(ptr2, 7);

  ASSERT_TRUE( ptr.sibling() == t.end()  );
  
  ASSERT_TRUE( ptr2.sibling() != t.end() );
  ASSERT_EQ( *(ptr2.sibling()), 5);
  ASSERT_TRUE( ptr2.sibling().sibling() != t.end() );
  ASSERT_EQ( *(ptr2.sibling().sibling()), 6);
  
  ASSERT_TRUE( ptr4.sibling() == t.end() );
  ASSERT_TRUE( ptr5.sibling() == t.end()); 
}


TEST(iter, parent)
{
  auto t = Tree<int>();
  auto rootPtr = t.insert(3);
  auto ptr1 = t.insert(rootPtr, 4);
  auto ptr2 = t.insert(rootPtr, 5);

  ASSERT_TRUE(  ptr1.parent() == rootPtr );
  ASSERT_TRUE( ptr2.parent() == rootPtr );
  ASSERT_TRUE( rootPtr.parent() == t.end() ); 
}



// 100 - 10 
TEST(iter, node2tree)
{
  auto t = Tree<int>();
  auto next = t.insert(t.begin(),100);

  ASSERT_EQ(*next, 100);
  
  t.insert(next , 10);
  
  auto res = t.begin();
  ASSERT_EQ(*res,100);
  ++res ;
  ASSERT_EQ(*res,10);
}


// (((13,1)23,(14,15)25)10) 
TEST(iter,largerTree)
{
  auto t = Tree<int>();
  
  auto iterRoot = t.insert(t.begin(), 10);

  auto iter = t.insert(iterRoot, 23);
  t.insert(iter,13);
  t.insert(iter,1);
  
  iter = t.insert(iterRoot, 25);
  t.insert(iter, 14);
  t.insert(iter, 15);

  auto seq = vector<int>{10,23,13,1,25,14,15};
  auto seqIter = begin(seq);
  for(auto iter = t.begin(); iter != t.end(); ++iter)
    {
      ASSERT_TRUE(iter != t.end()); 
      ASSERT_EQ(*seqIter, *iter );
      ++seqIter; 
    }
}




TEST(iter, rootInsertion)
{
  auto array = vector<int>{3,4,5}; 
  
  auto t = Tree<int>();
  t.insert(array[0]);
  t.insert(array[1]);
  t.insert(array[2]);

  auto arrayIter = array.rbegin();
  
  for(auto iter = t.begin();iter != t.end();  ++iter)
    {
      ASSERT_EQ(*arrayIter , *iter);
      ++arrayIter; 
    }
}


TEST(iter, emptyTree )
{
  auto t = Tree<int>() ;
  for(auto &v : t )
    {
      v = 0 ; 
      ASSERT_TRUE(false);
    }
}


TEST(iter , mixedInsert)
{
  auto array = vector<int>{3,9,18,20,25};
  auto arrayIter = begin(array); 

  auto t = Tree<int>{};
  auto ptrToFirst = t.insert(t.begin(), *arrayIter); // third
  ++arrayIter;
  t.insert( *arrayIter);  // second 
  ++arrayIter;
  ptrToFirst = t.insert(ptrToFirst, *arrayIter); 
  ++arrayIter;
  t.insert(ptrToFirst, *arrayIter);
  ++arrayIter;
  t.insert( *arrayIter ); // first 
  ++arrayIter;
  
  assert(arrayIter == end(array));

  auto result = vector<int>{25, 9 ,3, 18,20};
  auto resultIter = begin(result); 
  for(auto &v : t)
    {
      ASSERT_EQ(v , *resultIter);
      ++resultIter; 
    }
}





// FIXTURE


class TreeBaseTest : public ::testing::Test
{
};

// Derives a fixture FooTest from BaseTest.
class TreeTest1 : public TreeBaseTest
{
public:
  TreeTest1()
    :_tree{}
  { }
  
protected:
  virtual void SetUp()
  {
    _tree.insert(3);
    _tree.insert(_tree.begin(), 4);
    _tree.insert(_tree.begin(),5); 
    _tree.insert(_tree.begin().child(), 6);
    _tree.insert(_tree.begin().child(), 7);
    _tree.insert(_tree.begin().child().sibling(), 8);
    _tree.insert(_tree.begin().child().sibling(), 9); 
  }
  
  virtual void TearDown()
  {
    
  }

  void assertContent(vector<int> array)
  {
    auto content = set<int>(_tree.begin(), _tree.end()); 
    ASSERT_EQ(array.size(), content.size());
    for(auto &v : array)
      ASSERT_TRUE(content.find(v) != content.end()); 
  }

  
  void printDfsTraversal()  
  {
    for(auto &v : _tree)
      std::cout << v << "," ;
    std::cout << std::endl; 
  }

  
protected:
  Tree<int> _tree; 
};


// END 


TEST_F(TreeTest1, stateCheck)
{
  // is everything as I imagine it to be?
    assertContent({ 3,4,6,7,5,8,9});
}


TEST_F(TreeTest1, eraseTest)
{
  _tree.erase( _tree.begin().child().child() ); // erasing 6
  assertContent( { 3,4,7,5,8,9  } );
  _tree.erase( _tree.begin() );
  assertContent( { 4, 5, 8, 9, 7 } );
  _tree.erase( _tree.begin().child() );
  assertContent( { 4, 8, 9, 7   } );
  _tree.erase( _tree.begin()  );
  assertContent( {8, 7 , 9 } );
  _tree.erase(  _tree.begin());
  assertContent( {7,9});
  
  _tree.erase( _tree.begin());
  assertContent( {9});

  ASSERT_EQ(_tree.size() ,1); 
  _tree.erase( _tree.begin());
  ASSERT_EQ(_tree.size(),  0); 
}


TEST_F(TreeTest1, eraseAndInsert)
{
  // leaf 
  _tree.erase(_tree.begin().child().child().sibling());
  _tree.insert( _tree.begin().child().child(), 18);
  assertContent( {  3 ,4 , 6, 5, 8, 9, 18 }  );

  _tree.erase(_tree.begin().child());
  _tree.erase(_tree.begin().child());
  _tree.erase(_tree.begin().child());
  _tree.insert(_tree.begin(), 25 );
  assertContent( { 3 ,25 ,5, 8, 9} ) ;
  ASSERT_EQ(  _tree.capacity(), 7 );

  // complete erase 
  while(_tree.size() > 0 )
    _tree.erase(_tree.begin());
  
  _tree.insert(1);
  assertContent({1});

  _tree.erase(_tree.begin());

  _tree.insert(_tree.begin(),1);
  assertContent( {1});
}




int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  auto res = RUN_ALL_TESTS();
  if(res)
    {
      std::cout << "something went wrong ;-) (just silencing the warning) "<< std::endl; 
    }
  return 0;
}



