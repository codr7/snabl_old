#ifndef SNABL_LIB_HPP
#define SNABL_LIB_HPP

namespace snabl {
  struct M;
  
  struct Lib {
    M &m;
    
    Lib(M &m);
  };
}

#endif
