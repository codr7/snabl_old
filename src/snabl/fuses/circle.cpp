#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/circle.hpp"

namespace snabl::fuses {
  static bool is_cpmv(Op op) {
    switch (op_code(op)) {
    case OpCode::COPY:
    case OpCode::COPYS:
    case OpCode::MOVE:
    case OpCode::MOVES:
      return true;
    default:
      break;
    }
    
    return false;
  }
  
  int circle(Fun *fun, M &m) {
    int n = 0;
    
    for (PC pc1 = fun->start_pc; pc1 < m.emit_pc;) {
      Op &op1 = m.ops[pc1];

      if (is_cpmv(op1)) {
	for (PC pc2 = pc1+1; pc2 < m.emit_pc;) {
	  Op &op2 = m.ops[pc2];
	  
	  if (is_cpmv(op2) &&
	      ops::copy_src(op2) == ops::copy_dst(op1) &&
	      ops::copy_dst(op2) == ops::copy_src(op1)) {
	    cout << "Fusing " << fun << " CIRCLE: ";
	    op_trace(pc2, cout, m);

	    switch (op_code(op1)) {
	    case OpCode::MOVE:
	      ops::COPY(op1, ops::move_dst(op1), ops::move_src(op1));
	      break;
	    case OpCode::MOVES:
	      ops::COPYS(op1, ops::move_dst(op1), ops::move_src(op1), ops::moves_len(op1));
	      break;
	    default:
	      break;
	    }
	      
	    ops::NOP(op2);
	    n++;
	  } else if (op_writes(op2, ops::copy_src(op1)) || op_writes(op2, ops::copy_dst(op1))) {
	    break;
	  }
	  
	  pc2 += op_len(op2);
	}
      }
      
      pc1 += op_len(op1);
    }

    return n;
  }
}
