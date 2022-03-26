#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/nop.hpp"

namespace snabl::fuses {
  int nop(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::NOP) {
	int nops = 0;
	
	for (PC pc2 = pc1+1; pc2 < m.emit_pc; pc2++) {
	  Op op2 = m.ops[pc2];
	  if (op_code(op2) != OpCode::NOP) { break; }
	  nops++;
	}

	if (nops) {
	  cout << "Fusing " << fun << " NOP: " << pc1 << endl;
	  ops::GOTO(op1, pc1+nops+1);
	  n += nops;
	}
      }

      pc1 += op_len(op1);
    }

    return n;
  }
}
