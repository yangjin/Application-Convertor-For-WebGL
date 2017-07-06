#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <vector>

enum LineType { comment, // eg:the line start with "//"
                condition, // eg:if
                frameIndicator, // eg: W2N Frame
                function, // eg: void opengl function call
                assign,  // eg: return value opengl function call
                glcheck, //checkGlError
                eof,
                nofile,
                loadImage, // decodeAndUploadImage
                loadSubImage, // decodeAndUploadSubImage
                createZero, //createZeros
                undef_line
};

enum VariableType { glchar, glint, gluint, glbyte, glubyte, glfloat, novarible, glshort, glushort};

enum TokenType { number, macro, global, symbol, slash, rawstring, size_of ,reference ,zArray ,null};

enum ParameterType { imm_number, imm_marco, imm_string, imm_null, imm_symbol, val_variable, ref_variable, undef_parameter };

struct Token {
    TokenType type;
    std::string value;
};


struct Variable{
    VariableType type;
    std::string name;
    void * value;
    int size;
};

class Parser{
public:
    Parser();
    LineType parseLine(std::string,std::vector<Token> &);


private:


    std::string parseSymbol(std::string &);
    std::string parseNumber(std::string &);
    std::string parseRawstring(std::string &);
    Token parseToken(std::string &);


};

#endif
