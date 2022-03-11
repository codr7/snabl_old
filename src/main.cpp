#include "snabl/m.hpp"
#include "snabl/repl.hpp"

using namespace std;

int main() {
  snabl::M m;
  m.abc_lib.emplace(m);
  cout << "Snabl v1" << endl << endl;
  snabl::repl(cin, cout, m);
  return 0;
}
