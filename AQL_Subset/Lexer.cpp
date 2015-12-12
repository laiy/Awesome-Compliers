#include <vector>
#include <string>
#include <cstdio>
#include "Lexer.h"
#include "Tokenizer.h"

Lexer::Lexer(char *file_path) {
    Tokenizer tokenizer = Tokenizer(file_path);
    std::vector<document_token> document_tokens = tokenizer.get_tokens();
    std::vector<std::string> tokens;
    for (int i = 0; (size_t)i < document_tokens.size(); i++)
        tokens.push_back(document_tokens[i].value);
    std::string buffer;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
        if (*it == "/") {
            buffer = "/";
            it = tokens.erase(it);
            while (*it != "/")
                buffer += *it, it = tokens.erase(it);
            buffer += "/";
            it = tokens.erase(it);
            it = tokens.insert(it, buffer);
        }
        this->tokens.push_back(get_token(*it));
    }
    FILE *original_document = fopen(file_path, "r");
    fgets(this->content, MAX_LEN, original_document);
}

std::vector<token> Lexer::get_tokens() {
    return this->tokens;
}

inline token get_token(std::string str) {
    Type type;
    if (str == ".")
        type = DOT;
    else if (str == ",")
        type = COMMA;
    else if (str == "<")
        type = LESSTHAN;
    else if (str == ">")
        type = GREATERTHAN;
    else if (str == "(")
        type = LEFTBRACKET;
    else if (str == ")")
        type = RIGHTBRACKET;
    else if (str == "{")
        type = CURLYLEFTBRACKET;
    else if (str == "}")
        type = CURLYRIGHTBRACKET;
    else if (str == ";")
        type = SEMICOLON;
    else if (str == "create")
        type = CREATE;
    else if (str == "view")
        type = VIEW;
    else if (str == "as")
        type = AS;
    else if (str == "output")
        type = OUTPUT;
    else if (str == "select")
        type = SELECT;
    else if (str == "from")
        type = FROM;
    else if (str == "extract")
        type = EXTRACT;
    else if (str == "regex")
        type = REGEX;
    else if (str == "on")
        type = ON;
    else if (str == "return")
        type = RETURN;
    else if (str == "group")
        type = GROUP;
    else if (str == "and")
        type = AND;
    else if (str == "Token")
        type = TOKEN;
    else if (str == "pattern")
        type = PATTERN;
    else if (is_number(str))
        type = NUM;
    else if (str[0] == '/' && str[str.length() - 1] == '/')
        type = REG;
    else
        type = ID;
    return token(str, type);
}

char* Lexer::get_text() {
    return this->content;
}

inline bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

