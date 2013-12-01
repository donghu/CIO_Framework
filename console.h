//
//  console.h
//  Project0.1
//
//  Created by Donghu Zhang on 2012-10-8.
//  Copyright (c) 2012 Donghu Zhang. All rights reserved.
//

#ifndef __Project0_1__console__
#define __Project0_1__console__

#include "bconsole.h"

namespace cio {
  class Console: public bio::BConsole{
    unsigned int& tabsize;
  public:
    Console();
    void display(const char *str, int row, int col, int fieldLen=0);
    int edit(char *str, int row, int col, int fieldLength, int maxSgrLength,
             bool* insertMode, int*strOffset, int* curPosition,
             bool IsTextEditor = false, bool ReadOnly = false);
  };//end of class Console
  
  Console& operator>>(Console&, int&);
  Console& operator<<(Console&, char);
  Console& operator<<(Console&, const char*);
  
  extern unsigned int CIO_TABSIZE;
  extern Console console;
}//end of namespace cio

#endif /* defined(__Project0_1__console__) */
