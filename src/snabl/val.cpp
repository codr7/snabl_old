#include "snabl/val.hpp"

namespace snabl {
  ostream &operator <<(ostream &out, Val val) {
    val.type.imp->methods.dump(val, out);
    return out;
  }
}
