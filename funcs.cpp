template<typename K, typename V, typename CO>
template<typename RT>
std::pair<typename bst<K,V,CO>::iterator,bool> bst<K,V,CO>::_insert(RT&& x)
{
  iterator it = find(x.first);
  if(it) { return std::pair<iterator, bool>(it, false); }
  else 
  {
    iterator child, position {root.get()};
    if(position) 
    {
      while((child = op(x.first,position->first) ? position.left() : position.right())) { position = child; } //Doulbe parenthesis as requested by the "-Wparenthesis warning"

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
typename bst<K,V,CO>::node* bst<K,V,CO>::node_begin() const 
{
  const_iterator position {root.get()};
  if(position) { while(position.left()) position = position.left(); }

  return static_cast<node*>(position);
}

template<typename K, typename V, typename CO>
typename bst<K,V,CO>::node* bst<K,V,CO>::node_find(const typename bst<K,V,CO>::key_type& x) const //DI NUOVO STO TYPRNAME?
{
  const_iterator position {root.get()};
  while(position)
  {
    if(position->first == x) { return static_cast<node*>(position); }
    position = op(x,position->first) ? position.left() : position.right();
  }
  return nullptr;
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::balance()
{
  if(!size) return;

  bst nt {};
  pair_type* contents[size];
  
  int i = 0;
  for(iterator it = begin(); it != end(); ++it) { contents[i++] = &(*it);}

  recursive_insert(nt,contents,size,0);
  *this = std::move(nt);
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::recursive_insert(bst<K,V,CO> &nt, bst<K,V,CO>::pair_type** contents,int n_elem, int start_pt)
{
  nt.insert(std::move(*contents[start_pt+n_elem/2])); //THIS LEAVES *THIS IN AN UNSPECIFIED STATE, SO IT MUST BE MOVED AGAIN

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
  for(auto it : x){ os << "(" << it.first << " : "<< it.second << ") ";}
  return os;
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::erase(const bst<K,V,CO>::key_type& x) 
{
  iterator found = find(x);
  iterator position {found};
  childness ch {position.childness()};

  if (position) 
  {  
    while (position.right())
    {
      ++position;
      ch = position.childness();
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

template<typename K, typename V, typename CO>
void bst<K,V,CO>::clear_subtree (bst<K,V,CO>::iterator &a, bst<K,V,CO>::childness ch) 
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

    default: //THROW EXCEPTION MAYBE?
      break;
  }

  a_ptr->reset(nullptr);
};

template<typename K, typename V, typename CO>
bst<K,V,CO> bst<K,V,CO>::deepcopy() const
/*
* genera un albero vuoto
* condizione: copiato non ha null && find=end() (per evitare di copiare i figli due volte)
* se condizione -> copia nodo e scendi;
* altrimenti risali;
* 
*/
{
  bst nt {};
  const_iterator position { root.get() };

  if(position) nt.insert(*position);
  while(size > nt.get_size())
  {
    if(position.left() && !nt.find(position.left()->first)) 
    { 
      position = position.left(); 
      nt.insert(*position); 
    }
    else if(position.right() && !nt.find(position.right()->first)) 
    { 
      position = position.right(); 
      nt.insert(*position); 
    }
    else { position = position.parent(); }
  }
  return nt;
}
