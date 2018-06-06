#include<iostream>
#include<cstring>
#include<cmath>

#define SUC
//#define PREDEC

using namespace std;

class Node{
	private:
		int key[4];
		int cap;
		Node* parent;
		Node* child[5];
	public:
		Node()
		{
			cap = 0;
			for(int i = 0; i < 5; i++)
				child[i] = NULL;
		}
		void put_key(int k)
		{
			int i;
			for(i = 0; i < cap; i++)
				if(key[i] > k)
					break;
			for(int j = cap; j > i; j--)
				key[j] = key[j - 1];
			key[i] = k;
			cap++;
		}
		int del_key(int k)
		{
			int i;
			for(i = 0; i < cap; i++)
				if(key[i] == k)
					break;
			for(int j = i; j < cap - 1; j++)
				key[j] = key[j + 1];
			cap--;
			return i;
		}
		int ret_key(int i)
		{
			return key[i];
		}
		int ret_cap()
		{
			return cap;
		}
		void set_parent(Node* p)
		{
			parent = p;
		}
		Node* ret_parent()
		{
			return parent;
		}
		void set_child(int i, Node* c)
		{
			child[i] = c;
		}
		Node* ret_child(int i)
		{
			return child[i];
		}
};


class Tree{
	private:
		Node* root;
		int dep;
	public:
		Tree()
		{
			root = new Node();
			root->set_parent(NULL);
			dep = 1;
		}
		Node* find_node(int k)
		{
			Node *now = root;
			for(int i = 0; i < dep; i++)
			{
				int j = 0;
				for(; j < now->ret_cap(); j++)
					if(now->ret_key(j) >= k)
						break;
				if(j < now->ret_cap() && now->ret_key(j) == k)
					return now;
				if(i != dep -1)
					now = now->ret_child(j);
			}
			return now;
		}
		int find_key(int k)
		{
			Node* now = root;
			for(int i = 0; i < dep; i++)
			{
				int j;
				for(j = 0; j < now->ret_cap(); j++)
					if(now->ret_key(j) >= k)
						break;
				if(j < now->ret_cap() && now->ret_key(j) == k)
					return 1;
				if(i != dep -1)
					now = now->ret_child(j);
			}
			return 0;
		}
		void insert_key(int k)
		{
			if(find_key(k))
			{
				cout << k << " ALREADY EXISTS" << endl;
				return ;
			}
			Node *now = find_node(k);
			now->put_key(k);
			while(now->ret_cap() > 3)
			{
				if(now == root)
				{
					Node *new_root = new Node();
					new_root->set_parent(NULL);
					new_root->set_child(0, now);
					now->set_parent(new_root);
					root = new_root;
					dep++;
				}
				Node *p = now->ret_parent();
				Node *new_node = new Node();
				new_node->set_parent(p);
				new_node->set_child(0, now->ret_child(3));
				new_node->set_child(1, now->ret_child(4));
				new_node->put_key(now->ret_key(3));
				now->set_child(3, NULL);
				now->set_child(4, NULL);
				p->put_key(now->ret_key(2));
				now->del_key(now->ret_key(2));
				now->del_key(now->ret_key(2));
				int i;
				for(i = 0; i < p->ret_cap() + 1; i++)
					if(p->ret_child(i) == now)
						break;
				for(int j = p->ret_cap(); j > i; j--)
					p->set_child(j, p->ret_child(j - 1));
				p->set_child(i + 1, new_node);
				if(new_node->ret_child(0) != NULL && new_node->ret_child(1) != NULL)
				{
					new_node->ret_child(0)->set_parent(new_node);
					new_node->ret_child(1)->set_parent(new_node);
				}
				now = p;
			}
		}
		void delete_key(int k)
		{
			if(!find_key(k))
			{
				cout << k << " NOT FOUND" << endl;
				return ;
			}
			int loc;
			Node *now = find_node(k), *ch;
			loc = now->del_key(k);
#ifdef SUC
			ch = now->ret_child(loc + 1);
			while(ch != NULL)
			{
				if(ch->ret_child(0) == NULL)
				{
					now->put_key(ch->ret_key(0));
					ch->del_key(ch->ret_key(0));
					now = ch;
					break;
				}
				ch = ch->ret_child(0);
			}
#endif
#ifdef PREDEC
			ch = now->ret_child(loc);
			while(ch != NULL)
			{
				if(ch->ret_child(0) == NULL)
				{
					now->put_key(ch->ret_key(ch->ret_cap() - 1));
					ch->del_key(ch->ret_key(ch->ret_cap() - 1));
					now = ch;
					break;
				}
				ch = ch->ret_child(ch->ret_cap());
			}
#endif
			while(now->ret_cap() == 0)
			{
				int i;
				Node *p = now->ret_parent();
				if(p == NULL)
				{
					if(now->ret_child(0) == NULL)
						break;
					root = now->ret_child(0);
					root->set_parent(NULL);
					dep--;
					delete now;
					break;
				}
				Node *adj;
				for(i = 0; i < p->ret_cap() + 1; i++)
					if(p->ret_child(i) == now)
						break;
				if(i == 0)
					adj = p->ret_child(1);
				else
					adj = p->ret_child(i - 1);
				if(adj->ret_cap() == 1)
				{
					adj->put_key(p->ret_key(i > 0? i - 1 : 0));
					for(int j = i; j < p->ret_cap(); j++)
						p->set_child(j, p->ret_child(j + 1));
					p->del_key(p->ret_key(i > 0? i - 1 : 0));
					if(i == 0)
					{
						for(int j = adj->ret_cap() + 1; j > 0; j--)
							adj->set_child(j, adj->ret_child(j - 1));
						adj->set_child(0, now->ret_child(0));
					}
					else
						adj->set_child(adj->ret_cap(), now->ret_child(0));
					if(now->ret_child(0) != NULL)
						now->ret_child(0)->set_parent(adj);
					delete now;
				}
				else
				{
					if(i == 0)
					{
						now->put_key(p->ret_key(0));
						p->del_key(p->ret_key(0));
						p->put_key(adj->ret_key(0));
						adj->del_key(adj->ret_key(0));
						now->set_child(1, adj->ret_child(0));
						for(int j = 0; j < adj->ret_cap() + 1; j++)
							adj->set_child(j, adj->ret_child(j + 1));
					}
					else
					{
						now->put_key(p->ret_key(i - 1));
						p->del_key(p->ret_key(i - 1));
						p->put_key(adj->ret_key(adj->ret_cap() - 1));
						adj->del_key(adj->ret_key(adj->ret_key(adj->ret_cap() - 1)));
						now->set_child(0, adj->ret_child(adj->ret_cap() + 1));
					}
				}
				now = p;
			}
		}
		void rec_show(Node *now, int d)
		{
			if(now == NULL)
				return ;
			for(int i = 0; i < d; i++)
				cout << "    ";
			cout << now->ret_key(0);
			for(int i = 1; i < now->ret_cap(); i++)
				cout << "," << now->ret_key(i);
			cout << endl;
			for(int i = 0; i <= now->ret_cap(); i++)
				rec_show(now->ret_child(i), d + 1);
		}
		void show_tree()
		{
			Node *now = root;
			if(now->ret_cap() == 0)
			{
				cout << "NO KEYS" << endl;
				return;
			}
			rec_show(now, 0);
		}
		void free_tree(Node *now)
		{
			if(now == NULL)
				return ;
			for(int i = 0; i <= now->ret_cap(); i++)
				free_tree(now->ret_child(i));
			delete now;
		}
		~Tree()
		{
			free_tree(root);
		}
};

int main()
{
	int key;
	char op[10] = {0, };
	Tree t;
	while(1)
	{
		cin >> op;
		if(!strcmp(op, "SHOWTREE"))
			t.show_tree();
		else if(!strcmp(op, "INSERT"))
		{
			cin >> key;
			t.insert_key(key);
		}
		else if(!strcmp(op, "DELETE"))
		{
			cin >> key;
			t.delete_key(key);
		}
		else if(!strcmp(op, "QUIT"))
			break;
	}
	return 0;
}
