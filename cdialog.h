#pragma once
#ifndef __cdialog_SHL__
#define __cdialog_SHL__
#include "cuigh.h"
#include "cframe.h"
namespace cio{
  class CField;
  class CDialog: public CFrame{
    private:
    int _fnum;
    int _curidx;
    CField* _fld[C_MAX_NO_FIELDS];
    bool _dyn[C_MAX_NO_FIELDS];
    bool _editable;
    public:
    CDialog(CFrame *Container = (CFrame*)0,
             int Row = -1, int Col = -1, 
             int Width = -1, int Height = -1, 
             bool Borderd = false,
             const char* Border=C_BORDER_CHARS);
    virtual ~CDialog();
    void draw(int fn = C_FULL_FRAME);
    int edit(int fn = C_FULL_FRAME);
 
    int add(CField* field, bool dynamic = true);
    int add(CField& field, bool dynamic = false);
    CDialog& operator<<(CField* field);
    CDialog& operator<<(CField& field);
 
    bool editable();
    int fieldNum()const;
    int curIndex()const;
 
    CField& operator[](unsigned int index);
    CField& curField();
  };
}
#endif