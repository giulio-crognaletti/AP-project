#include<iostream>
#include<memory>

template<typename key_type, typename value_type> // typename comparison_operator = std::less<key_type>
class bst
/*
 * Implements a Binary Search Tree (bst)
 *
 * authors: Crognaletti Giulio, Marchiori Pietrosanti Giulia
 *
 */
{
  struct node
  /*
   * Define a single node of the bst: it contains a key-value pair, two pointers to the children and one to the parent.
   * They are implemented by means of unique pointers to ease the resource management
   *
   * There is only one constructor that simply initializes each attribute with some given value
   * The destructor is default destructor
   *
   */
  {
    std::pair<const key_type,value_type> content;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;
    node* parent;

    //node(): content{}, parent{nullptr}, left_child{nullptr}, right_child{nullptr} {}
    //explicit node(node* p): content{}, parent{p}, left_child{nullptr}, right_child{nullptr} {}

    node(std::pair<const key_type,value_type> c, node* p): content{c}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
  };

  /*
   * The tree is represented by a pointer to the root node and its size (the number of nodes)
   */
  
  std::unique_ptr<node> root;
  std::size_t size;
    
public:

  bst(): root{nullptr}, size{0} {}
  
  /*
  std::pair<iterator, bool> insert(const pair_type& x);
  std::pair<iterator, bool> insert(pair_type&& x);

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args);

  void clear();

  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;

  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

  iterator find(const key_type& x);
  const_iterator find(const key_type& x) const;

  void balance();

  value_type& operator[](const key_type& x);
  value_type& operator[](key_type&& x);

  friend
  std::ostream& operator<<(std::ostream& os, const bst& x);

  //copy and deepcopy

  void erase(const key_type& x);
  */
};
