#include<iostream>
#include<memory>
#include<utility>
#include<functional>

template<typename K, typename V, typename CO = std::less<K>> //verfica sto const
class bst
/*
 * Implements a Binary Search Tree (bst)
 *
 * authors: Crognaletti Giulio, Marchiori Pietrosanti Giulia
 * 
 */
{
  using key_type = K;
  using value_type = V;
  using comparison_operator = CO;
  using pair_type = std::pair<const key_type,value_type>;

  struct node
  /*
   * Define a single node of the bst: it contains a key-value pair, two pointers to the children and one to the parent.
   * They are implemented by means of unique pointers to ease the resource management
   * The parent pointer does not own any resource => it is a raw pointer (it is used to ease tree traversal) 
   *
   * There is only one constructor that simply initializes each attribute with some given value
   * The destructor is default destructor
   * 
   */
  {
    pair_type content;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;
    node* parent;

    node(pair_type c, node* p): content{c}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
    explicit node(node* p): content{}, parent{p} ,left_child{nullptr}, right_child{nullptr} {}
  };

  /*
   * The tree is represented by a pointer to the root node and its size (the number of nodes)
   * It also stores the comparison operator object-function => op
   */
  std::unique_ptr<node> root;
  std::size_t size;
  comparison_operator op; //default ctor should be called here
    
public:

  bst() = default;
  bst(const bst&) = default;
  bst(bst&&) = default;

  // controllare exception
  bst(std::initializer_list<pair_type> lst) : size{0}, root{nullptr}, op{} { for(auto i : lst) insert(i); }
  
  ~bst() = default;

  std::size_t get_size() { return size; }
  //funzioni piccole ok nella classe, funzioni grandi fuori

  //controlliamo le exception
  #include "iterator_class.cpp"
 
  std::pair<iterator, bool> insert(const pair_type& x)
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
  
  iterator find(const key_type& x)
  {
    node *position = root.get();
    while(position)
    {
      if((position->content).first == x) { return iterator(position); }
      position = op(x,(position->content).first) ? (position->left_child).get() : (position->right_child).get();
    }
    return end();
  }

  iterator begin()
  {
    node *position  = root.get();
    if(position) { while(position->left_child) { position = (position->left_child).get(); } }

    return iterator(position);
  }
  const_iterator begin() const { return cbegin(); }
  const_iterator cbegin() const
  {
    node *position  = root.get();
    if(position) { while(position->left_child) position = (position->left_child).get(); }

    return const_iterator(position);
  }

  iterator end() { return iterator(nullptr); }
  const_iterator end() const { return cend(); } 
  const_iterator cend() const { return const_iterator(nullptr); }

  void clear() { size = 0; root.reset(nullptr); }

  bst deepcopy();
  /*
  std::pair<iterator, bool> insert(const pair_type& x);
  std::pair<iterator, bool> insert(pair_type&& x);

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args);

  iterator find(const key_type& x);
  const_iterator find(const key_type& x) const;

  void balance();

  value_type& operator[](const key_type& x);
  value_type& operator[](key_type&& x);

  //copy and deepcopy

  void erase(const key_type& x);
  */

  friend
  std::ostream& operator<<(std::ostream& os, const bst& x)
  {
    for(auto it : x){ os << "(" << it.first << " : "<< it.second << ") "; }
    return os;
  }
};

template<typename K, typename V, typename CO>
bst<K,V,CO> bst<K,V,CO>::deepcopy()
{
  /*
  * genera un albero vuoto
  * condizione: copiato non ha null && copiante ha null (per evitare di copiare i figli due volte)
  * se condizione -> copia nodo e scendi;
  * altrimenti risali;
  * 
  */
}

/*template<typename K, typename V, typename CO>
std::pair<bst<K,V,CO>::iterator,bool> bst<K,V,CO>::insert(const bst<K,V,CO>::pair_type& x)
{

}*/