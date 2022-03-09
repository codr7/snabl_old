#ifndef SNABL_READER_HPP
#define SNABL_READER_HPP

#include "snabl/form.hpp"

namespace snabl {
  using ReadResult = pair<optional<Form>, optional<Error>>;
  using Reader = function<ReadResult (istream &in, Pos &pos, M &m)>;
    
  ReadResult read_form(istream &in, Pos &pos, M &m);
  ReadResult read_call(istream &in, Pos &pos, M &m);
  ReadResult read_id(istream &in, Pos &pos, M &m);
  ReadResult read_int(istream &in, Pos &pos, M &m);
  ReadResult read_ws(istream &in, Pos &pos, M &m);
}

#endif
