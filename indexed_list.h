#ifndef INDEXED_LIST_H_INCLUDED
#define INDEXED_LIST_H_INCLUDED
#include <iostream>
#include <vector>
#include <algorithm>
#include "iterator.h"



template <typename T>
class Indexed_list {

private:

    struct Item_wrapper{

        Item_wrapper* next = nullptr;
        Item_wrapper* previous = nullptr;

        size_t index;
        T item;
        
        Item_wrapper(size_t _index, T&& _item);
        Item_wrapper(size_t _index, const T& _item);
        ~Item_wrapper();

        void connect_next(Item_wrapper* new_wrapper);
        void connect_previous(Item_wrapper* new_wrapper);

        // TODO custom "new", "delete" operators from custom memory manager;
    };

    void m_edit_el(const T& added_el, size_t position);
    void m_update_indexes();
    void m_update_indexes(size_t start, size_t end);

    // Used in constructor
    template<typename Iterator> void m_create_list_from_iterator(Iterator&& start_it, Iterator&& end_it);

    void m_add_first(const T&  added_el);
    void m_add_first(      T&& added_el);
    Item_wrapper* m_get_wrapper_at_index(size_t index);

private:

    size_t m_size = 0;
    Item_wrapper* m_start = nullptr;
    Item_wrapper* m_end = nullptr;
    Item_wrapper* m_last_accessed = nullptr;

public:

    Indexed_list();

    Indexed_list( size_t size );
    Indexed_list( const T arr[] );
    Indexed_list( const std::initializer_list<T>& _arr);

    // std::vector initiation
    Indexed_list( const std::vector<T>&  _vec );
    Indexed_list( std::vector<T>&& _vec );

    // Copy
    Indexed_list( const Indexed_list<T>& _other );

    // Move
    Indexed_list(  Indexed_list<T>&& _other );

    ~Indexed_list();

    void push_back(T&& _element);
    void push_back(const T& _element);

    void push_front(T&& _element);
    void push_front(const T& _element);

    void pop_back();
    void pop_front();
    void clear();
    void delete_item(size_t position);

    T& at( size_t index );
    T& last_element ()const;
    T& first_element()const;
    T& last_accessed()const;

    T& operator[](size_t rhs);
    bool operator==(Indexed_list<T> rhs) const;
    bool operator!=(Indexed_list<T> rhs) const;
    size_t size() const;

    bool is_empty()const;


    using iterator = iterator<Indexed_list<T>,T>;
        iterator begin();                   iterator end();

    using const_iterator = const_iterator <Indexed_list<T>,T>;
        const_iterator cbegin();            const_iterator cend();
    
    using reverse_iterator = reverse_iterator <Indexed_list<T>,T>;
        reverse_iterator rbegin();          reverse_iterator rend();

    using const_reverse_iterator = const_reverse_iterator <Indexed_list<T>,T>;
        const_reverse_iterator crbegin();   const_reverse_iterator crend();

};


// Item Wrapper members
template<typename T>
Indexed_list<T>::Item_wrapper::Item_wrapper(size_t _index,T&& _item):
    index{_index},item{std::move(_item)}{
}
template<typename T>
Indexed_list<T>::Item_wrapper::Item_wrapper(size_t _index, const T& _item):
    index{_index},item{_item}{
}
template<typename T>
Indexed_list<T>::Item_wrapper::~Item_wrapper(){
    // Uncomment for Item_wrapper ~destructor invocation
    // std::cout << "USUNIENTO element: "<< index << "o wartosci: " << item << std::endl;
}
template<typename T>
void Indexed_list<T>::Item_wrapper::connect_next(Item_wrapper* new_wrapper){
    this->next = new_wrapper;
    new_wrapper->previous = this;
}
template<typename T>
void Indexed_list<T>::Item_wrapper::connect_previous(Item_wrapper* new_wrapper){
    this->previuos = new_wrapper;
    new_wrapper->next = this;
}

// Iterator methods
template<typename T>
iterator<Indexed_list<T>,T> Indexed_list<T>::begin(){
    return iterator(this,0);
};

template<typename T>
iterator<Indexed_list<T>,T> Indexed_list<T>::end(){

    // Pointing to out-of-bounds el. always will set iterator to nullptr
    return iterator(this,m_size);

};

template<typename T>
reverse_iterator<Indexed_list<T>,T> Indexed_list<T>::rbegin(){
    return reverse_iterator(this,m_size - 1);
};

template<typename T>
reverse_iterator<Indexed_list<T>,T> Indexed_list<T>::rend(){

    // -1 as a parameter - syntactic sugar, pointing to element before start
    return reverse_iterator(this,-1);

};

template<typename T>
const_iterator<Indexed_list<T>,T> Indexed_list<T>::cbegin(){
    return const_iterator(this,0);
};

template<typename T>
const_iterator<Indexed_list<T>,T> Indexed_list<T>::cend(){
    return const_iterator(this,m_size);

};

template<typename T>
const_reverse_iterator<Indexed_list<T>,T> Indexed_list<T>::crbegin(){
    return const_reverse_iterator(this,m_size - 1);
};

template<typename T>
const_reverse_iterator<Indexed_list<T>,T> Indexed_list<T>::crend(){

    // -1 as a parameter - syntactic sugar, pointing to element before start
    // Pointing to out-of-bounds el. always will set iterator to nullptr (::end())
    return const_reverse_iterator(this,-1);

};


// Access member functions
template<typename T> T& Indexed_list<T>::   last_element() const {return                    m_end ->item; };
template<typename T> T& Indexed_list<T>::  first_element() const {return                   m_start->item; };
template<typename T> T& Indexed_list<T>::  last_accessed() const {return           m_last_accessed->item; };
template<typename T> T& Indexed_list<T>:: at(size_t index)       {return m_get_wrapper_at_index(index)->item; };

template<typename T>
T& Indexed_list<T>::operator[](size_t rhs)
{
    return this->at(rhs);
};


template<typename T>
typename Indexed_list<T>::Item_wrapper* Indexed_list<T>::m_get_wrapper_at_index(size_t index){

    // Boundary items
    if (index == 0U){
        m_last_accessed = m_start;
        return m_start;
    }
    if (index == (m_size - 1U)){
        m_last_accessed = m_end;
        return m_end;
    }

    int from_start {static_cast<int>(index)};
    int from_end   {static_cast<int>(m_size - index)};
    int from_last  {static_cast<int>(m_last_accessed->index - index)};

    // ==========
    // Access IFs
    // ==========
    
    // Access from last_accessed - to start direction
    if(abs(from_last) <= from_end && abs(from_last) <= from_start && from_last > 0){
        // <= operator for optimization - avoiding m_last_accesed asigning change in more cases
        while (m_last_accessed->index != index){
            m_last_accessed = m_last_accessed->previous;
        }
        return m_last_accessed;
    };
    
    // Access from last_accessed - to end direction
    if(abs(from_last) <= from_end && abs(from_last) <= from_start && from_last < 0){
        while (m_last_accessed->index != index){
            m_last_accessed = m_last_accessed->next;
        };
        return m_last_accessed;
    };
    
    // Access from end
    if(from_end <= from_start){
        m_last_accessed = m_end;

        while (m_last_accessed->index != index){
            m_last_accessed = m_last_accessed->previous;
        };
        return m_last_accessed;
    };

    // Access from start
    if(from_start < from_end){
        m_last_accessed = m_start;

        while (m_last_accessed->index != index){
            m_last_accessed = m_last_accessed->next;
        };
        return m_last_accessed;
    }
}
template<typename T>
void Indexed_list<T>::push_back(const T& _element){
    if (!this->is_empty()){
        auto new_item = new Item_wrapper(m_size,_element);
        m_end->connect_next(new_item);
        m_end=new_item;
        ++m_size;
    }else{
        m_add_first(_element);
    };
};
template<typename T>
void Indexed_list<T>::push_back(T&& _element){
    if (!this->is_empty()){
        auto new_item = new Item_wrapper(m_size,std::move(_element));
        m_end->connect_next(new_item);
        m_end=new_item;
        ++m_size;
    }else{
        m_add_first(std::move(_element));
    };
};
template<typename T>
void Indexed_list<T>::push_front(const T& _element){

    if (!this->is_empty()){
        auto new_item = new Item_wrapper(0,_element);
        m_start->connect_previous(new_item);
        m_end=new_item;
        ++m_size;

         m_update_indexes();
    }else{
        m_add_first(_element);
    };

};
template<typename T>
void Indexed_list<T>::push_front(T&& _element)
{
    if (!this->is_empty()){
        auto new_item = new Item_wrapper(0,m_size,_element);
        m_start->connect_previous(new_item);
        m_end=new_item;
        ++m_size;
         m_update_indexes();
    }else{
        m_add_first(std::move(_element));
    };

};

// El. delete member functions
template<typename T>
void Indexed_list<T>::pop_back()
{
    m_end = m_end->previous;
    delete m_end->next;
    --m_size;
};
template<typename T>
void Indexed_list<T>::pop_front()
{
    m_start = m_start->next;
    delete m_start->previous;
    m_update_indexes();
    --m_size;
}

template<typename T>
void Indexed_list<T>::clear()
{
    while (m_start != m_end){
        pop_back();
    }
    delete m_start;
};
template<typename T>
void Indexed_list<T>::delete_item(size_t index)
{
    if(index == 0){
        this->pop_front();
    }else if(index == m_size - 1U){
        this->pop_back();
    }else{
        auto temp = m_get_wrapper_at_index(index);
        auto temp_next = temp->next;
        auto temp_previous = temp->previous;

        temp_previous ->connect_next(temp_next);

        delete temp;
        
        --m_size;
        m_update_indexes(index-1,m_size-1);
    };
};

// Utility
template<typename T>
void Indexed_list<T>::m_add_first( const T& _element )
{
    auto el = new Item_wrapper(0, _element);
    m_start = el;
    m_end = el;
    m_last_accessed = el;
    ++m_size;
};
template<typename T>
void Indexed_list<T>::m_add_first( T&& _element )
{
    auto el = new Item_wrapper(0, std::move(_element));
    m_start = el;
    m_end = el;
    m_last_accessed = el;
    ++m_size;
};
template<typename T>
void Indexed_list<T>::m_update_indexes()
{
    // Whole list update
    // Cannot use method overloading
    m_update_indexes(0, m_size - 1);
}
template<typename T>
void Indexed_list<T>::m_update_indexes( size_t start, size_t end )
{
    size_t index = start;
    Item_wrapper* wrapper_start = m_get_wrapper_at_index(start);
    Item_wrapper* wrapper_end = m_get_wrapper_at_index(end);

    while(wrapper_start != wrapper_end){
        wrapper_start->index = index;
        ++index;
        wrapper_start = wrapper_start->next;
    }
}
template<typename T>
size_t Indexed_list<T>::size()const
{
    return m_size;
};
template<typename T>
bool Indexed_list<T>::is_empty()const
{
    return m_size == 0;
};


template<typename T>
template<typename Iterator>
void Indexed_list<T>::m_create_list_from_iterator(Iterator&&  start_it, Iterator&& end_it)
{
    while(start_it != end_it)
    {
        push_back(*start_it);
        ++start_it;
    };
}

// Constructors and destructor
template<typename T>
Indexed_list<T>::Indexed_list() = default;

template<typename T>
Indexed_list<T>::Indexed_list( size_t _size )
{
    for ( size_t i {0}; i < _size; ++i ){
        this->push_back(T());
    };
};
template<typename T>
Indexed_list<T>::Indexed_list( const T _arr[] )
{
    m_create_list_from_iterator<const T*>(_arr,_arr + (sizeof(_arr)/sizeof(*_arr)) );
};
template<typename T>
Indexed_list<T>::Indexed_list( std::vector<T>&& _vec )
{
    m_create_list_from_iterator<std::move_iterator<typename std::vector<T>::iterator>>(
                std::make_move_iterator(  _vec.begin() ),
                std::make_move_iterator(  _vec.  end() ));
    _vec.clear();
};
template<typename T>
Indexed_list<T>::Indexed_list( const std::initializer_list<T>& _arr)
{
    m_create_list_from_iterator(_arr.begin(),_arr.end());
};
template<typename T>
Indexed_list<T>::Indexed_list( const std::vector<T>&  _vec )
{
    m_create_list_from_iterator<typename std::vector<T>::const_iterator>(_vec.cbegin(),_vec.cend());
};

// Move constructor
template<typename T>
Indexed_list<T>::Indexed_list( Indexed_list<T>&& _other )
    : m_start{_other.m_start}, m_end{_other.m_end}, m_last_accessed{_other.m_last_accessed},m_size{_other.m_size}
{
    _other.m_start = nullptr;
    _other.m_end = nullptr;
    _other.m_last_accessed = nullptr;
    _other.m_size = 0;
};

// Deep copy constructor
template<typename T>
Indexed_list<T>::Indexed_list(const Indexed_list<T>&  _other )
    : m_start{_other.m_start}, m_end{_other.m_end}, m_last_accessed{_other.m_last_accessed},m_size{_other.m_size}
{
    m_create_list_from_iterator<typename Indexed_list<T>::const_iterator>(_other.cbegin(),_other.cend());
};

template<typename T>
Indexed_list<T>::~Indexed_list()
{
    clear();
};
#endif // INDEXED_LIST_H_INCLUDED




