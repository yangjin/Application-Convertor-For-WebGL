#include "Object.h"
#include <stdlib.h>
#include <string.h>
using namespace std;

Macro::Macro(){}

void Macro::deserialize(ifstream &file){
   char a[50]={0};
   char t;
   file.read((char *)&t,sizeof(char));
   type=(Type)t;
   file.read((char *)&length,sizeof(int));
   file.read(a,length-sizeof(int));
   name=string(a);
   file.read((char *)&value,sizeof(int));

   return ;

}

Glglobal::Glglobal(){}

void Glglobal::deserialize(ifstream &file){
   char a[50]={0};
   int nl;
   char t;
   file.read((char *)&t,sizeof(char));
   type=(Type)t;
   file.read((char *)&length,sizeof(int));
   file.read((char*)&nl,sizeof(int));
   file.read(a,nl);
   name=string(a);
   size=length-nl-sizeof(int);
   value=malloc(size+1);
   memset(value,0,size+1);
   file.read((char*)value,size);

}

Sentence::Sentence(){}

void Sentence::deserialize(ifstream &file){
   char t;
   file.read((char *)&t,sizeof(char));
   type=(Type)t;
   file.read((char *)&length,sizeof(int));
   char * a=(char * )malloc(length+2);
   memset(a,0,length+2);
   file.read(a,length);
   sentence=string(a);  
   free(a);
}
