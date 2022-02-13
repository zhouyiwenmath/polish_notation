#include <iostream>
#include <map>

using namespace std;


map<char, int> valtab;


class Tree;


class Node {
	friend class Tree;
	friend ostream& operator<< (ostream&, const Tree&);
	int use;    //reference count -GC

protected:
	Node(){use = 1;}
	virtual void print(ostream&) = 0;    //pure virtual
	virtual ~Node(){}     //note virtual destructor
	virtual int eval() = 0;
	
};


class Tree {
	friend class Node;    //okay -between kissing cousin
	friend ostream& operator<< (ostream&, const Tree&);
	Node *p;   //polymorphic hierachy

public:
	Tree(int);   //constant
	Tree(char);   //variable
	Tree(char, Tree);   //unary operator
	Tree(char, Tree, Tree);  //binary operator
	Tree(const Tree& t){p = t.p; ++p -> use;}
	~Tree() {if (--p -> use == 0) delete p;}
	void operator=(const Tree& t);
	int eval() {return p->eval();}
};
	

ostream& operator<< (ostream& o, const Tree& t)
{
	t.p -> print(o);
	return (o);
}


class LeafNode: public Node {
	friend class Tree;
	void print(ostream& o) = 0;
	virtual int eval() = 0;
};


class IntNode: public LeafNode {
	friend class Tree;
	int n;
	void print(ostream& o){o << n;}
	IntNode(int k): n(k) {}
public:
	int eval(){return n;}
	
};


class IdNode: public LeafNode {
	friend class Tree;
	char name;
	void print(ostream& o) {o << name;}
	IdNode(char id): name(id) {}
public:
	int eval(){return valtab[name];}
};

class UnaryNode: public LeafNode {
	friend class Tree;
	char op;
	Tree opnd;
	UnaryNode(char a, Tree b): op(a), opnd(b) {}
	void print (ostream& o) {o << "(" << op << opnd << ")";}
public:
	int eval();
};

int UnaryNode::eval()
{
	switch (op){
	case '-': return (-opnd.eval());
	case '+': return (+opnd.eval());
	default: cerr << "no operand\n" << endl;
	return 0;
	}
}

class BinaryNode: public LeafNode {
	friend class Tree;
	char op;
	Tree left;
	Tree right;
	BinaryNode(char a, Tree b, Tree c): op(a), left(b), right(c) {}
	void print (ostream& o) {o << "(" << left << op << right << ")";}
public:
	int eval();
};

int BinaryNode::eval()
{
	switch (op){
	case '-': return (left.eval() - right.eval());	
	case '+': return (left.eval() + right.eval());
	case '*': return (left.eval() * right.eval());
	default: cerr << "no operand\n" << endl;
	return 0;
	}
}


Tree::Tree(int n)
{ p = new IntNode(n);}

Tree::Tree(char id)
{ p = new IdNode(id);}

Tree::Tree(char op, Tree t)
{ p = new UnaryNode(op, t);}

Tree::Tree(char op, Tree left, Tree right)
{ p = new BinaryNode(op, left, right);}


int main()
{
	cout << "running testing code...\n" << endl;
	
	valtab['A'] = 3; valtab['B'] = 4;
	cout << "A = 3, B = 4" << endl;
	Tree t1 = Tree('*', Tree('-', 5), Tree('+', 'A', 4));
	Tree t2 = Tree('+', Tree('-', 'A', 1), Tree('+', t1, 'B'));
	cout << "t1 = " << t1 << "; t2 = " << t2 << endl;
	cout << "t1: " << t1.eval() << " t2:" << t2.eval() << endl;
	
	cout << "\nfinished running testing code...\n" << endl;

}





