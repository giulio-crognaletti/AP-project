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
      op(x.first,(position->content).first) ? (position->left_child).reset(new node(x,position)) : (position->right_child).reset(new node(x,position));
      ++size;
      return std::pair<iterator, bool>(iterator(position), true);
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
typename bst<K,V,CO>::const_iterator bst<K,V,CO>::cfind(const typename bst<K,V,CO>::key_type& x) const //DI NUOVO STO TYPRNAME?
{
    node *position = root.get();
    while(position)
    {
    if((position->content).first == x) { return const_iterator(position); }
    position = op(x,(position->content).first) ? (position->left_child).get() : (position->right_child).get();
    }
    return cend();
}
