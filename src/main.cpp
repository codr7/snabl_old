#include "snabl/m.hpp"
#include "snabl/repl.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  snabl::M m;
  m.abc_lib.emplace(m);
  snabl::Pos pos("init", 1, 1);
  m.use(*m.abc_lib, {}, pos);

  if (argc == 1) {
    cout <<
      "Snabl v5" << endl << endl <<
      "Hit Enter twice to evaluate," << endl <<
      "Ctrl+D quits." << endl << endl;
    
    snabl::repl(cin, cout, m);
  } else {
    for (int i = 1; i < argc; i++) {
      if (auto err = m.include(argv[i], pos); err) {
	cout << *err << endl;
	return -1;
      }
    }
  }
  
  return 0;
}
