#include <iostream>
#include <vector>
#include <string>
#include <sstream> 
#include <fstream>
#include <string.h>
#include <algorithm>
#include <stack>

using namespace std;

struct attribute {
	string name;
	string type;
	attribute* next;
};

struct attributeList {
	attribute* head;
	attribute* tail;
};


struct table {
	string tableName;
	attributeList* m_attributeList;
};




void insertNodeToEndAttributeList(attributeList *lst, attribute *tail);
int scanQuery(vector<table*> i_tableArr);
vector<table*> scanTxtFile();
void makeEmptyAttributeList(attributeList* l);
bool isAsterisk(vector<string> arr, int fromIndex, int distinctIndex);
int findString(vector<string> arr, string check);
bool isAttList(string attributePart, vector<table*> tableArr);
string removeSpaces(vector<string> arr, int beginIndex, int endIndex);
int IsAttributeList(vector<table*> tableArr, vector<string> arr, int selectIndex, int fromIndex, int distinctIndex);
int IsTableList(vector<table*> tableArr, vector<string>arr, int fromIndex, int whereIndex);
bool areParanthesisBalanced(string expr);
bool IsCondition(vector<table*> tableArr, vector<string>arr, int whereIndex, int endIndex);
vector<string> getQuery();
void checkWhichConstant(vector<table*> tableArr, string constant, bool *isExistAttribute, bool *isAttriuteString,
	bool *isAttriuteInteger, bool *isConstantString, bool *isConstantNumber);
int GetNumber(string input, bool *isLegalNumber);
bool isDigit(char ch);
bool SimpleCondition(vector<table*> tableArr, string i_condition);
attribute* isAttributeExist(vector<table*> tableArr, string input);
void printResult(int result);
void freeLinkedList(attributeList* lst);
void freeTables(vector<table*> arr);