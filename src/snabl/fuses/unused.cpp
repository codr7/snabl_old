#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/unused.hpp"

namespace snabl::fuses {
  int unused(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if ((op_code(op1) >= OpCode::LOAD_BOOL && op_code(op1) <= OpCode::LOAD_TYPE) ||
	  op_code(op1) == OpCode::COPY ||
	  op_code(op1) == OpCode::MOVE) {
	Reg dst = ops::load_dst(op1);
	int state_depth = 0;
	bool used = false;
	
	for (PC pc2 = pc1+1; pc2 < m.emit_pc;) {
	  Op &op2 = m.ops[pc2];

	  if (op_reads(op2, dst)) {
	    used = true;
	    break;
	  }

	  if (op_code(op2) == OpCode::STATE_BEG) { state_depth += ops::state_beg_count(op2); }
	  if (op_code(op2) == OpCode::STATE_END) { state_depth--; }
	  if (!state_depth && op_writes(op2, dst)) { break; }
	  pc2 += op_len(op2);
	}

	if (!used) {
	  cout << "Fusing " << fun << " UNUSED: ";
	  op_trace(pc1, cout, m);
	  ops::NOP(op1);
	  n++;
	}
      }
      
      pc1 += op_len(op1);
    }

    return n;
  }
}
