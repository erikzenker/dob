#include <regex>
#include <string>
#include <iostream>



int main(){
  std::string s ("a");
  std::regex e ("\\w", std::regex_constants::extended);
  if (std::regex_match (s,e))
    std::cout << "string object matched\n";
  else
    std::cout << "string object not matched\n";
  return 0;
}
