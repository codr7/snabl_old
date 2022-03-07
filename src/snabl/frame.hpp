#ifndef SNABL_FRAME_HPP
#define SNABL_FRAME_HPP

#include <array>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;

  struct Frame {
    Frame *outer;
    int ref_count;

    Frame(Frame *outer);
  };
}

#endif
