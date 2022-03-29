#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/moves.hpp"

namespace snabl::fuses {
  int moves(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::MOVE) {
	int nops = 1;
	
	for (PC pc2 = pc1+1; pc2 < m.emit_pc;) {
	  Op op2 = m.ops[pc2];

	  if (op_code(op2) == OpCode::MOVE &&
	      ops::copy_src(op2) == ops::copy_src(op1)+1 &&
	      ops::copy_dst(op2) == ops::copy_dst(op1)+1) {
	    nops++;
	    pc2++;
	  } else {
	    break;
	  }
	}

	if (nops > 1) {
	  ops::MOVES(op1, ops::copy_dst(op1), ops::copy_src(op1), nops--);
	  
	  for (PC pc2 = pc1+1; nops; nops--, pc2++) {
	    cout << "Fusing " << fun << " MOVES: ";
	    op_trace(pc2, cout, m);
	    ops::NOP(m.ops[pc2]);
	  }
	}
      }

      pc1 += op_len(op1);
    }

    return n;
  }
}
