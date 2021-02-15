#include "bst.hpp"
#include <string>

int main() {

// test insert()
  
  bst<int,char> my_bst{};  

  auto p = std::pair<int,char>{5,'a'};    						
  auto output = my_bst.insert(p); // l-value insert
  std::cout << "Value inserted? " << (output.second ? "Yes\n" : "No\n") ;
  my_bst.insert(std::pair<const int,char>{4,'b'}); // r-value insert
  my_bst.insert(std::pair<const int,char>{7,'c'});
  output = my_bst.insert(std::pair<const int,char>{7,'d'});
  std::cout << "Value inserted? " << (output.second ? "Yes\n" : "No\n") ;

// test begin()
  auto mybegin = my_bst.begin();
  std::cout << "first key: " << mybegin->first << " first value: " << mybegin->second << "\n";

  auto cit = my_bst.cbegin();
  ++cit;
  std::cout << "second key: " << cit->first << " second value: " << cit->second << "\n";

// test put-to operator
  std::cout << "Print tree:\n" << my_bst << std::endl;
  
// test find()
  std::cout << "find 7... ";
  auto r = my_bst.find(7);
  std::cout << "found " << r->first << std::endl;

  std::cout << "find 47... ";
  r = my_bst.find(47);
  if(r==my_bst.end())
    std::cout << "value not found" << std::endl;



// test subscript operator
  auto value = my_bst[7];
  std::cout<< "my_bst[7] = " << value << std::endl;

  value = my_bst[56];
  std::cout<< "my_bst[56] = " << value << std::endl;

// test clear()
  std::cout << "before clearing: " << my_bst;
  my_bst.clear();
  std::cout << "\nafter clearing: ";
  std::cout << my_bst << std::endl;

// test emplace()
  my_bst.emplace(8, 'a');
  my_bst.emplace(4, 'b');
  my_bst.emplace(17, 'c');
  my_bst.emplace(10, 'd');
  my_bst.emplace(10, 'e');

// test erase()
  std::cout << "before erasing: " << my_bst;
  std::cout << "\nerase 8\n";
  my_bst.erase(8);
  std::cout << "after erasing: ";
  std::cout << my_bst;

// test copy ctor
  std::cout << "\ncall copy ctor: new bst is ";
  bst<int, char> my_bst2 {my_bst}; 
  std::cout << my_bst2 << "\n";
  my_bst.erase(17);
  std::cout << "erased 17 from my_bst - check if really deep copy: \n";
  std::cout << "my_bst: " << my_bst << "\n";       
  std::cout << "my_bst2: " << my_bst2 << "\n";

  std::cout << "call move ctor from my_bst2 to my_bst3\n";
  bst<int, char> my_bst3 {std::move(my_bst2)};
  std::cout << "my_bst3: " << my_bst3 << "\n"; 

  std::cout << "copy assign my_bst3 to my_bst2\n";
  my_bst2 = my_bst3;
  std::cout << "my_bst2: " << my_bst2 << "\n";
  std::cout << "my_bst3: " << my_bst3 << "\n";
  my_bst3.erase(17);
  std::cout << "erased 17 from my_bst3 - check if really deep copy: \n";
  std::cout << "my_bst2: " << my_bst2 << "\n";
  std::cout << "my_bst3: " << my_bst3 << "\n";

  std::cout << "move assign my_bst3 to my_bst2\n";
  my_bst2 = std::move(my_bst3);
  std::cout << "my_bst2: " << my_bst2 << "\n";

  return 0;
}