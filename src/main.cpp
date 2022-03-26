#include "snabl/m.hpp"
#include "snabl/repl.hpp"

using namespace std;

int main() {
  snabl::M m;
  m.abc_lib.emplace(m);
  snabl::Pos pos("init", 1, 1);
  m.use(*m.abc_lib, {}, pos);

  cout <<
    "Snabl v2" << endl << endl <<
    "Hit Enter twice to evaluate," << endl <<
    "Ctrl+D quits." << endl << endl;
  
  snabl::repl(cin, cout, m);
  return 0;
}
