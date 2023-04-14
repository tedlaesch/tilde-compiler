#include <stack>
#include <vector>

#include "token.h"
#include "scanner.h"
#include "parser.h"

token_t sc;
token_t scnext;
int lastline;
int lastlinenext;

void parserErr(int errnum) {
  switch (errnum) {
    case -1:
      // Non-terminal A
      printf("\033[1;31mPARSER ERROR: Line %d: Initialization instruction: Expected ^ or *, [, ], a variable, or end of file. Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -2:
      // Non-terminal C
      printf("\033[1;31mPARSER ERROR: Line %d: If block: Expected * Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -3:
      // Non-terminal D
      printf("\033[1;31mPARSER ERROR: Line %d: Read instruction: Expected [ Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -4:
      // Non-terminal E
      printf("\033[1;31mPARSER ERROR: Line %d: Print instruction: Expected ] Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -5:
      // Non-terminal F
      printf("\033[1;31mPARSER ERROR: Line %d: Assignment instruction: Expected | Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -6:
      // Non-terminal G
      printf("\033[1;31mPARSER ERROR: Line %d: Assignment instruction: Expected a variable or a number. Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -7:
      // Non-terminal H
      printf("\033[1;31mPARSER ERROR: Line %d: Math instruction: Expected { or } Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -8:
      // Non-terminal J
      printf("\033[1;31mPARSER ERROR: Line %d: Expected *, [, ], a variable, or ~, EOF_tk Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -9:
      // Non-terminal K
      printf("\033[1;31mPARSER ERROR: Line %d: Expected a variable or a number. Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -10:
      // Non-terminal L
      printf("\033[1;31mPARSER ERROR: Line %d: Expected an else block starting with [, ], or a variable, or another instruction starting with ~ or end of file. Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -11:
      // Non-terminal G-prime
      printf("\033[1;31mPARSER ERROR: Line %d: Math instruction: Expected a variable, a number, or ~ Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -12:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected ^ Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -13:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected ~ Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -14:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected * Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -15:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected | Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -16:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected [ Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -17:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected ] Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -18:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected { Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -19:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected } Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
    case -20:
      printf("\033[1;31mPARSER ERROR: Line %d: Expected end of file, Got %s\n\033[0m", lastline, sc.tstr.c_str());
      break;
  }
  
  exit(EXIT_FAILURE);
}

node_t parser(std::fstream& file) {
  lastline = 1;
  lastlinenext = 1;
  sc = scanner(file, lastlinenext);
  scnext = scanner(file, lastlinenext);
  
  node_t root = ntS(file);
  if (sc.tid != EOF_tk) {
    // Error: Extra characters after parsed program
    parserErr(-20);
  }
  std::cout << "\033[1;32m" << "Parsed Successfully!" << "\033[0m" << "\n";
  //printPreorder(root);
  
  return root;
}

// Non-terminals

node_t ntS(std::fstream& file) {
  node_t nodeS;
  nodeS.nodeid = Snt;
  nodeS.nodestr = "S";
  nodeS.linenum = lastline;
  
  node_t nodeA = ntA(file);
  nodeS.children.push_back(nodeA);
  
  node_t nodeJ = ntJ(file);
  nodeS.children.push_back(nodeJ);
  
  return nodeS;
}

node_t ntA(std::fstream& file) {
  node_t nodeA;
  nodeA.nodeid = Ant;
  nodeA.nodestr = "A";
  nodeA.linenum = lastline;
  
  if (sc.tstr[0] == '^') {
    node_t nodeCaret = t2caret(file);
    nodeA.children.push_back(nodeCaret);
    
    node_t nodeT1 = t1con(file);
    nodeA.children.push_back(nodeT1);
    
    node_t nodeA2 = ntA(file);
    nodeA.children.push_back(nodeA2);
    // First production
  } else if (sc.tid == EOF_tk || sc.tid == T1_tk || sc.tstr[0] == '*' || sc.tstr[0] == '[' || sc.tstr[0] == ']') {
    // Empty set
  } else {
    // error
    parserErr(-1);
  }
  
  return nodeA;
}

node_t ntC(std::fstream& file) {
  node_t nodeC;
  nodeC.nodeid = Cnt;
  nodeC.nodestr = "C";
  nodeC.linenum = lastline;
  
  if (sc.tstr[0] == '*') {
    node_t nodeAst = t2ast(file);
    nodeC.children.push_back(nodeAst);
    
    node_t nodeK = ntK(file);
    nodeC.children.push_back(nodeK);
  } else {
    // error
    parserErr(-2);
  }
  
  return nodeC;
}

node_t ntD(std::fstream& file) {
  node_t nodeD;
  nodeD.nodeid = Dnt;
  nodeD.nodestr = "D";
  nodeD.linenum = lastline;
  
  if (sc.tstr[0] == '[') {
    node_t nodeLeftBracket = t2lbracket(file);
    nodeD.children.push_back(nodeLeftBracket);
    
    node_t nodeT1 = t1con(file);
    nodeD.children.push_back(nodeT1);
  } else {
    // error
    parserErr(-3);
  }
  
  return nodeD;
}

node_t ntE(std::fstream& file) {
  node_t nodeE;
  nodeE.nodeid = Ent;
  nodeE.nodestr = "E";
  nodeE.linenum = lastline;
  
  if (sc.tstr[0] == ']') {
    node_t nodeRightBracket = t2rbracket(file);
    nodeE.children.push_back(nodeRightBracket);
    
    node_t nodeK = ntK(file);
    nodeE.children.push_back(nodeK);
    
    node_t nodeTilde = t2tilde(file);
    nodeE.children.push_back(nodeTilde);
  } else {
    // error
    parserErr(-4);
  }
  
  return nodeE;
}

node_t ntF(std::fstream& file) {
  node_t nodeF;
  nodeF.nodeid = Fnt;
  nodeF.nodestr = "F";
  nodeF.linenum = lastline;
  
  if (sc.tstr[0] == '|') {
    node_t nodePipe = t2pipe(file);
    nodeF.children.push_back(nodePipe);
    
    node_t nodeG = ntG(file);
    nodeF.children.push_back(nodeG);
    
    node_t nodeTilde = t2tilde(file);
    nodeF.children.push_back(nodeTilde);
  } else {
    // error
    parserErr(-5);
  }
  
  return nodeF;
}

node_t ntG(std::fstream& file) {
  node_t nodeG;
  nodeG.nodeid = Gnt;
  nodeG.nodestr = "G";
  nodeG.linenum = lastline;
  
  if (sc.tid == T1_tk || sc.tid == T3_tk) {
    node_t nodeK = ntK(file);
    nodeG.children.push_back(nodeK);
    
    node_t nodeGp = ntGp(file);
    nodeG.children.push_back(nodeGp);
  } else {
    // error
    parserErr(-6);
  }
  
  return nodeG;
}

node_t ntH(std::fstream& file) {
  node_t nodeH;
  nodeH.nodeid = Hnt;
  nodeH.nodestr = "H";
  nodeH.linenum = lastline;
  
  if (sc.tstr[0] == '{') {
    node_t nodeLeftCurlyBrace = t2lcbrace(file);
    nodeH.children.push_back(nodeLeftCurlyBrace);
  } else if (sc.tstr[0] == '}') {
    node_t nodeRightCurlyBrace = t2rcbrace(file);
    nodeH.children.push_back(nodeRightCurlyBrace);
  } else {
    // Error
    parserErr(-7);
  }
  
  return nodeH;
}

node_t ntJ(std::fstream& file) {
  node_t nodeJ;
  nodeJ.nodeid = Jnt;
  nodeJ.nodestr = "J";
  nodeJ.linenum = lastline;
  
  if (sc.tstr[0] == '*') {
    node_t nodeC = ntC(file);
    nodeJ.children.push_back(nodeC);
    
    node_t nodeTilde = t2tilde(file);
    nodeJ.children.push_back(nodeTilde);
    
    node_t nodeJ2 = ntJ(file);
    nodeJ.children.push_back(nodeJ2);
    
    node_t nodeTilde2 = t2tilde(file);
    nodeJ.children.push_back(nodeTilde2);
  
    node_t nodeL = ntL(file);
    nodeJ.children.push_back(nodeL);
  } else if (sc.tstr[0] == '[') {
    node_t nodeD = ntD(file);
    nodeJ.children.push_back(nodeD);
    
    node_t nodeTilde = t2tilde(file);
    nodeJ.children.push_back(nodeTilde);
    
    node_t nodeJ2 = ntJ(file);
    nodeJ.children.push_back(nodeJ2);
  } else if (sc.tstr[0] == ']') {
    node_t nodeE = ntE(file);
    nodeJ.children.push_back(nodeE);
    
    node_t nodeTilde = t2tilde(file);
    nodeJ.children.push_back(nodeTilde);
    
    node_t nodeJ2 = ntJ(file);
    nodeJ.children.push_back(nodeJ2);
  } else if (sc.tid == T1_tk) {
    node_t nodeT1 = t1con(file);
    nodeJ.children.push_back(nodeT1);
    
    node_t nodeF = ntF(file);
    nodeJ.children.push_back(nodeF);
    
    node_t nodeTilde = t2tilde(file);
    nodeJ.children.push_back(nodeTilde);
    
    node_t nodeJ2 = ntJ(file);
    nodeJ.children.push_back(nodeJ2);
  } else if (sc.tid == EOF_tk || sc.tstr[0] == '~') {
    // Empty set
  } else {
    // Error
    parserErr(-8);
  }
  
  return nodeJ;
}

node_t ntK(std::fstream& file) {
  node_t nodeK;
  nodeK.nodeid = Knt;
  nodeK.nodestr = "K";
  nodeK.linenum = lastline;
  
  if (sc.tid == T1_tk) {
    node_t nodeT1 = t1con(file);
    nodeK.children.push_back(nodeT1);
  } else if (sc.tid == T3_tk) {
    node_t nodeT3 = t3con(file);
    nodeK.children.push_back(nodeT3);
  } else {
    // Error
    parserErr(-9);
  }
  
  return nodeK;
}

node_t ntL(std::fstream& file) {
  node_t nodeL;
  nodeL.nodeid = Lnt;
  nodeL.nodestr = "L";
  nodeL.linenum = lastline;
  
  if (sc.tstr[0] == '[') {
    node_t nodeD = ntD(file);
    nodeL.children.push_back(nodeD);
    
    node_t nodeTilde = t2tilde(file);
    nodeL.children.push_back(nodeTilde);
  } else if (sc.tstr[0] == ']') {
    node_t nodeE = ntE(file);
    nodeL.children.push_back(nodeE);
    
    node_t nodeTilde = t2tilde(file);
    nodeL.children.push_back(nodeTilde);
  } else if (sc.tid == T1_tk) {
    node_t nodeT1 = t1con(file);
    nodeL.children.push_back(nodeT1);
  
    node_t nodeF = ntF(file);
    nodeL.children.push_back(nodeF);
    
    node_t nodeTilde = t2tilde(file);
    nodeL.children.push_back(nodeTilde);
  } else if (sc.tid == EOF_tk || sc.tstr[0] == '~') {
    // Empty set
    printf("L Empty\n");
  } else {
    // Error
    parserErr(-10);
  }
  
  return nodeL;
}

node_t ntGp(std::fstream& file) {
  node_t nodeGp;
  nodeGp.nodeid = Gpnt;
  nodeGp.nodestr = "G";
  nodeGp.linenum = lastline;
  
  if (sc.tid == T1_tk || sc.tid == T3_tk) {
    node_t nodeK = ntK(file);
    nodeGp.children.push_back(nodeK);
    
    node_t nodeH = ntH(file);
    nodeGp.children.push_back(nodeH);
  } else if (sc.tstr[0] == '~') {
    // Empty set
  } else {
    // Error
    parserErr(-11);
  }
  
  return nodeGp;
}

// Terminals

node_t t1con(std::fstream& file) {
  node_t nodeT1;
  nodeT1.nodeid = t1;
  nodeT1.nodestr = sc.tstr;
  nodeT1.linenum = lastline;
  
  // Consume token
  sc = scnext;
  scnext = scanner(file, lastlinenext);
  lastline = lastlinenext;
  lastlinenext = scnext.linenum;
  
  return nodeT1;
}

node_t t2caret(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '^') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-12);
  }
  
  return nodeT2;
}

node_t t2tilde(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '~') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-13);
  }
  
  return nodeT2;
}

node_t t2ast(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '*') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-14);
  }
  
  return nodeT2;
}

node_t t2pipe(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '|') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-15);
  }
  
  return nodeT2;
}

node_t t2lbracket(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '[') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-16);
  }
  
  return nodeT2;
}

node_t t2rbracket(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == ']') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-17);
  }
  
  return nodeT2;
}

node_t t2lcbrace(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '{') {    
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-18);
  }
  
  return nodeT2;
}

node_t t2rcbrace(std::fstream& file) {
  node_t nodeT2;
  nodeT2.nodeid = t2;
  nodeT2.nodestr = sc.tstr;
  nodeT2.linenum = lastline;
  if (sc.tstr[0] == '}') {
    // Consume token
    sc = scnext;
    scnext = scanner(file, lastlinenext);
    lastline = lastlinenext;
    lastlinenext = scnext.linenum;
    
    return nodeT2;
  } else {
    // Error
    parserErr(-19);
  }
  
  return nodeT2;
}

node_t t3con(std::fstream& file) {
  node_t nodeT3;
  nodeT3.nodeid = t3;
  nodeT3.nodestr = sc.tstr;
  nodeT3.linenum = lastline;
  
  // Consume token
  sc = scnext;
  scnext = scanner(file, lastlinenext);
  lastline = lastlinenext;
  lastlinenext = scnext.linenum;
  
  return nodeT3;
}

void printPreorder(node_t root, int level) {
  if (root.nodeid == t1 || root.nodeid == t2 || root.nodeid == t3) {
    std::cout << "\033[1;34m" << std::string(level, ' ') << stackitemName[root.nodeid] << ": " << "\033[0m";
    std::cout << "\033[1;36m" << root.nodestr << "\n" << "\033[0m";
  } else {
    std::cout << "\033[1;35m" << std::string(level, ' ') << stackitemName[root.nodeid] << ":\n" << "\033[0m";
  }
  for (unsigned int i = 0; i < root.children.size(); i++) {
    printPreorder(root.children[i], level + 4);
  }
}
