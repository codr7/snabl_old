#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/state.hpp"

namespace snabl::fuses {
  int state(Fun *fun, M &m) {
    int n = 0;

    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      bool done = false;
      Op &op1 = m.ops[pc1];

      if (op_code(op1) == OpCode::STATE_BEG) {
	vector<PC> pcs;

	for (PC pc2 = pc1+1; !done && pc2 < m.emit_pc;) {
	  Op op2 = m.ops[pc2];
	  
	  switch (op_code(op2)) {
	  case OpCode::GOTO:
	    pc2 = ops::goto_pc(op2);
	    break;
	  case OpCode::NOP:
	    pc2++;
	    break;
	  case OpCode::STATE_BEG:
	    pcs.push_back(pc2);
	    pc2++;
	    break;
	  default:
	    done = true;
	  }
	}

	for (PC pc: pcs) {
	  cout << "Fusing " << fun << " STATE: " << pc << endl;
	  ops::NOP(m.ops[pc]);
	  n++;
	}
	
	ops::STATE_BEG(op1, ops::state_beg_count(op1) + pcs.size(), ops::state_beg_reg_count(op1));
      }

      pc1 += op_len(op1);
    }

    return n;
  }
}
