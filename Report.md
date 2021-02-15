# Advanced Programming Project
Authors Giulio Crognaletti and Giulia Marchiori Pietrosanti

## Bst in C++ 

For our implementation of a Binary Search Tree, we used a class templated over ```key_type``` , ```value_type``` and ```comparison_operator```named ```bst```. The class uses a set of different substructures defined within it, such as ```node``` and ```iterator```, that will be briefly presented in the following report. \\
First, ```bst``` has the following attributes:

* ``` std::unique_ptr<node>  root```
* ``` std::size_t   size```

plus a static member defined as ```inline static constexpr comparison_operator```

A smart pointer is used to manage the first node: this is done because we wanted to apply RAII design pattern, and because by doing so, the default destructor of bst is sufficient to free all acquired nodes. 
It has 3 constructors, the default, move and a custom copy constructor that performs a deep copy. The deep copy semantic is designed as to copy also the tree structure of bst, not just the elements.

### Nodes

```node``` is an important subclass of the bst class that represent the internal elements of the tree. Nodes are composed of 4 attributes:

* ```std::unique_ptr<pair_type>``` where ```pair_type``` is defined as ```std::pair<const key_type, value_type>```
* ```std::unique_ptr<node>``` to a possible left child
* ```std::unique_ptr<node>``` to a possible right child
* An ordinary pointer to the parent.

The root will be the only node with a null pointer to the parent. The content is stored in the heap using a ```std::unique_ptr<pair_type>``` to make easier and cheaper the swapping of the content between the nodes (which is useful to make an efficient erase of a given key). Children use smart pointers because the are meant to own all subsequent nodes, while parent use a ordinary pointer because it is just a utility helpful in navigating the tree.

### Iterators

Iterators are a tool that allows us to iterate in an efficient and ordered way over the data structure thanks to some function that will allow us to find the previous or the next element of a given tree or to go from a node to its child or its parent. 
Their only attribute is an ordinary pointer to node. They come in two different kinds: ```iterator``` and ```const_iterator```. The two are actually the same except that using ```const_iterator``` won't allow modification of the pointed content, while using ```iterator``` will. Given the similarities, we used a boolean templated class to build them, called ```iterator_class``` to ease maintenance of the code and avoid duplication.

We implemented bidirectional iterators, meaning that they have both ```operator++``` and ```operator--```. We also added some utility function to go to children and parent. In the end we write a function that allow the iterator to know if the node at which it points is a left or right child or orphan (the root). We called this feature of nodes their "childness". All these functions are used throughout the code and help keeping it clean and easily readable.

We implemented the requested member functions and here we will describe only the functions in which some specific design choices were made or in which particular algorithms where used.

### Memeber functions

One of the first that has to be implemented is the insertion of a new node given a pair. We have to take care of the position where the it will be (eventually) inserted. It has to be overloaded, to accept both L-value and R-value references in input, while doing basically the same task.
To avoid code duplication, we used a support function ```_insert()``` which is templated over the reference type to support forwarding. By doing so, the same body is used to find the correct place (if any) and then, for the creation of the node, input is forwarded to the node constructors.

```emplace``` has almost the same request as ```insert```, but is capable to take all the value useful for the construction of a new node and when the insertion is computed it will create the needed value. Given the similarities with the two, this second function is obtained by creating a pair (using again forwarding) and then by invoking the R-value version of insert (which calls the content move constructor to the node). This causes of course the function to invoke the constructor of content an extra time, but if the first is dependent on input, the second is always a cheap move.

Begin and find are both required to return both an ```iterator``` and a ```const_iterator```. To solve the problem without writing almost-identical functions, we used utility functions that returned just a plain node pointer, which is then casted to the two flavors of iterator as needed.

For the finding member function we consider a binary search algorithm.

Regarding the ```balance``` function, first we save in an ordered way a pointer to all the contents of the node (the pair key-value) in an array. Then we insert (using move semantics and the stored pointers) all the nodes in a new tree using a recursive support function that compute the insertion of the central element of the considered array. Than it calls itself one time with left half of the original array and the second time with the right half. It stops when the last element is inserted. The fact that we take every time the central value guarantee the balance of the new tree.

Regarding the ```erase``` function, we want to remove a node with a specific key. To achieve this efficiently, we first find the node to be erased. If the node is present, we swap its content (i.e. we exchange the content pointers) with a node with adjacent key iteratively, until we swap it with a leaf, which can be easily removed. By exchanging contents this way we ensure that the validity of the tree is preserved.

## Dictionary reversal using Python

We propose two solutions to the same problem using a Python implementation of the ```reverse_dictionary``` function.
The first uses two nested for constructs to cycle over all values in each list, and uses a ```set()``` to add the keys avoiding repetitions. Next, we cast all the values of the new dictionary from set to list.

We tried also another algorithm, where we use comprehensions. First we create the set with all the new keys (to avoid repetitions), then we create the reversed dictionary by associating a list comprehension to each element.

The second algorithm results to be slower than the first one, the first one use more resources in terms of memory and theoretical time.

 ## Benchmark
