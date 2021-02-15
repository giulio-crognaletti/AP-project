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
    if (*this && right()) //Do I have a right child? YES...
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
bst<K,V,CO>::iterator_class<c>& bst<K,V,CO>::iterator_class<c>::prev() //noexcept(bst::op(const K&,const K&))
/*
* This method finds the previous node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
* 
* The algortim is identical, but this time we look for key pairs satisfying the condition  op(my_key, child_key) == true.
* This of course implies also swapping right and left child (e.g. The question should be "Has my node a LEFT child?" this time)
*/
{
    if (*this && left())
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

template<typename K, typename V, typename CO>
template<bool c>
typename bst<K,V,CO>::childness bst<K,V,CO>::iterator_class<c>::childness() const noexcept
{ 
    if(*this && !parent()) return childness::orphan;
    else if (*this) return op((*this)->first,parent()->first) ? childness::left : childness::right;
    return childness::eot; //end-of-tree: is the childness of end()
}