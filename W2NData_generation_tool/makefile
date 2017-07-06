convertTool : tool.o Convertor.o Parser.o TraceAnalysis.o
	g++ -o tool tool.o Convertor.o Parser.o  TraceAnalysis.o -g

tool.o : tool.cpp Convertor.h TraceAnalysis.h
	g++ -std=c++11 -c tool.cpp -g

Convertor.o : Convertor.cpp Convertor.h Parser.h
	g++ -std=c++11 -c Convertor.cpp -g
	
Parser.o : Parser.cpp Parser.h
	g++ -std=c++11 -c Parser.cpp -g
    
TraceAnalysis.o : TraceAnalysis.cpp TraceAnalysis.h
	g++ -std=c++11 -c TraceAnalysis.cpp -g
     
clean:
	rm tool *.o
