#include <string>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <vector>

#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "staticsem.h"
#include "asmgen.h"

// The help message
void usage_message() {
  printf("\nUsage: frontEnd [-h] [filename]\n-h flag: display the help message\nfilename: name of file to read from\n\n");
  return;
}

int main(int argc, char** argv) {
  std::string filename;
  std::fstream file;
  int input_mode;
  
  // Flag handling - only takes the -h flag for the help message
  int opt;
  while((opt = getopt(argc, argv, ":h")) != -1) {
    switch(opt) {
      case 'h':
        usage_message();
        return EXIT_SUCCESS;
      case '?':
        printf("Unknown option: %c\n", optopt);
        usage_message();
        return EXIT_FAILURE;
    }
  }
  
  // Additional argument handling - takes only one additional argument as an input file
  if (optind < argc - 1) {
    std::cout << "\033[1;31mError: Too many arguments supplied\033[0m\n";
    usage_message();
    return EXIT_FAILURE;
  } else if (optind == argc - 1) {
    // Filename supplied to program
    input_mode = 1;
    filename = argv[optind];
    //std::cout << "\033[1;32m" << filename << "\033[0m" << "\n";
  } else {
    // Filename not supplied, user input mode
    input_mode = 0;
  }
  
  if (input_mode) {
    // File entry
    std::cout << "\033[1;33m" << "File entry mode: scanning file..." << "\033[0m" << "\n";
    
    file.open(filename.c_str());
    if (!file.is_open()) {
      std::cout << "\033[1;31mError opening file\n\033[0m\n";
      return EXIT_FAILURE;
    }
  } else {
    // Keyboard entry
    printf("Text entry: type any number of tokens seperated by standard whitespace (space, tab, newline). End input by pressing Ctrl + D on a new line.\n\n");
    
    filename = "out";
    
    file.open("tmpkeyboardentry", std::fstream::out | std::fstream::in | std::fstream::trunc);
    if (!file.is_open()) {
      std::cout << "\033[1;31mError opening temporary file\n\033[0m\n";
      return EXIT_FAILURE;
    }
    
    // Reading characters from keyboard into temp file
    char read_character = '\0';
    while (1) {
      if ((read_character = fgetc(stdin)) == EOF) {
        break;
      }
      file << read_character;
    }
    // Resetting file read pointer before reading from it later
    file.clear();
    file.seekg(0);
  }
  
  // Get tree from parser
  node_t root = parser(file);
  
  // Check static semantics
  statSem(root);
  
  // Generate assembly code
  std::vector<std::string> var_table = getTable();
  std::string asmfilename = filename + ".asm";
  generate_code(root, asmfilename, var_table);
  
  // clean up files
  file.close();
  std::remove("tmpkeyboardentry");
  
  return EXIT_SUCCESS;
}