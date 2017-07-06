
#include "TraceAnalysis.h"
#include <stdlib.h>
using namespace std;

const static string endMark="END TRACE";
TraceAnalysis::TraceAnalysis(string url){
    file.open(url.c_str());
    if(!file){
       return;
    }
    string line;

    /* find the data segement in the file*/
    while(1){
       getline(file,line);
       if(line.find("BEGIN TRACE")==-1)
           continue;
       else break;
    }
    for(int i=0;i<13;i++)
        getline(file,line);


    begIndex=file.tellg();

}

bool TraceAnalysis::isExist(){
     return file!=NULL;
}

double TraceAnalysis::getAverageTime(string thread,string name,int start,int end){

    file.seekg(begIndex);
    for(int i=1;i<start;i++){
        if(getNextFunctionTime(thread,name)<0){
            cout<<"The function "<<name<<" does not range from "<<start<<" to "<<end<<endl;
            return 0;
        }
    }
    double time=0,inteval;
    for(int i=start;i<=end;i++){
        inteval=getNextFunctionTime(thread,name);
        if(inteval<0){
            cout<<"The function "<<name<<" does not range from "<<start<<" to "<<end<<endl;
            return 0;
        }
        time+=inteval;
    }
    return time/(end-start+1);
}

double TraceAnalysis::findFunctionClosure(){

    string line;
    getline(file,line);
    Line begin=extractLine(line);


    Line end;
    while(1){
        int cursor=file.tellg();

        getline(file,line);
        if(line.find(endMark)!=-1){
            return -1;
        }
        end=extractLine(line);

        if(end.type=='B'){
            file.seekg(cursor,ios::beg);

            if(findFunctionClosure()<0){
              return -1;
            }
        }
        else if(end.type=='E')
            break;
        else continue;
    }

    return end.ts-begin.ts;

}

double TraceAnalysis::getNextFunctionTime(string thread,string name){
    while(1){
        int cursor=file.tellg();
        string line;
        getline(file,line);

        if(line.find(endMark)!=-1){
            return -1;
        }
        Line begin=extractLine(line);
        if(begin.thread== thread && begin.name==name){
           file.seekg(cursor,ios::beg);
           return findFunctionClosure();
        }
    }
}

Line TraceAnalysis::extractLine(string line){
     //  cout<<"line:"<<line<<endl;
    string thread,name;
    int index=line.find_first_not_of(" ");
    line=line.substr(index);
    index=line.find("-");
    thread=line.substr(0,index);
    if((index=thread.find(" "))!=-1)
       thread=thread.substr(0,index);

    index=line.find(":");
    string tmp=line.substr(0,index);
    index=tmp.find_last_of(" ");
    tmp=tmp.substr(index+1);
    double ts=atof(tmp.c_str());

    index=line.find_last_of(":");
    if(line[index+2]=='E'){

        return {thread,ts,'E',""};
    }
    else if((line[index+2]=='B')){
        index=line.find_last_of("|");

        int index2=line.find("\\");

        return {thread,ts,'B',line.substr(index+1,index2-index-1)};
    }else{
        return {"",0,0,""};
    }

}
