#include "snabl/m.hpp"

using namespace std;

int main() {
  snabl::M m;
  m.abc_lib = make_optional<snabl::libs::Abc>(&m);
  
  return 0;
}
