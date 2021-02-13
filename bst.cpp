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
    //pair_type content;
    std::unique_ptr<pair_type> content;
    node* parent;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

    node(pair_type &&c, node* p): content{new pair_type(std::move(c))}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
    node(const pair_type &c, node* p): content{new pair_type(c)}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
    explicit node(node* p): content{}, parent{p} ,left_child{nullptr}, right_child{nullptr} {}
  };

  /*
   * The tree is represented by a pointer to the root node and its size (the number of nodes)
   * It also stores the comparison operator object-function => op
   */

  std::size_t size;
  std::unique_ptr<node> root;

  inline static constexpr comparison_operator op {};

public:

  template<bool c>
  class iterator_class;
  enum class childness {right, left, orphan};

  using iterator = iterator_class<false>;
  using const_iterator = iterator_class<true>;

  bst() = default;

  //custom ctor: controllare exception -- CONTROLLARE UNICITA' DELLE KEY AmbiguosKeysException : Two matching keys with different values. What should I insert?
  //controllare il numero di volte in cui il costruttore viene chiamato - 1.20h vector
  bst(std::initializer_list<pair_type> lst) : size{0}, root{nullptr} { for(auto i : lst) insert(i);}
  
  //copy (deepcopy) and move semantics
  bst(const bst& tree): bst(tree.deepcopy()) {} //uses move ctor 
  bst& operator = (const bst& tree) { *this = tree.deepcopy(); return *this;} //uses move copy

  bst(bst&&) = default;
  bst& operator = (bst&&) = default;

  ~bst() = default;
 
  std::size_t get_size() { return size; }

  std::pair<iterator, bool> insert(const pair_type& x) { return _insert(x); }
  std::pair<iterator, bool> insert(pair_type&& x) { return _insert(std::move(x)); }

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) { return insert(pair_type(std::forward<Types>(args)...)); }

  void clear() { size = 0; root.reset(nullptr); }

  iterator begin() { return iterator(node_begin()); }
  const_iterator begin() const { return cbegin(); }
  const_iterator cbegin() const { return const_iterator(node_begin());}

  iterator end() { return iterator(node_end()); }
  const_iterator end() const { return cend(); } 
  const_iterator cend() const { return const_iterator(node_end()); }

  iterator find(const key_type& x) { return iterator(node_find(x)); }
  const_iterator find(const key_type& x) const { return const_iterator(node_find(x)); }

  void balance();

  value_type& operator[](const key_type& x) { return subscript(x); };
  value_type& operator[](key_type&& x) { return subscript(std::move(x)); };

  template<typename _K, typename _V, typename _CO>
  friend std::ostream& operator<<(std::ostream& os, const bst<_K,_V,_CO>& x);

  void erase(const key_type& x);

private:
  /*
  * Auxiliary functions: they are stated as private and used throughout the class to ease maintenance and readability of the code
  */

  node* node_find(const key_type& x) const;
  node* node_begin() const ;
  node* node_end() const { return nullptr; }

  template<typename RT>
  std::pair<iterator, bool> _insert(RT&& x);
  
  template<typename RT>
  value_type& subscript(RT&& x);

  bst deepcopy() const; 
  void clear_subtree (iterator &a, childness ch);
  void recursive_insert(bst &nt, pair_type** contents,int n_elem, int start_pt);
};

#include "iterator_class.cpp"
#include "funcs.cpp"