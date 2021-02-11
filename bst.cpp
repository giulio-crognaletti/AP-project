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

  template<bool c>
  class iterator_class;

  using iterator = iterator_class<false>;
  using const_iterator = iterator_class<true>;

  friend const_iterator iterator_to_const(iterator &it) { return const_iterator(it.get()); }
  friend const_iterator iterator_to_const(iterator &&it) { return const_iterator(it.get()); }

  friend iterator const_iterator_to_no(const_iterator &c_it) { return iterator(c_it.get()); }
  friend iterator const_iterator_to_no(const_iterator &&c_it) { return iterator(c_it.get()); }

  bst() = default;
  bst(bst&&) = default;

  // controllare exception
  bst(std::initializer_list<pair_type> lst) : size{0}, root{nullptr}, op{} { for(auto i : lst) insert(i); }
  
  ~bst() = default;

  std::size_t get_size() { return size; }

  std::pair<iterator, bool> insert(const pair_type& x);

  const_iterator cfind(const key_type& x) const; //conversione end = false in boolean
  
  iterator find(const key_type& x) { return const_iterator_to_no(cfind(x)); }
  const_iterator find(const key_type& x) const { return find(x); }

  iterator begin() { return const_iterator_to_no(cbegin()); }
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

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) { return insert(pair_type(std::forward<Types>(args)...)); }

  value_type& operator[](const key_type& x)
  {
    iterator it {find(x)};
    if( it == end() ) it = insert(.?.)->first;
    return it->second;
  }

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

#include "iterator_class.cpp"
#include "funcs.cpp"