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
  node *position = root.get();

  if(position) nt.insert(position->content);
  while(size > nt.get_size())
  {
    if(position->left_child && (nt.find((position->left_child->content).first) == nt.end())) 
    { 
      position = position->left_child.get(); 
      nt.insert(position->content); 
    }
    else if(position->right_child && (nt.find((position->right_child->content).first) == nt.end())) 
    { 
      position = position->right_child.get(); 
      nt.insert(position->content); 
    }
    else { position = position->parent; }
  }
  return nt;
}

template<typename K, typename V, typename CO>
template<typename RT>
std::pair<typename bst<K,V,CO>::iterator,bool> bst<K,V,CO>::pinsert(RT&& x) // COS'E STO TYPE NAME??
{
  iterator it = find(x.first);
  if(it != end()) { return std::pair<iterator, bool>(it, false); }
  else 
  {
    node *child, *position = root.get();
    if(position) 
    {
      while(child = op(x.first,(position->content).first) ? (position->left_child).get() : (position->right_child).get()) { position = child; }

      if(op(x.first,(position->content).first)) { (position->left_child).reset(new node(std::forward<RT>(x),position)); child = (position->left_child).get(); } 
      else { (position->right_child).reset(new node(std::forward<RT>(x),position)); child = (position->right_child).get(); }

      ++size;
      return std::pair<iterator, bool>(iterator(child), true);
    }
    else
    {
      root.reset(new node(std::forward<RT>(x),nullptr));
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

template<typename K, typename V, typename CO>
template<typename RT>
typename bst<K,V,CO>::value_type& bst<K,V,CO>::subscript(RT&& x)
{
  iterator it {find(x)};
  if(!it) 
  {
    V val {};
    it = emplace(std::forward(x), val).first;
  }
  return it->second;
}
/*
template<typename K, typename V, typename CO>
void bst<K,V,CO>::erase(const bst<K,V,CO>::key_type& x) 
{
  iterator found = find(x);
  iterator position {found};
  if (position) 
  {  
    while (position.right())
    {
      swap(++position, found);
      position = found;
    }
    while (position.left())
    {
      swap(--position, found);
      position = found;
    }

    clear_subtree(found);
    --size;
  }
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::swap(bst<K,V,CO>::iterator &a, bst<K,V,CO>::iterator &b)
{
    std::unique_ptr<node>* a_ptr;
    std::unique_ptr<node>* b_ptr;

    switch (a.childness())
    {
      case iterator::childness::right:
        a_ptr = &(((node*)a.parent())->right_child);
        break;

      case iterator::childness::left:
        a_ptr = &(((node*)a.parent())->left_child);
        break;

      case iterator::childness::orphan:
        a_ptr = &root;
        break;

      default:
        break;
    }
    
    switch (b.childness())
    {
      case iterator::childness::right:
        b_ptr = &(((node*)b.parent())->right_child);
        break;

      case iterator::childness::left:
        b_ptr = &(((node*)b.parent())->left_child);
        break;

      case iterator::childness::orphan:
        b_ptr = &root;
        break;

      default:
        break;
    }
 
    node* node_a = (node*)a;
    node* node_b = (node*)b;

    std::swap(node_a->parent, node_b->parent);

    if(node_a->left_child == node_b) std::swap(node_a->left_child, node_b->left_child);
    else if(node_b->left_child == node_a) std::swap(node_a->left_child, node_b->left_child);
    else std::swap(node_a->left_child, node_b->left_child);

    std::swap( node_a->right_child, node_b->right_child);
    std::swap(*a_ptr, *b_ptr);
}

template<typename K, typename V, typename CO>
void bst<K,V,CO>::clear_subtree (bst<K,V,CO>::iterator &a) 
{
  std::unique_ptr<node>* a_ptr;
  switch (a.childness())
  {
    case iterator::childness::right:
      a_ptr = &(((node*)a.parent())->right_child);
      break;

    case iterator::childness::left:
      a_ptr = &(((node*)a.parent())->left_child);
      break;

    case iterator::childness::orphan:
      a_ptr = &root;
      break;

    default:
      break;
  }

  a_ptr->reset(nullptr);
};*/
