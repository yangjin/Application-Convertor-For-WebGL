#include <string>
#include "Parser.h"
#include <string.h>
#include <stdlib.h>
using namespace std;

inline void skipBlank(string &a){
    int i = 0;
    int length = a.size();
    while (i<length){
        if (((a[i] >= 'a'&&a[i] <= 'z')
            || (a[i] >= 'A'&&a[i] <= 'Z')
            || (a[i] >= '0'&&a[i] <= '9')
            || a[i] == '_' || a[i] == '/' || a[i] == '.' || a[i] == '"' || a[i] == '&' || a[i]=='-'))
            break;
        i++;
    }
    a=a.substr(i);
}

inline bool isSymbol(char a){
    if (((a >= 'a'&& a <= 'z')
        || (a >= 'A'&& a <= 'Z')
        || a == '_'))
        return true;
    return false;
}

inline bool isNumber(char a){
    if (((a >= '0'&& a <= '9')))
        return true;
    return false;
}

inline bool isRawstring(char a){
    return a == '"';
}


Parser::Parser(){}

LineType Parser::parseLine(string line ,vector<Token>& result){

    result.clear();
    skipBlank(line);
    while (line.size() != 0){
        result.push_back(parseToken(line));
        skipBlank(line);
    }
    if (result.size() > 0){
        switch (result[0].type){
        case symbol:
            if (result[0].value == "if")
                return condition;
            else if (result[0].value == "checkGlError")
                return glcheck;
            else if (result[0].value == "W2N")
                return frameIndicator;
            else if (result[0].value == "decodeAndUploadImage")
                return loadImage;
            else if (result[0].value == "decodeAndUploadSubImage")
                return loadSubImage;
            else if (result[0].value == "createZeros"){
                return createZero;
            }
            else
                return function;
        case slash:
            return comment;
        case global:
            return assign;
        default:
            return undef_line;

        }
    }
    return eof;
}
string Parser::parseSymbol(string &a){
    int i = 0;
    int length = a.size();
    while (i<length){
        if (!((a[i] >= 'a'&&a[i] <= 'z')
            || (a[i] >= 'A'&&a[i] <= 'Z')
            || (a[i] >= '0'&&a[i] <= '9')
            || a[i] == '_' ))
            break;
        i++;
    }
    string b = a;
    if (i == length)
        a.clear();
    else
        a = a.substr(i);
    return b.substr(0,i);
}


string Parser::parseNumber(string &a){
    int i = 0;
    bool exp = false;
    int length = a.size();
    if (a[0] == '-')
        i++;
    while (i<length){
        if (!((a[i] >= '0'&&a[i] <= '9') || a[i] == '.')) {
            if (exp || !((i < length - 2)&&(a[i]=='e'||a[i]=='E')&&(a[i+1]=='+'||a[i+1]=='-')&&(a[i+2]>='0'&&a[i+2]<='9'))) {
                break;
            }
            i++;
            exp = true;
        }
        i++;
    }
    string b = a;
    if (i == length)
        a.clear();
    else
        a = a.substr(i);
    return b.substr(0, i);
}

string Parser::parseRawstring(string &a){
    int len = a.size();
    int i;
    for (i = 1; i < len; i++){
        if (a[i] == '"')
            break;
    }
    string b = a;
    a = a.substr(i + 1);
    return b.substr(1, i-1);
}
Token Parser::parseToken(string &line){

    //delete some cast
    if (line.size()>3 && line.substr(0, 3) == "int"){
        line = line.substr(4);
    }
    else if (line.size()>13&&line.substr(0,13) == "const GLvoid*"){
        line = line.substr(14);
    }
    else if (line.size()>13 && line.substr(0, 13) == "const char **"){
        line = line.substr(14);
    }


    if (line.substr(0, 2) == "//" || line.substr(0, 2) == "/*"){
        string a = line;
        line.clear();
        return Token{ slash, a.substr(2) };
    }
    else if (isSymbol(line[0])){
        string sym = parseSymbol(line);
        if (sym.substr(0,3) == "GL_"){
            //macro
            return Token{ macro, sym };
        }
        else if (sym.substr(0, 2) == "g_"){
            //global
            return Token{ global, sym };
        }
        else if (sym == "false"){
            return Token{ number, "0" };
        }
        else if (sym == "NULL"){
            return Token{null,"NULL"};
        }
        else if (sym == "true"){
            return Token{ number, "1" };
        }
        else if (sym == "sizeof"){
            return Token{ size_of, "sizeof" };
        }
        else if(sym == "zerosArray"){
            return Token{zArray,"zerosArray"};
        }
        else{
            return Token{ symbol, sym };
        }
    }
    else if (isNumber(line[0]) || (line[0]=='-' &&isNumber(line[1]))){
        string num = parseNumber(line);
        return Token{ number, num };
    }
    else if (isRawstring(line[0])){
        string raw = parseRawstring(line);
        return Token{ rawstring, raw };
    }
    else if (line[0] == '&'){
        line = line.substr(1);
        return Token{ reference, "&" };
    }

    Token t;
    return t;
}



