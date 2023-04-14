#include "token.h"
#include "scanner.h"

void error_msg(int errnum, char character, int linenum) {
  switch(errnum) {
    case -1:
      printf("SCANNER ERROR: Line %d: no token may begin with %c\n", linenum, character);
      break;
    case -2:
      printf("SCANNER ERROR: Line %d: unexpected %c in token t1. Expected _\n", linenum, character);
      break;
    case -3:
      printf("SCANNER ERROR: Line %d: unexpected end of token t1. Expected _\n", linenum);
      break;
    case -4:
      printf("SCANNER ERROR: Line %d: unexpected %c in token t1. Expected a number 1-9\n", linenum, character);
      break;
    case -5:
      printf("SCANNER ERROR: Line %d: unexpected end of token t1. Expected a number 1-9\n", linenum);
      break;
    case -6:
      printf("SCANNER ERROR: Line %d: unexpected %c in token t3. Expected a number 0-9\n", linenum, character);
      break;
    case -7:
      printf("SCANNER ERROR: Line %d: unexpected end of token t3. Expected a number 0-9\n", linenum);
      break;
    case -8:
      printf("SCANNER ERROR: Line %d: unexpected %c in token t3. Expected a number 0-9\n", linenum, character);
      break;
    case -9:
      printf("SCANNER ERROR: Line %d: unexpected end of token t3. Expected a number 0-9\n", linenum);
      break;
    case -10:
      printf("SCANNER ERROR: Line %d: character %c not in alphabet\n", linenum, character);
      break;
  }
}

token_t scanner(std::fstream& file, int linenumber) {
  token_t newtoken;
  
  newtoken.tstr = "";
  newtoken.linenum = linenumber;
  
  char current;
  int state = 0;
  bool comment = false;
  
  std::string tmpstring;
  
  while(true) {
    if (!file.get(current)) {
      current = EOF;
    }
    
    if (comment) {
      if (current == '&') {
        comment = false;
        continue;
      } else {
        continue;
      }
    }
    
    if (current == '\n') {
      newtoken.linenum++;
    }
    
    if (current == EOF || file.eof()) {
      state = index[state][7];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }  
    } else if (current == '&') {
      if (comment) {
        comment = false;
        continue;
      } else {
        comment = true;
        continue;
      }
    } else if (comment) {
      continue;
    } else if (isspace(current)) {
      state = index[state][8];
      if (state >= 1000) {
        // completed token
        file.unget();
        if (current == '\n') {
          newtoken.linenum--;
        }
        break;
      } else if (state >= 0) {
        // next state
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (isalpha(current)) {
      state = index[state][0];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (current == '_') {
      state = index[state][1];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (current == 0) {
      state = index[state][2];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (isdigit(current)) {
      state = index[state][3];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (current == '^' || current == '~' || current == '*' || current == '|' || current == '[' || current == ']' || current == '{' || current == '}') {
      state = index[state][4];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (current == '+' || current == '-') {
      state = index[state][5];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      }
    } else if (current == '.') {
      state = index[state][6];
      if (state >= 1000) {
        // completed token
        file.unget();
        break;
      } else if (state >= 0) {
        // next state
        newtoken.tstr.push_back(current);
        continue;
      } else {
        // scanner error
        error_msg(state, current, newtoken.linenum);
        
        newtoken.tid = err_tk;
        newtoken.tstr = current;
        newtoken.linenum = -1;
        break;
      } 
    } else {
      error_msg(-10, current, newtoken.linenum);
      
      newtoken.tid = err_tk;
      newtoken.tstr = current;
      newtoken.linenum = -1;
      break;
    }
  }
  
  switch(state) {
    case 1001:
      newtoken.tid = T1_tk;
      break;
    case 1002:
      newtoken.tid = T2_tk;
      break;
    case 1003:
      newtoken.tid = T3_tk;
      break;
    case 1004:
      newtoken.tid = EOF_tk;
      break;
  }
  
  return newtoken;
}