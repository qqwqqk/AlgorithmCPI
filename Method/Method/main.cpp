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

	Head.S_initialize(netname);						//�����ݿ⵼������
	Head.NetworkSort();								//Ϊ�ڵ��еı��б��Լ����еĽڵ��б��������
	Head.Init_Connects();							//��ʼ���ڵ�������

	Head.Init_Blocs();
	Head.Init_Polys();
	Head.Update_Modus();							//����ģ���

	while (1){
		Head.Init_Mergers();						//��ʼ�����ε�������Ҫ�ϲ��Ĳ��������� Mergers �б�

		Head.Fusion_run();							//ִ�кϲ�����

		Head.Update_Polys();						//���¾ۺ϶���Ϣ
		Head.Update_Modus();						//����ģ��� 
		Head.Update_Stats();						//�����ܶ��봫����

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

		if (Head.GetBlocNumber() <= 1){ break; }	//ѭ����ֹ������������Ϊһ
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