#include <iostream>
#include <string.h>
#include <conio.h>
#include <fstream>

using namespace std;

template <class T>
class AVLtree;

template <class T>
class AVLnode;

template<class T>
class dlist;
template<class T>
class dnode										//Node for doubly linked list
{
	T issn;
	dnode<T> * next;
	dnode<T> * prev;
	char* title;
	char* name_of_authors;
	int carb;									//carb=copies as reference book
	int caib;									//caib=copies as issuable book
public:
	dnode()
	{
		next = prev = nullptr;
		title = name_of_authors = nullptr;
	}
	~dnode()
	{
		if (title != nullptr)
		{
			delete[] title;
		}
		if (name_of_authors != nullptr)
		{
			delete[] name_of_authors;
		}
	}
	dnode(T issn1, ifstream &fin)
	{
		issn = issn1;

		char temp[100];							//setting title of book
		fin.getline(temp, 99);
		int size = strlen(temp);
		title = new char[size + 1];
		strcpy(title, temp);

		fin.getline(temp, 99);					//setting name of authors
		size = strlen(temp);
		name_of_authors = new char[size + 1];
		strcpy(name_of_authors, temp);

		fin >> carb;
		fin >> caib;

		next = nullptr;
		prev = nullptr;
	}
	void printnode()
	{
		cout << "ISSN : " << this->issn << "\n";
		cout << "Title : " << this->title << endl;
		cout << "Author(s) : " << this->name_of_authors << endl;
		cout << "Copies available as reference books : " << this->carb << endl;
		cout << "Copies available as issuable books : " << this->caib << endl <<endl << endl;
	}
	void updatenode()
	{
		int i;
		bool flag = true;
		char temp[100];
		while (flag==true)
		{
			system("cls");
			cout << "Enter 1 to update name of book.\n";
			cout << "Enter 2 to update name of author(s).\n";
			cout << "Enter 3 to update copies available as reference books.\n";
			cout << "Enter 4 to update copies available as issuable books.\n";
			cout << "Enter 0 to exit.\n";
			cin >> i;
			if (i == 1)
			{
				cout << "Enter new name (in one line) : ";
				cin >> temp;
				int size = strlen(temp);
				char* newname = new char[size + 1];
				strcpy(newname, temp);
				delete[] title;
				title = temp;
			}
			else if (i == 2)
			{
				cout << "Enter new author(s) (in one line) : ";
				cin >> temp;
				int size = strlen(temp);
				char* newname = new char[size + 1];
				strcpy(newname, temp);
				delete[] name_of_authors;
				name_of_authors = newname;
			}
			else if (i == 3)
			{
				cout << "Enter new number: ";
				int m;
				cin >> m;
				if (m > 1)
				{
					carb = m;
				}
			}
			else if (i == 4)
			{
				cout << "Enter new number: ";
				int m;
				cin >> m;
				if (m > 1)
				{
					caib = m;
				}
			}
			else if (i == 0)
			{
				flag = false;
			}
		}
		cout << "Updated data is : \n";
		printnode();
	}
	friend class dlist<T>;
};

template <class T>
class dlist
{
	dnode<T> head; // dummy head

public:

	dlist()
	{
		head.next = nullptr;
	}

	void destroy_dlist()
	{
		dnode <T> * curr = head.next;
		while (curr != nullptr)
		{
			dnode <T> * next = curr->next;
			delete curr;
			curr = next;
		}
	}

	void printlist()
	{
		dnode<T> *temp = head.next;
		do
		{
			temp->printnode();
			temp = temp->next;
		} while (temp != nullptr);
	}

	dnode <T> *& insertafter(dnode<T> * &curr, T issn1, ifstream &fin)		//Inserting node at the 'next' of a node
	{
		dnode <T> * temp = new dnode <T>(issn1, fin);

		if (curr == nullptr)
		{
			head.next = temp;
		}
		else if (curr->issn < issn1)		//Inserting node in middle of list
		{
			if (curr->next != nullptr)
			{
				dnode <T> * temp1 = curr->next;
				curr->next = temp;
				temp->prev = curr;

				temp->next = temp1;
				temp1->prev = temp;
			}
			else
			{
				curr->next = temp;
				temp->prev = curr;
			}
		}
		else if (curr->issn > issn1)		//Inserting node at last
		{

			if (curr->prev != nullptr)
			{
				dnode <T> * temp1 = curr->prev;
				temp1->next = temp;
				temp->prev = temp1;

				temp->next = curr;
				curr->prev = temp;
			}
			else
			{
				curr->prev = temp;
				temp->next = curr;
				head.next = temp;
			}
		}
		return temp;
	}

	friend class AVLnode<T>;
	//friend class AVLtree<T>;
};

template <class T>
class AVLnode
{
private:
	T issn;
	AVLnode* left;
	AVLnode* right;
	dnode<T> * ptr_to_list;
	int height;
public:
	AVLnode(T issn1)
	{
		issn = issn1;
		left = right = nullptr;
		ptr_to_list = nullptr;
		height = 0;
	}
	friend class AVLtree<T>;
};

template <class T>
class AVLtree
{
private:
	AVLnode <T> * root;
	dlist <T> list;
public:
	AVLtree()
	{
		root = nullptr;
	}

	void destroy(AVLnode <T> *r)						//Destroy all tree
	{
		if (r != nullptr)
		{
			destroy(r->left);
			destroy(r->right);
			delete r->ptr_to_list;
			delete r;
		}
	}

	int find_balancefactor(AVLnode<T> * r)
	{
		int ht_left = 0, ht_right = 0;
		if (r->right != nullptr)
		{
			ht_right = r->right->height;
		}
		if (r->left != nullptr)
		{
			ht_left = r->left->height;
		}
		return (ht_left - ht_right);
	}

	void balance_node(AVLnode<T> * r)						//Balancing unbalanced node in tree
	{
		if (r != nullptr)
		{
			r->height = 1 + max(height(r->left), height(r->right));
			int balance_of_node = find_balancefactor(r);

			if (balance_of_node == 2 && find_balancefactor(r->left) == 0)
			{
				rotateright(r);
			}
			else if (balance_of_node == 2 && find_balancefactor(r->left) == 1)
			{
				rotateright(r);
			}
			else if (balance_of_node == 2 && find_balancefactor(r->left) == -1)
			{
				rotateleft(r->left);
				rotateright(r);
			}
			else if (balance_of_node == -2 && find_balancefactor(r->right) == 0)
			{
				rotateleft(r);
			}
			else if (balance_of_node == -2 && find_balancefactor(r->right) == -1)
			{
				rotateleft(r);
			}
			else if (balance_of_node == -2 && find_balancefactor(r->right) == 1)
			{
				rotateright(r->right);
				rotateleft(r);
			}
		}
	}

	AVLnode <T>* removenode(AVLnode <T> * r, T d)			//removing a specific node
	{
		if (r == nullptr)
		{
			return nullptr;
		}
		if (d < r->issn)
		{
			r->left = removenode(r->left, d);
			balance_node(r);
			return r;
		}
		else if (d > r->issn)
		{
			r->right = removenode(r->right, d);
			balance_node(r);
			return r;
		}

		//now r is at the node to be deleted

		AVLnode<T> * temp;
		if (r->right == nullptr)					//if r has one or no child
		{
			temp = r->left;
			delete r->ptr_to_list;
			delete r;
			cout << "Data removed i.e. " << d << endl;
			temp->height = 1 + max(height(temp->left), height(temp->right));
		}
		else if (r->left == nullptr)
		{
			temp = r->right;
			delete r->ptr_to_list;
			delete r;
			cout << "Data removed i.e. " << d << endl;
			temp->height = 1 + max(height(temp->left), height(temp->right));
		}

		else
		{
			AVLnode <T> *parent = r->right;			//if r has two childs then find minimum in right subtree, replace the data and remove the node
			temp = r->right;

			int i = 0;
			while (temp->left != nullptr)
			{
				parent = temp;
				temp = temp->left;
				i++;
			}

			if (i == 0)								//if the right subtree has only one node
			{
				r->issn = temp->issn;
				delete temp->ptr_to_list;
				delete temp;
				cout << "Data removed i.e. " << d << endl;
				r->right = nullptr;
			}
			else
			{
				parent->left = temp->right;
				r->issn = temp->issn;
				delete r->ptr_to_list;
				r->ptr_to_list = temp->ptr_to_list;
				delete temp;
				cout << "Data removed i.e. " << d << endl;
			}
			r->height = 1 + max(height(r->left), height(r->right));
			return r;
		}
		return temp;
	}

	int height(AVLnode <T> * r)
	{
		if (r != nullptr)
		{
			return r->height;
		}
		return 0;
	}

	void rotateright(AVLnode<T> * & x)
	{
		AVLnode <T> * y = x->left;
		x->left = y->right;
		y->right = x;
		x = y;
	}
	void rotateleft(AVLnode<T> * & x)
	{
		AVLnode <T> * y = x->right;
		x->right = y->left;
		y->left = x;
		x = y;
	}
	void doublerotateright(AVLnode<T> * & x)
	{
		rotateleft(x->left);
		rotateright(x);
	}
	void doublerotateleft(AVLnode<T> * & x)
	{
		rotateright(x->right);
		rotateleft(x);
	}
	int max(int x, int y)
	{
		if (x > y)
		{
			return x;
		}
		if (x < y)
		{
			return y;
		}
		return x;								//if same then return anyone
	}

	void insert(AVLnode<T> *&r, T issn1, dnode<T> *& prev, ifstream & fin)
	{
		if (r == nullptr)
		{
			AVLnode<T>* newnode = new AVLnode<T>(issn1);
			newnode->issn = issn1;

			newnode->ptr_to_list = list.insertafter(prev, issn1, fin);		//updating ptr to doubly list
			r = newnode;
			return;
		}
		if (issn1 < r->issn)
		{
			dnode<T> *prev = r->ptr_to_list;
			insert(r->left, issn1, prev, fin);
			if (height(r->left) - height(r->right) == 2)					//rotation for insertion
			{
				if (issn1 < (r->left->issn))
				{
					rotateright(r);
				}
				else
				{
					doublerotateright(r);
				}
			}
		}
		else
		{
			dnode<T> *prev = r->ptr_to_list;
			insert(r->right, issn1, prev, fin);
			if (height(r->left) - height(r->right) == -2)
			{
				if (issn1 < (r->right->issn))
				{
					doublerotateleft(r);
				}
				else
				{
					rotateleft(r);
				}
			}
		}
		r->height = 1 + max(height(r->left), height(r->right));			//updating height of tree
	}

	void reviewbook(AVLnode<T>*& r, T issn1)							//review node of an avl tree
	{
		if (r == nullptr)
		{
			return;
		}
		if (r->issn == issn1)
		{
			cout << r->issn << "\t";
			r->ptr_to_list->printnode();
		}
		else
		{
			if (r->issn > issn1)
			{
				reviewbook(r->left, issn1);
			}
			else
			{
				reviewbook(r->right, issn1);
			}
		}
	}

	void printSorted(AVLnode<T>*& r)
	{
		if (r != nullptr)
		{
			printSorted(r->left);
			//cout << r->issn << "\t";
			r->ptr_to_list->printnode();
			printSorted(r->right);
		}
	}

	void printRange(AVLnode<T>*& r,T issn, T issn1)					//printing nodes within specific range
	{
		if (r != nullptr)
		{
			printRange(r->left,issn,issn1);
			if (r->issn >= issn && r->issn <= issn1)
			{
				r->ptr_to_list->printnode();
			}
			if (r->issn > issn1)
			{
				return;
			}
			printRange(r->right,issn,issn1);
		}
	}

	void update_record(AVLnode<T>* r, T issn)
	{
		if (r == nullptr)
		{
			return;
		}
		if (r->issn == issn)
		{
			cout << r->issn << "\t";
			r->ptr_to_list->updatenode();
			return;
		}
		else
		{
			if (r->issn > issn)
			{
				update_record(r->left, issn);
			}
			else
			{
				update_record(r->right, issn);
			}
		}
	}

	void wrap_printsorted()
	{
		AVLnode<T> * temp = root;
		printSorted(temp);
	}

	void wrap_printrange(T issn1, T issn2)
	{
		AVLnode<T> * temp = root;
		printRange(temp, issn1, issn2);
	}

	void wrap_printlinklist()
	{
		list.printlist();
	}
	void wrap_reviewbook(T issn1)
	{
		AVLnode<T> * temp = root;
		reviewbook(temp, issn1);
	}

	void wrap_insert(T issn1, ifstream &fin)
	{
		AVLnode<T> * temp = root;
		dnode<T> *prev = nullptr;
		insert(root, issn1, prev, fin);
	}
	void wrap_destroy()								//wrapper function to remove tree
	{
		AVLnode <T> * temp = root;
		destroy(temp);
	}
	void wrap_removenode(T issn1)
	{
		AVLnode <T> * temp = root;
		removenode(temp, issn1);
	}
	void wrap_update(T issn)
	{
		AVLnode <T> * temp = root;
		update_record(temp, issn);
	}
};

int main()
{
	AVLtree <int> index;
	char temp[100];
	ifstream fin("input.txt");
	int total, issn;
	fin.getline(temp, 99);
	fin >> total;
	fin.getline(temp, 99);
	fin.getline(temp, 99);
	fin.getline(temp, 99);
	for (int i = 0; i < total; i++)
	{
		fin.getline(temp, 99);
		fin >> issn;
		index.wrap_insert(issn, fin);
	}

	//issn = 0;
	//cout << "Enter ISSN to find : ";
	//cin >> issn;
	//index.wrap_reviewbook(issn);

	index.wrap_printsorted();
	//index.wrap_printlinklist();

	//index.wrap_printrange(2, 3);

	//index.wrap_removenode(3);

	//index.wrap_printsorted();

	//index.wrap_update(5);

	index.wrap_destroy();
	_getch();
}