#include <stdio.h>
#include <stdlib.h>
#include <igraph.h>
#include <string.h>

#define INFOMAP_TRIALS 0x4


int main(void)
{
	//边存放的文件,空格分隔
	FILE *edgeListFile, *ResultFile;
	fopen_s(&edgeListFile, "Part_SR_AC_SingleGraphs.txt", "r");
	fopen_s(&ResultFile, "Compared_AC_InfoMAP.txt", "w");

	igraph_t wbNetwork;
	igraph_real_t Q_value;
	igraph_real_t codelength;
	igraph_vector_t membership;
	long int i;
	long int no_of_nodes;
	long int no_of_edges;
	int rstCode, count = 0;
	igraph_vector_t gtypes, vtypes, etypes;
	igraph_strvector_t gnames, vnames, enames;
	/* turn on attribute handling */
	igraph_i_set_attribute_table(&igraph_cattribute_table);

	//初始化对象
	igraph_vector_init(&membership, 0);


	//if (argc < 2){printf("Usage: %s <inputRelationFile> \n", argv[0]);exit(1);}

	//从文件中读入图
	igraph_read_graph_ncol(&wbNetwork,
		edgeListFile,
		0,  /*预定义的节点名称*/
		1, /*读入节点名称*/
		IGRAPH_ADD_WEIGHTS_NO,  /*是否将边的权重也读入*/
		0  /*0表示无向图*/
		);
	fclose(edgeListFile);
	igraph_simplify(&wbNetwork, 1, 1, 0);

	igraph_vector_init(&gtypes, 0);
	igraph_vector_init(&vtypes, 0);
	igraph_vector_init(&etypes, 0);
	igraph_strvector_init(&gnames, 0);
	igraph_strvector_init(&vnames, 0);
	igraph_strvector_init(&enames, 0);

	igraph_cattribute_list(&wbNetwork, &gnames, &gtypes, &vnames, &vtypes,
		&enames, &etypes);

	no_of_nodes = igraph_vcount(&wbNetwork);
	no_of_edges = igraph_ecount(&wbNetwork);
	printf("Graph node numbers: %d \n", no_of_nodes);
	printf("Graph edge numbers: %d \n", no_of_edges);


	//打印计算用ID与节点ID的对应过程
	/*
	if (igraph_cattribute_has_attr(&wbNetwork, IGRAPH_ATTRIBUTE_VERTEX, "name")){
	printf("Vertex names: ");
	for (i = 0; i<no_of_nodes; i++) {
	printf("Vertex ID: %d -> Vertex Name: %s \n", i, igraph_cattribute_VAS(&wbNetwork, "name", i));
	}
	}
	else{
	printf("The Graph does not have attribute of name \n");
	}
	*/

	//用随机游走算法对网络进行社区结构划分
	rstCode = igraph_community_infomap(&wbNetwork,
		/*edge weights*/ 0,0,
		INFOMAP_TRIALS,  /*随机游走的步数*/
		&membership,
		&codelength/*每个节点从属的community编号*/);
	if (rstCode != 0){
		printf("Error dealing with finding communities");
		return 1;
	}
	else{
		printf("Finding communities success! \n");
	}

	//打印出模块度演变的过程
	/*
	printf("Merges:\n");
	for (i = 0; i<igraph_matrix_nrow(&merges); i++) {
	printf("%2.1li + %2.li -> %2.li (modularity %4.2f)\n",
	(long int)MATRIX(merges, i, 0),
	(long int)MATRIX(merges, i, 1),
	no_of_nodes + i,
	VECTOR(modularity)[i]);
	}
	*/


	for (i = 0; i<igraph_vector_size(&membership); i++){
		printf("节点: %d -> 社区：%g \n", i + 1, VECTOR(membership)[i]);
		fprintf(ResultFile, "%s\t%g\n", igraph_cattribute_VAS(&wbNetwork, "name", i), VECTOR(membership)[i]);
		count = count>(int) VECTOR(membership)[i] ? count : (int)VECTOR(membership)[i];
	}
	fclose(ResultFile);

	rstCode = igraph_modularity(&wbNetwork,
		/* 成员关系 */&membership,
		/* 模块度 */&Q_value,
		/* 网络权重 */NULL);

	printf("部落数：%d \t模块度:%g \n", count + 1, Q_value);

	igraph_vector_destroy(&membership);

	igraph_vector_destroy(&gtypes);
	igraph_vector_destroy(&vtypes);
	igraph_vector_destroy(&etypes);
	igraph_strvector_destroy(&gnames);
	igraph_strvector_destroy(&vnames);
	igraph_strvector_destroy(&enames);

	igraph_destroy(&wbNetwork);

	system("pause");

	return 0;
}
