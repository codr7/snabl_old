#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/circle.hpp"

namespace snabl::fuses {
  int circle(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) != OpCode::COPY && op_code(op1) != OpCode::MOVE) { goto NEXT; }

      for (PC pc2 = pc1+1; pc2 < m.emit_pc;) {
	Op &op2 = m.ops[pc2];

	if ((op_code(op2) == OpCode::COPY || op_code(op2) == OpCode::MOVE) &&
	    ops::copy_src(op2) == ops::copy_dst(op1) &&
	    ops::copy_dst(op2) == ops::copy_src(op1)) {
	  cout << "Fusing " << fun << " CIRCLE: " << pc2 << endl;
	  if (op_code(op1) == OpCode::MOVE) { ops::COPY(op1, ops::move_dst(op1), ops::move_src(op1)); }
	  ops::NOP(op2);
	  n++;
	} else if (op_writes(op2, ops::copy_src(op1)) || op_writes(op2, ops::copy_dst(op1))) {
	  break;
	}

	pc2 += op_len(op2);
      }

    NEXT:
      pc1 += op_len(op1);
    }

    return n;
  }
}
