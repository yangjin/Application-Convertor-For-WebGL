#ifndef TRACEANALYSIS_H
#define TRACEANALYSIS_H
#include <iostream>
#include <fstream>
#include <string>

struct Line{
    std::string thread;
    double ts;
    char type;
    std::string name;
};

class TraceAnalysis{
public:
    TraceAnalysis(std::string url);
    double getAverageTime(std::string thread,std::string name,int start,int end);
    bool isExist();
private:
    double findFunctionClosure();
    Line extractLine(std::string);
    double getNextFunctionTime(std::string thread,std::string name);
    std::ifstream file;
    int begIndex;
};

#endif
