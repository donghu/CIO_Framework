#pragma once
#include "cfield.h"
#include "text.h"

using namespace cio;


class CText:public CField{
  Text _T;
  bool _displayOnly;
  int _curpos;
  bool* _insertmode;
  int _offset;
  int _lcurpos;
  int _loffset;
public:
  CText(int Row, int Col, int Width, int Height, bool* Insertmode,
        bool displayOnly = false, const char* Border=C_BORDER_CHARS);
  CText(const char* Str, int Row, int Col, int Width, int Height,
        bool* Insertmode, bool displayOnly = false,
        const char* Border=C_BORDER_CHARS);
  void draw(int fn = C_FULL_FRAME);
  
  void set(const void *Str);
  void *data()const;
  
  int edit();
  bool  editable()const;
  bool displayOnly();
  void displayOnly(bool val);
};