#include "snabl/repl.hpp"

#include <sstream>

#include "snabl/form.hpp"
#include "snabl/m.hpp"
#include "snabl/reader.hpp"

namespace snabl {  
   void repl(istream &in, ostream &out, M &m) {
    stringstream buf;
  
    for (;;) {
      out << "  ";
     
      string line;
      if (!getline(in, line)) { break; }
       
      if (line.empty()) {
	optional<Form> f;
	Pos pos("repl", 1, 1);
	PC start_pc = m.emit_pc;

	for (;;) {
	  auto [f, err] = read_form(buf, pos, m);

	  if (err) {
	    out << *err << endl;
	    break;
	  }
	  
	  if (err = f->emit(0, m); err) {
	    out << *err << endl;
	    break;
	  }
	}
	
	buf.str("");
	buf.clear();
	ops::STOP(m.emit());
	
	if (auto err = m.eval(start_pc); err) {
	    out << *err << endl;
	    break;
	}

	out << m.state->regs[0] << endl;
      } else {
	buf << line << endl;
      }
    }
  }
}
