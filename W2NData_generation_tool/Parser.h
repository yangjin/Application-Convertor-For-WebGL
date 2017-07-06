#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <fstream>

enum LineType { comment, condition, frameIndicator, function, assign, glcheck, eof, nofile, loadImage,undefine };

enum VariableType { novarible, glchar, gluint, glint, glubyte, glbyte, glfloat, glushort, glshort };

enum TokenType { number, macro, global, symbol, slash, rawstring, size_of ,reference };

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
    Variable parseSingleGlobal();
    bool load(std::string);

    
private:


    std::string peakSymbol(std::string&);
    std::string parseSymbol(std::string &);
    std::string parseSymbol(const char *&);
    std::string parseNumber(std::string &);
    std::string parseNumber(const char *&);
    std::string parseRawstring(std::string &);
    char parseRawchar(std::string &);
    Variable parseChar(std::string);
    Variable parseInt(std::string, bool);
    Variable parseByte(std::string, bool);
    Variable parseShort(std::string, bool);
    Variable parseFloat(std::string);
    Variable parseCharArray(std::string);
    Variable parseByteArray(std::string, bool);
    Variable parseShortArray(std::string, bool);
    Variable parseIntArray(std::string, bool);
    Variable parseFloatArray(std::string);
    Token parseToken(std::string &);
    std::ifstream in;

    
    
};
typedef unsigned int GLuint;
typedef int GLint;
typedef float GLfloat;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;

#endif
