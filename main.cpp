#include<iostream>
#include<string>
#include<initializer_list>
#include<memory>
#include"bst.cpp"

#include<vector>
#include<map>

int main(void)
{

  #define K int
  #define V std::string
  #define PT std::pair<const K,V>

  bst<K,V> my_bst {{75,"berlino"},{1,"andiamo"},{3,"a"},{88,"Beppe!"},{60,"maanz"}};

  
  return 0;
}
