#ifndef __ITERATOR__
#define __ITERAOTR__

template<typename K, typename V, typename CO>
template<bool c>
class bst<K,V,CO>::iterator_class
/*
* Implements a general bst iterator class. Bool value refers to const-ness: c=true -> const_iterator, c=false -> iterator
*/
{   
    using T = pair_type;
    using pointer = typename std::conditional<c ,const T*, T*>::type;
    using reference  = typename std::conditional<c ,const T&, T&>::type;
    using iterator_category = std::bidirectional_iterator_tag;

    node* node_ptr;

    iterator_class& next(); 
    iterator_class& prev();

public:


    iterator_class() noexcept = default;
    iterator_class(const iterator_class &) noexcept = default;
    iterator_class(iterator_class &&) noexcept = default;

    explicit iterator_class(node *np) noexcept: node_ptr{np} {};

    ~iterator_class() noexcept = default;

    //copy and move assignements
    iterator_class &operator = (const iterator_class &) noexcept = default;
    iterator_class &operator = (iterator_class &&) noexcept = default;

    bool operator == (const iterator_class i) const noexcept { return node_ptr == i.node_ptr; }
    bool operator != (const iterator_class i) const noexcept { return node_ptr != i.node_ptr; }

    reference operator * () const noexcept { return *(node_ptr->content); }   // This fragment can have undefined behaviour (like pointers) if you try to 
    pointer operator -> () const noexcept { return node_ptr->content.get(); } // derefernce the iterator corresponding to tree.end() (i.e. node_ptr = nullptr)

    // this function is more expensive but ensures that you only derefernce non-end iterators (may be useful for end user)
    reference at() const 
    {
        if(!*this) { throw bst::end_of_tree_exception{}; } 
        return **this;
    }

    //static casts are defined here
    operator bool() const noexcept { return (bool)node_ptr; }
    operator node*() const noexcept {return node_ptr; }

    iterator_class &operator ++() noexcept { return this->next(); }
    iterator_class operator ++(int) noexcept
    {
        iterator_class ret = *this;
        this->next();
        return ret;
    }

    iterator_class &operator --() noexcept { return this->prev(); }
    iterator_class operator --(int) noexcept
    {
        iterator_class ret = *this;
        this->prev();
        return ret;
    }

    //here we can also have potential undefined behavior if not used properly
    //they are public methods to allow freedom of exploring the bst, but they must be used consistenly
    iterator_class parent() const noexcept { return iterator_class(node_ptr->parent); }
    iterator_class left() const noexcept { return iterator_class((node_ptr->left_child).get()); }
    iterator_class right() const noexcept { return iterator_class((node_ptr->right_child).get()); }
    
    bst::childness childness() const noexcept; //returns information on the relationship between the pointed node and its parent
};

#include"iterator.cpp"

#endif