#include <string>
#include <utility>
#include <cstring>

enum class ArgParseResult {
    OK = 0,
    NEEDS_ARG = 1,
    NOT_FOUND = 2
};

struct PositionalArg {
    std::string value;
    ArgParseResult result;

    static PositionalArg create(std::string value, ArgParseResult result);
};

PositionalArg PositionalArg::create(std::string basicString, ArgParseResult apr) {
    PositionalArg ar;
    ar.result = apr;
    ar.value = std::move(basicString);
    return ar;
}

bool has(char ** in, int argc, const char * tofind) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(in[i],tofind) == 0) {
            return true;
        }
    }
    return false;
}

bool starts_with(char * string, const char * starts_with) {
    size_t len = strlen(string);
    size_t s_len = strlen(starts_with);
    if (len < s_len) {
        return false;
    }

    for (int i = 0; i < s_len; i++) {
        if (string[i] != starts_with[i]) {
            return false;
        }
    }
    return true;
}

std::string replace(char * chars, char toReplace, char toReplaceWith) {
    for (int i = 0; i < strlen(chars); i++) {
        if (chars[i] == toReplace) {
            chars[i] = toReplaceWith;
        }
    }
    return chars;
}

PositionalArg get(char ** in, int argc, const char * tofind) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(in[i], tofind) == 0) {
            if (i + 1 == argc) {
                return PositionalArg::create("", ArgParseResult::NEEDS_ARG);
            }
            if (starts_with(in[i + 1], "--")) {
                return PositionalArg::create("", ArgParseResult::NEEDS_ARG);
            }
            return PositionalArg::create(replace(in[i + 1], '_', ' '), ArgParseResult::OK);
        }
    }
    return PositionalArg::create("", ArgParseResult::NOT_FOUND);
}
