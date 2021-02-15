# Advanced Programming Project
Authors Giulio Crognaletti and Giulia Marchiori Pietrosanti

## BST in C++ 

For our implementation of a Binary Search Tree, we used a class templated over ```key_type``` , ```value_type```` and ```comparison_operator```named ```bst```. The class uses a set of different substructures defined within it, such as ```node``` and ```iterator```, that will be briefly presented in the following report. \\
First, ```bst``` has the following attributes:

* ```std::unique\_ptr<node>  root```
* ```std::size\_t   size```

plus a static member defined as ```inline static constexpr comparison_operator```

A smart pointer is used to manage the first node: this is done because we wanted to apply RAII design pattern, and because by doing so, the default destructor of bst is sufficient to free all acquired nodes. 
It has 3 constructors, the default, move and a custom copy constructor that performs a deep copy. The deep copy semantic is designed as to copy also the tree structure of bst, not just the elements.
