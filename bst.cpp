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
   * The parent pointer does not own any resource => it is a raw pointer (it is used to ease tree traversal) 
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
   * It also stores the comparison operator object-function => op
   */
  
  std::unique_ptr<node> root;
  std::size_t size;
  comparison_operator op; //default ctor should be called here
    
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
    //TENATIVO
    comparison_operator op; // default ctor should be called here
  
  public:
    iterator() = default;
    iterator(node *np): node_ptr{np} {};
    ~iterator() = default;

    //copy-move constructors e volendo anche i copy-move assignement

    bool operator == (const iterator i) const { return node_ptr == i.node_ptr; }
    bool operator != (const iterator i) const { return node_ptr != i.node_ptr; }
    std::pair<const key_type,value_type> operator * () { return node_ptr->content; } // QUESTI DUE POSSONO AVERE O NON AVERE UN CONST -> rvalue vs lvalue -> const_iterator vs iterator
    std::pair<const key_type,value_type> operator -> () { return *(node_ptr->content); } //DA PROVARE E CAPIRE
    //*a = t

    /*
    * !! NON AVEVAMO USATO IL COMPARISON OPERATOR ----> VA BENE COME HO FATTO? !!
    *   ||
    *   \/
    */

    void next() //QUESTO USA OP OPPURE !(OP) ? --> HO IMMAGINATO !(OP)
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

    iterator operator ++() 
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

    iterator operator --() 
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
