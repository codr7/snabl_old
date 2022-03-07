#include "snabl/m.hpp"

using namespace std;

int main() {
  snabl::M m;
  m.abc_lib.emplace(m);
  return 0;
}
