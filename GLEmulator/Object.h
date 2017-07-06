#ifndef OBJECT_H
#define OBJECT_H
#include <fstream>
#include <GLES2/gl2.h>
enum Type {MACRO=1,GLCHAR,GLINT,GLUINT,GLBYTE,GLUBYTE,GLFLOAT,GLSHORT,GLUSHORT,SENTENCE};
class Object{
public:
    Object(){}
    virtual ~Object(){}
    virtual  void deserialize(std::ifstream&) = 0 ;
protected:
    Type type;
    int length;

};

class Macro :public Object{
public:
    Macro();
    virtual ~Macro(){}
    virtual  void deserialize(std::ifstream&);
    int getValue(){return value;}
    std::string getName(){return name;}
private:
    std::string name;
    int value;
};

class Glglobal :public Object {
public:
    Glglobal();
    virtual ~Glglobal(){}
    virtual  void deserialize(std::ifstream&);
    std::string getName() {return name;}
    void * getValue() {return value;}
    int getSize(){return size;}
    Type getType(){return type;}
private:
    std::string name;
    void* value;
    int size;
};

class Sentence :public Object {
public:
    Sentence();
    virtual ~Sentence(){}
    std::string getSentence(){return sentence;}
    virtual  void deserialize(std::ifstream&);
private:
    std::string sentence;
};


#endif
