#pragma once
#include "base.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

void main(){
	string netname = "Special_Test";
	string str = netname + "_percent_program.txt";
	string str_bloc = netname + "_Polymerization.txt";
	Network Head;
	Bloc B_Cache, *pb = NULL, *pbt = NULL;

	int Number_M = 0, Number_S = 0, step = 0, index = 0;
	double Modularity = 0.0, Density = 0.0, Conductivity = 0.0;

	ofstream ofile;
	ofile.open(str);
	if (!ofile){ abort(); }

	Head.S_initialize(netname);						//从数据库导入数据
	Head.NetworkSort();								//为节点中的边列表以及边中的节点列表进行排序
	Head.Init_Connects();							//初始化节点间的连接

	Head.Init_Blocs();
	Head.Init_Polys();
	Head.Update_Modus();							//计算模块度

	while (1){
		Head.Init_Mergers();						//初始化本次迭代中需要合并的部落以生成 Mergers 列表

		Head.Fusion_run();							//执行合并操作

		Head.Update_Polys();						//更新聚合度信息
		Head.Update_Modus();						//计算模块度 
		Head.Update_Stats();						//计算密度与传导率

		step++;
		cout << step << '\t' << Head.GetBlocNumber() << '\t' << setfill(' ') << setprecision(4) << setw(7) << Head.GetModularity() << "\t\t" << setfill(' ') << setprecision(4) << setw(7) << Head.GetDensity() << "\t\t" << setfill(' ') << setprecision(4) << setw(7) << Head.GetConductivity() << endl;
		ofile << step << '\t' << Head.GetBlocNumber() << '\t' << Head.GetModularity() << '\t' << Head.GetDensity() << '\t' << Head.GetConductivity() << '\n';

		if (Head.GetModularity() > Modularity){
			Modularity = Head.GetModularity();
			Number_M = Head.GetBlocNumber();
			Density = Head.GetDensity();
			Conductivity = Head.GetConductivity();

			pbt = Head.Blocs->next;
			pb = &B_Cache;
			index = 0;
			while (pbt){
				pb->next = new Bloc(index++);
				pb->next->NodeID.clear();
				for (int id : pbt->NodeID){
					pb->next->NodeID.push_back(id);
				}
				pb = pb->next;
				pbt = pbt->next;
			}
		}

		if (Head.GetBlocNumber() <= 1){ break; }	//循环中止条件：部落数为一
	}
	ofile.close();

	cout << endl << "Network\t" << netname << endl << endl;
	cout << "Blocs \t Modularity \t  Density \t Conductivity" << endl;
	cout << setfill(' ') << Number_M << '\t' << setw(7) << Modularity << "\t\t" << setw(7) << Density <<  "\t\t" << setw(7) << Conductivity << endl;

	ofile.open(str_bloc);
	if (!ofile){ abort(); }
	pb = B_Cache.next;
	while (pb){
		for (int id : pb->NodeID){
			ofile << id << '\t' << pb->GetID() << '\n';
		}
		pb = pb->next;
	}
	ofile.close();

	system("pause");
}