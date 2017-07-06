#include <string>
#include <map>
#include <iostream>
#include "Parser.h"
#include <string.h>
#include <sstream>
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

inline void skipBlank(const char* &a ){
    int i = 0;
    while (a[i]!=0){
        if (((a[i] >= 'a'&&a[i] <= 'z')
            || (a[i] >= 'A'&&a[i] <= 'Z')
            || (a[i] >= '0'&&a[i] <= '9')
            || a[i] == '_' || a[i] == '/' || a[i] == '.' || a[i] == '"' || a[i] == '&' || a[i]=='-'))
            break;
        i++;
    }
    a=&(a[i]);
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

void replaceAll(string&   str, const   string&   old_value, const   string&   new_value)
{
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length())   {
        if ((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else   break;
    }
    
}
Parser::Parser(){}

bool Parser::load(string filePath){
    if (in.is_open())
        in.close();
    in.open(filePath.c_str());

    if (!in.is_open()){
        cout << "Cannot open or there is no " << filePath << endl;
        return false;
    }
    return true;
}

string Parser::peakSymbol(string &a) {
    int i = 0, j = 0;
    int length = a.size();

    while (i<length){
        if (((a[i] >= 'a'&&a[i] <= 'z') ||
             (a[i] >= 'A'&&a[i] <= 'Z') ||
             (a[i] >= '0'&&a[i] <= '9') ||
             (a[i] == '_')))
            break;
        i++;
    }
    j = i; 
    while (j<length){
        if (!((a[j] >= 'a'&&a[j] <= 'z') ||
              (a[j] >= 'A'&&a[j] <= 'Z') ||
              (a[j] >= '0'&&a[j] <= '9') ||
              (a[j] == '_' )))
            break;
        j++;
    }
    string b = a;
    return b.substr(i,j-i);
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

string Parser::parseSymbol(const char* &a){
    int i = 0;
    string b="";
    while (a[i]!=0){
        if (!((a[i] >= 'a'&&a[i] <= 'z')
            || (a[i] >= 'A'&&a[i] <= 'Z')
            || (a[i] >= '0'&&a[i] <= '9')
            || a[i] == '_' ))
            break;
        b+=a[i];
        i++;
    }

    a=&(a[i]);
    
    return b;
}

 string Parser::parseNumber(const char * &a ){
    int i = 0;
    string b="";
    if (a[0] == '-')
        i++;
    while (a[i]!=0){
        if ( !((a[i] >= '0'&&a[i] <= '9')
            || a[i] == '.'))
            break;
        b+=a[i];
        i++;
    }
    a=&(a[i]);
    
    return b;
}

 string Parser::parseNumber(string &a){
    int i = 0;
    int length = a.size();
    if (a[0] == '-')
        i++;
    while (i<length){
        if ( !((a[i] >= '0'&&a[i] <= '9')
            || a[i] == '.'))
            break;
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
    for (i = len-1; i >=0; i--){
        if (a[i] == '"')
            break;
    }
    string b = a;
    a = a.substr(i + 1);
    return b.substr(1, i-1);
}

char Parser::parseRawchar(string &a) {
    int len = a.size();
    int i;
    for (i = 1; i < len; i++) {
        if (a[i] == '\'')
            break;
    }
    string b = a;
    a = a.substr(i + 1);
    return b[i+1];
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
        else if (sym == "false" || sym == "NULL"){
            return Token{ number, "0" };
        }
        else if (sym == "true"){
            return Token{ number, "1" };
        }
        else if (sym == "sizeof"){
            return Token{ size_of, "sizeof" };
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
    
    return Token{};
}


Variable Parser::parseSingleGlobal(){

    Variable v; 

    v.type = gluint;

    if (!in.is_open()){
        cout << "File is not loaded!" << endl;
        return { novarible ,"",0};
    }

    string line;
    getline(in, line);

    if (in.fail()){
        return {};
    }
    skipBlank(line);

    Token token = parseToken(line);
    if (token.value == "const"){
        skipBlank(line);
        token = parseToken(line);
    }

    string::size_type pos(0); 
    string symbol_name = peakSymbol(line);
    bool uflag = token.value[2] == 'u';
    if (token.value == "GLchar") {
        pos = symbol_name.find("_String_", pos);
    } else {
        pos = symbol_name.find("Array_", pos);
    }

    if (token.value == "GLchar") { 
        if (pos == string::npos) {
            return parseChar(line);
        } else {         
            return parseCharArray(line);
        }
    }
    else if (token.value == "GLuint" || token.value == "GLint") {
        if (pos == string::npos) {
            return parseInt(line, uflag);
        } else {
            return parseIntArray(line, uflag);
        }
    }
    else if (token.value == "GLubyte" || token.value == "GLbyte") {
        if (pos == string::npos) {
            return parseByte(line, uflag);
        } else {
            return parseByteArray(line, uflag);
        }
    }
    else if (token.value == "GLushort" || token.value == "GLshort") {
        if (pos == string::npos) {
            return parseShort(line, uflag);
        } else {
            return parseShortArray(line, uflag);
        }
    }
    else if (token.value == "GLdouble" || token.value == "GLfloat") {
        if (pos == string::npos) {
            return parseFloat(line);
        } else {
            return parseFloatArray(line);
        }
    }

    return {};
}

Variable Parser::parseCharArray(string line){
    
    skipBlank(line);
    Variable v;
    v.type = glchar;

    Token t = parseToken(line);
    v.name = t.value;

    skipBlank(line);
    string value=parseRawstring(line);
    while (line[0] != ';'){
        getline(in, line);
        skipBlank(line);
        value += parseRawstring(line);
    }
    replaceAll(value, "\\n", "\n");
    char * c = new char[value.size() + 1];
    strcpy(c, value.c_str());
    v.value = c;
    v.size = value.size();
    return v;
}

Variable Parser::parseChar(string line) {
    skipBlank(line);
    Token t = parseToken(line);
    unsigned char *c = new unsigned char[1];
    *c = 0;
    return Variable{ glchar, t.value, c, 1 };
}

Variable Parser::parseInt(string line, bool uflag){
    skipBlank(line);
    Token t = parseToken(line);
    unsigned int *i = new unsigned int[1];
    *i = 0;
    return Variable{ uflag? gluint : glint, t.value, i, 4 };
}

Variable Parser::parseShort(string line, bool uflag){
    skipBlank(line);
    Token t = parseToken(line);
    unsigned short *i = new unsigned short[1];
    *i = 0;
    return Variable{ uflag? glushort : glshort, t.value, i, 2 };
}

Variable Parser::parseByte(string line, bool uflag){
    skipBlank(line);
    Token t = parseToken(line);
    unsigned char *i = new unsigned char[1];
    *i = 0;
    return Variable{ uflag? glubyte : glbyte, t.value, i, 1 };
}

Variable Parser::parseFloat(string line){
    skipBlank(line);
    Token t = parseToken(line);
    float *f = new float[1];
    *f = 0.0;
    return Variable{ glfloat, t.value, f, 1 };
}

Variable Parser::parseIntArray(string line, bool uflag) {
    const char *l= line.c_str();
    skipBlank(l);
    string s = parseSymbol(l);

    skipBlank(l);

    string n = parseNumber(l);
    int num = atoi(n.c_str());

    unsigned int *c = new unsigned int[num];

    for (int i = 0; i < num; i++){
        skipBlank(l);
        n = parseNumber(l);
        c[i] = atoi(n.c_str());
    }
    return Variable{uflag? gluint:glint, s, c, num * (int)sizeof(GLuint) };
}

Variable Parser::parseByteArray(string line, bool uflag) {
    
    const char *l= line.c_str();
    skipBlank(l);
    string s = parseSymbol(l);
    
    skipBlank(l);

    string n = parseNumber(l);
    int num = atoi(n.c_str());

    unsigned char *c = new unsigned char[num];

    
    for (int i = 0; i < num; i++){
        skipBlank(l);
        n = parseNumber(l);
        c[i] = atoi(n.c_str());
    }

    return Variable{ uflag? glubyte:glbyte, s, c,num * (int)sizeof(GLubyte)};
}

Variable Parser::parseShortArray(string line, bool uflag){

    const char *l= line.c_str();
    skipBlank(l);
    string s = parseSymbol(l);
    
    skipBlank(l);

    string n = parseNumber(l);
    int num = atoi(n.c_str());

    unsigned short *c = new unsigned short[num];


    for (int i = 0; i < num; i++){
        skipBlank(l);
        n = parseNumber(l);
        c[i] = atoi(n.c_str());
    }
    return Variable{uflag? glushort:glshort, s, c, num * (int)sizeof(GLushort) };
}


Variable Parser::parseFloatArray(string line){
    const char *l= line.c_str();
    skipBlank(l);
    string s = parseSymbol(l);
    
    skipBlank(l);

    string n = parseNumber(l);
    int num = atoi(n.c_str());

    float *c = new float[num];

    for (int i = 0;i < num; i++){ // yqf fix
        skipBlank(l);        
        
        int j=0;
        string f;
        while(l[j]!=',' && l[j] != '\0'){
            f+=l[j];
            j++;
        }
        l=l+j;
        stringstream ss(f);
        ss>>c[i];

    }

    return Variable{ glfloat, s, c, num * (int)sizeof(GLfloat) };


}
