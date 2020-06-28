#include "Graph_Betweenness.h"

#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<iostream>
#include<iomanip>

using namespace std;

Graph_Betweenness::Graph_Betweenness(void)
{
	node_num=0;
	edge_num=0;
	distance_MAX=INT_MAX;        //make the indefinite distance become INT_MAX km
}

Graph_Betweenness::~Graph_Betweenness(void)
{
	delete[] dis;
	for(int i=0;i<node_num;i++)
	{
		delete arc[i];
	}
	delete arc;
}

void Graph_Betweenness::readVertex(ifstream &fin)
{
	string line;
	string vt1,vt2,dis3;        //three temproal variable for one line of the input
	while(getline(fin,line))    //read line by line
	{
		istringstream sin(line);   
		string field;
		int i=1;
		while (getline(sin,field,','))   
		{
			if(i==1)       
			{
				vt1=field;     //put starting node first
				i++;
				continue;
			}
			if(i==2)
			{
				vt2=field;      //put reachable out-degree node
				i++;
				continue;
			}
			if(i==3)
			{
				dis3=field;
				i++;
				break;
			}	
		}
		vertexO.push_back(convertStringNumberToInt(vt1));     //there might be some bugs if the edge cost<0 or node num is wrong
		vertexD.push_back(convertStringNumberToInt(vt2));     //because there is no validation for node
		dis_value.push_back(convertStringNumberToInt(dis3));	
	}
}

int Graph_Betweenness::convertStringNumberToInt(string str1)
{
	double n1;
	const char* ch= str1.c_str();
	n1=atof(ch);                   //the result will be zero instead of double for none-num
	return int(n1);
}

int Graph_Betweenness::countNodeAndEdge(vector<int> &NE)
{
	pair<map<int,int>::iterator,bool> Insert_Pair;
	map<int,int> tempNode;
	int myNum=0;
	for(unsigned int i=0;i<NE.size();i++)
	{
		Insert_Pair=tempNode.insert(pair<int,int>(NE[i],1));   //put none-repeatable node by using map
		if(Insert_Pair.second==true)
		{
			myNum++;
		}
	}
	if(myNum!=tempNode.size())
	{
		cout<<"warning: the num of nodes is wrong!"<<endl;
		system("pause");
	}
	this->node_num=tempNode.size();
	this->edge_num=NE.size()/2;    //the num of edge will always be twice of the edges we see directly because of undirected graph
	//cout<<this->node_num<<"  "<<this->edge_num<<endl;
	cout<<"The num of node and edge is: "<<node_num<<"  "<<edge_num<<endl;
	return 0;
}

void Graph_Betweenness::createGraph()
{
	//initialise these two arrays but remeber to delete them
	dis=new Dis[node_num];
	arc=new int*[node_num];
	for(int i=0;i<node_num;i++)
	{
		arc[i]=new int[node_num];
		for(int j=0;j<node_num;j++)
		{
			arc[i][j]=distance_MAX;
		}
	}

	//assign value to some parts of Adjacency Matrix for inputed edges
	for(unsigned int i=0;i<vertexO.size();i++)
	{
		arc[vertexO[i]-1][vertexD[i]-1]=dis_value[i];
	}
}

void Graph_Betweenness::printGraph()
{
	//print the Adjacency Matrix to check the correctness
	ofstream outfile1;
	outfile1.open("..\\result\\printGraph.csv");
	//outfile1.open("D:\\data\\深圳数据\\修剪数据3\\printGraph.csv");
	
	for(int i=0;i<node_num;i++)
	{
		for(int j=0;j<node_num;j++)
		{
			if(arc[i][j]==INT_MAX)    //it would be an indefinite value if unacessable directly
			{
				outfile1<<"∞"<<",";
			}
			else
			{
				outfile1<<arc[i][j]<<",";
			}
		}
		outfile1<<endl;
	}
	outfile1.close();
}

void Graph_Betweenness::dijkstra(int begin)     //this 'begin' index here starts from not zero but one 
{
	//initialise dis array first
	for(int i=0;i<node_num;i++)
	{
		dis[i].path="v"+toString(begin)+"-->v"+toString(i+1);         //set current path
		dis[i].cost=arc[begin-1][i];
	}
	//set the distance to zero for nodes themselves
	dis[begin-1].cost=0;
	dis[begin-1].visit=true;
	//calculate the distance for the residue
	int countLeft=1;
	while(countLeft!=node_num)
	{
		int tempPos=0;              //remember the index of the minimum dis
		int tempMin=distance_MAX;   //remember current minimum value and see its other out-degree and updating
		for(int i=0;i<node_num;i++)
		{
			if(!dis[i].visit&&dis[i].cost<tempMin)
			{
				tempMin=dis[i].cost;
				tempPos=i;            //find the index of current minimum at this iteration
			}
		}
		//put the value in to indicate it has found the shortest path for this node
		dis[tempPos].visit=true;
		countLeft++;
		//看找到的那个最小值去看他的所有出度，也就是遍历一遍顶点个数，希望找到非∞的数即为出度
		for(int i=0;i<node_num;i++)
		{
			if(arc[tempPos][i]==distance_MAX||dis[i].visit)    //如果当前点的出度为无穷则不可能通过它缩短距离
			{                                                   //同时，如果这个点的出度已经被确定了最短距离，则不用比较了
				continue;
			}
			int tempCost=arc[tempPos][i]+dis[tempPos].cost;   //记录一下当前的松弛距离，后面会和直接去的距离进行比较
			if(tempCost<dis[i].cost)
			{
				dis[i].cost=tempCost;
				dis[i].path=dis[tempPos].path+"-->v"+toString(i+1);
				
			}
		}
	}
	//while loop over and try to see print and validate
}

string Graph_Betweenness::toString(int number)
{
	stringstream stream;
	stream<<number;
	string caa=stream.str();
	return caa;
}

void Graph_Betweenness::printPath(int begin)
{
	ofstream outfile1;
	outfile1.open("..\\result\\printPath.csv");
	//outfile1.open("D:\\data\\深圳数据\\修剪数据3\\printPath.csv");

	string vtr;
	vtr="v"+toString(begin);
	outfile1<<"start at "<<vtr<<", and its shortest path are:"<<endl;
	for(int i=0;i<node_num;i++)
	{
		if(dis[i].cost!=distance_MAX)
		{
			outfile1<<dis[i].path<<"="<<dis[i].cost<<endl;
		}
		else
		{
			outfile1<<dis[i].path<<"there is no shortes path"<<endl;
		}
	}
	outfile1.close();
}

void Graph_Betweenness::checkInputVE()
{
	//the input vertexO is sorted already, then use algorithm sort again to check whether input is actually ordered
	vector<int> sortedVet;
	insertionSort(sortedVet,vertexO); 
	if(sortedVet.size()!=vertexO.size())
	{
		cout<<"forced stop :sortedVet.size()!=vertexO.size()!!!!"<<endl;
		system("pause");
	}
	//check the input order
	for(unsigned int i=0;i<vertexO.size();i++)
	{
		if(sortedVet[i]!=vertexO[i])
		{
			cout<<"forced stop :sortedVet[i]!=vertexO[i]!!!!        ---> "<<i<<endl;
			system("pause");
		}
	}
	sortedVet.clear();
	cout<<"the order of the input node id is non-descending right? finished!"<<endl; 

	//check the input vertexD
	vector<int> sortedVetD;
	insertionSort(sortedVetD,vertexD);
	if(sortedVetD.size()!=vertexD.size())
	{
		cout<<"forced stop :sortedVetD.size()!=vertexD.size()!!!!"<<endl;
		system("pause");
	}
	for(unsigned int i=0;i<vertexO.size();i++)
	{
		if(sortedVetD[i]!=vertexO[i])
		{
			cout<<"forced stop :sortedVetD[i]!=vertexO[i]!!!!        ---> "<<i<<endl;
			system("pause");
		}
	}
	sortedVetD.clear();
	cout<<"the order of the input node id is exactly the same as the sorted result? Yes!"<<endl;

	//to check the node id from input is strictly sorted and whether it is symmetric V1->V2==V2->V1
	createGraph();                //create arc so as to check it
	for(int i=0;i<node_num;i++)
	{
		for(int j=0;j<node_num;j++)
		{
			if(arc[i][j]!=arc[j][i])
			{
				cout<<"forced stop :arc[i][j]!=arc[j][i]!!!!        ---> "<<i+1<<"    "<<j+1<<endl;
				system("pause");
			}
		}
	}
	cout<<"check whether V(i)->V(i+1)==V(i)->V(i+1) is symmetry? Yes"<<endl;
	cout<<"checking over, seems good!"<<endl<<endl<<endl;
}
void Graph_Betweenness::insertionSort(vector<int> &vec1,vector<int> &vec2)
{
	vec1=vec2; // in order to use vec2 to fill up the empty vec1
	int temp;
	int times=10000;
	int insertPos;
	for(unsigned int i=1;i<vec2.size();i++) 
	{
		temp=vec1[i];                                         //element waiting to insert
		insertPos=i;                                          ////if no exchange below, it might already the biggest when inserting
		for(unsigned int j=i-1;j>=0 && temp<vec1[j];j--)  
		{
			vec1[j+1]=vec1[j];                                //stable sort, relative order for those equal elements will not change
			insertPos=j;
			if(j==0)
			{
				//cout<<"j is unsigned,so no j-- when j==0, choose to break"<<endl;
				break;
			}
		}
		vec1[insertPos]=temp;

		if(i==times)
		{
			cout<<i<<endl;
			times=times+10000;
		}
	}
}
void Graph_Betweenness::printAllPath()
{
	//print as 167*167 size, allPath[i][j] represents the specific path for i node to j node
	ofstream outfile1;
	outfile1.open("..\\result\\printAllPath.csv");
	//outfile1.open("D:\\data\\深圳数据\\修剪数据3\\printAllPath.csv");               

	cout<<"printing all path to the file: "<<endl;
	int tempBegin=0;
	for(int i=0;i<node_num;i++)
	{
		dijkstraAll(i+1);                      //will build dis array
		tempBegin=i+1;
		for(int j=0;j<node_num;j++)
		{
			if(dis[j].cost!=distance_MAX)
			{
				outfile1<<allPath[i][j]<<",";
			}
			else
			{
				outfile1<<dis[i].path<<"There is no shortest path"<<endl;
				system("pause");
			}
		}
		outfile1<<endl;
		for(int j=0;j<node_num;j++)              //run dijkstraAll 167(node num) times, every time you need to free the old dis array 
		{
			dis[j].cost=0;
			dis[j].visit=false;
		}
		if ((i+1)%10==0) cout<<"dijkstraAll function run at: "<<i+1<<"th times"<<endl;
	}
	outfile1.close();
}
void Graph_Betweenness::dijkstraAll(int begin)
{
	//initialise allPath matrix first
	vector<string> temp;
	allPath.push_back(temp);              //the num of allPath's row will be equal to the times that dijkstraAll was called

	//initialise dis array first
	for(int i=0;i<node_num;i++)
	{
		dis[i].cost=arc[begin-1][i];
		allPath[begin-1].push_back(toString(begin)+" "+toString(i+1));
	}
	//set the distance to zero for nodes themselves
	dis[begin-1].cost=0;
	dis[begin-1].visit=true;
	//calculate the distance for the residue
	int countLeft=1;
	while(countLeft!=node_num)
	{
		int tempPos=0;              //remember the index of the minimum dis
		int tempMin=distance_MAX;   //remember current minimum value and see its other out-degree and updating
		for(int i=0;i<node_num;i++)
		{
			if(!dis[i].visit&&dis[i].cost<tempMin)
			{
				tempMin=dis[i].cost;
				tempPos=i;            //find the index of current minimum at this iteration
			}
		}
		//put the value in to indicate it has found the shortest path for this node
		dis[tempPos].visit=true;
		countLeft++;
		//看找到的那个最小值去看他的所有出度，也就是遍历一遍顶点个数，希望找到非∞的数即为出度
		for(int i=0;i<node_num;i++)
		{
			if(arc[tempPos][i]==distance_MAX||dis[i].visit)    //如果当前点的出度为无穷则不可能通过它缩短距离
			{                                                   //同时，如果这个点的出度已经被确定了最短距离，则不用比较了
				continue;
			}
			int tempCost=arc[tempPos][i]+dis[tempPos].cost;   //记录一下当前的松弛距离，后面会和直接去的距离进行比较
			if(tempCost<dis[i].cost)
			{
				dis[i].cost=tempCost;
				allPath[begin-1][i]=allPath[begin-1][tempPos]+" "+toString(i+1);
			}
		}
	}
	//while loop over and try to see print and validate
}
void Graph_Betweenness::calculateStopsBC()
{
	ofstream outfile1;
	outfile1.open("..\\result\\calculateStopsBC.csv");
	//outfile1.open("D:\\data\\深圳数据\\修剪数据3\\calculateStopsBC.csv");   

	vector<double> BetweennessCentrality;
	string tempBC;
	cout<<endl<<"calculating the betweenness:"<<endl;
	//three for loops to calculate the node betweeness in the graph
	for(int i=0;i<node_num;i++)
	{
		BetweennessCentrality.push_back(0);
		for(int j=0;j<node_num;j++)
		{
			for(int k=0;k<node_num;k++)
			{
				if(i==j || i==k || j==k)                    //i,j,k cannot be equal for any of them
				{
					continue;
				}
				tempBC=allPath[j][k];
				if(isInCurrentStops(tempBC,i+1))
				{
					BetweennessCentrality[i]++;
				}
			}
		} 
		if ((i+1)%10==0) cout<<i+1<<"/"<<node_num<<endl;
	}
	double BCmax=0;
	for(int i=0;i<node_num;i++)
	{
		if(BetweennessCentrality[i]>BCmax)
		{
			BCmax=BetweennessCentrality[i];
		}
	}
	for(unsigned int i=0;i<BetweennessCentrality.size();i++)
	{
		outfile1<<fixed<<setprecision(6)<<i+1<<","<<BetweennessCentrality[i]<<","<<BetweennessCentrality[i]/BCmax<<endl;
	}
	outfile1.close();
}
bool Graph_Betweenness::isInCurrentStops(string str1,int is_I)
{
	istringstream sin(str1);   
	string field;
	while (getline(sin,field,' '))
	{
		if(field==toString(is_I))
		{
			return true;
		}
	}
	return false;
}

void Graph_Betweenness::run()
{
	//read the Shenzhen subway graph with 167 stations for calculating the station betweeness, in order to pick theoretical POI
	ifstream finXY("..\\data\\subwayToSubway.csv");
	readVertex(finXY);                          //read as the form of "start node->neighbor node->edge cost"
	countNodeAndEdge(vertexO);                  //count the total num of the non-repeatable nodes in the graph
	checkInputVE();                             //check whether the neighbor realtion has some issues
	printGraph();                               //print the Adjacency Matrix to see the correctness
	printAllPath();                             //store all the shortest path string for every node to all other nodes in csv file
	calculateStopsBC();                         //betweeness calculation started
	dijkstra(18);                               //try an example
	printPath(18);                              //you can set any node id from 1 to 167
}