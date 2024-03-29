#pragma once
#include "cfield.h"
#include "ccheckmark.h"
namespace cio{
 
  class CCheckList : public CField{
    CCheckMark* _checkmarks[32];
    bool _radio;
    char _format[4];
    unsigned int _cnt;
    unsigned int _flags;
    unsigned int _cur;
  public:
    CCheckList(const char* Format, int Row, int Col, int Width,bool radio, bool Bordered = true,const char* Border=C_BORDER_CHARS);
    ~CCheckList(void);
    CCheckList& add(const char* Text, bool selected = false);
    CCheckList& operator<<(const char* Text);
    void draw(int fn = C_FULL_FRAME);
    int edit();
    void* data();
    void set(const void* data);
    CCheckMark& operator[](unsigned int index);
    bool editable()const;
    bool radio()const;
    void radio(bool val);
    unsigned int flags()const;
    void flags(unsigned int theFlags);
    int selectedIndex()const;
    void selectedIndex(int index);
    unsigned int length();
  };
}
