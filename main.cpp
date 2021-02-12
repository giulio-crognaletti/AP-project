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

  bst<K,V> my_bst {{75,"berlino"},{1,"andiamo"},{3,"a"},{88,"Beppe!"},{60,"maanz"}};

  PT a {67,"zeb89"};
  my_bst.insert(a);
  my_bst.emplace(67,"zeb89 emplaced");
  
  std::cout << "Binary Search Tree: list of all " << my_bst.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst << std::endl;

  bst<K,V> my_bst2 {my_bst};

  std::cout << "Binary Search Tree: list of all " << my_bst2.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst2 << std::endl;

  bst<K,V> my_bst3 {};
  my_bst3 = std::move(my_bst);

  std::cout << "Binary Search Tree: list of all " << my_bst3.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst3 << std::endl;
  
  /*const bst<K,V> my_bst2 {{75,"berlino"},{1,"andiamo"},{3,"a"},{88,"Beppe!"},{60,"maanz"}};
  c_it = my_bst2.find(75);
  std::cout << c_it->second << std::endl;
  std::cout << my_bst2 << std::endl;*/

  return 0;

  // L'ALGORITMO DI NEXT() AVEVA UN PICCOLO PROBLEMINO CHE GENERAVA UN LOOP INFINTO... ORA DOVREBBE ESSERE SISTEMATO MA MEGLIO RICONTROLLARLO
  // TUTTO QUELLO CHE HO PROVATO SEMBRA ANDARE, SONO LE 2.30am MA SONO FELICE COMUNQUE AHAHAA
}
