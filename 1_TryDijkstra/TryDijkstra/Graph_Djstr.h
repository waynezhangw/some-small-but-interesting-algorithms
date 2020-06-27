#pragma once           //prevent headfile being included multiple times

#include<vector>
#include<string>
using namespace std;

//define a struct of 'dis' for using visit symbol, 
//otherwise we have to build an array to judge whether the node is visited
struct Dis {
	string path; 
	int cost;         
	bool visit;
	Dis() {
		path="";
		cost=0;
		visit=false;
	}
};


class Graph_Djstr
{
public:
	Graph_Djstr(void);
	~Graph_Djstr(void);

	void readVertex(ifstream &fin);                     //read graph structure, including out-degree and edge cost
	int convertStringNumberToInt(string str1);          //convert string to int
	int countNodeAndEdge(vector<int> &);                //count the number of nodes and edges in the graph
	void createGraph();                                 //build the Adjacency Matrix
	void printGraph();                                  //print the Adjacency Matrix to see the correctness
	void dijkstra(int begin);                           //the core idea of the shortest distance
	string toString(int number);                        //convert string int to string
	void printPath(int begin);                          //print all the path from the current node




private:
	int node_num;        //the number of node, initialized by input
	int edge_num;
	int distance_MAX;    //define indefinite distance
	int **arc;           //define the Adjacency Matrix to store the graph
	Dis *dis;            //record the distance from starting node to other nodes, will keep updating this array
	

	vector<int> vertexO;         //the current start node from reading
	vector<int> vertexD;         //the heading to node from current node
	vector<int> dis_value;       //the distance for a pair node

};

