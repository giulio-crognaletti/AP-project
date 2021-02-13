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

    node* node_ptr;

    iterator_class& next();
    iterator_class& prev();

public:

    //ctors -> EXCEPTIONS
    iterator_class() = default;
    iterator_class(const iterator_class &) = default;
    iterator_class(iterator_class &&) = default;

    explicit iterator_class(node *np): node_ptr{np} {};

    //dtor
    ~iterator_class() = default;

    //copy and move assignements
    iterator_class &operator = (const iterator_class &) = default;
    iterator_class &operator = (iterator_class &&) = default;

    bool operator == (const iterator_class i) const { return node_ptr == i.node_ptr; }
    bool operator != (const iterator_class i) const { return node_ptr != i.node_ptr; }
    reference operator * () const { return *(node_ptr->content); }  //error if nullptr / undefined behavior
    pointer operator -> () const { return node_ptr->content.get(); } //error if nullptr / undefined behavior

    operator bool() const { return (bool)node_ptr; }
    operator node*() const {return node_ptr; }

    iterator_class &operator ++() { return this->next(); }
    iterator_class operator ++(int)
    {
      iterator_class ret = *this;
      this->next();
      return ret;
    }

    iterator_class &operator --() { return this->prev(); }
    iterator_class operator --(int)
    {
      iterator_class ret = *this;
      this->prev();
      return ret;
    }

    iterator_class parent() const { return iterator_class(node_ptr->parent); }
    iterator_class left() const { return iterator_class((node_ptr->left_child).get()); }
    iterator_class right() const { return iterator_class((node_ptr->right_child).get()); }
    
    bst::childness childness() const 
    { 
      if(!parent()) return childness::orphan;
      return op((*this)->first,parent()->first) ? childness::left : childness::right; 
    }
};

template<typename K, typename V, typename CO>
template<bool c>
bst<K,V,CO>::iterator_class<c>& bst<K,V,CO>::iterator_class<c>::next()
/*
* This method finds the next node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
* 
* The algorithm is simple and it follows this scheme:
* Has my node a right child ? (i.e. is there some child for which op(my_key, child_key) = false ?)
* 
*  -> YES: Then the next must be the leftmost child of my right child.
*  -> NO: Then the next must be the first parent of the node that satisfies op(my_key, child_key) == false
* 
* The final node is the one for wich no node satisfies the request, and it must return nullptr as a result. 
*/ 
{
    if (right()) //Do I have a right child? YES...
    {
        *this = right();
        while(left()) { *this = left(); }
    }
    else //NO ...
    {
        while (parent() && !bst::op((*this)->first,parent()->first)) { *this = parent(); }
        if (!parent()) { *this = iterator_class(nullptr); } //I have reached the end of the tree
        else { *this = parent(); }
    }

    return *this;
}

template<typename K, typename V, typename CO>
template<bool c>
bst<K,V,CO>::iterator_class<c>& bst<K,V,CO>::iterator_class<c>::prev()
/*
* This method finds the previous node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
* 
* The algortim is identical, but this time we look for key pairs satisfying the condition  op(my_key, child_key) == true.
* This of course implies also swapping right and left child (e.g. The question should be "Has my node a LEFT child?" this time)
*/
{
    if (left())
    {
        *this = left();
        while (right()) { *this = right(); }
    }
    else 
    {
        while (parent() && bst::op((*this)->first,parent()->first)) { *this = parent(); }
        if (!parent()) { *this = iterator_class(nullptr); }
        else { *this = parent(); } 
    }

    return *this;
}