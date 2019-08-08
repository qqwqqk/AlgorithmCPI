#include "base.h"

Node::Node(int id, int dgree){
	ID = id; Tag = id; Dgree = dgree;
	EdgeID.clear();
	next = NULL;
}
int Node::GetNodeID(){
	return ID;
}
int Node::GetNodeTag(){
	return Tag;
}
int Node::GetNodeDgree(){
	return Dgree;
}
void Node::AddEdgeID(int eid){
	Dgree++;
	EdgeID.push_back(eid);
}
void Node::PrintNode(){
	//cout <<"NodeID:" <<ID << "\tNodeDgree" << Dgree << "\tEdgeInformations:";
	for (int i = 0; i < Dgree; i++){
		cout << '\t' << EdgeID[i];
	}
	cout << endl;
}

Edge::Edge(int id, int dgree){
	ID = id; Dgree = dgree;
	NodeID.clear();
	next = NULL;
}
int Edge::GetEdgeID(){
	return ID;
}
int Edge::GetEdgeDgree(){
	return Dgree;
}
void Edge::AddNodeID(int nid){
	Dgree++;
	NodeID.push_back(nid);
}
void Edge::PrintEdge(){
	//cout << "EdgeID:" << ID << "\tEdgeDgree" << Dgree << "\tNodeInformations:";
	for (int i = 0; i < Dgree; i++){
		cout << '\t' << NodeID[i];
	}
	cout << endl;
}

Link::Link(int id, int node, int edge,double weight){
	ID = id; NodeID = node; EdgeID = edge;
	Weight = weight;
	next = NULL;
}
int Link::GetLinkID(){
	return ID;
}
int Link::GetNodeID(){
	return NodeID;
}
int Link::GetEdgeID(){
	return EdgeID;
}
void Link::PrintLink(){
	cout << ID << '\t' << NodeID << '\t' << EdgeID << endl;
}

Poly::Poly(int x, int y, double mo, int en){
	NodeX = x; NodeY = y; Modulus = mo;
	EdgeNumber = en;
	next = NULL;
}
double Poly::GetModulus(){
	return Modulus;
}
void Poly::PrintPoly(){
	cout << "部落一：" << NodeX << "\t部落二：" << NodeY << "\t聚合度：" << Modulus << "\t连边数：" << EdgeNumber << endl;
}

Bloc::Bloc(int id, double mo){
	ID = id; Modularity = mo;
	EdgeNumber = 0;
	NodeID.push_back(id);
	next = NULL;
}
int Bloc::GetID(){
	return ID;
}
double Bloc::GetModularity(){
	return Modularity;
}
void Bloc::PrintBloc(){
	cout << "部落：" << ID << "\t内部边数：" << EdgeNumber << "\t模块度：" << Modularity << "\t部落中节点有：";
	for (int i = 0; i < (int)NodeID.size(); i++){
		cout << '\t' << NodeID[i];
	}
	cout << endl;
}

Merge::Merge(int id){ 
	ID = id; next = NULL; Lists.clear(); 
}

Connect::Connect(int x, int y, double we){
	NodeX = x; NodeY = y; Weight = we;
	next = NULL;
}
int Connect::GetNodeX(){
	return NodeX;
}
int Connect::GetNodeY(){
	return NodeY;
}
double Connect::GetWeight(){
	return Weight;
}
void Connect::PrintConn(){
	cout << "节点一：" << NodeX << "\t节点二：" << NodeY << "\t权重值：" << Weight << endl;
}

Network::Network(int id, string name){
	ID = id; Name = name; 
	NodeNumber = 0; EdgeNumber = 0; LinkNumber = 0; BlocNumber = 0; BetwNumber = 0;
	Ploy_max = 0.0; Modularity = 0.0; Density = 0.0, Conductivity = 0.0;
	Nodes = new Node;
	Edges = new Edge;
	Links = new Link;
	Polys = new Poly;
	Blocs = new Bloc;
	Merges = new Merge;
	Connects = new Connect;
	next = NULL;
}
Network::~Network(){
	delete Nodes;
	delete Edges;
	delete Links;
	delete Polys;
	delete Blocs;
	delete Merges;
	delete Connects;
}

void Network::S_initialize(string netname){
	Node *pn = Nodes;
	Edge *pe = Edges;
	Link *pl = Links;
	string sql;
	_bstr_t str;
	ADOLINK record;

	_variant_t vid, vnodeid, vedgeid;
	int id, nodeid, edgeid;

	Name = netname;

	record.Connect();
	_RecordsetPtr m_pRecordset;

	sql = "select * from [" + Name + "]";				//初始化链接信息
	str = sql.c_str();
	m_pRecordset = record.GetRecordset(str);

	if (!m_pRecordset->BOF)
		m_pRecordset->MoveFirst();
	else{
		cout << "链接表空！" << endl;
	}
	while (!m_pRecordset->adoEOF){
		try{
			vid = m_pRecordset->GetCollect("ID");
			id = vid;
			vnodeid = m_pRecordset->GetCollect("Nodes");
			nodeid = vnodeid;
			nodeid = nodeid<0 ? -1 : nodeid;
			vedgeid = m_pRecordset->GetCollect("Edges");
			edgeid = vedgeid;
			edgeid = edgeid<0 ? -1 : edgeid;
		}
		catch (_com_error e){
			cout << e.Description() << endl;
		}

		//建立链接表
		m_pRecordset->MoveNext();
		pl->next = new Link(id, nodeid, edgeid);
		LinkNumber++;
		pl = pl->next;

		//建立边表
		pe = Edges;
		while (1){
			if (pe->next == NULL){
				pe->next = new Edge(edgeid, 0);
				EdgeNumber++;
				pe->next->AddNodeID(nodeid);
				break;
			}
			if (pe->next->ID == edgeid){
				pe->next->AddNodeID(nodeid);
				break;
			}
			pe = pe->next;
		}

		//建立节点表
		pn = Nodes;
		while (1){
			if (pn->next == NULL){
				pn->next = new Node(nodeid, 0);
				NodeNumber++;
				pn->next->AddEdgeID(edgeid);
				break;
			}
			if (pn->next->ID == nodeid){
				pn->next->AddEdgeID(edgeid);
				break;
			}
			pn = pn->next;
		}


	}

	record.ExitConnect();

	Nodes = Nodes->next;
	Edges = Edges->next;
	Links = Links->next;
}
void Network::NetworkSort(){
	Node* pn = Nodes;
	Edge* pe = Edges;
	while (pn){
		sort(pn->EdgeID.begin(), pn->EdgeID.end());
		pn = pn->next;
	}
	while (pe){
		sort(pe->NodeID.begin(), pe->NodeID.end());
		pe = pe->next;
	}
}
void Network::Init_Connects(){
	Connect *pc;
	Edge *pe = Edges;
	int node1, node2, count = 0;
	BetwNumber = 0;
	double weight;

	while (pe != NULL){
		weight = 1.0 / pe->Dgree;
		count = (int)pe->NodeID.size();
		for (int i = 0; i < count; i++){
			for (int j = i + 1; j < count; j++){
				node1 = pe->NodeID[i];
				node2 = pe->NodeID[j];
				pc = Connects;
				while (1){
					if (node1 == pc->NodeX && node2 == pc->NodeY){
						pc->Weight = pc->Weight + weight;
						break;
					}
					if (node1 == pc->NodeY && node2 == pc->NodeX){
						pc->Weight = pc->Weight + weight;
						break;
					}
					if (pc->next == NULL){
						pc->next = new Connect(node1, node2, weight);
						BetwNumber++;
						break;
					}
					pc = pc->next;
				}
			}
		}
		pe = pe->next;
	}
}

void Network::Init_Blocs(){
	BlocNumber = NodeNumber;
	Bloc *pb = Blocs;
	double modularity = 0.0;
	for (int i = 1; i <= BlocNumber; i++){
		modularity = 0;

		pb->next = new Bloc(i,modularity);
		pb = pb->next;
	}
}
void Network::Init_Polys(){
	Poly *pp = Polys;
	int edgenumbers = 0;
	double modulus = 0.0;
	for (int i = 1; i < NodeNumber; i++){
		for (int j = i + 1; j <= NodeNumber; j++){
			modulus = CalculatePoly(i, j, Nodes, Edges, Blocs);
			edgenumbers = CalculateEdge(i, j, Blocs, Connects);
			//modulus = (i*j) % 8;				//调试——自定义聚合指数进行程序调试	
			if (modulus > 0){
				pp->next = new Poly(i, j, modulus, edgenumbers);
				if (modulus > Ploy_max){ Ploy_max = modulus; }
				pp = pp->next;
			}
		}
	}
}
void Network::Init_Mergers(){
	vector<int> NodeID_cache;
	vector<int> BlocID_cache;

	Poly *pp = Polys;
	Merge *pm = Merges;
	int Bloc1, Bloc2, Length, Tag, Tag1, Tag2, Index1, Index2, Count = 0;

	while (pp){
		Length = (int)NodeID_cache.size();
		if (pp->GetModulus() == Ploy_max){
			Tag1 = Tag2 = Index1 = Index2 = 0;
			Bloc1 = pp->NodeX;
			Bloc2 = pp->NodeY;

			for (int i = 0; i < Length; i++){
				if (Bloc1 == NodeID_cache[i]){ Tag1 = 1; Index1 = BlocID_cache[i]; }
				if (Bloc2 == NodeID_cache[i]){ Tag2 = 1; Index2 = BlocID_cache[i]; }
				if (Tag1 == 1 && Tag2 == 1){ break; }
			}

			Tag = Tag1 + Tag2;

			switch (Tag){
			case 0:
				Count++;
				NodeID_cache.push_back(Bloc1);
				NodeID_cache.push_back(Bloc2);
				BlocID_cache.push_back(Count);
				BlocID_cache.push_back(Count);
				break;
			case 1:
				if (Tag1){
					NodeID_cache.push_back(Bloc2);
					BlocID_cache.push_back(Index1);
				}
				else{
					NodeID_cache.push_back(Bloc1);
					BlocID_cache.push_back(Index2);
				}
				break;
			case 2:
				if (Index1 != Index2){
					Count--;
					if (Index1 < Index2){
						for (int i = 0; i < Length; i++){
							if (BlocID_cache[i] == Index2){ BlocID_cache[i] = Index1; }
							if (BlocID_cache[i] > Index2){ BlocID_cache[i]--;}
						}
					}
					else{
						for (int i = 0; i < Length; i++){
							if (BlocID_cache[i] == Index1){ BlocID_cache[i] = Index2; }
							if (BlocID_cache[i] > Index1){ BlocID_cache[i]--;}
						}
					}
				}
				break;
			default:
				cout << "未知的错误！" << endl;
				system("pause");
				break;
			}
		}
		pp = pp->next;
	}

	//vector<int> cache(Count, 0);

	Length = (int)NodeID_cache.size();

	//for (int i = 0; i < Length; i++){cout << setw(3) << NodeID_cache[i];} cout << endl;
	//for (int i = 0; i < Length; i++){cout << setw(3) << BlocID_cache[i];} cout << endl;

	for (int i = 1; i <= Count; i++){
		pm->next = new Merge(i);
		for (int j = 0; j < Length; j++){
			if (i == BlocID_cache[j]){
				pm->next->Lists.push_back(NodeID_cache[j]);
				sort(pm->next->Lists.begin(), pm->next->Lists.end());
			}
		}
		//pm->Lists.push_back(1);
		pm = pm->next;
	}

	//Merges = Merges->next;
}
void Network::Fusion_run(){
	Merge *pm = NULL;
	Node *pn = NULL;
	Bloc *pb = Blocs, *pbr = NULL;
	int index, tag;

	while (pb->next){
		tag = 0;
		pm = Merges->next;
		while (pm){
			for (int i = 1; i < (int)pm->Lists.size(); i++){
				if (pb->next->ID == pm->Lists[i]){
					tag++;
					BlocNumber--;
					pbr = Blocs->next;
					while (pbr){
						if (pbr->ID == pm->Lists[0]){
							for (int j = 0; j < (int)pb->next->NodeID.size(); j++){
								pbr->NodeID.push_back(pb->next->NodeID[j]);
							}

							sort(pbr->NodeID.begin(), pbr->NodeID.end());

							pbr->EdgeNumber = CalculateEdges(pbr->ID, Blocs, Connects);		//计算部落内边数

							pn = Nodes; index = 0;											//修正节点Tag标识
							while (pn){
								if (pn->ID == pbr->NodeID[index]){
									pn->Tag = pbr->NodeID[0];
									index++;
								}
								if (index == (int)pbr->NodeID.size()){ break; }
								pn = pn->next;
							}
							break;
						}
						pbr = pbr->next;
					}
				}
			}
			pm = pm->next;
		}

		if (tag){
			pb->next = pb->next->next;
			continue;
		}

		pb = pb->next;
	}
}
void Network::Update_Polys(){
	vector<int> cache;
	int tag, flag, edgenumbers = 0;
	double modulus = 0.0;
	Poly *pp = Polys;
	Merge *pm;
	Ploy_max = 0;

	while (pp->next){
		tag = 0;
		pm = Merges->next;
		while (pm){
			for (int i = 0; i < (int)pm->Lists.size(); i++){
				if (pp->next->NodeX == pm->Lists[i]){ tag++; pp->next->NodeX = pm->Lists[0]; }
				if (pp->next->NodeY == pm->Lists[i]){ tag++; pp->next->NodeY = pm->Lists[0]; }
			}
			pm = pm->next;
		}

		if (tag){
			flag = 0;
			if (pp->next->NodeX == pp->next->NodeY){ flag++; }
			else{
				for (int i = 0; i < (int)cache.size(); i = i + 2){
					if (pp->next->NodeX == cache[i] && pp->next->NodeY == cache[i + 1]){ flag++; }
					if (pp->next->NodeY == cache[i] && pp->next->NodeX == cache[i + 1]){ flag++; }
				}
			}

			if (flag){
				if (pp->next->next){
					pp->next = pp->next->next;
					continue;
				}
				else{
					pp->next = NULL;
					break;
				}
				
			}
			else{
				cache.push_back(pp->next->NodeX);
				cache.push_back(pp->next->NodeY);
				modulus = CalculatePoly(pp->next->NodeX, pp->next->NodeY, Nodes, Edges, Blocs);
				edgenumbers = CalculateEdge(pp->next->NodeX, pp->next->NodeY, Blocs, Connects);
				if (modulus > 0){
					pp->next->Modulus = modulus;
					pp->next->EdgeNumber = edgenumbers;
					if (modulus > Ploy_max){ Ploy_max = modulus; }
				}
				else{
					pp->next = pp->next->next;
					continue;
				}
			}
		}
		else{
			if (pp->next->GetModulus() > Ploy_max){ Ploy_max = pp->next->GetModulus(); }
		}
		pp = pp->next;
	}
}
void Network::Update_Modus(){
	Modularity = 0.0;
	Bloc *pb = Blocs->next;
	Poly *pp = NULL;

	double temp_i, temp_o;

	while(pb){
		temp_i = 1.0 * pb->EdgeNumber / BetwNumber;
		temp_o = temp_i;
		pp = Polys->next;
		while (pp){
			if (pp->NodeX == pb->ID || pp->NodeY == pb->ID){
				temp_o += 0.5 * pp->EdgeNumber / BetwNumber;
			}
			pp = pp->next;
		}
		Modularity += temp_i - temp_o * temp_o;
		pb = pb->next;
	}

}
void Network::Update_Stats(){
	Density = 0.0;
	Conductivity = 0.0;

	int tag = 0;
	double temp = 0.0;
	double link = 0.0;
	Connect *pc = NULL;

	Bloc *pb = Blocs->next;
	while (pb){
		link = 0;
		pc = Connects->next;
		while (pc){
			tag = 0;
			for (int id : pb->NodeID){
				if (pc->NodeX == id){ tag++; }
				if (pc->NodeY == id){ tag++; }
			}
			if (tag == 1){ link++; }
			pc = pc->next;
		}
		temp = 1.0 * link / (link + 2 * pb->EdgeNumber);
		Density += pb->EdgeNumber;
		Conductivity += temp;
		pb = pb->next;
	}

	Density = Density / BetwNumber;
	Conductivity = Conductivity / BlocNumber;
}

int Network::GetBlocNumber(){
	return BlocNumber;
}
double Network::GetPloyMax(){
	return Ploy_max;
}
double Network::GetModularity(){
	return Modularity;
}
double Network::GetDensity(){
	return Density;
}
double Network::GetConductivity(){
	return Conductivity;
}

double CalculatePoly(int x, int y, Node* pn, Edge* pe, Bloc* pb){
	double result = -1.0;

	vector<int> tribal_x;
	vector<int> tribal_y;
	vector<int> edge_lists;

	Node* prn;
	Edge* pre;
	Bloc* prb;

	int tag, size_x, size_y;
	double percent_x, percent_y;

	prb = pb;
	tag = 0;
	while (prb){
		if (x == prb->NodeID[0]){
			tag++;
			for (int i = 0; i < (int)prb->NodeID.size(); i++){
				tribal_x.push_back(prb->NodeID[i]);
				edge_lists.push_back(prb->NodeID[i]);
			}
		}
		if (y == prb->NodeID[0]){
			tag++;
			for (int i = 0; i < (int)prb->NodeID.size(); i++){
				tribal_y.push_back(prb->NodeID[i]);
				edge_lists.push_back(prb->NodeID[i]);
			}
		}
		if (tag == 2){ 
			break; 
		}
		prb = prb->next;
	}
	
	//将包含有部落一节点或部落二节点的边的ID全部放入 edge_lists
	prn = pn;
	tag = 0;
	while (prn){
		for (int i = 0; i < (int)tribal_x.size(); i++){
			if (tribal_x[i] == prn->GetNodeID()){ tag++; }
		}
		for (int i = 0; i < (int)tribal_y.size(); i++){
			if (tribal_y[i] == prn->GetNodeID()){ tag++; }
		}
		if (tag){
			for (int i = 0; i < (int)prn->EdgeID.size(); i++){
				edge_lists.push_back(prn->EdgeID[i]);
			}
		}
		prn = prn->next;
	}
	sort(edge_lists.begin(), edge_lists.end());
	vector<int>::iterator iter = unique(edge_lists.begin(), edge_lists.end());
	edge_lists.erase(iter, edge_lists.end());		//将 edge_lists 进行排序与去重

	size_x = (int)tribal_x.size();
	size_y = (int)tribal_y.size();

	int index = 0, xi, yi, repeat_x, repeat_y;
	double factor = 0.0, revise = 0.0;
	double molecular = 0.0, denominator = 0.0;
	vector<int> temp;
	pre = pe;
	while (pre && index < (int)edge_lists.size()){
		if (pre->GetEdgeID() == edge_lists[index]){
			temp.clear();
			xi = yi = 0;
			repeat_x = repeat_y = 0;

			for (int i = 0; i < (int)pre->NodeID.size(); i++){
				temp.push_back(pre->NodeID[i]);
			}

			//部落与边的重复数量统计 repeat_x 为部落一与边的重复数 repeat_y 为部落二与边的重复数
			for (int i = 0; i < (int)temp.size(); i++){
				while (xi < size_x){
					if (tribal_x[xi] < temp[i]){ xi++; }
					else{
						if (temp[i] == tribal_x[xi]){
							repeat_x++; xi++;
						}
						break;
					}
				}
				while (yi < size_y){
					if (tribal_y[yi] < temp[i]){ yi++; }
					else{
						if (temp[i] == tribal_y[yi]){
							repeat_y++; yi++;
						}
						break;
					}
				}
			}

			percent_x = 1.0 * repeat_x / size_x;
			percent_y = 1.0 * repeat_y / size_y;

			//单边贡献的权重计算
			if (repeat_x != 0 && repeat_y != 0){											
				factor = sqrt(percent_x * percent_y);										
				molecular += factor;
				denominator += factor;
			}
			else{
				if (repeat_x == 0){
					revise = 0.5 * percent_y;
				}
				else {
					revise = 0.5 * percent_x;
				}
				denominator += revise;
			}
			index++;
		}
		pre = pre->next;
	}

	result = molecular / denominator;

	return result;
}
int CalculateEdge(int x, int y, Bloc* pb, Connect* pc){
	int result = 0;

	vector<int> tribal_x;
	vector<int> tribal_y;

	Bloc* prb;
	Connect* prc;

	int tag;

	prb = pb;
	tag = 0;
	while (prb){
		if (x == prb->NodeID[0]){
			tag++;
			for (int i = 0; i < (int)prb->NodeID.size(); i++){
				tribal_x.push_back(prb->NodeID[i]);
			}
		}
		if (y == prb->NodeID[0]){
			tag++;
			for (int i = 0; i < (int)prb->NodeID.size(); i++){
				tribal_y.push_back(prb->NodeID[i]);
			}
		}
		if (tag == 2){
			break;
		}
		prb = prb->next;
	}

	prc = pc;
	while (prc){
		tag = 0;
		for (int i = 0; i < (int)tribal_x.size(); i++){
			if (prc->GetNodeX() == tribal_x[i] || prc->GetNodeY() == tribal_x[i]){ tag++; break; }
		}
		for (int i = 0; i < (int)tribal_y.size(); i++){
			if (prc->GetNodeX() == tribal_y[i] || prc->GetNodeY() == tribal_y[i]){ tag++; break; }
		}
		if (tag == 2){
			result++;
		}
		prc = prc->next;
	}

	return result;
}
int CalculateEdges(int x, Bloc* pb, Connect* pc){
	int result = 0;

	vector<int> tribal_x;

	Bloc* prb;
	Connect* prc;

	int tag;

	prb = pb;
	tag = 0;
	while (prb){
		if (x == prb->NodeID[0]){
			for (int i = 0; i < (int)prb->NodeID.size(); i++){
				tribal_x.push_back(prb->NodeID[i]);
			}
			break;
		}
		prb = prb->next;
	}

	prc = pc;
	while (prc){
		tag = 0;
		for (int i = 0; i < (int)tribal_x.size(); i++){
			if (prc->GetNodeX() == tribal_x[i] || prc->GetNodeY() == tribal_x[i]){ 
				tag++;
				if (tag == 2){
					result++;
					break;
				}
			}
		}
		
		prc = prc->next;
	}

	return result;
}
