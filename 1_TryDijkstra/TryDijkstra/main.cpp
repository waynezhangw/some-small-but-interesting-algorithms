
#include<iostream>
#include<fstream>
#include<time.h>


#include "Graph_Djstr.h"

using namespace std; 

int main() {
	clock_t start,end;   
	start=clock();    //get the time of running at this line

	ifstream finXY("..\\data\\eightNodesGraph.csv");          //read graph example with eight nodes
	Graph_Djstr Me;
	Me.readVertex(finXY);

	end=clock();
	cout<<"running time:  "<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	system("pause");
	return 0;
}