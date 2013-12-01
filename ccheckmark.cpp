/* Coded by Donghu Zhang (Nov 22)*/
#include "ccheckmark.h"
#include "console.h"
#include <string.h>

namespace cio{

  CCheckMark::CCheckMark(bool Checked,const char* Format, const char* Text, int Row, int Col, int Width, bool IsRadio)
  :CField(Row, Col, Width, 1), _Label(Text,0, 4, Width-4){
    /*Passes the Row, Col, Width and "1" to row, col, width and height arguments of CField and directly initializes _Label with Text, 0, 4, and (Width-4) for Str, Row, Col and Len, arguments of CLabel's Constructor.*/
    _Label.frame(this);//Sets the frame of _Label to its owner
    _flag = Checked;
    _radio = IsRadio;
    strcpy(_format,Format);
    _data = &_flag;
  }//end of constructor
  
  CCheckMark::CCheckMark(const CCheckMark& C)
  :CField(C), _Label(C._Label){//Passes incoming CCheckMark reference ("C") toCField's copy constructor,
    //and directly initializes the _Label with the _Label of C
    _flag = C._flag;
    _radio = C._radio;
    strcpy(_format, C._format);
    _data = &_flag;
  }//end of CCkeckMark
  
  void CCheckMark::draw(int fn){
    _Label.draw(fn);
    console.setPos(absRow(), absCol());
    if (!_flag) {
      _format[1] = ' ';
    }
    console.display(_format, absRow(), absCol());
    console.setPos(absRow(), absCol()+1);//sets the position of the cursor at the checkmark 
  }//end of draw()
  
  int CCheckMark::edit(){
    int key;
		draw();//draw the checkbox
    console >> key;
    switch (key) {
      case SPACE:
        if (_radio) {
          _flag = true;//if _radio is true, it will set the _flag to true
        }else{
          _flag = !_flag;//if _radio is false, it will flip the value of _flag.
        }
        draw();//draw()s the checkmark again
        break;
    }
    return key;
  }//end of edit()
  
  bool CCheckMark::editable()const{
		return true;
  }
  
  /*Casts the incoming flag pointer to an (bool*) and sets the content of _flag to where flag is pointing to.*/
  void CCheckMark::set(const void* flag){
    if((bool*)flag){
      _flag = true;
    }else{
      _flag =false;
    }
  }
  
  bool CCheckMark::checked()const{
		return _flag;
  }
  
  void CCheckMark::checked(bool val){
    _flag = val;
  }
  
  bool CCheckMark::radio(){
		return _radio;
  } // addition for R0.6
  
  void CCheckMark::radio(bool isRadio){
    _radio = isRadio;
  }// addition for R0.6
  
  /*Overload the bool cast to return the value of _flag*/
  CCheckMark::operator bool(){
    return _flag;
  } // addtion for R0.6
  
  /*Overload the char* cast to return the value of _Label.data()*/
  CCheckMark::operator char*(){
    return (char*)_Label.data();
  } // addition for R0.6
  
  /*Overload the operator= and set the _flag to flag*/
  bool CCheckMark::operator=(bool flag){
		return _flag = flag;
  }
  
}//end of namespace cio

