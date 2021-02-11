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
  
  std::cout << "Binary Search Tree: list of all " << my_bst.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst << std::endl;

  bst<K,V>::iterator it = my_bst.begin();
  it->second = "uanm";
  bst<K,V>::const_iterator c_it = bst<K,V>::iterator_to_const(it);

  std::cout << "Binary Search Tree: list of all " << my_bst.get_size() << " (key,value) pairs. \n => ";
  std::cout << my_bst << std::endl;

  //bst<K,V> my_bst2 {my_bst};

  return 0;

  // L'ALGORITMO DI NEXT() AVEVA UN PICCOLO PROBLEMINO CHE GENERAVA UN LOOP INFINTO... ORA DOVREBBE ESSERE SISTEMATO MA MEGLIO RICONTROLLARLO
  // TUTTO QUELLO CHE HO PROVATO SEMBRA ANDARE, SONO LE 2.30am MA SONO FELICE COMUNQUE AHAHAA
}
