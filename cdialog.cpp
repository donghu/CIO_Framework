/*Coded by Donghu Zhang(15 Nov, 2012)*/
#include "cfield.h"
#include "cdialog.h"
#include "console.h"

namespace cio{
  /* 
   Constructor
   Organizes CField objects on the screen, displays them and then
   lets the user edit them one by one. 
	 */
	CDialog::CDialog(CFrame *Container, int Row, int Col, int Width, int Height, bool Borderd, const char* Border)
	:CFrame( Row, Col, Width, Height, Borderd, Border, Container){  //nov20
    
    _fnum = 0;
    _curidx = 0;
    _editable = false;
    /* set field pointers to NULL and set dynamic flags to false*/
    int i =0;
    for (; i < C_MAX_NO_FIELDS; i++) {
      _fld[i] = 0;
      _dyn[i] = false;
    }//end of for loop
	}//end of constructor
	
  /*
   Destructor
   The destructor will loop through all the field pointers and if the 
   corresponding dynamic flag is true then it will delete the field pointed 
   to by the field pointer.
   */
	CDialog::~CDialog() {
    int i =0;
    for (; i < _fnum; i++) {
      if(_dyn[i]){
        _fld[i] = (CField*)0;//Nov 23: fixed key "esc" crashed
        delete _fld[i];
      }
    }//end of for loop
	}//end of destructor
	
  /*
   If fn is C_FULL_FRAME, it will call its parent draw. Then It will draw all the Fields in the Dialog.
   If fn is not C_FULL_FRAME, then it will just draw all the Fields in the Dialog.
   If fn is a non-zero positive value, then it will only draw Field number fn in the dialog. (First added Field is field number one.)
   */
	void CDialog::draw(int fn ){
    if (fn == C_FULL_FRAME) {
      CFrame::draw();//call parent draw()
      for (int i=0; i<_fnum; i++) {//draw all the fields
				_fld[i]->draw(fn);
      }
    }//fn is C_FULL_FRAME
    //if (fn != C_FULL_FRAME) {
    if (fn ==0) {
      for (int i =0; i <_fnum; i++) {
        _fld[i]->draw();
      }
    }//fn is NOT C_FULL_FRAME
    if (fn > 0) {
      _fld[fn-1]->draw(fn);
    }
  }
	
	int CDialog::edit(int fn ){// done
		bool done = false;
    int key = 0;
    
    if(!editable()){
      draw(fn);//Nov 23: fixed wrong cursor
      console >> key;
      done = true;
    }// end of not editable
    else if(fn <=0){
      draw(fn);
			while (!_fld[_curidx]->editable()) {//go to the first editable field
        _curidx = (_curidx < (_fnum-1)) ? _curidx+1 : 0;//next index
      }
    }//fn <=0
    else{
      _curidx = fn -1;
    }//fn >0
    
    while (!done) {
			int newFnum = _fnum;//_fnum holder
			while(!key && key!=UP && key != DOWN && newFnum--){
        key=_fld[_curidx]->edit();
        if(!key) _curidx = (_curidx<_fnum-1) ? _curidx+1 : 0;
      }//
      switch (key) {
        case ENTER:
        case TAB:
        case DOWN:
          do{
            _curidx = (_curidx < (_fnum-1)) ? _curidx+1 : 0;//next index
            key = _fld[_curidx]->edit();
          }while (!key);
          break;
          
        case UP:
          do {
            _curidx = (_curidx > 0) ? _curidx-1 : _fnum -1;//previous index
            key = _fld[_curidx]->edit();
          } while (!key);
          break;
          
        default:
          done = true;
          break;
      }//end of switch
    }//end of while(!done)
    return key;
	}

	
	int CDialog::add(CField* field, bool dynamic){
		if (_fnum < C_MAX_NO_FIELDS) {
			_fld[_fnum] = field;
      _fld[_fnum] -> container(this);
      if(field->editable()) {//Nov 23
        _editable = true;
      }
      _dyn[_fnum] = dynamic;
    }//end of if(_fnum < C_MAX_NO_FIELDS) 
    return _fnum++;//Nov 23
	}
		
	int CDialog::add(CField& field, bool dynamic){
		return add(&field, dynamic);
	}
		
	CDialog& CDialog::operator<<(CField* field) {
    add(field);
		return *this;	
	}
	
	CDialog& CDialog::operator<<(CField& field) {
    add(&field);
		return *this;	
	}
	
	bool CDialog::editable(){
		return _editable; 
	}
	
	int CDialog::fieldNum()const{
		return _fnum;
	}
	
	int CDialog::curIndex()const{
		return _curidx;
	}
	
	CField& CDialog::operator[](unsigned int index){
		 return *_fld[index];
	}
	
	CField& CDialog::curField(){
		 return *_fld[_curidx];
	}

}//end of namespace cio
