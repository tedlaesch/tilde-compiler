#include <vector>
#include <algorithm>
#include <cstdio>
#include <ostream>

#include "parser.h"
#include "staticsem.h"

std::vector<std::string> s_table;
std::string last;

void semErr(int errnum, node_t node) {
  switch (errnum) {
    case -1:
      printf("\033[1;31mSEMANTICS ERROR: Line %d: Variable already initialized: %s\n\033[0m", node.linenum, node.nodestr.c_str());
      break;
    case -2:
      printf("\033[1;31mSEMANTICS ERROR: Line %d: Tried to use a variable that was not initialized: %s\n\033[0m", node.linenum, node.nodestr.c_str());
      break;
  }
  
  exit(EXIT_FAILURE);
}

void statSem(node_t root) {
  if (root.nodeid == t1) {
    if (last == "^") {
      // Initalization
      if (std::count(s_table.begin(), s_table.end(), root.nodestr)) {
        // Already in table, error
        semErr(-1, root);
      } else {
        // Not in table, add
        s_table.push_back(root.nodestr);
      }
    } else {
      // Variable access
      if (std::count(s_table.begin(), s_table.end(), root.nodestr)) {
        // Found in table
      } else {
        // Not in table, error
        semErr(-2, root);
      }
    }
  } else if (root.nodeid != t2 && root.nodeid != t3) {
    for (unsigned int i = 0; i < root.children.size(); i++) {
      statSem(root.children[i]);
    }
  }
  last = root.nodestr;
  if (root.nodeid == Snt) {
    std::cout << "\033[1;32m" << "Static Semantics OK!" << "\033[0m" << "\n";
    //printTable();
  }
}

void printTable() {
  std::cout << "\033[1;35m" << "\nSymbol Table:" << "\033[0m" << std::endl;
  for (unsigned int i = 0; i < s_table.size(); i++) {
    std::cout << "\033[1;35m" << s_table[i] << "\033[0m" << std::endl;
  }
}

std::vector<std::string> getTable() {
  return s_table;
}