#ifndef CONVERTOR_H
#define CONVERTOR_H
#include <string>
#include <fstream>
#include "Parser.h"


class Convertor{
public:
      Convertor(std::string,std::string);

      void convertMacro();
      void convertInit();
      void convertGlobal();
      void convertFunction();
      void setConfig();

private:
     Parser parser;
     std::ofstream file;
     int GCursor,ICursor,FCursor;
     std::string dir;

};

#endif
