#include <string>

void generate_code(node_t, std::string, std::vector<std::string>);

void genErr(int);

std::string gen_init(std::string);
std::string gen_read(std::string);
std::string gen_write(std::string);
std::string gen_add(std::string, std::string, std::string);
std::string gen_subtract(std::string, std::string, std::string);
std::string gen_assign(std::string, std::string);
std::string gen_conditional(std::string);
std::string gen_else();
std::string gen_else_end();