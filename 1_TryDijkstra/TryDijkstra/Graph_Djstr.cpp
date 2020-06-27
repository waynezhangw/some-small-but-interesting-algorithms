#include "Graph_Djstr.h"

#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<iostream>


using namespace std;

Graph_Djstr::Graph_Djstr(void)
{
	node_num=0;
	edge_num=0;
	distance_MAX=INT_MAX;        //make the indefinite distance become INT_MAX km
}

Graph_Djstr::~Graph_Djstr(void)
{
	delete[] dis;
	for(int i=0;i<node_num;i++)
	{
		delete arc[i];
	}
	delete arc;
}

void Graph_Djstr::readVertex(ifstream &fin)
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
	countNodeAndEdge(vertexO);
	createGraph();
	printGraph();
	//start Dijkstra's Algorithm, build dis array
	dijkstra(3);
	printPath(3);
}

int Graph_Djstr::convertStringNumberToInt(string str1)
{
	double n1;
	const char* ch= str1.c_str();
	n1=atof(ch);                   //the result will be zero instead of double for none-num
	return int(n1);
}

int Graph_Djstr::countNodeAndEdge(vector<int> &NE)
{
	map<int,int> tempNode;
	for(unsigned int i=0;i<NE.size();i++)
	{
		tempNode.insert(pair<int,int>(NE[i],1));   //put none-repeatable node by using map
	}
	this->node_num=tempNode.size();
	this->edge_num=NE.size()/2;    //the num of edge will always be twice of the edges we see directly because of undirected graph
	//cout<<this->node_num<<"  "<<this->edge_num<<endl;
	//cout<<node_num<<"  "<<edge_num<<endl;
	return 0;
}

void Graph_Djstr::createGraph()
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

void Graph_Djstr::printGraph()
{
	//print the Adjacency Matrix to check the correctness
	cout<<"Adjacency Matrix:"<<endl;
	for(int i=0;i<node_num;i++)
	{
		for(int j=0;j<node_num;j++)
		{
			if(arc[i][j]==INT_MAX)    //it would be an indefinite value if unacessable directly
			{
				cout<<"��"<<"\t";
			}
			else
			{
				cout<<arc[i][j]<<"\t";
			}
		}
		cout<<endl;
	}
}

void Graph_Djstr::dijkstra(int begin)     //this 'begin' index here starts from not zero but one 
{
	//initialise dis array first
	for(int i=0;i<node_num;i++)
	{
		dis[i].path="v"+toString(begin)+"-->v"+toString(i+1);   //set current path
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
		//���ҵ����Ǹ���Сֵȥ���������г��ȣ�Ҳ���Ǳ���һ�鶥�������ϣ���ҵ��ǡ޵�����Ϊ����
		for(int i=0;i<node_num;i++)
		{
			if(arc[tempPos][i]==distance_MAX||dis[i].visit)    //�����ǰ��ĳ���Ϊ�����򲻿���ͨ�������̾���
			{                                                   //ͬʱ����������ĳ����Ѿ���ȷ������̾��룬���ñȽ���
				continue;
			}
			int tempCost=arc[tempPos][i]+dis[tempPos].cost;   //��¼һ�µ�ǰ���ɳھ��룬������ֱ��ȥ�ľ�����бȽ�
			if(tempCost<dis[i].cost)
			{
				dis[i].cost=tempCost;
				dis[i].path=dis[tempPos].path+"-->v"+toString(i+1);
			}
		}
	}
	//while loop over and try to see print and validate
}

string Graph_Djstr::toString(int number)
{
	stringstream stream;
	stream<<number;
	string caa=stream.str();
	return caa;
}

void Graph_Djstr::printPath(int begin)
{
	string vtr;
	vtr="v"+toString(begin);
	cout<<"the shortest path for "<<vtr<<" is: "<<endl;
	for(int i=0;i<node_num;i++)
	{
		if(dis[i].cost!=distance_MAX)
		{
			cout<<dis[i].path<<" = "<<dis[i].cost<<endl;
		}
		else
		{
			cout<<dis[i].path<<"has no shortest path"<<endl;
		}
	}
}