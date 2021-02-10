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
  //typedef K key_type;
  //typedef V value_type;
  //typedef CO comparison_operator;
  //typedef std::pair<const key_type,value_type> pair_type;

  using key_type = K;
  using value_type = V;
  using comparison_operator = const CO;
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

    //node(): content{}, parent{nullptr}, left_child{nullptr}, right_child{nullptr} {}
    //explicit node(node* p): content{}, parent{p}, left_child{nullptr}, right_child{nullptr} {}

    node(pair_type c, node* p): content{c}, parent{p}, left_child{nullptr}, right_child{nullptr} {}
  };

  /*
   * The tree is represented by a pointer to the root node and its size (the number of nodes)
   * It also stores the comparison operator object-function => op
   */
  std::unique_ptr<node> root;
  std::size_t size;
  comparison_operator op; //default ctor should be called here
    
public:

  //bst() = default;
  bst(): root{nullptr}, size{0}, op{} {}
  // controllare exception
  bst(std::initializer_list<pair_type> lst) : size{0}, root{nullptr}, op{} { for(auto i : lst) insert(i); }
  ~bst() = default;
  
  //funzioni piccole ok nella classe, funzioni grandi fuori

  //controlliamo le exception
  struct iterator
  {
  private:
    node* node_ptr;
    comparison_operator op; // default ctor should be called here
  
  public:
    iterator() = default;
    iterator(node *np): node_ptr{np}, op{} {};

    iterator(const iterator &) = default;
    iterator(iterator &&) = default;

    ~iterator() = default;

    //copy-move constructors e volendo anche i copy-move assignement
    iterator &operator = (const iterator &) = default;
    iterator &operator = (iterator &&) = default;

    bool operator == (const iterator i) const { return node_ptr == i.node_ptr; }
    bool operator != (const iterator i) const { return node_ptr != i.node_ptr; }
    pair_type &operator * () const { return node_ptr->content; } // QUESTI DUE POSSONO AVERE O NON AVERE UN CONST -> rvalue vs lvalue -> const_iterator vs iterator
    pair_type *operator -> () const { return &(node_ptr->content); } //DA PROVARE E CAPIRE (ANCHE SIGNIFICATO DEI CONST)
    //*a = t

    void next()
    /*
    * This method finds the next node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
    * 
    * The algorithm is simple and it follows this scheme:
    * Has my node a right child ? (i.e. is there some child for which op(my_key, child_key) = false ?)
    * 
    *  -> YES: Then the next must be the leftmost child of my right child.
    *  -> NO: Then the next must be the first parent of the node that satisfies op(my_key, child_key) == false
    * 
    * The final node is the one for wich no node satisfies the request, and it must return nullptr as a result. 
    */ 
     {
      if (node_ptr->right_child) //Do I have a right child? YES...
      {
        node_ptr = (node_ptr->right_child).get();
        while (node_ptr->left_child) { node_ptr = (node_ptr->left_child).get(); }
      }
      else //NO ...
      {
        while (node_ptr->parent && !op((node_ptr->content).first,(node_ptr->parent->content).first)) { node_ptr = node_ptr->parent; }
        if (!node_ptr->parent) { node_ptr = nullptr; } 
        else { node_ptr = node_ptr->parent; }
      }
    }

    void prev()
    /*
    * This method finds the previous node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
    * 
    * The algortim is identical, but this time we look for key pairs satisfying the condition  op(my_key, child_key) == true.
    * This of course implies also swapping right and left child (e.g. The question should be "Has my node a LEFT child?" this time)
    */
    {
      if (node_ptr->left_child)
      {
        node_ptr = (node_ptr->left_child).get();
        while (node_ptr->right_child) { node_ptr = (node_ptr->right_child).get(); }
      }
      else 
      {
        while (node_ptr->parent && op(node_ptr->content).first,(node_ptr->parent->content).first) { node_ptr = (node_ptr->parent).get(); }
        if (!node_ptr->parent) { node_ptr = nullptr; }
        else { node_ptr = node_ptr->parent; } 
      }
    }

    //PER QUESTI HO VISTO SU INTERNET CHE SI USANO LE REFERENCE, IN CASO CI PENSIAMO...
    iterator &operator ++() 
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

    iterator &operator --() 
    {
      this->prev();
      return *this;
    }
    iterator operator --(int)
    {
      iterator ret = *this;
      this->prev();
      return ret;
    }
  };

  struct const_iterator
  {
  private:
    node* node_ptr;
    comparison_operator op; // default ctor should be called here
  
  public:
    const_iterator() = default;
    explicit const_iterator(node *np): node_ptr{np}, op{} {}
    ~const_iterator() = default;

    //copy-move constructors e volendo anche i copy-move assignement
  
    bool operator == (const const_iterator i) const { return node_ptr == i.node_ptr; }
    bool operator != (const const_iterator i) const { return node_ptr != i.node_ptr; }
    const pair_type &operator * () const { return node_ptr->content; }
    const pair_type *operator -> () const { return &(node_ptr->content); }
    //*a = t

    void next()
    /*
    * This method finds the next node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
    * 
    * The algorithm is simple and it follows this scheme:
    * Has my node a right child ? (i.e. is there some child for which op(my_key, child_key) = false ?)
    * 
    *  -> YES: Then the next must be the leftmost child of my right child.
    *  -> NO: Then the next must be the first parent of the node that satisfies op(my_key, child_key) == false
    * 
    * The final node is the one for wich no node satisfies the request, and it must return nullptr as a result. 
    */ 
     {
      if (node_ptr->right_child) //Do I have a right child? YES...
      {
        node_ptr = (node_ptr->right_child).get();
        while (node_ptr->left_child) { node_ptr = (node_ptr->left_child).get(); }
      }
      else //NO ...
      {
        while (node_ptr->parent && !op(node_ptr->content).first,(node_ptr->parent->content).first) { node_ptr = (node_ptr->parent).get(); }
        if (!node_ptr->parent) { node_ptr = nullptr; } 
      }
    }

    void prev()
    /*
    * This method finds the previous node in the BST from the one pointed by the iterator, based on the comparison operator defined inside bst used on keys.
    * 
    * The algortim is identical, but this time we look for key pairs satisfying the condition  op(my_key, child_key) == true.
    * This of course implies also swapping right and left child (e.g. The question should be "Has my node a LEFT child?" this time)
    */
    {
      if (node_ptr->left_child)
      {
        node_ptr = (node_ptr->left_child).get();
        while (node_ptr->right_child) { node_ptr = (node_ptr->right_child).get(); }
      }
      else 
      {
        while (node_ptr->parent && op(node_ptr->content).first,(node_ptr->parent->content).first) { node_ptr = (node_ptr->parent).get(); }
        if (!node_ptr->parent) { node_ptr = nullptr; } 
      }
    }

    //PER QUESTI HO VISTO SU INTERNET CHE SI USANO LE REFERENCE, IN CASO CI PENSIAMO...
    const_iterator &operator ++() const
    {
      this->next();
      return *this;
    }
    const_iterator operator ++(int)
    {
      iterator ret = *this;
      this->next();
      return ret;
    }

    const_iterator &operator --() 
    {
      this->prev();
      return *this;
    }
    const_iterator operator --(int)
    {
      iterator ret = *this;
      this->prev();
      return ret;
    }
  };
  
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
        return std::pair<iterator, bool>(*new iterator(position), true);
      }
      else
      {
        root.reset(new node(x,nullptr));
        ++size;
        return std::pair<iterator, bool>(*new iterator(root.get()), true);
      }
    }
  }

  
  iterator find(const key_type& x)
  {
    node *position = root.get();
    while(position)
    {
      if((position->content).first == x) { return *new iterator(position); }
      position = op(x,(position->content).first) ? (position->left_child).get() : (position->right_child).get();
    }
    return end();
  }

  iterator begin()
  {
    node *position  = root.get();
    if(position) { while(position->left_child) { position = (position->left_child).get(); } }

    return *new iterator(position);
  }
  const_iterator begin() const { return cbegin(); }
  const_iterator cbegin() const
  {
    node *position  = root.get();
    if(position) { while(position->left_child) position = (position->left_child).get(); }

    return *new const_iterator(position);
  }

  iterator end() { return *new iterator(nullptr); } //this is meant to represent the right_child of the last element (which is by construction nullptr)
  const_iterator end() const { return cend(); } 
  const_iterator cend() const { return *new const_iterator(nullptr); }


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
