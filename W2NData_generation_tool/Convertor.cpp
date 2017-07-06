#include "Convertor.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
enum InstanceType {MACRO=1,GLCHAR,GLINT,GLUINT,GLBYTE,GLUBYTE,GLFLOAT,GLSHORT,GLUSHORT,GLFUNCTION};
Convertor::Convertor(string url,string dir){
    file.open(url.c_str(),ios::out);
    file.seekp(sizeof(int)*3,ios::beg);
    this->dir=dir;

}

void Convertor::convertMacro(){
    string name=dir+"/macro.txt";
    ifstream in(name.c_str());
    if (!in){
        cout << "Cannot open macro.txt" << endl;
        return ;
    }

    while (1){
        string s;
        getline(in, s);
        if (in.fail())
            break;
        stringstream ss(s);
        string tmp, key;
        int value;
        ss >> tmp >> key >> value;
        char c=(char)MACRO;
        file.write((char *)&c,sizeof(char));
        int i=key.size()+sizeof(int);
        file.write((char *)&i,sizeof(int));
        file.write(key.c_str(),key.size());
        file.write((char*)&value,sizeof(int));


    }
}


void Convertor::convertGlobal(){
    GCursor=file.tellp();
    int i = 1;
    char n[10];
    sprintf(n, "%d", i);
    while (parser.load(dir+"/global" + string(n) + ".txt")){
        Variable v;
        while (1){
            v = parser.parseSingleGlobal();
            if (v.value == 0)
                break;

            cout<<v.name<<" ";
            cout<<v.size<<endl;
            /*
            switch(v.type){
            case glchar:
               cout<<(char *)v.value;break;
            case glubyte:
               for(int j=0;j<v.size;j++)
                   cout<<(int)((char *)v.value)[j]<<" ";
               break;
            case gluint:
               for(int j=0;j<v.size/4;j++)
                   cout<<(int)((int *)v.value)[j]<<" ";
               break;
            case glushort:
               for(int j=0;j<v.size/2;j++)
                   cout<<(int)((short *)v.value)[j]<<" ";
               break;
            case glfloat:
               for(int j=0;j<v.size/4;j++)
                   cout<<((float *)v.value)[j]<<" ";
               break;
            }
            cout<<endl;
            */
            char e;
            int tl,nl;
            switch(v.type){
            case glchar:
                e=(char)GLCHAR;
                break;
            case glint:
                e=(char)GLINT;
                break;
            case gluint:
                e=(char)GLUINT;
                break;
            case glbyte:
                e=(char)GLBYTE;
                break;
            case glubyte:
                e=(char)GLUBYTE;
                break;
            case glfloat:
                e=(char)GLFLOAT;
                break;
            case glshort:
                e=(char)GLSHORT;
                break;
            case glushort:
                e=(char)GLUSHORT;
                break; 
            }
            tl=v.size+v.name.size()+sizeof(int);
            nl=v.name.size();
            file.write((char*)&e,sizeof(char));
            file.write((char*)&tl,sizeof(int));
            file.write((char*)&nl,sizeof(int));
            file.write(v.name.c_str(),nl);
            file.write((char*)v.value,v.size);

            delete[] v.value;
        }
        i++;
        sprintf(n, "%d", i);

    }
    cout<<"Global done\n";
}
void Convertor::convertInit(){
    ICursor=file.tellp();
    string name=dir+"/init.txt";
    ifstream in(name.c_str());

    string s;
    getline(in,s);

    while(!in.fail()){
        char e=(char)GLFUNCTION;
        file.write((char*)&e,sizeof(char));
        int l=s.size()-1;
        file.write((char*)&l,sizeof(int));
        file.write(s.c_str(),s.size()-1);
        getline(in,s);
    }
    cout<<"init done\n";
}

void Convertor::convertFunction(){
    FCursor=file.tellp();
    ifstream in;
    char n[10];
    int t=1;
    sprintf(n,"%d",t);
    string name=dir+"/function"+string(n)+".txt";
    in.open(name.c_str());
    while(in){
        string s;
        getline(in,s);

        while(!in.fail()){
            if(s.find("W2N CONTINUE")!=-1)
                break;
            char e=(char)GLFUNCTION;
            file.write((char*)&e,sizeof(char));
            int l=s.size()-1;
            file.write((char*)&l,sizeof(int));
            file.write(s.c_str(),s.size()-1);
            getline(in,s);
        }
        t++;
        sprintf(n,"%d",t);
        name=dir+"/function"+string(n)+".txt";
        in.close();
        in.open(name.c_str());
    }
    cout<<"function done\n";
}

void Convertor::setConfig(){
    file.seekp(ios::beg);
    file.write((char *)&GCursor,sizeof(int));
    file.write((char *)&ICursor,sizeof(int));
    file.write((char *)&FCursor,sizeof(int));
}


