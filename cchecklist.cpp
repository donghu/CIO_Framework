#include "cchecklist.h"
#include "string.h"
#include "console.h"

namespace cio{
  
  CCheckList::CCheckList(const char* Format, int Row, int Col, int Width,bool radio, bool Bordered,const char* Border)
  :CField(Row, Col, Width, 1, (void*)0, Bordered, Border){
    _data = & _flags;
    strcpy(_format, Format);
    _radio = radio;
    _cnt =0;
    _cur =0;
    _flags =0;
    
  }
  
  CCheckList::~CCheckList(void){
    int i;
    for (i=0; i<_cnt; i++) {
      delete _checkmarks[i];
    }
  }
  
  CCheckList& CCheckList::add(const char* Text, bool selected){
    long length = strlen(Text);
    if (_cnt <32) {
      CCheckMark* cm = new CCheckMark(selected, _format, Text, _cnt+1, 1, (int)length+4);
      cm->frame(this);
      if(cm->width() > width()) {
        width(cm->width() + 3);
      }
			this->height(_cnt +3);
			_checkmarks[_cnt++]=cm;
			if(selected) {
        _flags = _flags | (1<<(_cnt));
      }
    }
    return *this;
  }
  
  CCheckList& CCheckList::operator<<(const char* Text){
		return add(Text);
  }
  
  void CCheckList::draw(int fn){
    bool here;
    CFrame :: draw();//draw the frame
    int i;
    for (i=0; i < _cnt; i++) {
      _checkmarks[i] -> draw(); //draw all the _ckeckmark
      if (_checkmarks[i] -> checked() && !here) {//cursor is under the first check mark
        console.setPos(absRow(), absCol()+1);
        here = true;
      }//end of if
    }//end for
  }//end method
  
  
  /*
   Draws the CCheckList then starts editing the _checkmarks form _cur and according to the return key of CCheckMark::edit():
   If Down or Right key is hit it goes to the next _checkmark, if _cur is the last one, then it exits the edit, returning the last key entered.
   UP and Left key works in opposite direction of Down and Right, if _cur is already zero, then it exits the edit, returning the last key entered.
   If Space is hit, then if _radio is true, it will uncheck all the _checkmarks other than the _current one.
   */
  int CCheckList::edit(){
		int key, i;
		bool done = false;
		draw();
		while(!done){
			key = _checkmarks[_cur]->edit();
			switch (key){
				case DOWN:
				case RIGHT:
					if (_cur == (_cnt - 1))
						done = true;
					else {
            ++_cur;
          }//end if else
					break;
          
          //opposite direction of Down and Right
				case UP:
				case LEFT:
					if (_cur == 0)
						done = true;
					else --_cur;
					break;
          
				case SPACE:
					if (_radio){
						_checkmarks[_cur]->checked(true);
						for (i = 0; i < _cnt; i++){
							if (i != _cur){
								_checkmarks[i]->checked(false);
							}
						}
					}
					_flags = 0 | (1<<(_cur));
					draw();
					break;
          
				default:
					done = true;
					break;
			}
		}
		return key;
  }
  
  /*
   returns the bit pattern held in _flags.
   make sure _flags are updated to the status of the _checkmarks
   */
  void* CCheckList::data(){
    return (void*)&_flags;
  }
  
  
  /*
   sets the _flags and updates the _checkmarks to the bitpattern of _flags
   */
  void CCheckList::set(const void* data){
    _flags = (unsigned int)data;
    for( int i=0; i<_cnt; i++) {
      if (_flags & (1<<i-1)) {
        _checkmarks[i] -> checked(true);
      }else{
        _checkmarks[i] -> checked(false);
      }
    }
  }
  
  CCheckMark& CCheckList::operator[](unsigned int index){
		//returns the CCheckMark corresponding the index value.
    return *_checkmarks[index];
  }
  
  bool CCheckList::editable()const{
		return true;
  }
  
  bool CCheckList::radio()const{
		return _radio;
  }
  
  void CCheckList::radio(bool val){
    //sets the _radio and updates all _checkMarks radio value.
    _radio = val;
    for (int i=0; i < _cnt; i++) {
      _checkmarks[i] -> radio(_radio);
    }
  }
  
  unsigned int CCheckList::flags()const{
		return _flags;
  }
  
  void CCheckList::flags(unsigned int theFlags){
    _flags = theFlags;
  }
  
  /*
   returns the index of the first CCheckMark that is selected, and -1 if nothing is selected.
   */
  int CCheckList::selectedIndex()const{
		int i=0;
		while(!_checkmarks[i++]->checked()&&i<32){
      if(i==32) {
        i=0;
      }
    }
		return i-1;
  }
  
  
  /*
   sets the selectedindex. (only un-selects the rest if object is in radio mode)
   if index is less than zero, then it will un-selects all
   */
  void CCheckList::selectedIndex(int index){ 
    _checkmarks[index]->checked(true);
		if(radio()) {
      _flags=0;
    }
		_flags=_flags|(1<<(31-index));
  }
  
  unsigned int CCheckList::length(){
		return _cnt;
  }
}