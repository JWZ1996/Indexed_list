# Indexed_list
Implementation of bidirectional list with some unusual features

1. Indexed access

  Usual list has a O(n) access time. My remembers the last accessed element, 
  and checks in which direction (From start, from end, from beginning)
  access will be optimal. Sequential access (each element from start/end),
  will result in O(1) access time.

2. Iterator initialization

  For easy expansibility, I provided the all types of fundamental iterators.
  Almost every constructor use the same template function initializing
  the whole container.

  Iterator classes can be used for every numerical indexed containers.
  
3. Modern C++ semantics

  Container has the move semantics implemented. Also it is possible to 
  move other STL containers to it.  Thanks to method overloading, new 
  list elements can be automatically copied or moved into the my container.

  List can be also initialized with std::initializer_list<T>.
  
  
