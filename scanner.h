#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <cctype>

struct token_t {
  tokenID tid;
  std::string tstr;
  int linenum;
};

void error_msg(int, char, int);
token_t scanner(std::fstream&, int);