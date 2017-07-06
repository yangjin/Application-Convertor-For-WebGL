#include <iostream>
#include <fstream>
#include <string>
#include "Convertor.h"
#include "TraceAnalysis.h"
#include <map>
#include <unordered_map>
#include <sstream>
using namespace std;

const static string file="W2NData";
const static string reverseTxtFile="TXTData";
void dump2Binary(string dir){
    Convertor c(file,dir);
    c.convertMacro();
    c.convertGlobal();
    c.convertInit();
    c.convertFunction();
    c.setConfig();

}
void optimize(string url)
{
	ifstream table(url+"/table.txt");
	if(table)
	{
		cout<<"optimized"<<endl;
		return;
	}else
		table.close();
	map<int,unordered_map<string,int> > m;
	map<int,int> change;
	int arrNumber = 1;	

	ifstream file(url+"/global1.txt");
	ofstream outfile(url+"/global1(modified).txt");
	string temp;
	string pre;
	int a=0;
	while(getline(file,temp))
	{
		a++;
		if(a%100==0)
			cout<<a<<endl;
		if(temp.compare(0,16,"GLfloat g_Array_")!=0&&temp.compare(0,17,"GLushort g_Array_")!=0&&temp.compare(0,16,"GLubyte g_Array_")!=0)
		{
			outfile<<temp<<endl;
			continue;
		}	
		if(temp.compare(0,16,"GLfloat g_Array_")==0)
			pre = "GLfloat g_Array_";
		else if(temp.compare(0,17,"GLushort g_Array_")==0)
			pre = "GLushort g_Array_";
		else if(temp.compare(0,16,"GLubyte g_Array_")==0)
			pre = "GLubyte g_Array_";
		int i=pre.size();
		stringstream ss;
    		ss << arrNumber;
		string str;
		ss >> str;
		if(temp.compare(i,str.size(),str)!=0)
		{
			cout<<"wrong2"<<endl;
			cout<<temp.substr(0,20)<<endl;
			break;
		}else
		{
			i+=str.size();
			if(temp[i]=='[')
				i++;
			else
			{
				cout<<"wrong"<<endl;
				cout<<temp.substr(0,20)<<endl;
				break;
			}
			int size = 0;
			while(temp[i]!=']')
			{
				size*=10;
				size+=int(temp[i]-'0');
				i++;
			}
			i+=4;
			unordered_map<string,int> tempMap = m[size];
			unordered_map<string,int>::iterator it = tempMap.find(temp.substr(i));
			if(it==tempMap.end())
			{
				tempMap[temp.substr(i)] = arrNumber;
				m[size] = tempMap;
				outfile<<temp<<endl;
			}else
			{
				int preNumber = it->second;
				stringstream ss2;
		    		ss2 << preNumber;
				string str2;
				ss2 >> str2;
				change[arrNumber]=preNumber;
			}
			arrNumber++;
		}
		    
	}
	file.close();
	outfile.close();
	string path1,path2;
	path1 = url + "/global1(modified).txt";
	path2 = url + "/global1.txt";
	rename(path1.c_str(),path2.c_str());
	cout<<"global1.txt done"<<endl;
	file.open(url+"/init.txt");
	outfile.open(url+"/init(modified).txt");
	while(getline(file,temp))
	{
		map<int,int>::iterator it= change.begin();
		while(it!=change.end())
		{
			stringstream first,second;
	    		first << it->first;
	    		second << it->second;
			string firstStr,secondStr;
			first >> firstStr;
			second >> secondStr;
			firstStr="g_Array_"+firstStr;
			secondStr="g_Array_"+secondStr;
			std::size_t found = temp.find(firstStr);
			if(found!=string::npos&&(temp.at(found+firstStr.size())>'9'||temp.at(found+firstStr.size())<'0'))
			{
				temp = temp.substr(0,found)+secondStr+temp.substr(found+firstStr.size());
				found = temp.find(firstStr);
				if(found!=string::npos)
				{
					temp = temp.substr(0,found)+secondStr+temp.substr(found+firstStr.size());
				}
				break;
			}
			++it;
		}
		outfile<<temp<<endl;
	}
	file.close();
	outfile.close();

	path1 = url + "/init(modified).txt";
	path2 = url + "/init.txt";
	rename(path1.c_str(),path2.c_str());
	cout<<"init.txt done"<<endl;
	file.open(url+"/function1.txt");
	outfile.open(url+"/function1(modified).txt");
	a=0;
	map<int,int>::iterator it= change.begin();
	while(getline(file,temp))
	{
		a++;
		if(a%100==0)
			cout<<a<<endl;
		stringstream first,second;
    		first << it->first;
    		second << it->second;
		string firstStr,secondStr;
		first >> firstStr;
		second >> secondStr;
		firstStr="g_Array_"+firstStr;
		secondStr="g_Array_"+secondStr;
		std::size_t found = temp.find(firstStr);
		if(found!=string::npos&&(temp.at(found+firstStr.size())>'9'||temp.at(found+firstStr.size())<'0'))
		{
			temp = temp.substr(0,found)+secondStr+temp.substr(found+firstStr.size());
			found = temp.find(firstStr);
			if(found!=string::npos)
			{
				temp = temp.substr(0,found)+secondStr+temp.substr(found+firstStr.size());
			}
		}else
		{
			outfile<<temp<<endl;
			continue;
		}
		outfile<<temp<<endl;
		++it;
		if(it==change.end())
			break;
	}
	file.close();
	outfile.close();
	path1 = url + "/function1(modified).txt";
	path2 = url + "/function1.txt";
	rename(path1.c_str(),path2.c_str());
	outfile.open(url+"/table.txt");
	it= change.begin();
	while(it!=change.end())
	{
		outfile<<it->first<<" "<<it->second<<endl;
		++it;
	}
	outfile.close();
	cout<<"function.txt done"<<endl;
	return;
}

void analysis(string url){

    TraceAnalysis a(url);

    if(!a.isExist()){
        cout<<url<<" does not exist!\n";
        return;
    }


    cout<<"setupGraphics time : "<<a.getAverageTime("GLThread","setupGraphics",1,1)*1000<<"ms"<<endl;
    cout<<"First 900 frames"<<endl;

    cout<<"average renderFrame time : "<<a.getAverageTime("GLThread","renderFrame",1,900)*1000<<"ms"<<endl;
    cout<<"average eglSwapBuffers time : "<<a.getAverageTime("GLThread","eglSwapBuffers",1,900)*1000<<"ms"<<endl;
    cout<<endl;
    cout<<"Next 600 frames"<<endl;
    cout<<"average renderFrame time : "<<a.getAverageTime("GLThread","renderFrame",901,1500)*1000<<"ms"<<endl;
    cout<<"average eglSwapBuffers time : "<<a.getAverageTime("GLThread","eglSwapBuffers",901,1500)*1000<<"ms"<<endl;

}

void help(){
    cout<<"--optimize --d2b dirurl to optimize dumped file and make binary data from dumped file.\n";
    cout<<"--d2b dirurl to make binary data from dumped file.\n";
    cout<<"--analysis to analysis the trace file.\n";

}

int main(int argc ,char **argv){

    if (argc < 2){
        cout<<"Too few parameter. Use --help for help!"<<endl;
        return 0;
    }
    else if(string(argv[1])=="--d2b"){
        if(argc<3){
            cout<<"Too few parameter in --d2b,please input the dump file url\n";
            return 0;
        }
        dump2Binary(string(argv[2]));
        return 0;
    }
    else if(string(argv[1])=="--help"){
        help();
        return 0;
    }
    else if(string(argv[1])=="--analysis"){
        if(argc<3){
            cout<<"Too few parameter in --analysis,please input the trace url\n";
            return 0;
        }
        analysis(string(argv[2]));
        return 0;
    }else if(string(argv[1])=="--optimize" && string(argv[2])=="--d2b"){
        if(argc<4){
            cout<<"Too few parameter in --analysis,please input the trace url\n";
            return 0;
        }
	optimize(string(argv[3]));
        dump2Binary(string(argv[3]));
        return 0;
    }
    else {
        cout<<"No such parameter!\n";
    }
    return 0;
}
