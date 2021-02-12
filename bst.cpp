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

  node* node_find(const key_type& x) const; //conversione end = false in boolean
  node* node_begin() const 
  {
    node *position  = root.get();
    if(position) { while(position->left_child) position = (position->left_child).get(); }

    return position;
  }
  node* node_end() const { return nullptr; }
    
public:

  template<bool c>
  class iterator_class;

  using iterator = iterator_class<false>;
  using const_iterator = iterator_class<true>;

  bst() = default;
  bst(bst&&) = default;

  // controllare exception
  bst(std::initializer_list<pair_type> lst) : size{0}, root{nullptr}, op{} { for(auto i : lst) insert(i); }
  
  ~bst() = default;

  std::size_t get_size() { return size; }

  std::pair<iterator, bool> insert(const pair_type& x); //min. 01:44:58 forwarding reference r and l value
  
  iterator find(const key_type& x) { return iterator(node_find(x)); }
  const_iterator find(const key_type& x) const { return const_iterator(find(x)); }

  iterator begin() { return iterator(node_begin()); }
  const_iterator begin() const { return cbegin(); }
  const_iterator cbegin() const { return const_iterator(node_begin());}

  iterator end() { return iterator(node_end()); }
  const_iterator end() const { return cend(); } 
  const_iterator cend() const { return const_iterator(node_end()); }

  void clear() { size = 0; root.reset(nullptr); }

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) { return insert(pair_type(std::forward<Types>(args)...)); }

  value_type& operator[](const key_type& x)
  {
    iterator it {find(x)};
    if(!it) 
    {
      V val {};
      it = emplace(x, val).first;
    }
    return it->second;
  }

  bst deepcopy();

  void swap(const iterator& it1, const iterator& it2);
  void erase(const key_type& x);

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

#include "iterator_class.cpp"
#include "funcs.cpp"