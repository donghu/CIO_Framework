#include "cfield.h"
#include "clabel.h"
namespace cio{
  class CCheckMark : public CField{
    bool _flag;
    bool _radio;
    char _format[4];
    CLabel _Label;
  public:
    CCheckMark(bool Checked,const char* Format, const char* Text, int Row, int Col, int Width, bool IsRadio = false);
    CCheckMark(const CCheckMark& C);
    void draw(int fn = C_NO_FRAME) ;
    int edit();
    bool editable()const;
    void set(const void* flag);
    bool checked()const;
    void checked(bool val);
    bool radio(); // addition for R0.6
    void radio(bool isRadio); // addition for R0.6
    operator bool(); // addtion for R0.6
    operator char*(); // addition for R0.6
    bool operator=(bool flag);
  };
}
