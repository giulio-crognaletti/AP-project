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

private:

    node* node_ptr;
    comparison_operator op;

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
    reference operator * () const { return node_ptr->content; } 
    pointer operator -> () const { return &(node_ptr->content); } 

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

    iterator_class left() { return iterator(node_ptr->left_child); }
    iterator_class right() { return iterator(node_ptr->right_child); }
    
    operator bool() const { return (bool)node_ptr; }
    
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
    if (node_ptr->right_child) //Do I have a right child? YES...
    {
        node_ptr = (node_ptr->right_child).get();
        while (node_ptr->left_child) { node_ptr = (node_ptr->left_child).get(); }
    }
    else //NO ...
    {
        while (node_ptr->parent && !op((node_ptr->content).first,(node_ptr->parent->content).first)) { node_ptr = node_ptr->parent; }
        if (!node_ptr->parent) { node_ptr = nullptr; } 
        else { node_ptr = node_ptr->parent; }
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
    if (node_ptr->left_child)
    {
        node_ptr = (node_ptr->left_child).get();
        while (node_ptr->right_child) { node_ptr = (node_ptr->right_child).get(); }
    }
    else 
    {
        while (node_ptr->parent && op((node_ptr->content).first,(node_ptr->parent->content).first)) { node_ptr = node_ptr->parent; }
        if (!node_ptr->parent) { node_ptr = nullptr; }
        else { node_ptr = node_ptr->parent; } 
    }

    return *this;
}

