#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/unused.hpp"

namespace snabl::fuses {
  int unused(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (op_code(op1) >= OpCode::LOAD_BOOL && op_code(op1) <= OpCode::LOAD_TYPE) {
	Reg dst = ops::load_dst(op1);
	bool used = false;
	
	for (PC pc2 = pc1+1; pc2 < m.emit_pc;) {
	  Op &op2 = m.ops[pc2];

	  if (op_reads(op2, dst) || op_code(op2) == OpCode::BRANCH) {
	    used = true;
	    break;
	  }

	  if (op_writes(op2, dst)) { break; }

	  if (op_code(op2) == OpCode::GOTO) {
	    pc2 = ops::goto_pc(op2);
	  } else {
	    pc2 += op_len(op2);
	  }
	}

	if (!used) {
	  cout << "Fusing " << fun << " UNUSED: " << pc1 << endl;
	  ops::NOP(op1);
	  n++;
	}
      }
      
      pc1 += op_len(op1);
    }

    return n;
  }
}
