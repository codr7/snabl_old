#include "snabl/m.hpp"
#include "snabl/op.hpp"
#include "snabl/fuses/goto_ret.hpp"

namespace snabl::fuses {
  int goto_ret(Fun *fun, PC start_pc, M &m) {
    int n = 0;
    bool done = false;
    
    for (PC pc = start_pc; !done && pc < m.emit_pc;) {
      Op op = m.ops[pc];
      
      switch (op_code(op)) {
      case OpCode::GOTO: {
	vector<PC> ret_pcs;
	ret_pcs.push_back(pc);
	Op *gop = &m.ops[ops::goto_pc(op)];

	NEXT: {
	  switch (op_code(*gop)) {
	  case OpCode::GOTO:
	    ret_pcs.push_back(gop - &m.ops[0]);
	    gop = &m.ops[ops::goto_pc(*gop)];
	    goto NEXT;
	  case OpCode::RET:
	    for (PC rpc: ret_pcs) {
	      cout << "Fusing " << fun << " GOTO RET: " << rpc << endl;
	      ops::RET(m.ops[rpc]);
	    }
	  
	    ret_pcs.clear();
	    break;
	  case OpCode::NOP:
	    ret_pcs.push_back(gop - &m.ops[0]);
	    gop++;
	    goto NEXT;
	  default:
	    break;
	  }
	
	  break;
	}
      }
	
      case OpCode::RET:
	done = true;
	break;
      default:
	break;
      }

      pc += op_len(op);
    }

    return n;
  }
}
