/*
By Joshua Ball
CS 3361 Interpreter Programming Project

Input a string into the console no longer which will then be evaluated to be valid or invalid for syntax or input reasons. This also outputs a string of the tree created.

This program utilizes an Abstract Syntax Tree to analyze the given input. Note that the value X in "Evaluation: X" is the evaluated value of the given expression.

At the bottom of the output is an output of the AST.

The output of success shows that the given string was able to successfully tokenize and parse with recursive decent.
Example input:

Input: T -> F
Output:

Success 
Evaluation: F

Tree output:
(-> T F)

*/



#include<iostream>
#include<string>
#include<vector>
using namespace std;

//Structs
typedef struct AST *pNODE;
struct AST { string info; pNODE children[2]; };
struct tokRslt { bool success; vector<string> syms; };
struct parseRslt { bool success; AST ast; };
struct TPERslt { bool val; string msg; };

pNODE cons(string s, pNODE c1, pNODE c2) {
	pNODE ret = new AST;
	ret->info = s;  // same as (*ret).info = s
	ret->children[0] = c1;
	ret->children[1] = c2;
	return ret;
}

tokRslt tokenize(string s);
bool validCharacter(char character);
parseRslt parse(vector<string> V);
bool isSymbol(string v);

TPERslt TPE(string s);
bool eval(AST T);
string TPEOut(string s);
pNODE Bool_stmt(vector<string> V, int i, int j);
pNODE Atom(vector<string> V, int i, int j);
pNODE Literal(vector<string> V, int i, int j);
pNODE Implcation(vector<string> V, int i, int j);
pNODE Disjunction(vector<string> V, int i, int j);
pNODE Conjunction(vector<string> V, int i, int j);
pNODE Unbreakable(vector<string> V, int i, int j);
void prinTree(AST T) {
	// If both children are NULL, just print the symbol
	if (T.children[0] == NULL) {
		cout << T.info;
		return;
	}
	cout << "(" << T.info << " ";
	prinTree(*(T.children[0]));
	cout << " ";

	if (T.children[1] != NULL)
		prinTree(*(T.children[1]));
	cout << ")";
}

int main() {
	string s;
	tokRslt h;
	parseRslt p;
	
		cout << "Please input a string below" << endl << endl;
		cout << "Input: ";
		getline(cin, s);
	
		h = tokenize(s);
		if (h.success == false) {
			cout << "Invalid input in string" << endl;
			exit(1);
		}
		
		TPERslt a = TPE(s);
		cout << a.msg << endl;
		p = parse(h.syms);

		if (p.success == false) {
			cout << "Invald syntax" << endl;
			exit(1);
		}

		string b = TPEOut(s);
		if (a.val == 1) {
			cout << "Evaluation: T" << endl;
		}
		else
			cout << "Evaluation: F" << endl;
			cout << endl;
			cout << "Tree output: " << endl;
			prinTree(p.ast);
			cout << endl;
	
	
	return 0;
}
//given a character of type string, will verify the given symbol is valid of the While Language
bool validCharacter(char character) {
	if (isdigit(character)) {
		return true;
	}
	//T and F might be true and false 
	else if (character == 'T' || character == 'F' || character == '-' || character == ':' || character == '=' || character == ' ' || character == '=' || character == '*' || character == '/' || isalpha(character)) {
		return true;
	}
	else { return false; }


}
bool isSymbol(string v) {
	return (v == ":=" || v == "(" || v == ")" || v == "*" || v == "-" || v == "/" || v == "" || v == "=" || v == ":" || v == ";" || v == ">" || v == "<" || v == "=<" || v == "<=" || v == "<>" || v == "+");
}


tokRslt tokenize(string s) {
	tokRslt t;
	t.success = false;
	string n;
	for (int i = 0; i < s.length(); i++) {
		if ((s[i] == 'T' || s[i] == 'F' || s[i] == '-' || s[i] == 'v' || s[i] == '>' || s[i] == '^' || s[i] == '->' || s[i] == '~' || s[i] == '(' || s[i] == ')' || s[i] == ' ')) {
			n = "";
			n = n + s[i];
			//Work time..
			//:= assignment
			if (s[i] == '-') {
				//checking next element
				if (s[i + 1] == '>') {
					i++;
					t.syms.push_back("->");
					t.success = true;
				}
				else {
					t.syms.push_back(n);
					t.success = false;
					break;
				}
			}

			
			else if (s[i] == 'T') {
				t.syms.push_back("T");
				t.success = true;
				i++;
			}
			else if (s[i] == 'F') {
				t.syms.push_back("F");
				t.success = true;
				i++;
			}
			
		}
		else {
			t.success = false;
			break;
			return t;
		}
		
			/*
			else if (isalpha(s[i])) {
				t.syms.push_back(n);
				t.success = true;
			}
			*/

		//end of valid character block
	}
		//invalid character block
		return t;
	}

parseRslt parse(vector<string> V) {
	parseRslt result;

	if (V.size() == 1) {
		if (V[0] == " " || V[0] == "")
			result.success = false;
	}

	int i = 0;//start of V
	int j = V.size();//size of V
	pNODE A = Bool_stmt(V, i, j);

	result.success = (A != NULL);

	if (result.success == true) {
		result.ast = *A;
		return result;
	}
	else
		result.success = false;
	return result;

}

//Functions from B -> IT -> IT_Tail -> OT -> OT_Tail -> AT -> AT_TAIL -> L -> A
//For recursive decent


pNODE Bool_stmt(vector<string> V, int i, int j) {
	pNODE W = Implcation(V, i, j);
	if (W != NULL) {
		return W;
	}

	return NULL;

}

pNODE Literal(vector<string> V, int i, int j) {

	for (int c = i; c<j; c++) {
		if (V[c] == "~") {

			//	pNODE t1 = Expression(V, i ,c);
			pNODE t2 = Literal(V, i + 1, j);
			if (t2 != NULL) {

				return cons("~", t2, NULL);
			}
		}
	}
	pNODE test = Unbreakable(V, i, j);
	if (test != NULL) {
		return test;
	}
	// All attempts failed
	return NULL;
}

pNODE Atom(vector<string> V, int i, int j) {

	if (i != j - 1 || i >= V.size()) {
		return NULL;
	}
	if (V[i] == "T" || V[i] == "F") {
		return cons(V[i], NULL, NULL);
	}
	return NULL;
}

pNODE Unbreakable(vector<string> V, int i, int j) {
	// U::=  "(" Expression ")"
	pNODE test = Atom(V, i, j);
	if (test != NULL) {

		return test;
	}

	else if (V[i] == "(" && V[j - 1] == ")") {
		pNODE temp = Bool_stmt(V, i + 1, j - 1);
		if (temp != NULL) {
			return temp;
		}
	}

	// All attempts failed
	return NULL;
}

pNODE Conjunction(vector<string> V, int i, int j) {

	for (int c = i; c<j; c++) {
		if (V[c] == "^") {
			pNODE t1 = Conjunction(V, i, c);
			pNODE t2 = Literal(V, c + 1, j);
			if (t1 != NULL && t2 != NULL)
				return cons("^", t1, t2);
		}
	}

	pNODE A = Literal(V, i, j);
	if (A != NULL) {

		return A;
	}
	return NULL;
}
//Checks for the following parameneters: v(OR)
pNODE Disjunction(vector<string> V, int i, int j) {

	for (int c = i; c<j; c++) {

		if (V[c] == "v") {
			pNODE t1 = Disjunction(V, i, c);
			pNODE t2 = Conjunction(V, c + 1, j);
			if (t1 != NULL && t2 != NULL)
				return cons("v", t1, t2);
		}
	}
	pNODE A = Conjunction(V, i, j);
	if (A != NULL) {
		return A;
	}
	return NULL;
}
//Checks for the following parameneters: =>(false and true is false otherwise is true)
pNODE Implcation(vector<string> V, int i, int j) {
	for (int c = i; c<j; c++) {

		if (V[c] == "->") {
			pNODE t1 = Disjunction(V, i, c);
			pNODE t2 = Implcation(V, c + 1, j);
			if (t1 != NULL && t2 != NULL)
				return cons("->", t1, t2);
		}
	}
	pNODE A = Disjunction(V, i, j);
	if (A != NULL) {
		return A;
	}
	return NULL;
}


bool eval(AST T) {
	string op = T.info;
	bool temp;
	if (op == "~") {
		temp = eval(*T.children[0]);
		if (!temp) {

			return  1;
		}
		else

			return 0;
	}
	//F
	else if (op == "F") {
		return 0;
	}
	//T
	else if (op == "T") {
		return 1;
	}
	//OR
	else if (op == "v") {
		if (eval(*T.children[0]) || eval(*T.children[1])) {
			return 1;
		}
		else
			return 0;
	}
	//AND
	else if (op == "^") {
		if ((eval(*T.children[0]) && eval(*T.children[1]))) {
			return 1;
		}
		else
			return 0;
	}
	//false and true is false otherwise is true
	else if (op == "->") {
		if (eval(*T.children[0]) && !eval(*T.children[1])) {
			return 0;
		}
		else
			return 1;
	}
	else if (op == "<=>") {
		if ((eval(*T.children[1]) == eval(*T.children[0]))) {

			return 1;
		}
		else

			return 0;
	}

	return NULL;
}

TPERslt TPE(string s) {

	TPERslt tpe;
	tokRslt tempToken;
	parseRslt tempParse;

	tempToken = tokenize(s);

	tempParse = parse(tempToken.syms);
	if (!tempToken.success) {
		tpe.msg = "symbol error";
		return tpe;
	}
	else if (!tempParse.success) {
		tpe.msg = "grammar error";
		return tpe;
	}
	else if (tempParse.success && tempToken.success) {
		tpe.msg = "success";
		tpe.val = eval(tempParse.ast);
		return tpe;
	}
}

string TPEOut(string s) {
	string outputMsg;

	if (TPE(s).msg == "success") {
		if (TPE(s).val == true) {
			outputMsg = "true";
			return outputMsg;
		}
		else if (TPE(s).val == false) {
			outputMsg = "false";
			return outputMsg;
		}
	}
	else if (TPE(s).msg == "grammar error") {
		outputMsg = "grammar error";
		return outputMsg;
	}

	else if (TPE(s).msg == "symbol error") {
		outputMsg = "symbol error";
		return outputMsg;
	}

	return "FAILED";
}
