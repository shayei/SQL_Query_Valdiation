
#include "Header.h"

int main()
{
	vector<table*> tableArr = scanTxtFile();
	int queryResult = scanQuery(tableArr);
	printResult(queryResult);
	freeTables(tableArr);
	system("pause");
}

void printResult(int result)
{
	switch (result)
	{
	case 0:
		cout << "valid" << endl;
		break;
	case 1:
		cout << "Parsing <o_d> failed" << endl;
		break;
	case 2:
		cout << "Parsing <attribute_list> failed" << endl;
		break;
	case 3:
		cout << "Parsing <table_list> failed" << endl;
		break;
	case 4:
		cout << "Parsing <condition> failed" << endl;
		break;

	default:
		break;
	}

}

vector<string> getQuery()
{
	string input;
	cout << "enter an sql query" << endl;
	getline(cin, input);

	vector<string> result;
	istringstream iss(input);
	for (string s; iss >> s; )
		result.push_back(s);

	return result;
}

int scanQuery(vector<table*> i_tableArr)
{
	vector<string> query = getQuery();
	int result = 0;
	int i = 0;

	int selectIndex = findString(query, "SELECT");
	int distinctIndex = findString(query, "DISTINCT");
	int fromIndex = findString(query, "FROM");
	int whereIndex = findString(query, "WHERE");


	if (selectIndex == -1 || fromIndex == -1 || whereIndex == -1)
	{
		result = 1;
	}
	if (!((selectIndex< fromIndex) && (fromIndex< whereIndex)))
		result = 1;
	//if (!(selectIndex == 0))
	//	result = 2;
	//if (!(fromIndex == 0))
	//	result = 3;
	//if (!(whereIndex == 0))
	//	result = 4;

	if (distinctIndex == 1)
	{
		i++;
	}
	else if (distinctIndex != -1)
	{
		result = 1; //distinct is not in valid place
	}

	if (IsAttributeList(i_tableArr, query, selectIndex, fromIndex, distinctIndex) == false)
	{
		result = 2;
	}
	if (IsTableList(i_tableArr, query, fromIndex, whereIndex) == false)
		result = 3;
	if (IsCondition(i_tableArr, query, whereIndex, query.size() - 1) == false)
		result = 4;

	return result;
}

string removeSpaces(vector<string> arr, int beginIndex, int endIndex)
{
	string result = "";
	for (int i = beginIndex; i < endIndex; i++)
	{
		result = result + arr[i];
	}

	return result;
}

// check paranthesis are balanced 
bool areParanthesisBalanced(string expr)
{
	stack <char> s;
	char x;

	// Traversing the Expression 
	for (size_t i = 0; i < expr.length(); i++)
	{
		if (expr[i] == '(' || expr[i] == '[' || expr[i] == '{')
		{
			// Push the element in the stack 
			s.push(expr[i]);
			continue;
		}

		// IF current current character is not opening 
		// bracket, then it must be closing. So stack 
		// cannot be empty at this point. 

		//if (s.empty()) 
		//	return false;

		switch (expr[i])
		{
		case ')':

			// Store the top element in a 
			x = s.top();
			s.pop();
			if (x == '{' || x == '[')
				return false;
			break;

		case '}':

			// Store the top element in b 
			x = s.top();
			s.pop();
			if (x == '(' || x == '[')
				return false;
			break;

		case ']':

			// Store the top element in c 
			x = s.top();
			s.pop();
			if (x == '(' || x == '{')
				return false;
			break;
		}
	}
	// Check Empty Stack 
	return (s.empty());
}

bool IsCondition(vector<table*> tableArr, vector<string>arr, int whereIndex, int endIndex)
{
	bool checkAll = true;
	bool checkOne = false;

	string conditionPart = "", simpleConditionPart = "";
	conditionPart = removeSpaces(arr, whereIndex + 1, arr.size());
	bool isBalancedPara = areParanthesisBalanced(conditionPart);
	if (!isBalancedPara)
		return false;

	conditionPart.erase(remove(conditionPart.begin(), conditionPart.end(), '('), conditionPart.end());
	conditionPart.erase(remove(conditionPart.begin(), conditionPart.end(), ')'), conditionPart.end());
	conditionPart.erase(remove(conditionPart.begin(), conditionPart.end(), ';'), conditionPart.end());


	int andPos = conditionPart.find("AND");
	int orPos = conditionPart.find("OR");
	int flag = 0;
	while ((andPos > 0 || orPos > 0) && flag == 0) //scan attributes
	{
		if (conditionPart.find("AND") == -1 && conditionPart.find("OR") == -1)
		{
			flag = 1;
			checkOne = SimpleCondition(tableArr, conditionPart);

		}
		else if (orPos != -1 && (orPos < andPos || (andPos == -1 && orPos != -1)))
		{
			simpleConditionPart = conditionPart.substr(0, orPos);
			checkOne = SimpleCondition(tableArr, simpleConditionPart);
			conditionPart = conditionPart.substr(orPos + 2);
		}
		else if (andPos != -1 && (andPos < orPos || (orPos == -1 && andPos != -1)))
		{
			simpleConditionPart = conditionPart.substr(0, andPos);
			checkOne = SimpleCondition(tableArr, simpleConditionPart);
			conditionPart = conditionPart.substr(andPos + 3);
		}

		checkAll = checkAll && checkOne;
		checkOne = false;
	}
	return checkAll;
}

bool SimpleCondition(vector<table*> tableArr, string i_condition)
{
	bool isLegal = false;
	int little = 0; // '<'
	int big = 0; // '>'
	int equal = 0; // '=='
	int bigEqual = 0; // '>='
	int littleEqual = 0; // '<='
	int littleBig = 0; // '<>'
	string symbolExist;
	int pos1 = i_condition.find("<>");

	if (pos1 > 0)
	{
		littleBig = 1;
	}
	pos1 = i_condition.find(">=");
	if (pos1 > 0)
	{
		bigEqual = 1;
	}
	pos1 = i_condition.find("<=");
	if (pos1 > 0)
	{
		littleEqual = 1;
	}
	if (littleBig == 0 && bigEqual == 0 && littleEqual == 0)
	{
		pos1 = i_condition.find(">");
		if (pos1 > 0)
		{
			big = 1;
		}
		pos1 = i_condition.find("<");
		if (pos1 > 0)
		{
			little = 1;
		}
		pos1 = i_condition.find("==");
		if (pos1 > 0)
		{
			equal = 1;
		}
	}


	if ((little > 0 || big > 0 || littleEqual > 0 || bigEqual > 0 || equal > 0 || littleBig > 0)
		&& (little + big + littleEqual + bigEqual + equal + littleBig == 1))
		//one from the symbols is exist + just one is exist
	{
		int littleEqual = i_condition.find("<=");
		int bigEqual = i_condition.find(">=");
		int equal = i_condition.find("==");
		int littleBig = i_condition.find("<>");
		int little = i_condition.find("<");
		int big = i_condition.find(">");

		int pos = max(little, max(big, (max(littleEqual, (max(bigEqual, (max(equal, littleBig))))))));
		string leftConstant, rightConstant;
		leftConstant = i_condition.substr(0, pos);
		rightConstant = (littleEqual > 0 || bigEqual > 0 || equal > 0 || littleBig > 0) ? i_condition.substr(pos + 2) : i_condition.substr(pos + 1);

		bool isLeftIsAttribute = false;
		bool isLeftIsAttriuteString = false;
		bool isLeftIsAttriuteInteger = false;
		bool isLeftIsConstantString = false;
		bool isLeftIsConstantNumber = false;
		checkWhichConstant(tableArr, leftConstant, &isLeftIsAttribute, &isLeftIsAttriuteString,
			&isLeftIsAttriuteInteger, &isLeftIsConstantString, &isLeftIsConstantNumber);

		bool isRightIsAttribute = false;
		bool isRightIsAttriuteString = false;
		bool isRightIsAttriuteInteger = false;
		bool isRightIsConstantString = false;
		bool isRightIsConstantNumber = false;
		checkWhichConstant(tableArr, rightConstant, &isRightIsAttribute, &isRightIsAttriuteString,
			&isRightIsAttriuteInteger, &isRightIsConstantString, &isRightIsConstantNumber);

		if (isLeftIsAttribute || isRightIsAttribute)
		{//Mandatory condition

			if (isLeftIsAttribute && isRightIsAttribute)
			{
				if ((isLeftIsAttriuteString && isRightIsAttriuteString) ||
					isLeftIsAttriuteInteger && isRightIsAttriuteInteger)
				{//if Both sides are attributes: both must be the same type
					isLegal = true;
				}
			}

			if (isLeftIsAttribute)
			{//so right constant is not an attribute
				if (isLeftIsAttriuteString && isRightIsConstantString)
				{
					isLegal = true;
				}
				if (isLeftIsAttriuteInteger && isRightIsConstantNumber)
				{
					isLegal = true;
				}
			}

			if (isRightIsAttribute)
			{//so left constant is not an attribute
				if (isRightIsAttriuteString && isLeftIsConstantString)
				{
					isLegal = true;
				}
				if (isRightIsAttriuteInteger && isLeftIsConstantNumber)
				{
					isLegal = true;
				}
			}

		}

	}

	return isLegal;
}

bool isDigit(char ch)
{
	return (ch >= 48 && ch <= 57);
}

//check if legal number
int GetNumber(string input, bool *isLegalNumber)
{
	*isLegalNumber = true;
	int num = 0;
	bool isNegative = false;
	isNegative = input[0] == '-'; //check if negative number
	if (input[0] == '-' || input[0] == '+')
	{
		input = input.substr(1); //delete the first symbol
	}
	for (size_t i = 0; i < input.length(); i++)
	{
		if (isDigit(input[i]))
		{
			num = num * 10 + input[i];
		}
		else
		{
			*isLegalNumber = false;
		}
	}

	if (isNegative)
		num *= -1;

	return num;
}

//check if string or number or attribute
void checkWhichConstant(vector<table*> tableArr, string constant, bool *isExistAttribute, bool *isAttriuteString,
	bool *isAttriuteInteger, bool *isConstantString, bool *isConstantNumber)
{
	if (constant[0] == '\"' && constant[constant.length() - 1] == '\"' && constant.length() > 2)
	{/*constant is a string: quotes in the beginning and in the end of the string
	  and contains at least one character inside*/
		*isConstantString = true;
	}
	else if (constant[0] == '-' || constant[0] == '+' || isDigit(constant[0]))
	{//constant is a number
		int number = GetNumber(constant, isConstantNumber);
	}
	else
	{//check if constant is an attribute
		attribute* curr = isAttributeExist(tableArr, constant);
		if (curr != nullptr)
		{
			*isExistAttribute = true;
			if (curr->type == "STRING")
			{
				*isAttriuteString = true;
			}
			if (curr->type == "INTEGER")
			{
				*isAttriuteInteger = true;
			}

		}
	}

}

int IsTableList(vector<table*> tableArr, vector<string>arr, int fromIndex, int whereIndex)
{
	string tablePart = "";
	tablePart = removeSpaces(arr, fromIndex + 1, whereIndex);

	bool checkAll = true;
	bool checkOne = false;
	int pos;
	string currentTable;
	int flag = 0;
	pos = tablePart.find(",");
	while (pos > 0 || flag == 0)
	{
		pos = tablePart.find(",");
		if (pos > 0)
		{
			currentTable = tablePart.substr(0, pos);
			tablePart = tablePart.substr(pos + 1);
		}
		else //last attribute
		{
			currentTable = tablePart;
		}

		for (size_t i = 0; i < tableArr.capacity(); i++)
		{
			if (tableArr[i]->tableName == currentTable)
			{
				checkOne = true;
				break;
			}
		}
		checkAll = checkAll && checkOne;
		checkOne = false;
		if (pos < 0)
		{
			flag = 1;
		}
	}


	return checkAll;
}

int IsAttributeList(vector<table*> tableArr, vector<string> arr, int selectIndex, int fromIndex, int distinctIndex)
{
	string attributePart = "";
	if (distinctIndex == -1)
		attributePart = removeSpaces(arr, selectIndex + 1, fromIndex);
	else
		attributePart = removeSpaces(arr, distinctIndex + 1, fromIndex);


	return isAsterisk(arr, fromIndex, distinctIndex) || isAttList(attributePart, tableArr);
}

bool isAttList(string attributePart, vector<table*> tableArr)
{
	bool checkAll = true;
	bool checkOne = false;
	int pos;
	string currentAttribute, tableName, attributeName;
	int flag = 0;
	pos = attributePart.find(",");
	while (pos > 0 || flag == 0)
	{
		pos = attributePart.find(",");
		if (pos > 0)
		{
			currentAttribute = attributePart.substr(0, pos);
			attributePart = attributePart.substr(pos + 1);
		}
		else //last attribute
		{
			currentAttribute = attributePart;
		}

		if (isAttributeExist(tableArr, currentAttribute) != nullptr)
			checkOne = true;
		checkAll = checkAll && checkOne;
		checkOne = false;
		if (pos < 0)
		{
			flag = 1;

		}
	}

	return checkAll;
}

attribute* isAttributeExist(vector<table*> tableArr, string input)
{
	bool check = false;
	int dotPos;
	attribute* curr = nullptr;
	string tableName, attributeName;
	dotPos = input.find(".");
	if (dotPos != -1) //check if found "."
	{
		tableName = input.substr(0, dotPos);
		attributeName = input.substr(dotPos + 1);
		for (size_t i = 0; i < tableArr.capacity(); i++)
		{
			if (tableArr[i]->tableName == tableName)
			{
				curr = tableArr[i]->m_attributeList->head;
				while (curr != nullptr)
				{
					if (curr->name == attributeName)
					{
						check = true;
						break;
					}
					curr = curr->next;
				}
			}
			if (check == true)
				break;
		}
	}
	if (check == true)
		return curr;
	else return nullptr;
}

bool isAsterisk(vector<string> arr, int fromIndex, int distinctIndex)
{
	bool valid = false;
	if (distinctIndex == 1 && fromIndex == 3 && arr[2] == "*")
	{//"DISTINCT" is exist
		valid = true;
	}
	if (distinctIndex == -1 && fromIndex == 2 && arr[1] == "*")
	{//"DISTINCT" is not exist
		valid = true;
	}

	return valid;
}

int findString(vector<string> arr, string check)
{
	int result = -1;
	for (size_t i = 0; i < arr.capacity(); i++)
	{
		if (arr[i] == check)
		{
			result = i;
			break;
		}
	}

	return result;
}

vector<table*> scanTxtFile()
{
	vector<table*> tableArr;
	table* newTable;

	string line;
	ifstream myfile;
	myfile.open("scheme.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			line.erase(remove(line.begin(), line.end(), ' '), line.end());

			newTable = new table();
			newTable->m_attributeList = new attributeList();
			makeEmptyAttributeList(newTable->m_attributeList);


			int pos = line.find("(");
			newTable->tableName = line.substr(0, pos); //find table name

			line = line.substr(pos + 1);
			pos = line.find(",");
			string newAttribute;
			int typePos;
			attribute* newAtt;
			int flag = 0;
			while (pos > 0 || flag == 0) //scan attributes
			{
				if (line.find(",") == -1)
				{
					flag = 1;
				}
				newAtt = new attribute();
				newAttribute = line.substr(0, pos);
				typePos = newAttribute.find(":");
				newAtt->name = newAttribute.substr(0, typePos);
				newAtt->type = newAttribute.substr(typePos + 1);
				newAtt->next = nullptr;
				insertNodeToEndAttributeList(newTable->m_attributeList, newAtt);
				newAtt = nullptr;
				line = line.substr(pos + 1);
				pos = line.find(",");
			}
			newTable->m_attributeList->tail->type = (newTable->m_attributeList->tail->type).substr(0, newTable->m_attributeList->tail->type.length() - 1);
			tableArr.push_back(newTable);
			newTable = nullptr;
		}
		myfile.close();
	}

	return tableArr;
}

void makeEmptyAttributeList(attributeList* l)
{
	l->head = l->tail = NULL;
}


void insertNodeToEndAttributeList(attributeList *lst, attribute *tail)
{
	if (lst->head == NULL)
	{
		lst->head = tail;
		lst->tail = tail;
	}
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
}

void freeLinkedList(attributeList* lst)
{
	attribute* curr = lst->head;
	attribute* temp;
	while (curr != nullptr)
	{
		temp = curr;
		curr = curr->next;
		delete(temp);
	}

	delete(lst);
}

void freeTables(vector<table*> arr)
{
	for (size_t i = 0; i < arr.capacity(); i++)
	{
		freeLinkedList(arr[i]->m_attributeList);
		delete(arr[i]);
	}
}