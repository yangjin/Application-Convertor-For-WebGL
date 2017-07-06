#ifndef GLEMULATOR_H
#define GLEMULATOR_H
#include <string>
#include <map>
#include "Parser.h"
#include "Interface.h"
#include "Object.h"
#include <GLES2/gl2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <dlfcn.h>
#include <stdlib.h>


struct Function{
    std::string name;
    FuncID id;
};
struct FunctionValue{
    void *ptr;
    FuncID id;
};

struct Parameter{
    void* Ptr;
    int Int;
    GLfloat Float;
    ParameterType type;
};

struct Line{
    LineType type;
    std::vector<Parameter>paras;
};

class GLEmulator{
    public:
        GLEmulator(std::string libPath);
        GLEmulator();
        ~GLEmulator();
        int getALPHA(){ return macroTable["ALPHA"]; }
        int getDEPTH(){ return macroTable["DEPTH"]; }
        int getSTENCIL(){ return macroTable["STENCIL"]; }
        int getANTIALIAS(){ return macroTable["ANTIALIAS"]; }
        int getPREMULTIPLIE_ALPHA(){ return macroTable["PREMULTIPLIE_ALPHA"]; }
        int getPRESERVE_DRAWING_BUFFER(){ return macroTable["PRESERVE_DRAWING_BUFFER"]; }
        int getWIDTH_DEF(){ return macroTable["WIDTH_DEF"]; }
        int getHEIGHT_DEF(){ return macroTable["HEIGHT_DEF"]; }
        unsigned int getFRAMENUM(){ return (unsigned int)macroTable["FRAMENUM"]; }
        void setEnableTxtData(bool enableTxtData) { this->enableTxtData=enableTxtData; }
        bool GL_Init(int,int,char **);
        void explicitInit();
        void GL_Frame(unsigned int );
        void loadLib(std::string libPath);
    private:
        std::ifstream file;

        std::ofstream reverseTxtFile;
        std::string currentFile;
        int currentLine;
        int currentFrame;
        int GCursor,ICursor,FCursor;
        bool isLoad;

        bool enableTxtData;
        void* handle;
        std::vector<std::vector<Line> >frameTable;
        std::map<std::string,FunctionValue>funcTable;
        std::map<std::string, Variable>globalTable;
        void init();
        void loadMacro();
        void loadGlobal();
        void executeLine(Line);
        void voidCall( std::vector<Parameter> &);
        void reCall( std::vector<Parameter>&);
        void loadFrame(unsigned int);
        std::vector<Parameter> getPara(LineType,const std::vector<Token> &tokens);
        std::map<std::string, int>macroTable;
        Parser parser;

        char **zero;
};

#endif
