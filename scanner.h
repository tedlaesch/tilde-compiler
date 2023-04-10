#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <cctype>

#include "token.h"

struct token_t {
  tokenID tid;
  std::string tstr;
  int linenum;
};

token_t scanner(std::fstream&, int);