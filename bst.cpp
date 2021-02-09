#include<iostream>
#include<memory>
#include<functional>

template<typename key_type, typename value_type, typename comparison_operator = std::less<key_type>>
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
  comparison_operator op;
    
public:

  //bst() = default;
  bst(): root{nullptr}, size{0} {}
  ~bst() = default;
  
  //funzioni piccole ok nella classe, funzioni grandi fuori

  //controlliamo le exception
  struct iterator
  {
  private:
    node* node_ptr; //std::pair<const key_type,value_type> *content_ptr = nullptr;
  
  public:
    iterator() = default;
    iterator(node *np): node_ptr{np} {};
    ~iterator() = default;

    bool operator == (iterator i) { return node_ptr == i.node_ptr; }
    bool operator != (iterator i) { return node_ptr != i.node_ptr; }
    std::pair<const key_type,value_type> operator * () { return node_ptr->content; }
    std::pair<const key_type,value_type> operator -> () { return *(node_ptr->content); } //da provare e da capire
    //*a = t
    void next()
    {
      /**
       * ho figlio destro? 
       *  si -> vai a figlio destro e poi scendi fino al figlio più a sinistra possibile
       *  no -> il primo padre maggiore di me
       *    se arrivo a root -> sono il nodo finale
       * 
      */
      if (node_ptr->right_child) //ho un figlio destro? se si...
      {
        node_ptr = (node_ptr->right_child).get();
        while (node_ptr->left_child) { node_ptr = (node_ptr->left_child).get(); }
      }
      else 
      {
        while (node_ptr->parent && (node_ptr->content).first > (node_ptr->parent->content).first) { node_ptr = (node_ptr->parent).get(); }
        if (!node_ptr->parent) { node_ptr = nullptr; } 
      }
    }
    iterator operator ++ () 
    {
      this->next();
      return *this;
    }
    iterator operator ++(int)
    {
      iterator ret = *this;
      this->next();
      return ret;
    }
  };

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
