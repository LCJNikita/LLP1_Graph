#include "graph.h"
#include <string.h>

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

// Example how use createNode
//int main() {
//	
//	bool createNodeResult1 = createNode("data.bin", "10000,228.56,max,false");
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
//	setNewRelation("data.bin", 1, 2);
//	
//	return 0;
//}

// Example of clearAllRelationsOfNode
//int main() {
//	
//	clearAllRelationsOfNode("data.bin", 0);
//	
//	return 0;
//}

int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

float random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

bool random_bool() {
    return rand() % 2 == 0;
}

char* random_string() {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const int max_length = 254;
    int length = random_int(1, max_length);
    char* str = malloc(length + 1);
    if (str) {
    	int i;
        for (i = 0; i < length; i++) {
            int index = rand() % (sizeof(charset) - 1);
            str[i] = charset[index];
        }
        str[length] = '\0';
    }
    return str;
}

// Testing randomizer (example)
//int main() {
//	
//    srand(time(NULL));
//
//    int random_int_value = random_int(1, 100);
//    float random_float_value = random_float(1.0, 10.0);
//    bool random_bool_value = random_bool();
//    char* random_string_value = random_string();
//
//    printf("Random int value: %d\n", random_int_value);
//    printf("Random float value: %f\n", random_float_value);
//    printf("Random bool value: %s\n", random_bool_value ? "true" : "false");
//    printf("Random string value: %s\n", random_string_value);
//
//    free(random_string_value);
//
//    return 0;
//}

// example of how write to CV file timing of execution programm
//int main() {
//	
//	FILE *fp;
//	
//	fp = fopen("timings.csv", "w+");
//	
//	int i;
//	int n = 100;
//	for (i = 0; i < n; i++) {
//
//  		time_t rawtime;
//  		time(&rawtime);
//  		
//  		// operation here
//
//  		fprintf(fp, "%d,%d\n", i, rawtime);
//  		
//  		sleep(1);
//	}
//	
//	fclose(fp);
//}

//int main() {
//    time_t rawtime;
//    struct tm * timeinfo;
//
//    time(&rawtime);
//    
//    printf("Unix time: %ld\n", (long) rawtime);
//    
//    timeinfo = localtime(&rawtime);
//    printf("Текущее локальное время: %s", asctime(timeinfo));
//
//    return 0;
//}

// Print header info and file size
//int main() {
//	
//	struct GraphDB db;
//   	loadHeaderStructFromFile(&db, "data.bin");
//   	
//   	printf("File size: %d\n", getFileSize("data.bin"));
//   	
//   	printHeaderGraphDB(&db);
//   	
////	iterateByEachNode("data.bin");
//    
//    return 0;
//}

// fill file with data
//int main() {
//
//	srand(time(NULL));	
//
//	int i;
//	
//	for(i = 0; i < 10000; i++) {
//		
//		int random_int_value = random_int(1, 100);
//	    float random_float_value = random_float(1.0, 10.0);
//	    char* random_string_value = random_string();
//	    bool random_bool_value = random_bool();
//	    
//		char* db_string;
//		asprintf(&db_string, "%d,%f,%s,%s", random_int_value, random_float_value, random_string_value, random_bool_value ? "true" : "false");
//
//		bool result = createNode("data.bin", db_string);
//		
//		free(db_string);	
//	}
//
//	return 0;
//}


//
//	 TEST 1
//
//int main() {
//
//	srand(time(NULL));	
//	
//	FILE *fp;
//	fp = fopen("timings.csv", "w+");
//	
//	int i;
//	
//	for(i = 0; i < 20000; i++) {
//		
//		time_t rawtime;
//  		time(&rawtime);
//  		
//  		fprintf(fp, "%d,%d\n", i, rawtime);
//		
//		int random_int_value = random_int(1, 100);
//	    float random_float_value = random_float(1.0, 10.0);
//	    char* random_string_value = random_string();
//	    bool random_bool_value = random_bool();
//	    
//		char* db_string;
//		asprintf(&db_string, "%d,%f,%s,%s", random_int_value, random_float_value, random_string_value, random_bool_value ? "true" : "false");
//
//		bool result = createNode("data.bin", db_string);
//		
//		free(db_string);	
//	}
//
//	return 0;
//}

//
// TEST 2
//	 

//int main() {
//
//	srand(time(NULL));	
//	
//	FILE *fp;
//	fp = fopen("timings.csv", "w+");
//	
//	int i;
//	
//	for(i = 0; i < 100; i++) {
//		
//		time_t rawtime_start;
//  		time(&rawtime_start);
//		
//		int random_int_value = random_int(1, 100);
//	    float random_float_value = random_float(1.0, 10.0);
//	    char* random_string_value = random_string();
//	    bool random_bool_value = random_bool();
//	    
//		char* db_string;
//		asprintf(&db_string, "%d,%f,%s,%s", random_int_value, random_float_value, random_string_value, random_bool_value ? "true" : "false");
//		
//		bool result = createNode("data.bin", db_string);
//		
//		sleep(0.3);
//		
//		time_t rawtime_end;
//  		time(&rawtime_end);
//  		
//  		fprintf(fp, "%f,%d\n", rawtime_end - rawtime_start, getFileSize("data.bin"));
//		
//		free(db_string);	
//	}
//
//	return 0;
//}

//
// TEST 3
//	 

//int main() {
//
//	srand(time(NULL));	
//	
//	FILE *fp;
//	fp = fopen("timings.csv", "w+");
//	
//	int i;
//	
//	for(i = 0; i < 10000; i++) {
//		
//		time_t rawtime;
//  		time(&rawtime);
//  		
//  		fprintf(fp, "%d,%d\n", i, rawtime);
//
//		deleteNodeByIndex("data.bin", i);	
//	}
//
//	return 0;
//}

//
// TEST 4
//	 

int main() {

	srand(time(NULL));	
	
	FILE *fp;
	fp = fopen("timings.csv", "w+");
	
	int i;
	
	for(i = 0; i < 100; i++) {
		
		findNodes("data.bin", "isAdult", "true");
			
		time_t rawtime;
  		time(&rawtime);
  		
  		fprintf(fp, "%d,%d\n", i, rawtime);
	}

	return 0;
}
























