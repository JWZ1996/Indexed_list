
#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include<stddef.h>
#include<iostream>

// ========== ======================================================= ==========
// ==========                                                         ==========
// ==========                   Abstract classes                      ==========
// ==========                                                         ==========
// ========== ======================================================= ==========

// Abstract_iterator -> base class of all iterators
template <typename container_t, typename T>
class Abstract_iterator{

protected:
    container_t* m_target_container = nullptr;
    size_t m_pointed_index = 0;

    inline T* get_item();

public:

    Abstract_iterator( container_t* target_container, size_t pointed_index = 0);
    virtual ~Abstract_iterator();

    inline bool  operator== (const Abstract_iterator& rhs);
    inline bool  operator!= (const Abstract_iterator& rhs);
};

// Constructor
template <typename container_t,typename T>
Abstract_iterator<container_t,T>::Abstract_iterator(container_t* target_container, size_t pointed_index):
    m_target_container{target_container}, m_pointed_index{pointed_index}{

};
// Destructor
template <typename container_t,typename T>
Abstract_iterator<container_t,T>::~Abstract_iterator(){

};

template <typename container_t,typename T>
T* Abstract_iterator<container_t,T>::get_item(){

    if (m_pointed_index < m_target_container -> size()) {
        return &(m_target_container -> at(m_pointed_index));
    };
    return static_cast<T*>(nullptr);
};

template <typename container_t,typename T>
bool Abstract_iterator<container_t,T>:: operator== (const Abstract_iterator& rhs){
    return (this->m_pointed_index == rhs.m_pointed_index && this->m_target_container == rhs.m_target_container)? true : false;
};
template <typename container_t,typename T>
bool Abstract_iterator<container_t,T>:: operator!= (const Abstract_iterator& rhs){
    return (this->m_pointed_index != rhs.m_pointed_index)? true : false;
};



// Direction_iterator -> derivative class providing increment and decrement operators.
// Reverse (direction = -1 ) and forward (direction = -1 ), depends on template parameter
template <typename container_t, typename T, int direction>
struct Direction_iterator:public Abstract_iterator<container_t,T>{

    Direction_iterator( container_t* target_container, size_t pointed_index = 0);
    virtual ~Direction_iterator();

    inline void operator+ (long long int rhs);
    inline void operator++ ();

    inline void operator- (long long int rhs);
    inline void operator-- ();

};

// Constructor
template <typename container_t, typename T, int direction>
Direction_iterator<container_t,T,direction>::Direction_iterator( container_t* target_container, size_t pointed_index)
    :Abstract_iterator<container_t,T>(target_container,pointed_index){};

// Destructor
template <typename container_t, typename T, int direction>
Direction_iterator<container_t,T,direction>::~Direction_iterator(){};


template <typename container_t, typename T, int direction>
void Direction_iterator<container_t,T,direction>:: operator+ (long long int rhs){
    this->m_pointed_index += rhs*direction;
};
template <typename container_t, typename T, int direction>
void Direction_iterator<container_t,T,direction>:: operator++ (){
    this->m_pointed_index += direction;
};

template <typename container_t, typename T, int direction>
void Direction_iterator<container_t,T,direction>:: operator- (long long int rhs){
    this->m_pointed_index -= rhs*direction;
};
template <typename container_t, typename T, int direction>
void Direction_iterator<container_t,T,direction>:: operator-- (){
    this->m_pointed_index -= direction;
};



//   ------------------------------------------------------------------------
//                                 iterator
//   ------------------------------------------------------------------------
template <typename container_t,typename T>
struct iterator: public Direction_iterator<container_t,T, 1>{

    iterator( container_t* target_container, size_t pointed_index = 0 );
    ~iterator();
    T& operator*();
};

// Constructor -> iterator
template <typename container_t,typename T>
iterator<container_t,T>::iterator(container_t* target_container, size_t pointed_index)
    :Direction_iterator<container_t,T,1>(target_container, pointed_index){};

// Destructor
template <typename container_t,typename T>
iterator<container_t,T>::~iterator(){};

template <typename container_t,typename T>
T& iterator<container_t,T>:: operator*() {
  return *(this->get_item());
}


//   -----------------------------------------------------------------------
//                           reverse_iterator
//   -----------------------------------------------------------------------
template <typename container_t,typename T>
struct reverse_iterator: public Direction_iterator<container_t,T, -1>{

    reverse_iterator( container_t* target_container, size_t pointed_index = 0 );
    ~reverse_iterator();
    T& operator*();
};

// Constructor -> iterator
template <typename container_t,typename T>
reverse_iterator<container_t,T>::reverse_iterator(container_t* target_container, size_t pointed_index)
    :Direction_iterator<container_t,T,-1>(target_container, pointed_index){};

// Destructor
template <typename container_t,typename T>
reverse_iterator<container_t,T>::~reverse_iterator(){};

template <typename container_t,typename T>
T& reverse_iterator<container_t,T>:: operator*() {

  return *(this->get_item());
};
//  --------------------------------------------------------------------------
//                            const_iterator
//  --------------------------------------------------------------------------

template <typename container_t,typename T>
struct const_iterator:public Direction_iterator<container_t,T,1>{

    const_iterator( container_t* target_container, size_t pointed_index = 0 );
    ~const_iterator();
    const T& operator*();
};

// Constructor
template <typename container_t,typename T>
const_iterator<container_t,T>::const_iterator(container_t* target_container, size_t pointed_index)
    :Direction_iterator<container_t,T,1>(target_container, pointed_index){};

// Destructor
template <typename container_t,typename T>
const_iterator<container_t,T>::~const_iterator(){};

template <typename container_t,typename T>
const T& const_iterator<container_t,T>:: operator*() {
  return const_cast<T&>(*(this->get_item()));
}

//  --------------------------------------------------------------------------
//                            const_reverse_iterator
//  --------------------------------------------------------------------------

template <typename container_t,typename T>
struct const_reverse_iterator: public Direction_iterator<container_t,T,-1>{

    const_reverse_iterator( container_t* target_container, size_t pointed_index = 0 );
    ~const_reverse_iterator();

    const T& operator*();
};

// Constructor
template <typename container_t,typename T>
const_reverse_iterator<container_t,T>::const_reverse_iterator(container_t* target_container, size_t pointed_index)
    :Direction_iterator<container_t,T,-1>(target_container, pointed_index){};

// Destructor
template <typename container_t,typename T>
const_reverse_iterator<container_t,T>::~const_reverse_iterator(){};

template <typename container_t,typename T>
const T& const_reverse_iterator<container_t,T>:: operator*() {
  return const_cast<T&>(*(this->get_item()));
}




#endif // ITERATOR_H_INCLUDED

