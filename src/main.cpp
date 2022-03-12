#include "snabl/m.hpp"
#include "snabl/repl.hpp"

using namespace std;

int main() {
  snabl::M m;
  m.abc_lib.emplace(m);

  cout <<
    "Snabl v1" << endl << endl <<
    "Hit Enter twice to evaluate," << endl <<
    "Ctrl+D quits." << endl << endl;
  
  snabl::repl(cin, cout, m);
  return 0;
}
