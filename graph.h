#ifndef LLP1_GRAPH_H
#define LLP1_GRAPH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_NAME_LENGTH 50
#define MAX_CHAR_VALUE 256
#define MAX_NODE_RELATIONS 50

enum DataType {
    INT32,
    FLOAT,
    CHAR_PTR,
    BOOL
};

struct Column {
    char name[MAX_NAME_LENGTH];
    enum DataType type;
    union {
        int32_t int32Value;
        float floatValue;
        char charValue[MAX_CHAR_VALUE];
        bool boolValue;
    };
};

struct Node {
    struct Column* columns;
    int relationsCount;
    int relations[MAX_NODE_RELATIONS];
};

struct GraphDB {
    int nodesCount;
    int columnsCount;
    struct Column* scheme;
    struct Node* nodes;
};

// Utils
void printHeaderGraphDB(struct GraphDB *db);
void printNode(struct GraphDB *db, struct Node *node);
void saveHeaderStructToFile(struct GraphDB* db, const char* fileName);
void loadHeaderStructFromFile(struct GraphDB* db, const char* fileName);
void movePointerToEndOfHeader(FILE* file);
void addNodeToFile(const char* fileName, struct Node* node);
void iterateByEachNode(const char* fileName);
size_t getFileSize(const char* fileName);
bool parseAndSetNode(struct GraphDB* db, char* inputString, struct Node* setted_node);
void clearFileData(const char* fileName);

// CRUD
void setScheme(struct GraphDB* db, struct Column *scheme, int columnsCount);
bool createNode(const char* fileName, char* inputString);
void findNodeByIndex(const char* fileName, int index);
void findNodes(const char* fileName, const char* columnName, const char* columnValue);
void updateNodeByIndex(const char* fileName, const char* columnName, const char* columnValue, int index);

void deleteNodeByIndex(const char* fileName, int index);
void setNewRelation(const char* fileName, int index1, int index2);
void clearAllRelationsOfNode(const char* fileName, int index);

#endif // LLP1_GRAPH_H
