#include "graph.h"

void setScheme(struct GraphDB* db, struct Column *scheme, int columnsCount) {
	db->scheme = scheme;
	db->columnsCount = columnsCount;
}

void printHeaderGraphDB(struct GraphDB *db) {
	
    printf("Column count: %d\n", db->columnsCount);
    
    printf("Scheme:\n");
    int k;
    for (k = 0; k < db->columnsCount; k++) {
        printf("\t%d. %s", k + 1, db->scheme[k].name);
        
        switch (db->scheme[k].type) {
            case INT32:
                printf(" (INT32)\n");
                break;
            case FLOAT:
                printf(" (FLOAT)\n");
                break;
            case CHAR_PTR:
                printf(" (CHAR_PTR)\n");
                break;
            case BOOL:
                printf(" (BOOL)\n");
                break;
        }
    }
    
    printf("Number of nodes: %d\n", db->nodesCount);
}

void printNode(struct GraphDB *db, struct Node *node) {
	
	int i;
    for (i = 0; i < db->columnsCount; i++) {
    	switch (node->columns[i].type) {
            case INT32:
                printf("%d ", node->columns[i].int32Value);
                break;
            case FLOAT:
                printf("%.2f ", node->columns[i].floatValue);
                break;
            case CHAR_PTR:
                printf("%s ", node->columns[i].charValue);
                break;
            case BOOL:
            	printf("%s ", node->columns[i].boolValue ? "true" : "false");
                break;
        }
    }
    
    printf("| relations number (%d) ", node->relationsCount);
    printf("relations: [");
    
    if (node->relationsCount != 0) {
		int k;
    	for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    		if (node->relations[k] != -1) {
    			printf(" %d ", node->relations[k]);
			} 
    	}
	}
	printf("] \n");
}

void saveHeaderStructToFile(struct GraphDB* db, const char* fileName) {
	FILE* file = fopen(fileName, "wb");
	
	if (file == NULL) {
        printf("Error opening file (probably it not exists'): %s\n", fileName);
        return;
    }
    
    fwrite(&db->columnsCount, sizeof(int), 1, file);
    
    int k;
    for (k = 0; k < db->columnsCount; k++) {
    	fwrite(&db->scheme[k], sizeof(struct Column), 1, file);
    }
    
    fwrite(&db->nodesCount, sizeof(int), 1, file);
    
    fclose(file);
}

void loadHeaderStructFromFile(struct GraphDB* db, const char* fileName) {

    FILE* file = fopen(fileName, "rb");
    
//    printf("current offset: %d\n", ftell(file));
    
    if (file == NULL) {
        printf("Error opening file (probably it not exists'): %s\n", fileName);
        return;
    }

    // Read the number of columns in the scheme
    int columnsCount;
    fread(&columnsCount, sizeof(int), 1, file);
    
//    printf("current offset: %d\n", ftell(file));

    // Read the data type of each column in the scheme
    db->scheme = malloc(columnsCount * sizeof(struct Column));
    db->columnsCount = columnsCount;
    int k;
    for (k = 0; k < columnsCount; k++) {
        fread(&db->scheme[k], sizeof(struct Column), 1, file);
    }
    
    int nodesCount;
    fread(&nodesCount, sizeof(int), 1, file);
    
    db->nodesCount = nodesCount;
    
    fclose(file);
}

void movePointerToEndOfHeader(FILE* file) {
	
	// count of colums
    int columnsCount;
    fread(&columnsCount, sizeof(int), 1, file);
    
    int k;
    for (k = 0; k < columnsCount; k++) {
        fseek(file, sizeof(struct Column), SEEK_CUR);
    }
	    
    // count of rows
    fseek(file, sizeof(int), SEEK_CUR);
}

// before use parseAndSetRow to create a row
void addNodeToFile(const char* fileName, struct Node* node) {
	
	// for columns and rows count
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
//	printf("rowCount: %d \n", db.rowCount);
	
	// write new count of rows
	FILE* write_file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(write_file);
	fseek(write_file, -sizeof(int), SEEK_CUR);
	
	int newNodesCount = db.nodesCount + 1;
	fwrite(&newNodesCount, sizeof(int), 1, write_file);
	
//	printf("newRowsCount: %d\n", newRowsCount);
//	printf("current offset: %d\n", ftell(write_file));
	
	// write new row
	fseek(write_file, 0, SEEK_END);
	
//	printf("current offset: %d\n", ftell(write_file));

	int i;
    for (i = 0; i < db.columnsCount; i++) {
    	fwrite(&node->columns[i], sizeof(struct Column), 1, write_file);
    }
    
    int relationsCount = 0;
    fwrite(&relationsCount, sizeof(int), 1, write_file);
    
    int k;
    for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    	int relationValue = -1;
    	fwrite(&relationValue, sizeof(int), 1, write_file);
    }
    
//    printf("current offset: %d\n", ftell(write_file));

	fclose(write_file);
}

// file needs offset to the row before calling
void setNodeFromFile(FILE* file, struct Node* node, const int columnsCount) {

	node->columns = malloc(columnsCount * sizeof(struct Column));
	
	int i;
    for (i = 0; i < columnsCount; i++) {
    	fread(&node->columns[i], sizeof(struct Column), 1, file);
    }
    
    fread(&node->relationsCount, sizeof(int), 1, file);
    
    int k;
    for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    	fread(&node->relations[k], sizeof(int), 1, file);
    }
}

// file needs offset to the row before calling
void writeNodeToFile(FILE* file, struct Node* node, const int columnsCount) {
	
	int i;
    for (i = 0; i < columnsCount; i++) {
    	fwrite(&node->columns[i], sizeof(struct Column), 1, file);
    }
    
    fwrite(&node->relationsCount, sizeof(int), 1, file);
    
    int k;
    for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    	fwrite(&node->relations[k], sizeof(int), 1, file);
    }
}

void iterateByEachNode(const char* fileName) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (db.nodesCount == 0) {
		printf("Error: db is empty");
		return;
	}
	
	FILE* file = fopen(fileName, "rb");
	movePointerToEndOfHeader(file);
	
	int i;
	for (i = 0; i < db.nodesCount; i++) {
		printf("%d. ", i + 1);
		struct Node node;
		setNodeFromFile(file, &node, db.columnsCount);
		printNode(&db, &node);
		
		free(node.columns);
	}
	
	fclose(file);
}

size_t getFileSize(const char* fileName) {
	FILE* file = fopen(fileName, "rb");
	
	if (file == NULL) {
    	printf("Error opening file (probably it not exists'): %s\n", fileName);
    	return 0;
	}
	
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fclose(file);
	
	return size;
}

bool parseAndSetNode(struct GraphDB* db, char* inputString, struct Node* setted_node) {
    // Create a copy of the input string to tokenize it
    char* inputStringCopy = strdup(inputString);

    // Tokenize the input string using the comma as the delimiter
    char* token = strtok(inputStringCopy, ",");
    int columnIndex = 0;
    struct Node node;
    node.columns = malloc(db->columnsCount * sizeof(struct Column));
    while (token != NULL && columnIndex < db->columnsCount) {
        struct Column* column = &node.columns[columnIndex];
        strncpy(column->name, db->scheme[columnIndex].name, MAX_NAME_LENGTH);
        column->type = db->scheme[columnIndex].type;

        switch (column->type) {
            case INT32:
                if (sscanf(token, "%d", &column->int32Value) != 1) {
                	free(inputStringCopy);
                	printf("Error add operation, argument %d has ivalid type (needs INT32)\n", columnIndex);
                    return false;
                }
                break;
            case FLOAT:
                if (sscanf(token, "%f", &column->floatValue) != 1) {
                	free(inputStringCopy);
                	printf("Error add operation, argument %d has ivalid type (needs FLOAT)\n", columnIndex);
                    return false;
                }
                break;
            case CHAR_PTR:
				if (strlen(token) + 1 > MAX_CHAR_VALUE) {
                    printf("Error add operation, argument %d is too long (CHAR_PTR)\n", columnIndex);
                }
                strcpy(column->charValue, token);
                break;
            case BOOL:
                if (strcmp(token, "true") == 0) {
                    column->boolValue = true;
                } else if (strcmp(token, "false") == 0) {
                    column->boolValue = false;
                } else {
                	free(inputStringCopy);
                	printf("Error add operation, argument %d has ivalid type (needs BOOL)\n", columnIndex);
                    return false;
                }
                break;
            default:
                break;
        }

        token = strtok(NULL, ",");
        columnIndex++;
    }

    // Check if all the columns were parsed
    if (columnIndex != db->columnsCount) {
        free(inputStringCopy);
        return false;
    }
	node.relationsCount = 0;
	*setted_node = node;

    free(inputStringCopy);

    return true;
}

void clearFileData(const char* fileName) {
	
	FILE* file = fopen(fileName, "r+");
	
	if (file == NULL) {
    	printf("Error opening file (probably it not exists'): %s\n", fileName);
	}
	
	ftruncate(fileno(file), 0);
	fclose(file);
}

// =========================== CRUD ================================
// =========================== CRUD ================================

// ======= CREATE ==========

bool createNode(const char* fileName, char* inputString) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	struct Node newNode;
	bool parseResult = parseAndSetNode(&db, inputString, &newNode);
	
	if (!parseResult) {
		printf("CreateNode Error: invalid inputString\n");
		return false;
	}
	
	addNodeToFile(fileName, &newNode);
	
	free(newNode.columns);
	
	return true;
}

// ======= READ ============

void findNodeByIndex(const char* fileName, int index) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (index < 0) {
		printf("FIND Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("FIND Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index) {
		printf("FIND Error: Out if range\n");
		return;
	}
	
	FILE* file = fopen(fileName, "rb");
	
	movePointerToEndOfHeader(file);
	
	// ��������� � ������ node
	int offset = ((int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS) * index;
	fseek(file, offset, SEEK_CUR);
	
	// ������ node
	struct Node node;
	setNodeFromFile(file, &node, db.columnsCount);
	
	printNode(&db, &node);
	
	fclose(file);
}

void findNodes(const char* fileName, const char* columnName, const char* columnValue) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	int columnIndex;
	
    for (columnIndex = 0; columnIndex < db.columnsCount; columnIndex++) {
    	
    	struct Column column = db.scheme[columnIndex];
    	
    	if (!(strcmp(column.name, columnName) == 0)) {
    		
    		if (columnIndex == db.columnsCount - 1) {
    			printf("FIND Error: column not found");
    			return;
			}
    		
    		continue;
		}
    	
    	switch (column.type) {
            case INT32:
            	{
	            	int32_t int32Value;
	            	
	            	if (sscanf(columnValue, "%d", &int32Value) != 1) {
	            		printf("FIND Error: argument is not INT32");
	    				return;
	            	}
	            	
	            	FILE* file = fopen(fileName, "rb");
					movePointerToEndOfHeader(file);
					
					printf("Nodes founded: \n");
					
					int i;
					for (i = 0; i < db.nodesCount; i++) {
						struct Node node;
						setNodeFromFile(file, &node, db.columnsCount);
						
						if (node.columns[columnIndex].int32Value == int32Value) {
							printf("%d) ", i + 1);
							printNode(&db, &node);
						}
						
						free(node.columns);
					}
					
					fclose(file);
	            	
	            	return;
	            }
            case FLOAT:
            	{
	            	float floatValue;
	            	
	            	if (sscanf(columnValue, "%f", &floatValue) != 1) {
	            		printf("FIND Error: argument is not FLOAT");
	    				return;
	            	}
	            	
	            	FILE* file = fopen(fileName, "rb");
					movePointerToEndOfHeader(file);
					
					printf("Nodes founded: \n");
					
					int i;
					for (i = 0; i < db.nodesCount; i++) {
						struct Node node;
						setNodeFromFile(file, &node, db.columnsCount);
						
						if (node.columns[columnIndex].floatValue == floatValue) {
							printf("%d) ", i + 1);
							printNode(&db, &node);
						}
						
						free(node.columns);
					}
					
					fclose(file);
	            	
	            	return;
	            }
            case CHAR_PTR:
            	if (strlen(columnValue) + 1 > MAX_CHAR_VALUE) {
            		printf("FIND Error: string value is too long");
    				return;
				}
				
				FILE* file = fopen(fileName, "rb");
				movePointerToEndOfHeader(file);
				
				printf("Nodes founded: \n");
				
				int i;
				for (i = 0; i < db.nodesCount; i++) {
					struct Node node;
					setNodeFromFile(file, &node, db.columnsCount);
					
					if (strcmp(node.columns[columnIndex].charValue, columnValue) == 0) {
						printf("%d) ", i + 1);
						printNode(&db, &node);
					}
					
					free(node.columns);
				}
					
				fclose(file);
				
            	return;
            case BOOL:
            	{
	            	bool boolValue;
	            	
	            	if (strcmp(columnValue, "true") == 0) {
	                    boolValue = true;
	                } else if (strcmp(columnValue, "false") == 0) {
	                    boolValue = false;
	                } else {
	                    printf("FIND Error: argument is not BOOL");
	                    return;
	                }
	                
	                FILE* file = fopen(fileName, "rb");
					movePointerToEndOfHeader(file);
					
					printf("Nodes founded: \n");
					
					int i;
					for (i = 0; i < db.nodesCount; i++) {
						
						struct Node node;
						setNodeFromFile(file, &node, db.columnsCount);
						
						if (node.columns[columnIndex].boolValue == boolValue) {
							printf("%d) ", i + 1);
							printNode(&db, &node);
						}
						
						free(node.columns);
					}
						
					fclose(file);
	                
	            	return;
	            }
            default:
            	return;
    	}
	}
}

// ======= UPDATE ==========

void updateNodeByIndex(const char* fileName, const char* columnName, const char* columnValue, int index) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (db.nodesCount - 1 < index) {
		printf("UPDATE Error: index out of range");
    	return;
	}
	
	if (index < 0) {
		printf("UPDATE Error: Invalid index");
    	return;
	}
	
	int columnIndex;
    for (columnIndex = 0; columnIndex < db.columnsCount; columnIndex++) {
    	
    	struct Column column = db.scheme[columnIndex];
    	
    	if (strcmp(column.name, columnName) == 0) {
    		break;
		} else {
			if (columnIndex == db.columnsCount - 1) {
    			printf("UPDATE Error: column not found");
    			return;
			}
		}
	}
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	
	// ��������� � ������ node
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	int offset = nodeSize * index;
	fseek(file, offset, SEEK_CUR);
	
	struct Node node;
	setNodeFromFile(file, &node, db.columnsCount);
	
	// ��������� ������� � ��� node, ������� ����� ������
	fseek(file, -(nodeSize), SEEK_CUR);
	
	switch (db.scheme[columnIndex].type) {
		case INT32:
			{
				int32_t int32Value;
	            
            	if (sscanf(columnValue, "%d", &int32Value) != 1) {
            		printf("UPDATE Error: argument is not INT32");
    				return;
            	}
            	
            	printf("old int value: %d\n", node.columns[columnIndex].int32Value);
            	
            	node.columns[columnIndex].int32Value = int32Value;
            	
            	printf("new int value: %d\n", node.columns[columnIndex].int32Value);
            	
				break;	
			}
		case FLOAT:
			{
				float floatValue;
	            	
            	if (sscanf(columnValue, "%f", &floatValue) != 1) {
            		printf("UPDATE Error: argument is not FLOAT");
    				return;
            	}
            	
            	printf("old floatValue value: %f\n", node.columns[columnIndex].floatValue);
            	
            	node.columns[columnIndex].floatValue = floatValue;
            	
            	printf("new floatValue value: %f\n", node.columns[columnIndex].floatValue);
            	
				break;
			}
		case CHAR_PTR:
			if (strlen(columnValue) + 1 > MAX_CHAR_VALUE) {
        		printf("UPDATE Error: string value is too long");
				return;
			}
			
			printf("old char: ");
			printf(node.columns[columnIndex].charValue);
			printf("\n");
			
			strncpy(node.columns[columnIndex].charValue, columnValue, MAX_CHAR_VALUE);
			
			printf("new char: ");
			printf(node.columns[columnIndex].charValue);
			printf("\n");
			
			break;
		case BOOL:
			{
				bool boolValue;
	            	
            	if (strcmp(columnValue, "true") == 0) {
                    boolValue = true;
                } else if (strcmp(columnValue, "false") == 0) {
                    boolValue = false;
                } else {
                    printf("UPDATE Error: argument is not BOOL");
                    return;
                }
                
                printf("old bool value: %d\n", node.columns[columnIndex].boolValue);
                
                node.columns[columnIndex].boolValue = boolValue;
                
                printf("new bool value: %d\n", node.columns[columnIndex].boolValue);
                
				break;
			}
		default:
			return;
	}
	
	int i;
	for (i = 0; i < db.columnsCount; i++) {
		fwrite(&node.columns[i], sizeof(struct Column), 1, file);
	}
	
	fclose(file);
}

// ======= DELETE ==========

void deleteNodeByIndex(const char* fileName, int index) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (index < 0) {
		printf("DELETE Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("DELETE Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index) {
		printf("DELETE Error: Out if range\n");
		return;
	}
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	
	printf("current offset: %d\n", ftell(file));
	
	int headerSize = ftell(file);
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	
	if (db.nodesCount == 1) {
		// remove without replacing (if db contains only 1 element and delete it)
		printf("HERE nodesCount == 1 \n");
		ftruncate(fileno(file), headerSize);
	} else if (db.nodesCount - 1 == index) {
		// remove without replacing (if delete last element)
		printf("HERE db.nodesCount - 1 == index \n");
		
		// ���� ���������� � ��������� node, ������� ����� �������
		fseek(file, -nodeSize, SEEK_END);
		struct Node lastNode;
		setNodeFromFile(file, &lastNode, db.columnsCount);
		
		// ������� ����� � ������ nodes ����� ��������� ����� ����
		int k;
    	for (k = 0; k < MAX_NODE_RELATIONS; k++) {
	    	if (lastNode.relations[k] != -1) {
	    		dropRelation("data.bin", lastNode.relations[k], index);
			}
		}
		
		int newFileSize = headerSize + nodeSize * (db.nodesCount - 1);
		ftruncate(fileno(file), newFileSize);
		
	} else {
		printf("HERE deleting with replacing \n");
		
		// ��������� � node, ������� ����� �������, ����� ������� �����
		fseek(file, nodeSize * index, SEEK_CUR);
		struct Node deletingNode;
		setNodeFromFile(file, &deletingNode, db.columnsCount);
		
		// ������� �����
		int i;
    	for (i = 0; i < MAX_NODE_RELATIONS; i++) {
	    	if (deletingNode.relations[i] != -1) {
	    		dropRelation("data.bin", deletingNode.relations[i], index);
			}
		}
		
		fclose(file);
		file = fopen(fileName, "rb+");
		
		// ����� ��������� � ��������� node, ����� �������� ��
		fseek(file, -nodeSize, SEEK_END);
		struct Node lastNode;
		setNodeFromFile(file, &lastNode, db.columnsCount);
		
		int lastIndex = db.nodesCount - 1;
		
		int k;
    	for (k = 0; k < MAX_NODE_RELATIONS; k++) {
	    	if (lastNode.relations[k] != -1) {
	    		updateRelation("data.bin", lastNode.relations[k], lastIndex, index);
			}
		}
		
		// ������ ���������� ���� ��������� node
		fseek(file, 0, SEEK_SET);
		movePointerToEndOfHeader(file);
		fseek(file, nodeSize * index, SEEK_CUR);
		int j;
	    for (j = 0; j < db.columnsCount; j++) {
	    	fwrite(&lastNode.columns[j], sizeof(struct Column), 1, file);
	    }
	    
	    // ����� ��������� �� �� (���� - �������� � ������ nodes �����)
	    int relationsCount = lastNode.relationsCount;
	    fwrite(&relationsCount, sizeof(int), 1, file);
	    
	    int a;
	    for (a = 0; a < MAX_NODE_RELATIONS; a++) {
	    	int relationValue = lastNode.relations[a];
	    	fwrite(&relationValue, sizeof(int), 1, file);
	    }
			
		// ����� �������� ���� �� �������� newFileSize
		int newFileSize = headerSize + nodeSize * (db.nodesCount - 1);
		ftruncate(fileno(file), newFileSize);
	}
	
	fseek(file, 0, SEEK_SET);
	movePointerToEndOfHeader(file);
	fseek(file, -sizeof(int), SEEK_CUR);
	int newNodesCount = db.nodesCount - 1;
	fwrite(&newNodesCount, sizeof(int), 1, file);
	
	fclose(file);
}

// =========================== CRUD ================================
// =========================== CRUD ================================

// ====================== Relations ================================

void setNewRelation(const char* fileName, int index1, int index2) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (index1 < 0 || index2 < 0) {
		printf("NewRelation Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("NewRelation Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index1 || db.nodesCount - 1 < index2) {
		printf("NewRelation Error: Out if range\n");
		return;
	} else if (index1 == index2) {
		printf("NewRelation Error: indexes are equal\n");
		return;
	}
	
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index1, SEEK_CUR);
	
	struct Node node1;
	setNodeFromFile(file, &node1, db.columnsCount);
	
	fseek(file, 0, SEEK_SET);
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index2, SEEK_CUR);
	
	struct Node node2;
	setNodeFromFile(file, &node2, db.columnsCount);
	 
	// ��������, ��� �� ������������ ��� 50 ����������
	if (node1.relationsCount == 50) {
		printf("NewRelation Error: node1 has max relations\n");
		return;
	}
	
	if (node2.relationsCount == 50) {
		printf("NewRelation Error: node2 has max relations\n");
		return;
	}
	
	// ��������, ��� ������� ��� �� ���������
	int k;
    for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    	if (node1.relations[k] == index2) {
    		printf("NewRelation Error: relation already exists\n");
    		return;
		}
	}
	
	// ��������� ����� ��� ������ ����
	int i;
    for (i = 0; i < MAX_NODE_RELATIONS; i++) {
    	if (node1.relations[i] == -1) {
    		node1.relations[i] = index2;
    		break;
		}
	}
	
	node1.relationsCount = node1.relationsCount + 1;
	
	fseek(file, 0, SEEK_SET);
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index1, SEEK_CUR);
	
	writeNodeToFile(file, &node1, db.columnsCount);
	
	// ��������� ����� ��� ������ ����
	int j;
    for (j = 0; j < MAX_NODE_RELATIONS; j++) {
    	if (node2.relations[j] == -1) {
    		node2.relations[j] = index1;
    		break;
		}
	}
	
	node2.relationsCount = node2.relationsCount + 1;
	
	fseek(file, 0, SEEK_SET);
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index2, SEEK_CUR);
	
	writeNodeToFile(file, &node2, db.columnsCount);
	
	fclose(file);
}

// index1 - node where drop relation
// index2 - drop index
// function helper, when we update relations or delete some nodes
void dropRelation(const char* fileName, int index1, int index2) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);

	if (index1 < 0 || index2 < 0) {
		printf("DropRelation Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("DropRelation Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index1 || db.nodesCount - 1 < index2) {
		printf("DropRelation Error: Out if range\n");
		return;
	} else if (index1 == index2) {
		printf("DropRelation Error: indexes are equal\n");
		return;
	}
	
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index1, SEEK_CUR);
	
	struct Node node;
	setNodeFromFile(file, &node, db.columnsCount);
	fseek(file, -(nodeSize), SEEK_CUR);
	
	node.relationsCount = node.relationsCount - 1;
	
	int j;
    for (j = 0; j < MAX_NODE_RELATIONS; j++) {
    	if (node.relations[j] == index2) {
    		node.relations[j] = -1;
		}
	}
	
	writeNodeToFile(file, &node, db.columnsCount);
	
	fclose(file);
}

// index1 - node where update relation
// oldIndex
// newIndex
// function helper, when we delete some nodes
void updateRelation(const char* fileName, int index, int oldIndex, int newIndex) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);

	if (index < 0 || oldIndex < 0 || newIndex < 0) {
		printf("DropRelation Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("DropRelation Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index || db.nodesCount - 1 < oldIndex || db.nodesCount - 1 < newIndex) {
		printf("DropRelation Error: Out if range\n");
		return;
	} else if (index == oldIndex || index == newIndex || oldIndex == newIndex) {
		printf("DropRelation Error: indexes are equal\n");
		return;
	}
	
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index, SEEK_CUR);
	
	struct Node node;
	setNodeFromFile(file, &node, db.columnsCount);
	fseek(file, -(nodeSize), SEEK_CUR);
	
	int j;
    for (j = 0; j < MAX_NODE_RELATIONS; j++) {
    	if (node.relations[j] == oldIndex) {
    		node.relations[j] = newIndex;
		}
	}
	
	writeNodeToFile(file, &node, db.columnsCount);
	
	fclose(file);
}

void clearAllRelationsOfNode(const char* fileName, int index) {
	
	struct GraphDB db;
	loadHeaderStructFromFile(&db, fileName);
	
	if (index < 0) {
		printf("ClearRelation Error: Invalid index\n");
		return;
	} else if (db.nodesCount == 0) {
		printf("ClearRelation Error: db is empty\n");
		return;
	} else if (db.nodesCount - 1 < index) {
		printf("ClearRelation Error: Out if range\n");
		return;
	}
	
	int nodeSize = (int)sizeof(struct Column) * db.columnsCount + (int)sizeof(int) + (int)sizeof(int) * MAX_NODE_RELATIONS;
	
	FILE* file = fopen(fileName, "rb+");
	movePointerToEndOfHeader(file);
	fseek(file, nodeSize * index, SEEK_CUR);
	
	struct Node node;
	setNodeFromFile(file, &node, db.columnsCount);
	if (node.relationsCount == 0) {
		printf("ClearRelation: node has zero relations");
		fclose(file);
		return;
	}
	fseek(file, -(nodeSize), SEEK_CUR);
	
	// drop relations in other nodes
	int k;
    for (k = 0; k < MAX_NODE_RELATIONS; k++) {
    	if (node.relations[k] != -1) {
    		dropRelation("data.bin", node.relations[k], index);
		}
	}
	
	// update current node with zero relations
	node.relationsCount = 0;
	int j;
    for (j = 0; j < MAX_NODE_RELATIONS; j++) {
    	node.relations[j] = -1;
	}
	
	writeNodeToFile(file, &node, db.columnsCount);
	
	fclose(file);
}



