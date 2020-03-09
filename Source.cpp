#include <iostream>
#include <fstream>
#include<string>
#include <time.h>

using namespace std;


//retrieves array index of a character
int index(char letter) {
	return letter - 97;
}

class LNode {
public:
	string word;
	LNode* prev;
	LNode* next;

	LNode(string w) {
		word = w;
		next = NULL;
		prev = NULL;

	}

	LNode() {
		next = NULL;
		prev = NULL;
	}
};

class ResultList {
private:
	LNode* head; // List head pointer
	LNode* tail;
public:
	ResultList() { // Constructor
		head = NULL;
		tail = NULL;
	}
	~ResultList(void) { }; // Destructor


	void insert(string val) { //function to insert a node at the tail
		LNode* newNode = new LNode(val), *temp;

		if (tail == NULL) {
			tail = newNode;
			head = newNode;
			newNode->word = val;
			newNode->next = NULL;
			newNode->prev = NULL;
		}

		else {
			temp = tail;
			tail->next = newNode;
			tail = newNode;
			newNode->word = val;
			newNode->next = NULL;
			newNode->prev = temp;

		}

	}
	bool searchForDuplicate(ResultList l) {
		LNode* nodePtr = this->head;
		LNode* search = l.head;
		while (nodePtr != NULL && l.head != NULL) {
			if (nodePtr->word == search->word) {
				return false;
			}
			nodePtr = nodePtr->next;
		}

		return true;
	}
	void clear() {

		if (head != NULL) {
			LNode* nodeptr = head;
			LNode* nextnode;

			while (nodeptr != NULL)
			{
				nextnode = nodeptr->next;
				delete nodeptr;
				nodeptr = nextnode;
			}

			head = NULL;
			tail = NULL;

		}



	}
	void displayList() { //function to display the node values present in the list
		LNode* nodePtr;
		nodePtr = head;
		if (head != NULL) {

			while (nodePtr != NULL) {
				cout << nodePtr->word << endl;
				nodePtr = nodePtr->next;
			}
			cout << endl;
		}

	}
	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		else
			return false;
	}
	//append an entire list to the end
	void append(ResultList list) {
		LNode* temp;
		LNode* newTail = list.tail;
		LNode* nextNode = list.head;

		if (this->tail == NULL && list.head != NULL) {
			this->tail = newTail;
			this->head = nextNode;
		}

		else if (list.head != NULL) {
			this->tail->next = nextNode;
			this->tail = newTail;
		}

	}




};
//a node
class Node {

public:
	char letter;			//the letter being stored
	Node* children[26];		//array of pointers to child nodes, a = 0 to z = 25. NULL by default
	bool end; //if this letter is the end of the word

	//CONSTRUCTOR   
	Node(char l) {
		letter = l;
		//initialise all children to NULL
		for (int x = 0; x < 26; x++)
			children[x] = NULL;
		end = false;
	}

};

class Trie {

public:
	Node* root = new Node('0'); //the root node has no letter

	void insert(string word) {
		int length = word.length();
		char addend; //current letter that needs to be inserted into trie
		int aI; //addend's index
		Node* nodeptr = root;
		Node* newnode;

		for (int i = 0; i < length; i++) {
			addend = word.at(i); //store the current character to add
			aI = index(addend); //computer and store the current character's index in a children array

			//check if that letter's node doesn't exist as a child
			if (nodeptr->children[aI] == NULL) {
				newnode = new Node(addend); //make a new node with that character
				nodeptr->children[aI] = newnode; //connect the new node to the correct index as a child of the previous node
				nodeptr = newnode; //now work on adding a letter to this new node
			}
			//if it already does
			else {
				nodeptr = nodeptr->children[aI]; //move directly to that node
			}
		}

		nodeptr->end = true;

	}

	void del(string word) {



	}



	bool search(string word) {
		int length = word.length();
		char searchend; //current letter that needs to be searched in the trie
		int sI; //searchend's index
		Node* nodeptr = root;

		for (int i = 0; i < length; i++) {
			searchend = word.at(i); //store the current character to search
			sI = index(searchend);
			//check if the next letter exists
			if (nodeptr->children[sI] != NULL) {
				nodeptr = nodeptr->children[sI]; //go to this node
			}
			//if it doesnt
			else {
				return false;
			}
		}

		//if it succeeds all iterations to reach an end
		return nodeptr->end;
	}

	/* Suggestion Algorithms */


	ResultList suggestInsertions(string word, int character) {
		int length = word.length();
		char searchend; //character being searched in order to traverse the tree
		int sI; //searchend's index
		Node* nodeptr = root; //current node being visited
		Node* subtree; //the root node of the subtree from where words will be searched
		bool found = false;//stores whether a suggestion has been found
		bool complete = false; //stores whether a search is complete
		string result = ""; //stores word formed through traversal
		string prefix;
		ResultList results;

		//traverse to the node to which you want to insert a letter after
		for (int i = 0; i < character; i++) {
			searchend = word.at(i); //store the current character to search
			sI = index(searchend);
			//check if the next letter exists
			if (nodeptr->children[sI] != NULL) {
				nodeptr = nodeptr->children[sI]; //go to this node
				result += nodeptr->letter;
			}
		}

		prefix = result;
		subtree = nodeptr; //store root of this subtree

		//try inserting a single letter
		for (int i = 0; i < 26; i++) {
			found = false; //reset found status
			complete = false; //reset complete status
			nodeptr = subtree; //go to start of subtree
			result = prefix; //reset result to initial value

			//check if there are potential words with that letter
			if (nodeptr->children[i] != NULL) {
				nodeptr = nodeptr->children[i]; //go to the node of each letter
			}
			else {
				continue; //otherwise try next letter
			}

			result += nodeptr->letter;

			//search the rest of the characters
			for (int i = character; i < length; i++) {
				searchend = word.at(i); //store the current character to search
				sI = index(searchend);
				//check if the next letter exists
				if (nodeptr->children[sI] != NULL) {
					nodeptr = nodeptr->children[sI]; //go to this node
					result += nodeptr->letter;
				}
				//if it doesnt
				else {
					found = false;
					complete = true;
				}
			}


			if (!complete)
				found = nodeptr->end;

			if (found && result.length() == word.length() + 1) {
				if (results.searchForDuplicate(results)) {
					results.insert(result);
				}
			}
			//cout << result << endl;

		}

		return results;
	}




	ResultList suggestDeletions(string word, int character) {
		int length = word.length();
		ResultList results;
		char searchend; //character being searched in order to traverse the tree
		int sI; //searchend's index
		Node* nodeptr = root; //current node being visited
		Node* subtree; //the root node of the subtree from where words will be searched
		bool found = false;//stores whether a suggestion has been found
		bool complete = false; //stores whether a search is complete
		string result = ""; //stores word formed through traversal
		string prefix;

		//traverse to the node to which you want to insert a letter after
		for (int i = 0; i < character; i++) {
			searchend = word.at(i); //store the current character to search
			sI = index(searchend);
			//check if the next letter exists
			if (nodeptr->children[sI] != NULL) {
				nodeptr = nodeptr->children[sI]; //go to this node
				result += nodeptr->letter;
			}
		}

		prefix = result;
		subtree = nodeptr; //store root of this subtree

		//search the rest of the characters except the one to delete
		for (int i = character + 1; i < length; i++) {
			searchend = word.at(i); //store the current character to search
			sI = index(searchend);
			//check if the next letter exists
			if (nodeptr->children[sI] != NULL) {
				nodeptr = nodeptr->children[sI]; //go to this node
				result += nodeptr->letter;
			}
			//if it doesnt
			else {
				found = false;
				complete = true;
			}
		}


		if (!complete)
			found = nodeptr->end;

		if (found && result.length() == word.length() - 1) {
			results.insert(result);
		}
		return results;
	}



	ResultList suggestSubstitutions(string word, int character) {
		int length = word.length();
		ResultList results;
		char searchend; //character being searched in order to traverse the tree
		int sI; //searchend's index
		Node* nodeptr = root; //current node being visited
		Node* subtree; //the root node of the subtree from where words will be searched
		bool found = false;//stores whether a suggestion has been found
		bool complete = false; //stores whether a search is complete
		string result = ""; //stores word formed through traversal
		string prefix;

		//traverse to the node to which you want to insert a letter after
		for (int i = 0; i < character; i++) {
			searchend = word.at(i); //store the current character to search
			sI = index(searchend);
			//check if the next letter exists
			if (nodeptr->children[sI] != NULL) {
				nodeptr = nodeptr->children[sI]; //go to this node
				result += nodeptr->letter;
			}
		}

		prefix = result;
		subtree = nodeptr; //store root of this subtree

		//try inserting a single letter
		for (int i = 0; i < 26; i++) {
			found = false; //reset found status
			complete = false; //reset complete status
			nodeptr = subtree; //go to start of subtree
			result = prefix; //reset result to initial value

			//check if there are potential words with that letter
			if (nodeptr->children[i] != NULL) {
				nodeptr = nodeptr->children[i]; //go to the node of each letter
			}
			else {
				continue; //otherwise try next letter
			}

			result += nodeptr->letter;

			//search the rest of the characters
			for (int i = character + 1; i < length; i++) {
				searchend = word.at(i); //store the current character to search
				sI = index(searchend);
				//check if the next letter exists
				if (nodeptr->children[sI] != NULL) {
					nodeptr = nodeptr->children[sI]; //go to this node
					result += nodeptr->letter;
				}
				//if it doesnt
				else {
					found = false;
					complete = true;
				}
			}


			if (!complete)
				found = nodeptr->end;

			if (found && result.length() == word.length())
				results.insert(result);

		}
		return results;
	}






};







int main() {

	Trie dictionary;
	string read;
	int x = 0;
	int y = 0;
	string write;

	fstream myfile("ok.txt");
	cout << "\t\t------------------------------------------------------" << endl;
	cout << "\t\t\t\t WELCOME TO SPELL CHECKER             " << endl;
	cout << "\t\t------------------------------------------------------" << endl << endl;
	/* import dictionary */
	if (myfile.is_open())
	{
		while (getline(myfile, read))
		{
			//insert words into dictionary
			dictionary.insert(read);

		}
		myfile.close();
	}
	else cout << "Unable to open file";
	while (true) {
		cout << endl << "\tPress 1 to insert into database" << endl << "\tPress 2 to check spelling" << endl << "\tEnter Choice: ";
		cin >> x;
		if (x == 1) {
			do {
				myfile.open("ok.txt");
				if (myfile.is_open()) {

					cout << "\tEnter the word: ";
					cin >> write;

					// write inputted data into the file and Trie
					myfile << write << endl;
					dictionary.insert(write);

					myfile.close();
				}
				else
				{
					cout << "Unable to open file";
				}
				cout << endl << "\tPress 1 to insert another word: " << endl << "\tPress 5 to return to main menu : " << endl << "\tEnter Choice ";
				cin >> y;
				cout << endl;

			} while (y != 5);

		}




		if (x == 2) {

			do {
				ResultList resultIns;

				ResultList resultDel;

				ResultList resultSub;

				string input;
				cout << "\tEnter word to spell check: ";
				cin >> input;

				//check if they exist
				if (dictionary.search(input)) {
					cout << "CORRECT" << endl;

				}
				else {

					cout << "INCORRECT" << endl;
					cout << "_____________" << endl;
					cout << endl << "Search by Insertion" << endl;
					cout << endl << "Did you forget a letter?" << endl << endl;


					clock_t startTime1 = clock();
					for (int i = 0; i <= input.length(); i++) {
						if (resultIns.searchForDuplicate(dictionary.suggestInsertions(input, i))) {

							resultIns.append(dictionary.suggestInsertions(input, i));
						}
					}
					cout << "Time: " << double(clock() - startTime1) / CLOCKS_PER_SEC << " ms" << endl << endl;
					if (resultIns.isEmpty()) {
						cout << "No Word Found using insertion method" << endl;
					}
					resultIns.displayList();

					cout << "_____________" << endl;
					cout << endl << "Search by Deletions" << endl;
					cout << endl << "Did you type an extra letter?" << endl << endl;


					clock_t startTime3 = clock();
					for (int i = 0; i < input.length(); i++) {
						if (resultDel.searchForDuplicate(dictionary.suggestDeletions(input, i))) {

							resultDel.append(dictionary.suggestDeletions(input, i));
						}
					}


					cout << "Time: " << double(clock() - startTime3) / CLOCKS_PER_SEC << " ms" << endl << endl;
					resultDel.displayList();
					if (resultDel.isEmpty()) {
						cout << "No Word Found using deletion method" << endl;
					}
					cout << "_____________" << endl;
					cout << endl << "Search by Substitution" << endl;
					cout << endl << "Did you mistype a letter?" << endl << endl;

					clock_t startTime5 = clock();
					for (int i = 0; i < input.length(); i++) {
						resultSub.append(dictionary.suggestSubstitutions(input, i));
					}
					cout << "Time: " << double(clock() - startTime5) / CLOCKS_PER_SEC << " ms" << endl << endl;
					if (resultSub.isEmpty()) {
						cout << "No Word Found using substitution method" << endl;
					}
					resultSub.displayList();
					cout << "_____________" << endl;

				}

				cout << endl << "\tPress 2 to check another word: " << endl << "\tPress 5 to return to main menu: " << endl << "\tEnter Choice: ";
				cin >> y;
				cout << endl;
				resultIns.clear();
				resultDel.clear();
				resultSub.clear();
			} while (y != 5);


		}
	}
	system("pause");

	return 0;
}
