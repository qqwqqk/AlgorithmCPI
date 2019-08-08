#include "base.h"
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <string>
using namespace std;

void main(){
	ifstream ifile;
	ofstream ofile;

	string name = "AC";
	string method = "LabelPropagation";
	string edgeread = "Part_SR_" + name + "_SingleGraphs.txt";
	string noderead = "Compared_" + name + "_" + method + ".txt";

	string str = "", temp = "";
	string::size_type sz;
	int tag = 0;
	int NodeX, NodeY, ID, Tag;

	int NodeNumber = 0, EdgeNumber = 0, BlocNumber = 0;
	double Density = 0.0, Conductivity = 0.0;

	Node N_star(-1, -1), *pn = NULL;
	Edge E_star(-1, -1, 0), *pe = NULL;
	Bloc B_star(-1), *pb = NULL;

	cout << "Community Loading..." << endl;
	//读取社区划分结果数据
	ifile.open(noderead);
	pn = &N_star;
	while (getline(ifile, str)){
		tag = 0;
		for (int i = 0; i < (int)str.length(); i++){
			if (str[i] == '\t'){
				tag++;
				if (tag == 1){
					ID = std::stoi(temp, &sz);
					temp.clear();
				}
				//if (tag == 2){node = std::stoi(temp, &sz);temp.clear();}
			}
			else{
				temp += str[i];
			}
		}
		Tag = std::stoi(temp, &sz);
		temp.clear();
		pn->next = new Node(ID, Tag);
		NodeNumber++;
		pn = pn->next;
		//cout << ID << '\t' << Tag << endl;
	}
	ifile.close();
	cout << "Community Load Success!" << endl;

	cout << "Connect Loading..." << endl;
	//读取连接信息数据
	ifile.open(edgeread);
	pe = &E_star;
	while (getline(ifile, str)){
		tag = 0;
		for (int i = 0; i < (int)str.length(); i++){
			if (str[i] == ' '){
				tag++;
				if (tag == 1){
					NodeX = std::stoi(temp, &sz);
					temp.clear();
				}
				//if (tag == 2){node = std::stoi(temp, &sz);temp.clear();}
			}
			else{
				temp += str[i];
			}
		}
		NodeY = std::stoi(temp, &sz);
		temp.clear();
		pe->next = new Edge(NodeX, NodeY, 1);
		EdgeNumber++;
		pe = pe->next;
		//cout << NodeX << '\t' << NodeY << endl;
	}
	ifile.close();
	cout << "Connect Load Success!" << endl;

	cout << "Blocs Loading..." << endl;
	//构建社团信息
	pn = N_star.next;
	while (pn){
		pb = &B_star;
		while (1){
			if (pb->ID == pn->CommunityTag){
				pb->Nodes.push_back(pn->ID);
				break;
			}
			if (pb->next == NULL){
				pb->next = new Bloc(pn->CommunityTag);
				pb->next->Nodes.push_back(pn->ID);
				BlocNumber++;
				break;
			}
			pb = pb->next;
		}
		pn = pn->next;
	}

	pe = E_star.next;
	while (pe){
		pb = B_star.next;
		while (pb){
			tag = 0;
			for (int id : pb->Nodes){
				if (pe->NodeX == id){ tag++; }
				if (pe->NodeY == id){ tag++; }
			}
			switch (tag){
			case 1:
				pb->LinkNumber++;
				break;
			case 2:
				pb->EdgeNumber++;
				break;
			default:
				break;
			}
			pb = pb->next;
		}
		pe = pe->next;
	}
	cout << "Blocs Load Success!" << endl << endl;


	cout << "Density Calculating..." << endl;
	//计算Density
	int IncludeNumber = 0;
	pb = B_star.next;
	while (pb){
		IncludeNumber += pb->EdgeNumber;
		pb = pb->next;
	}
	Density = 1.0 * IncludeNumber / EdgeNumber;
	cout << "Density :\t" << Density << endl << endl;

	cout << "Conductivity Calculating..." << endl;
	//计算Conductivity
	double link = 0.0, cache = 0.0;

	pb = B_star.next;
	while (pb){
		link = pb->LinkNumber;
		cache = 1.0 * link / (link + 2 * pb->EdgeNumber);
		Conductivity += cache;
		pb = pb->next;
	}
	Conductivity = Conductivity / BlocNumber;
	cout << "Conductivity :\t" << Conductivity << endl;

	cout << endl << name << '_' << method << ":\nDensity:" << Density << "\tConductivity:" << Conductivity << endl << endl;
	system("pause");
}