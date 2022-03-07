#include "snabl/m.hpp"

#define DISPATCH(next_pc)						\
  goto *dispatch[static_cast<int>(op_code(op = ops[(pc = (next_pc))]))];

namespace snabl {
  using namespace std;
  
  optional<Error> M::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&GOTO, &&NOP, &&RET,
      /* STOP */
      &&STOP};

    PC pc = -1;
    Op op = 0;
    
    DISPATCH(start_pc);
    
  NOP: {
      DISPATCH(pc+1);
    }
    
  GOTO: {
      DISPATCH(ops::goto_pc(op));
    }

  RET: {
      Frame *f = end_frame();
      end_env();
      DISPATCH(f->ret_pc);
    }
    
    /* STOP */
    
  STOP: {}

    return nullopt;
  }
}

