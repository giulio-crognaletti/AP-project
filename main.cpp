#include<iostream>
#include<string>
#include<initializer_list>
#include<memory>
#include"bst.cpp"

int main(int argc, char** argv)
{

  #define K int
  #define V std::string
  #define PT std::pair<const K,V>

  std::initializer_list<PT> s  = {{75,"berlino"},{1,"andiamo"},{3,"a"},{88,"Beppe!"},{60,"maanz"}};
  bst<K,V> my_bst(s);

  PT a {67,"zeb89"};
  my_bst.insert(a);
  
  for(auto i : my_bst)
  {
    std::cout << i.second << " ";
  }
  std::cout << std::endl;   

  std::cout << (*my_bst.find(3)).second << std::endl; //OK works
  std::cout << (*my_bst.find(68)).second << std::endl; //error: try to derefernce nullptr!!
  return 0;
}
