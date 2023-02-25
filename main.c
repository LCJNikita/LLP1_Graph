#include "graph.h"
#include <string.h>

// Set scheme db and save it (header) to the file (Create file before!)
//int main() {
//	
//	struct GraphDB db;
//    db.nodesCount = 0;
//    db.columnsCount = 0;
//    db.nodes = NULL;
//    db.scheme = NULL;
//    
//    int columnsCount = 4;
//
//    struct Column scheme[columnsCount];
//    scheme[0].type = INT32;
//    strncpy(scheme[0].name, "Age\0", MAX_NAME_LENGTH);
//    
//    scheme[1].type = FLOAT;
//    strncpy(scheme[1].name, "Balance\0", MAX_NAME_LENGTH);
//    
//    scheme[2].type = CHAR_PTR;
//    strncpy(scheme[2].name, "Name\0", MAX_NAME_LENGTH);
//    
//    scheme[3].type = BOOL;
//    strncpy(scheme[3].name, "isAdult\0", MAX_NAME_LENGTH);
//    
//    setScheme(&db, scheme, columnsCount);
//    
//    saveHeaderStructToFile(&db, "data.bin");
//    
//    return 0;
//}

// Reading header
//int main() {
//	
//	struct GraphDB db;
//    loadHeaderStructFromFile(&db, "data.bin");
//    printHeaderGraphDB(&db);
//    
//    return 0;
//}

// Check node printing
//int main() {
//	
//	struct GraphDB db;
//   	loadHeaderStructFromFile(&db, "data.bin");
//
//	struct Node node;
//	parseAndSetNode(&db, "10000,228.56,nikita aboba,true", &node);
//	printNode(&db, &node);
//    
//    return 0;
//}

// Adding row to the file
//int main() {
//	
//	struct GraphDB db;
//   	loadHeaderStructFromFile(&db, "data.bin");
//   	
//   	struct Node node;
//	parseAndSetNode(&db, "222,131.12,myname,false", &node);
//
////	printRow(&db, &row);
//	
//	printf("file size BEFORE ADD %d\n", (int)getFileSize("data.bin"));
//	
//	addNodeToFile("data.bin", &node);
//
//	printf("file size AFTER ADD %d\n", (int)getFileSize("data.bin"));
//    
//    return 0;
//}

// Example how use findRNodeByIndex
//int main() {
//	
//	struct GraphDB db;
//   	loadHeaderStructFromFile(&db, "data.bin");
//   	printHeaderGraphDB(&db);
//   	
//   	findNodeByIndex("data.bin", 1);
//    
//    return 0;
//}

// Print each node in db
int main() {
	
	struct GraphDB db;
   	loadHeaderStructFromFile(&db, "data.bin");
   	printHeaderGraphDB(&db);
   	
	iterateByEachNode("data.bin");
    
    return 0;
}

// Example how use createNode
//int main() {
//	
//	bool createNodeResult1 = createNode("data.bin", "10000,228.56,nikita,false");
//	if (createNodeResult1) {
//		printf("Adding createRowResult1 success\n");
//	} else {
//		printf("Adding createRowResult1 failure\n");
//	}
//	
//	bool createNodeResult2 = createNode("data.bin", "123, kek");
//	if (createNodeResult2) {
//		printf("Adding createRowResult2 success\n");
//	} else {
//		printf("Adding createRowResult2 failure\n");
//	}
//    
//    return 0;
//}

// Example how use findNodes
//int main() {
//	
//	iterateByEachNode("data.bin");
//	printf("\n");
//	
//	findNodes("data.bin", "Name", "nikita123");
//    
//    return 0;
//}

// Example how use updateNodeByIndex
//int main() {
//	
//	iterateByEachNode("data.bin");
//	printf("\n");
//	
//	updateNodeByIndex("data.bin", "Name", "new_nikita_name", 1);
//    
//    return 0;
//}

// Example how use deleteNodeByIndex
//int main() {
//	
//	printf("before deleting: %d\n", getFileSize("data.bin"));
//	deleteNodeByIndex("data.bin", 0);
//	printf("after deleting: %d\n", getFileSize("data.bin"));
//    
//    return 0;
//}

// clear file data
//int main() {
//	
//	clearFileData("data.bin");
//	
//	return 0;
//}

// Example of adding relations
//int main() {
//	
//	setNewRelation("data.bin", 0, 1);
//	
//	return 0;
//}































