template<typename K, typename V, typename CO>
bst<K,V,CO> bst<K,V,CO>::deepcopy()
/*
* genera un albero vuoto
* condizione: copiato non ha null && find=end() (per evitare di copiare i figli due volte)
* se condizione -> copia nodo e scendi;
* altrimenti risali;
* 
*/
{
    bst<K,V,CO> *nt = new bst<K,V,CO> {};
    node *position = root.get();

    if(position) nt->insert(position->content);
    while(size > nt->get_size())
    {
        if(position->left_child && (nt->find((position->left_child->content).first) == nt->end())) { position = position->left_child.get(); nt->insert(position->content); }
        else if(position->right_child && (nt->find((position->right_child->content).first) == nt->end())) { position = position->right_child.get(); nt->insert(position->content); }
        else { position = position->parent; }
    }
    return std::move(*nt);
}

template<typename K, typename V, typename CO>
std::pair<typename bst<K,V,CO>::iterator,bool> bst<K,V,CO>::insert(const bst<K,V,CO>::pair_type& x) // COS'E STO TYPE NAME??
{
  iterator it = find(x.first);
  if(it != end()) { return std::pair<iterator, bool>(it, false); }
  else 
  {
    node *child, *position = root.get();
    if(position) 
    {
      while(child = op(x.first,(position->content).first) ? (position->left_child).get() : (position->right_child).get()) {position = child;}

      if(op(x.first,(position->content).first)) { (position->left_child).reset(new node(x,position)); child = (position->left_child).get(); } 
      else { (position->right_child).reset(new node(x,position)); child = (position->right_child).get(); }

      ++size;
      return std::pair<iterator, bool>(iterator(child), true);
    }
    else
    {
      root.reset(new node(x,nullptr));
      ++size;
      return std::pair<iterator, bool>(iterator(root.get()), true);
    }
  }
}

template<typename K, typename V, typename CO>
typename bst<K,V,CO>::node* bst<K,V,CO>::node_find(const typename bst<K,V,CO>::key_type& x) const //DI NUOVO STO TYPRNAME?
{
  node *position = root.get();
  while(position)
  {
    if((position->content).first == x) { return position; }
    position = op(x,(position->content).first) ? (position->left_child).get() : (position->right_child).get();
  }
  return nullptr;
}

/*template<typename K, typename V, typename CO>
void bst<K,V,CO>::erase(const typename bst<K,V,CO>::key_type& x) 
{
  node *found = node_find(x);
  node *position {found};
  if (position) 
  {  
    while (position->right_child)
    {
      while(position->left_child) { position = position->left_child; }
      swap(position, found);
      found = position;
    }
    while (position->left_child)
    {
      while(position->right_child) { position = position->right_child; }
      swap(position, found);
      found = position;
    }
  }
}*/