#ifndef __BST__
#define __BST__

#include<memory>
#include<iostream>
#include<utility>
#include<functional>

template<typename K, typename V, typename CO = std::less<K>>
class bst
/**
 * Implements a Binary Search Tree (bst)
 *
 * authors: Crognaletti Giulio, Marchiori Pietrosanti Giulia
 */
{
  using key_type = K;
  using value_type = V;
  using comparison_operator = CO;
  using pair_type = std::pair<const key_type,value_type>;

  struct node
  /**
   * Define a single node of the bst: it contains a key-value pair, two pointers to the children and one to the parent
   * They are implemented by means of unique pointers to ease the resource management
   * The parent pointer does not own any resource => it is a raw pointer (it is used for tree traversal) 
   */
  {
    std::unique_ptr<pair_type> content;
    node* parent;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

    node() noexcept = default;

    node(node&&) noexcept = default;
    node(pair_type &&c, node* p): content{new pair_type(std::move(c))}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
    node(const pair_type &c, node* p): content{new pair_type(c)}, parent{p}, left_child{nullptr}, right_child{nullptr} {}

    ~node() noexcept = default; 
  };

  /**
   * The tree is represented by a pointer to the root node and its size (the number of nodes)
   */

  std::size_t size;
  std::unique_ptr<node> root;

  /**
   * The comparison object is a static member of the class, so that each tree uses the same and it is not required to give in input
   * or constructing one in each instantiation 
   */
  inline static constexpr comparison_operator op {};

public:

  template<bool c>
  class iterator_class; //Declared in file iterator.hpp

  enum class childness {right, left, orphan, eot}; //useful to understand each node's relationship with its parent

  //exception to be thrown when trying to access the tree using a bound-checking tool.
  struct end_of_tree_exception : public std::exception {const char* what() const noexcept override { return "The end of the binary search tree was reached.";}};

  using iterator = iterator_class<false>;
  using const_iterator = iterator_class<true>;

  bst() noexcept = default;

  //copy (deepcopy) and move semantics
  bst(const bst& tree); 
  bst& operator = (const bst& tree) { return *this = bst(tree); }

  bst(bst&&) noexcept = default;
  bst& operator = (bst&& tree) noexcept = default;

  ~bst() noexcept = default;
 
  std::size_t get_size() const noexcept { return size; }

  std::pair<iterator, bool> insert(const pair_type& x) { return _insert(x); }
  std::pair<iterator, bool> insert(pair_type&& x)  { return _insert(std::move(x)); }

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args) { return insert(pair_type(std::forward<Types>(args)...)); }

  void clear() noexcept { size = 0; root.reset(nullptr); }

  iterator begin() noexcept { return iterator(node_begin()); }
  const_iterator begin() const noexcept { return cbegin(); }
  const_iterator cbegin() const noexcept { return const_iterator(node_begin()); }

  iterator end() noexcept { return iterator(node_end()); }
  const_iterator end() const noexcept { return cend(); } 
  const_iterator cend() const noexcept { return const_iterator(node_end()); }

  iterator find(const key_type& x) { return iterator(node_find(x)); }
  const_iterator find(const key_type& x) const { return const_iterator(node_find(x)); }

  void balance();

  value_type& operator[](const key_type& x) { return subscript(x); }
  value_type& operator[](key_type&& x) { return subscript(std::move(x)); }

  template<typename _K, typename _V, typename _CO>
  friend std::ostream& operator<<(std::ostream& os, const bst<_K,_V,_CO>& x);

  void erase(const key_type& x);

private:
  /*
  * Auxiliary functions: they are stated as private and used throughout the class to ease maintenance and readability of the code
  */

  node* node_find(const key_type& x) const; //just a plain binary search of the content x through the present tree. It returns a node (to be casted in the future).
  node* node_begin() const noexcept; //returns a node pointer to the one with "smallest" key (wrt op) 
  node* node_end() const noexcept { return nullptr; } 

  template<typename RT>
  std::pair<iterator, bool> _insert(RT&& x); //auxiliary function that accepts forwarding
  
  template<typename RT>
  value_type& subscript(RT&& x); //auxiliary function that accepts forwarding

  void clear_subtree (iterator &a, childness ch) noexcept; //eliminates all the nodes under iterator a, including it
  void recursive_insert(bst &nt, pair_type** contents,int n_elem, int start_pt); //inserts elements of an array in a tree in a balanced way by recursion
};

#include"iterator.hpp"
#include"bst.cpp"

#endif
