#include "scanner.h"
#include <stack>
#include <vector>

enum stackitemID {Snt, Ant, Cnt, Dnt, Ent, Fnt, Gnt, Hnt, Jnt, Knt, Lnt, Gpnt, t1, t2, t3};
std::string stackitemName[] = {"S", "A", "C", "D", "E", "F", "G", "H", "J", "K", "L", "G'", "t1", "t2", "t3"};

struct node_t {
  stackitemID nodeid;
  std::string nodestr;
  std::vector<node_t> children;
  int linenum;
};

struct stackitem_t {
  stackitemID siid;
  std::string sistr;
  node_t* nodepos;
  int linenum;
};

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

int parser(std::fstream&);

void printPreorder(node_t root, int level = 0) {
  if (root.nodeid == t1 || root.nodeid == t2 || root.nodeid == t3) {
    //std::cout << "\033[1;33m" << "File entry mode: scanning file..." << "\033[0m" << "\n";
    std::cout << "\033[1;34m" << std::string(level, ' ') << stackitemName[root.nodeid] << ": " << "\033[0m";
    std::cout << "\033[1;36m" << root.nodestr << "\n" << "\033[0m";
  } else {
    std::cout << "\033[1;35m" << std::string(level, ' ') << stackitemName[root.nodeid] << ":\n" << "\033[0m";
  }
  for (unsigned int i = 0; i < root.children.size(); i++) {
    printPreorder(root.children[i], level + 4);
  }
}