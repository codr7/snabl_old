#include "snabl/pos.hpp"

namespace snabl {
  const Pos Pos::INVALID;

  ostream &operator <<(ostream &out, Pos pos) {
    out << "'" << pos.source << "' at line " << pos.line << ", column " << pos.column;
    return out;
  }
}
