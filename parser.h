#include <vector>
#include <string>
#include <iostream>

enum stackitemID {Snt, Ant, Cnt, Dnt, Ent, Fnt, Gnt, Hnt, Jnt, Knt, Lnt, Gpnt, t1, t2, t3};
static std::string stackitemName[] = {"S", "A", "C", "D", "E", "F", "G", "H", "J", "K", "L", "G'", "t1", "t2", "t3"};

struct node_t {
  stackitemID nodeid;
  std::string nodestr;
  std::vector<node_t> children;
  int linenum;
};

void parserErr(int);

node_t ntS(std::fstream&);
node_t ntA(std::fstream&);
node_t ntC(std::fstream&);
node_t ntD(std::fstream&);
node_t ntE(std::fstream&);
node_t ntF(std::fstream&);
node_t ntG(std::fstream&);
node_t ntH(std::fstream&);
node_t ntJ(std::fstream&);
node_t ntK(std::fstream&);
node_t ntL(std::fstream&);
node_t ntGp(std::fstream&);
node_t t1con(std::fstream&);
node_t t2caret(std::fstream&);
node_t t2tilde(std::fstream&);
node_t t2ast(std::fstream&);
node_t t2pipe(std::fstream&);
node_t t2lbracket(std::fstream&);
node_t t2rbracket(std::fstream&);
node_t t2lcbrace(std::fstream&);
node_t t2rcbrace(std::fstream&);
node_t t3con(std::fstream&);

node_t parser(std::fstream&);
void printPreorder(node_t, int=0);