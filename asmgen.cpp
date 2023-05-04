#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "parser.h"
#include "asmgen.h"

int jumpcounter = 0;
std::fstream asmfile;

void genErr(int errnum) {
  switch (errnum) {
    case -1:
      printf("\033[1;31mError opening output file\n\033[0m\n");
      break;
  }
  
  exit(EXIT_FAILURE);
}

void generate_code(node_t root, std::string filename, std::vector<std::string> var_table) {
  // If root node
  if (root.nodeid == Snt) {
    asmfile.open(filename.c_str(), std::fstream::out | std::fstream::in | std::fstream::trunc);
    if (!asmfile.is_open()) {
      genErr(-1);
    }
  }
  
  // If A node (init)
  if (root.nodeid == Ant) {
    // Check that we're not empty
    if (!root.children.empty()) {
      asmfile << gen_init(root.children[1].nodestr) << std::endl;
    }
  }
  
  // If D node (read)
  if (root.nodeid == Dnt) {
    asmfile << gen_read(root.children[1].nodestr) << std::endl;
  }
  
  // If E node (write)
  if (root.nodeid == Ent) {
    asmfile << gen_write(root.children[1].children[0].nodestr) << std::endl;
  }
  
  // If F node (assignment, add, and subtract) (we're checking in J here because the t1 of this operation lives in J)
  if (root.nodeid == Jnt && !root.children.empty() && root.children[0].nodeid == t1) {
    // If Gprime within F is empty, we are just doing an assignment
    if (root.children[1].children[1].children[1].children.empty()) {
      asmfile << gen_assign(root.children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
    } else {
      // If Gprime is not empty, we are adding or subtracting
      if (root.children[1].children[1].children[1].children[1].children[0].nodestr == "{") {
        // Add
        asmfile << gen_add(root.children[1].children[1].children[0].children[0].nodestr, root.children[1].children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
      } else {
        // Subtract
        asmfile << gen_subtract(root.children[1].children[1].children[0].children[0].nodestr, root.children[1].children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
      }
    }
  }
  
  // If C node (first part of conditional)
  if (root.nodeid == Cnt) {
    asmfile << gen_conditional(root.children[1].children[0].nodestr) << std::endl;
  }
  
  // If L node (last part of conditional)
  if (root.nodeid == Lnt) {
    asmfile << gen_else() << std::endl;
    if (!root.children.empty()) {
      if (root.children[0].nodeid == Dnt) {
        //asmfile << gen_read(root.children[0].children[1].children[0].nodestr) << std::endl;
      } else if (root.children[0].nodeid == Ent) {
        //asmfile << gen_write(root.children[0].children[1].children[0].nodestr) << std::endl;
      } else if (root.children[1].nodeid == Fnt) {
        // If Gprime within F is empty, we are just doing an assignment
        if (root.children[1].children[1].children[1].children.empty()) {
          asmfile << gen_assign(root.children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
        } else {
          // If Gprime is not empty, we are adding or subtracting
          if (root.children[1].children[1].children[1].children[1].children[0].nodestr == "{") {
            // Add
            asmfile << gen_add(root.children[1].children[1].children[0].children[0].nodestr, root.children[1].children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
          } else {
            // Subtract
            asmfile << gen_subtract(root.children[1].children[1].children[0].children[0].nodestr, root.children[1].children[1].children[1].children[0].children[0].nodestr, root.children[0].nodestr) << std::endl;
          }
        }
      }
    }
  }
  
  // Recursion
  if (root.nodeid != t1 && root.nodeid != t2 && root.nodeid != t3) {
    for (unsigned int i = 0; i < root.children.size(); i++) {
      generate_code(root.children[i], filename, var_table);
    }
  }
  
  if (root.nodeid == Lnt) {
    asmfile << gen_else_end() << std::endl;
  }
  
  // Before we quit, make sure to add STOP, initialize variables, and close the output file
  if (root.nodeid == Snt) {
    asmfile << "STOP" << std::endl;
    for (unsigned int i = 0; i < var_table.size(); i++) {
      asmfile << var_table[i] << " 0" << std::endl;
    }
    asmfile.close();
    std::cout << "\n" << "\033[1;32m" << "Compiled Successfully!" << "\033[0m" << "\n";
    std::cout << "\033[1;36m" << "Output file --> " << filename << "\033[0m" << "\n";
  }
}

std::string gen_init(std::string x) {
  std::string out = "LOAD 0\nSTORE " + x + "\n";
  return out;
}

std::string gen_read(std::string x) {
  std::string out = "READ " + x + "\n";
  return out;
}

std::string gen_write(std::string x) {
  std::string out = "WRITE " + x + "\n";
  return out;
}

std::string gen_add(std::string x, std::string y, std::string z) {
  std::string out = "LOAD " + x + "\nADD " + y + "\nSTORE " + z + "\n";
  return out; 
}

std::string gen_subtract(std::string x, std::string y, std::string z) {
  std::string out = "LOAD " + x + "\nSUB " + y + "\nSTORE " + z + "\n";
  return out; 
}

std::string gen_assign(std::string x, std::string y) {
  std::string out = "LOAD " + x + "\nSTORE " + y + "\n";
  return out; 
}

std::string gen_conditional(std::string x) {
  jumpcounter++;
  std::stringstream jumpstream;
  jumpstream << jumpcounter;
  std::string out = "LOAD " + x + "\nBRZNEG jump" + jumpstream.str() + "\n";
  return out;
}

std::string gen_else() {
  std::stringstream jumpstream;
  jumpstream << jumpcounter;
  std::string out = "BR out" + jumpstream.str() + "\njump" + jumpstream.str() + ": NOOP\n";
  return out;
}

std::string gen_else_end() {
  std::stringstream jumpstream;
  jumpstream << jumpcounter;
  jumpcounter--;
  std::string out = "out" + jumpstream.str() + ": NOOP\n";
  return out;
}