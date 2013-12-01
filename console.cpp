//
//
//  console.cpp
//  Project0.1
//
//  Created by Donghu Zhang on 2012-10-8.
//  Copyright (c) 2012 Donghu Zhang. All rights reserved.
//

#include "console.h"

namespace cio {
  using namespace bio;
  unsigned int CIO_TABSIZE = 4;
  Console console;
  
  
  Console::Console(): tabsize(CIO_TABSIZE){
    
  }
  /*************** display function *****************/
  /*This method outputs the C-style, null-terminated string pointed to
   by str starting at row row and column col of the screen in a field of fieldLen characters.*/
  void cio::Console::display(const char *str, int row, int col, int fieldLen){
    console.setPos(row, col);
    if (fieldLen) {
      int i;
      for (i=0; i<fieldLen && str[i]; i++) {
        this->putChar(str[i]);
      }
      for ( ; i <fieldLen; i++) {
        this->putChar(' ');
      }
    }else{
      (*this)<<str;
    }//end of if else
  }/* end of display function */
  
  /*************** operator>> *****************/
  /*Get a key from keyboard using cn and store it in ch;*/
  Console& operator>>(Console& cn, int& ch){
    ch = cn.getKey();
    return cn;
  }//end of operator>>
  
  /*************** operator<< *****************/
  /*Print the character ch on the screen, where the cursor is located, using cn and return the cn reference;*/
  Console& operator<<(Console& cn, char ch){
    cn.putChar(ch);
    return cn;
  }//end of operator<<
  
  /*************** operator<< *****************/
  /*print the string str on the screen where the cursor is using cn and return the cn reference;*/
  Console& operator<<(Console& cn, const char* str){
    for (int i=0; str[i]; i++) {
      cn.putChar(str[i]);
    }
    return cn;
  }/* end of operator<< */
  
  /*************** edit function *****************/
  int  Console::edit(char* str, int row, int col,
                     int fieldLength, int maxStrLength,
                     bool* insertMode, int* strOffset, int* curPosition,
                     bool IsTextEditor, bool ReadOnly){
    bool done = false;
    int key = 0;
    int locOff=0;
    int locCur = 0;
    int i = 0;
    int stringLength = strlen(str);
    
    //save the original value for *strOffset and *curPosition..
    //and save the original value before you change *cur and *off, or you will fial test417
    if (strOffset) {
      locOff = *strOffset;
    }else{
      strOffset = &locOff;
    }
    if (curPosition) {
      locCur=*curPosition;
    }else{
      curPosition = &locCur;
    }
    
    //initialize *strOffset and *curPosition
    if (stringLength < *strOffset) {
      *strOffset = stringLength;
    }
    if(*curPosition > fieldLength){
      *curPosition = fieldLength -1;
    }else if(*curPosition >stringLength) {
      *curPosition = stringLength;    }
    
    char* temStr = new char[stringLength+1];
    strcpy(temStr, str);//copy str to temStr
    
    //start of while loop
    while (!done) {
      //display the string and set position
      display(str + *strOffset, row, col, fieldLength);
      setPos(row, col + *curPosition);
      
      switch (key = getKey()) {
        case LEFT:
          if (*curPosition >0) {
            (*curPosition)--;
          }else if(*strOffset>0){
            (*strOffset)--;
          }
          break;
          
        case RIGHT:
          if((*strOffset + *curPosition) < stringLength){
            if(*curPosition < fieldLength-1) {
              (*curPosition)++;
            }
            else if(*strOffset >= 0) (*strOffset)++;
          }
          break;
          
        case HOME:
          *curPosition = 0;
          *strOffset = 0;
          break;
          
        case END:
          if (stringLength - *strOffset >= fieldLength) {
            *curPosition = fieldLength -1;
            *strOffset = stringLength - fieldLength +1;
          }else{//If the last character is at the edge of the screen
            *curPosition = stringLength - *strOffset;
          }
          break;
          
        case INSERT:
          *insertMode = !(*insertMode);
          break;
          
        case DEL:
          if (!ReadOnly) {
            for (int i = *strOffset +*curPosition; str[i]; i++) {
              str[i] = str[i+1];
            }
          }
          break;
          
        case BACKSPACE:
          if (!ReadOnly) {
            if (*curPosition >0) {
              for (i = *curPosition + *strOffset; str[i-1]; i++) {
                str[i-1]= str[i];
              }
              (*curPosition)--;
            }
            else if(*strOffset >0){
              for (i = *curPosition + *strOffset; str[i-1]; i++) {
                str[i-1]= str[i];
              }
              (*strOffset)--;
            }//end of else if(*strOffset >0)
          }
          break;
          
        case F(1):
        case F(2):
        case F(3):
        case F(4):
        case F(5):
        case F(6):
        case F(7):
        case F(8):
        case F(9):
        case F(10):
        case F(11):
        case F(12):
        case ENTER:
        case UP:
        case DOWN:
        case PGDN:
        case PGUP:
          done = true;
          break;
          
        case TAB:
          if (IsTextEditor) {
            //step 1: deal with the string
            //shift the string to right
            if (stringLength <= maxStrLength) {
              for (i = stringLength; i > *curPosition + *strOffset; i--) {
                str[i+CIO_TABSIZE] = str[i];
              }
              //fill up the string with space('')
              for (i = CIO_TABSIZE; i > 0; i--) {
                str[*strOffset + *curPosition + i] = ' ';
              }
              
              //step 2: deal with the *curPos and *strOffset
              if (*curPosition < fieldLength -1) {
                *curPosition = *curPosition + CIO_TABSIZE;
                if (*curPosition >= fieldLength -1) {
                  *curPosition = fieldLength -1;
                  *strOffset = *strOffset + CIO_TABSIZE;
                }
              }else{
                *curPosition = fieldLength -1;
                *strOffset = *strOffset + CIO_TABSIZE;
              }
            }
            else{
              done = true;
            }//end of if (stringLength <= maxStrLength)
          }
          else {
            done = true;
          }//end of if (IsTextEditor)
          break;
          
        case ESCAPE:
          if (!IsTextEditor) {
            *strOffset = locOff;
            *curPosition = locCur;
            strcpy(str, temStr);
            display(str + *strOffset, row, col, fieldLength);//display the str in the field
            setPos(row, col + *curPosition);
          }
          done = true;
          break;
          
        default:
          if (!ReadOnly) {
            if (key >=32 && key <=126) {
              int stringLength2 = strlen(str);//length of string holder in default
              if (*insertMode) {
                //edit in insert mode
                if (stringLength2 < maxStrLength  && !ReadOnly) {
                  for (i = stringLength2; i >= (*curPosition + *strOffset); i--) {
                    str[i+1] = str[i];
                    str[i] = key;
                  }
                  if (*curPosition < fieldLength -1) {
                    (*curPosition)++;
                  }else if(*curPosition == fieldLength -1){
                    (*strOffset)++;
                  }
                }//end of insert mode
                //edit in overStrike mode
              }else{
                if (stringLength2 <= maxStrLength && !ReadOnly) {
                  str[*strOffset + *curPosition] = key;
                  if (*curPosition < fieldLength -1) {
                    (*curPosition)++;
                  }else if(*curPosition == fieldLength -1){
                    (*strOffset)++;
                  }
                }//end of overstrike mode
              }//end of *insertMode ifelse
            }//end of if (key >=32 && key <=126)
          }//end of if(!ReadOnly)
          break;
      }//end of switch
    }//end of while
    
    /* assignemnt step 2!! */
    if (IsTextEditor) {
      if(locOff != *strOffset){
        done = true;
        key = 0;
      }//end of if(lastStroffset != *strOffset)
    }//end of if (IsTextEditor)
    if (temStr) {
      delete [] temStr;
    }
    return key;
  }/* end of edit fuction */
}//end of cio
