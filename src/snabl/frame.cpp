#include "snabl/fun.hpp"
#include "snabl/frame.hpp"

namespace snabl {  
  Frame::Frame(Frame *outer): outer(outer), ref_count(1) {
    if (outer) {
      outer->ref_count++;
    }
  }
}
