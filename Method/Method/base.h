#pragma once
#include"adosql.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class Node{
protected:
	int ID;
	int Tag;
	int Dgree;
public:
	vector<int> EdgeID;
	Node *next;
	Node(int id = -1, int dgree = 0);

	int GetNodeID();
	int GetNodeTag();
	int GetNodeDgree();

	void AddEdgeID(int eid = -1);

	void PrintNode();
	friend class Network;
};

class Edge{
protected:
	int ID;
	int Dgree;
public:
	vector<int> NodeID;
	Edge *next;
	Edge(int id = -1, int dgree = 0);

	int GetEdgeID();
	int GetEdgeDgree();

	void AddNodeID(int nid = -1);

	void PrintEdge();
	friend class Network;
};

class Link{
protected:
	int ID;
	int NodeID;
	int EdgeID;
	double Weight;
public:
	Link *next;

	int GetLinkID();
	int GetNodeID();
	int GetEdgeID();

	void PrintLink();
	Link(int id = -1, int node = -1, int edge = -1, double weight = 1.0);
	friend class Network;
};

class Bloc{
protected:
	int ID;
	int EdgeNumber;
	double Modularity;
public:
	vector<int> NodeID;
	Bloc *next;

	int GetID();
	double GetModularity();

	void PrintBloc();
	Bloc(int id = -1, double m = -1.0);
	friend class Network;
};

class Poly{
protected:
	int NodeX;
	int NodeY;
	int EdgeNumber;
	double Modulus;
public:
	Poly *next;

	double GetModulus();

	void PrintPoly();
	Poly(int x = -1, int y = -1, double m = -1.0, int en = -1);
	friend class Network;
};

class Merge{
protected:
	int ID;
public:
	vector<int> Lists;
	Merge *next;
	Merge(int id = -1);
	friend class Network;
};

class Connect{
protected:
	int NodeX;
	int NodeY;
	double Weight;
public:
	Connect *next;

	int GetNodeX();
	int GetNodeY();
	double GetWeight();

	void PrintConn();
	Connect(int x = -1, int y = -1, double w = -1.0);
	friend class Network;
};

class Network{
private:
	int ID;
	string Name;
	int NodeNumber;
	int EdgeNumber;
	int LinkNumber;
	int BlocNumber;
	int BetwNumber;
	double Ploy_max;
	double Modularity;
	double Density;
	double Conductivity;
public:
	Node *Nodes;
	Edge *Edges;
	Link *Links;
	Poly *Polys;
	Bloc *Blocs;
	Merge *Merges;
	Connect *Connects;
	Network *next;

	void S_initialize(string netname = "Test");

	void NetworkSort();
	void Init_Connects();

	void Init_Blocs();
	void Init_Polys();
	void Init_Mergers();
	void Fusion_run();
	void Update_Polys();
	void Update_Modus();
	void Update_Stats();

	int GetBlocNumber();
	double GetPloyMax();
	double GetModularity();
	double GetSurprise();
	double GetDensity();
	double GetConductivity();

	Network(int id = -1, string name = "");
	~Network();
};

double CalculatePoly(int x, int y, Node* pn, Edge* pe, Bloc* pb);
int CalculateEdge(int x, int y, Bloc* pb, Connect* pc);
int CalculateEdges(int x, Bloc* pb, Connect* pc);

class R_cache{
public:
	int ID;
	double Real;
	double Power;
	R_cache *next;
	R_cache(int id, double r, double p){ ID = id; Real = r; Power = p; next = NULL; }
};
double CalculateSurprisePart(int F, int M, int p, int n,double &power);
int CalculateCombination(int n, int m);