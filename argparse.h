#ifndef FAKE_ESSENTIALS_ARGPARSE_H
#define FAKE_ESSENTIALS_ARGPARSE_H
enum class ArgParseResult {
    OK = 0,
    NEEDS_ARG = 1,
    NOT_FOUND = 2
};
struct PositionalArg {
    std::string value;
    ArgParseResult result;

    static PositionalArg create(std::string basicString, ArgParseResult apr);
};
PositionalArg get(char ** in, int argc, const char * tofind);
bool has(char ** in, int argc, const char * tofind);
#endif //FAKE_ESSENTIALS_ARGPARSE_H
