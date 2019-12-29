#include <iostream>
#include <regex>

int main() {
  const std::regex r1("(c(a|b)*ab)*ca");
  std::cmatch m;
  bool res1 = std::regex_match ("caabca", m, r1);
  bool res2 = std::regex_match ("cbbbab", m, r1);
  std::cout << std::boolalpha << res1 << " " << res2 << std::endl;
}
