
#include<iostream>
#include<fstream>
#include<time.h>


#include "Graph_Betweenness.h"

using namespace std; 

int main()
{
	clock_t start,end;   
	start=clock(); 
	
	Graph_Betweenness* Me=new Graph_Betweenness();
	Me->run();
	delete Me;
	Me=NULL;

	end=clock();
	cout<<"running time:  "<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	system("pause");
	return 0;
}