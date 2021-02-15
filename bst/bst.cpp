template<typename K, typename V, typename CO>
bst<K,V,CO>::bst(const bst<K,V,CO>& tree): size{0}, root{}
/**
 * Deep-copies the tree in a new instance. The fuction is desgined to copy both the content and the structure
 */
{
  const_iterator position {tree.root.get()};
  iterator my_position;

  if(position)  //check if tree is empty
  {
    insert(*position);
    my_position = iterator(root.get());
  }
  while(tree.size > size) 
  {
    if(position.left() && !my_position.left()) 
    { 
      position = position.left(); 
      (static_cast<node*>(my_position)->left_child).reset(new node(*position,static_cast<node*>(my_position))); 
      my_position = my_position.left();
      ++size;
    }
    else if(position.right() && !my_position.right()) 
    { 
      position = position.right(); 
      (static_cast<node*>(my_position)->right_child).reset(new node(*position,static_cast<node*>(my_position)));
      my_position = my_position.right();
      ++size; 
    }
    else 
    { 
      position = position.parent(); 
      my_position = my_position.parent();
    }
  }
}

template<typename K, typename V, typename CO>
template<typename RT>
std::pair<typename bst<K,V,CO>::iterator,bool> bst<K,V,CO>::_insert(RT&& x) 
{
  iterator it = find(x.first);
  if(it) { return std::pair<iterator, bool>(it, false); } //check if already present
  else 
  {
    iterator child, position {root.get()};
    if(position) 
    {
      //Doulbe parenthesis as requested by the "-Wparenthesis warning"
      while((child = op(x.first,position->first) ? position.left() : position.right())) { position = child; } 
      if(op(x.first,position->first)) 
      { 
        (static_cast<node*>(position)->left_child).reset(new node(std::forward<RT>(x),static_cast<node*>(position)));
        child = position.left();
      } 
      else 
      { 
        (static_cast<node*>(position)->right_child).reset(new node(std::forward<RT>(x),static_cast<node*>(position)));
        child = position.right(); 
      }

      ++size;
      return std::pair<iterator, bool>(child, true);
    }
    else
    {
      root.reset(new node(std::forward<RT>(x),nullptr));
      ++size;
      return std::pair<iterator, bool>(const_iterator(root.get()), true);
    }
  }
}

template<typename K, typename V, typename CO>
typename bst<K,V,CO>::node* bst<K,V,CO>::node_begin() const noexcept
{
  node* position {root.get()};
  if(position) { while(position->left_child) position = position->left_child.get(); }

  return position;
}

template<typename K, typename V, typename CO>
typename bst<K,V,CO>::node* bst<K,V,CO>::node_find(const typename bst<K,V,CO>::key_type& x) const
{
  node* position {root.get()};
  while(position)
  {
    if(!op(position->content->first,x) && !op(x,position->content->first)) { return position; } //Like a binary search
    position = op(x,position->content->first) ? position->left_child.get() : position->right_child.get();
  }
  return nullptr;
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::balance()
{
  if(size < 3) return; //trees with less then 3 nodes are always balanced

  bst nt {}; //creates the balanced tree
  pair_type* contents[size];
  
  int i = 0;
  for(iterator it = begin(); it != end(); ++it) { contents[i++] = &(*it); } //fills a vector with the adresses of the contents of each nodes

  recursive_insert(nt,contents,size,0); //move-inserts the content of this in a balnced way inside nt
  *this = std::move(nt);
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::recursive_insert(bst<K,V,CO> &nt, bst<K,V,CO>::pair_type** contents,int n_elem, int start_pt)
{
  nt.insert(std::move(*contents[start_pt+n_elem/2]));

  int elem_first_half = n_elem/2;
  int elem_second_half = n_elem-1-n_elem/2;

  if(elem_first_half > 0) recursive_insert(nt,contents,elem_first_half,start_pt);
  if(elem_second_half > 0) recursive_insert(nt,contents,elem_second_half,start_pt+elem_first_half+1);

  return;
}

template<typename K, typename V, typename CO>
template<typename RT>
typename bst<K,V,CO>::value_type& bst<K,V,CO>::subscript(RT&& x)
{
  iterator it {find(x)};
  if(!it) 
  {
    V val {};
    it = emplace(std::forward<RT>(x), val).first;
  }
  return it->second;
}

template<typename K, typename V, typename CO>
std::ostream& operator<<(std::ostream& os, const bst<K,V,CO>& x) 
{
  os << "["<<x.size<<"] ";
  for(auto it : x){ os << "(" << it.first << " : " << it.second << ") ";} // To work, key_type and value_type must implent ther own operator<<
  return os;
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::clear_subtree (bst<K,V,CO>::iterator &a, bst<K,V,CO>::childness ch) noexcept
{
  std::unique_ptr<node>* a_ptr;
  switch (ch)
  {
    case childness::right:
      a_ptr = &(static_cast<node*>(a.parent())->right_child);
      break;

    case childness::left:
      a_ptr = &(static_cast<node*>(a.parent())->left_child);
      break;

    case childness::orphan:
      a_ptr = &(this->root);
      break;

    default:
      return; //invalid childness will result in nothing done! (e.g. if i try to clear end() iterator)
  }

  a_ptr->reset(nullptr);
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::erase(const bst<K,V,CO>::key_type& x) 
{
  iterator found = find(x);
  iterator position {found};
  childness ch {position.childness()};

  if (position) //check if there is something to clear in the first place
  {  
    while (position.right())
    {
      ++position;
      ch = position.childness(); //childness must be calculated before swapping the content (it uses it to deduce position), otherwise we may clear the wrong subtree!
      std::swap(static_cast<node*>(position)->content, static_cast<node*>(found)->content);
      found = position;
    }
    while (position.left())
    {
      --position;
      ch = position.childness();
      std::swap(static_cast<node*>(position)->content, static_cast<node*>(found)->content);
      found = position;
    }

    clear_subtree(position, ch);
    --size;
  }
}