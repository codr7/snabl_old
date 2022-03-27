#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/goto.hpp"
#include "snabl/fuses/util.hpp"

namespace snabl::fuses {
  int _goto(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::GOTO) {
	pc1 = ops::goto_pc(op1);
	
	if (PC pc2 = drill_pc(pc1, m); pc2 != pc1) {
	  cout << "Fusing " << fun << " GOTO: " << pc2 << endl;
	  ops::GOTO(op1, pc2);
	  n++;
	}
      }

      pc1 += op_len(op1);
    }

    return n;
  }
}
