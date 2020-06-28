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


class Graph_Betweenness
{
public:
	Graph_Betweenness(void);
	~Graph_Betweenness(void);

	void readVertex(ifstream &fin);                     //read graph structure, including out-degree and edge cost
	int convertStringNumberToInt(string str1);          //convert string to int
	int countNodeAndEdge(vector<int> &);                //count the number of nodes and edges in the graph
	void createGraph();                                 //build the Adjacency Matrix
	void printGraph();                                  //print the Adjacency Matrix to see the correctness
	void dijkstra(int begin);                           //the core idea of the shortest distance
	string toString(int number);                        //convert string int to string
	void printPath(int begin);                          //print all the path from the current node

	void checkInputVE();                       //to check the node id from input is strictly sorted and whether it is symmetric V1->V2==V2->V1
	void insertionSort(vector<int> &vec1,vector<int> &vec2);  //using the insertion sort to fill up empty vec1 and leave vec2 as what it is
	void printAllPath();                                //print all the path from all nodes
	void dijkstraAll(int begin);                        //calculating all nodes, the above dijkstra only calculate one node 
	void calculateStopsBC();                            //calculating the betweeness value of every node
	bool isInCurrentStops(string str1,int is_I);        //to judge whether node is_I is in the path str1 

	void run();

private:
	int node_num;                //the number of node, initialized by input
	int edge_num;
	int distance_MAX;            //define indefinite distance
	int **arc;                   //define the Adjacency Matrix to store the graph
	Dis *dis;                    //record the distance from starting node to other nodes, will keep updating this array

	vector<int> vertexO;         //the current start node from reading
	vector<int> vertexD;         //the heading to node from current node
	vector<int> dis_value;       //the distance for the neighbor nodes
	vector<vector<string>> allPath;   //to store all the shortest path string for every node to all other nodes, iteration 167 times
	
};

