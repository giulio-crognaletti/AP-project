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

  PT a {67,"zeb89"};
  my_bst.insert(a);
  my_bst.emplace(68,"zeb89 emplaced");
  my_bst.emplace(7,"numero massimo di ogni cosa");
  
  std::cout << "Binary Search Tree: list of all " << my_bst.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst << std::endl;
  
  my_bst.balance();

  /*
  my_bst.erase(88);
  my_bst.erase(75);
  my_bst.erase(60); 
  */

  bst<K,V>::iterator it {my_bst.root_it()};
  std::cout << it->first << std::endl;
  std::cout << it.left()->first << std::endl;
  std::cout << it.right()->first << std::endl;


  std::cout << "Binary Search Tree: list of all " << my_bst.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst << std::endl;

  const std::map<K,V> m {{75,"berlino"},{1,"andiamo"},{3,"a"},{88,"Beppe!"},{60,"maanz"}};
  std::map<K,V>::const_iterator itm = m.begin();
  auto pt1 = &(*itm);
  std::cout << itm->first << std::endl;
  itm = m.find(88);
  //auto pt2 = &(*itm);

  auto b = std::move(*pt1);
  std::cout << b.second << std::endl;
  
  for(auto itm : m) { std::cout << itm.second << std::endl; }

  return 0;
}
