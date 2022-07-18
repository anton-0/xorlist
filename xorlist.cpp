#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>

class Node
{
private:
	int val = 0;
public:
	Node* nxr;
    
	Node(int val)
	{
		nxr = nullptr;
		this->val = val;
	}

	int getVal() const
	{
		return val;
	}

	void setVal(int value)
	{
		this->val = value;
	}
};

Node* xr(Node* prev, Node* next)
{
    return (Node*)((uintptr_t)(prev) ^ (uintptr_t)(next));
}

class ListXor
{
public:
    int size;
	Node* first;
	Node* last;
	Node* act;
	Node* prev;
	Node* next;

    ListXor()
	{
		size = 0;
		first = nullptr;
		last = nullptr;
		act = nullptr;
		prev = nullptr;
		next = nullptr;
	}

    ~ListXor()
	{
        if (size == 0)
            return;

		find_first();
        while(next)
        {
            Next();
            delete prev;
        }
        delete act;
	}

    void Actual()
	{
		if (act)
			printf("%d\n", act->getVal());
		else
			printf("NULL\n");
	}

    void Next()
	{
		if (next) {
			prev = act;
			act = next;
			next = xr(prev, next->nxr);
		}
		else if (act) {
			prev = NULL;
			act = first;
			next = xr(NULL, first->nxr);
		}
	}

    void Prev()
	{
		if (prev) {
			next = act;
			act = prev;
			prev = xr(prev->nxr, next);
		}
		else if (act) {
			next = NULL;
			act = last;
			prev = xr(last->nxr, NULL);
		}
	}

	void print_forward()
	{
		if (first == NULL) {
			printf("NULL\n");
			return;
		}

		act = first;
		prev = NULL;
		next = first->nxr;

		while (next)
		{
			printf("%d ", act->getVal());
			Next();
		}
		printf("%d\n", act->getVal());
	}

	void print_backward()
	{
		if (last == NULL) {
			printf("NULL\n");
			return;
		}

		act = last;
		prev = last->nxr;
		next = NULL;

		while (prev)
		{
			printf("%d ", act->getVal());
			Prev();
		}
		printf("%d\n", act->getVal());
	}

    void add_beg(int val)
	{
		Node* newNode = new Node(val);

		if (first)
		{
			newNode->nxr = first;

			if (first == last)
				first->nxr = xr(newNode, NULL);
			else
				first->nxr = xr(newNode, first->nxr);

			if (act == first)
				prev = newNode;
		}
		else {
			act = newNode;
			last = newNode;
		}

		first = newNode;
		size++;
	}

    void add_end(int val)
	{
		Node* newNode = new Node(val);

		if (last)
		{
			newNode->nxr = last;

			if (last == first)
				last->nxr = xr(NULL, newNode);
			else
				last->nxr = xr(last->nxr, newNode);

			if (act == last)
				next = newNode;
		}
		else {
			act = newNode;
			first = newNode;
		}

		last = newNode;
		size++;
	}

	// adding a node as a previous to actual
	void add_act(int val)
	{
		Node* newNode = new Node(val);

		if (act == first)
			first = newNode;

		newNode->nxr = xr(prev, act);

		// changing xor values of prev and actual
		// double xr function because we have to keep adress of next
		// form present xor and use it to create new xor
		if (prev)
			prev->nxr = xr(xr(prev->nxr, act), newNode);
		act->nxr = xr(xr(prev, act->nxr), newNode);

		prev = newNode;
		size++;
	}

    void del_beg()
	{
		if (!first) {
            printf("empty list, can not delete\n");
			return;
        }
		
		find_first();
		del_act();
	}

	void del_end()
	{
		if (!last) {
            printf("empty list, can not delete\n");
			return;
        }
		
		find_last();
		del_act();
	}

    void del_val(int val)
	{
		if (!first)
			return;

		find_first();
		while (next)
		{
			if (act->getVal() == val)
				del_act();
			Next();
		}
		if (act->getVal() == val)
			del_act();
	}

    void del_act()
	{
		if (!act) {
            printf("empty list, can not delete\n");
            return;
        }			

		if (act == first)
		{
			// if list has more than 1 element
			// move act to last node
			if (act != last) {
				act = last;
				prev = last->nxr;

				// changing next's xor, because it becomes the first
				// checking if next is set, because it could be a list with only one node
				if (next)
					next->nxr = xr(first, next->nxr);
				first = next;
				next = NULL;
			}
			else {
				act = NULL;
				first = NULL;
				last = NULL;
			}
		}
		else
		{
			// changing xor values, double xor in order to
			// reach prev of prev
			prev->nxr = xr(xr(prev->nxr, act), next);
			if (next)
				next->nxr = xr(xr(act, next->nxr), prev);

			// moving last to the left if act was pointing at it
			else if (act == last)
				last = prev;

			act = prev;
			prev = xr(prev->nxr, next);
		}

		size--;
	}

    void find_first()
	{
		act = first;
		if (first)
			next = first->nxr;
		prev = NULL;
	}

	void find_last()
	{
		act = last;
		if (last)
			prev = last->nxr;
		next = NULL;
	}
};

void print_description()
{
    printf("XOR List implemented by Anton Delinac\n");
    printf("xor list - double linked list where each node\n");
    printf("holds xor value of previous and next node adresses\n");
    printf("avaible commands:\n");
    printf("ACTUAL - print the value of actual node\n");
    printf("NEXT - change the actual to the next node and print it's value\n");
    printf("PREV - change the actual to the previous node and print it's value\n");
    printf("ADD_BEG - add node to the beginning\n");
    printf("ADD_END - add node to the end\n");
    printf("ADD_ACT - add node as a previous to actual node\n");
    printf("DEL_BEG - delete the first node\n");
    printf("DEL_END - delete the last node\n");
    printf("DEL_VAL - delete the first occurrence of provided value\n");
    printf("PRINT_FORWARD - print the list from first to last\n");
    printf("PRINT_BACKWARD - print the list from last to first\n");
    printf("q - quit\n");
    printf("\nprovide command: ");
}

void demonstration()
{
    char cmd[30];
    int val = 0;

    ListXor* list = new ListXor;

    while (scanf("%s", &cmd) > 0)
    {
        if (!strcmp(cmd, "ACTUAL"))
        {
            list->Actual();
        }
        else if (!strcmp(cmd, "NEXT"))
        {
            list->Next();
        }
        else if (!strcmp(cmd, "PREV"))
        {
            list->Prev();
        }
        else if (!strcmp(cmd, "ADD_BEG"))
        {
            printf("provide value: ");
            scanf("%d", &val);
            list->add_beg(val);
        }
        else if (!strcmp(cmd, "ADD_END"))
        {
            printf("provide value: ");
            scanf("%d", &val);
            list->add_end(val);
        }
        else if (!strcmp(cmd, "ADD_ACT"))
        {
            printf("provide value: ");
            scanf("%d", &val);
            list->add_act(val);
        }
        else if (!strcmp(cmd, "DEL_BEG"))
        {
            list->del_beg();
        }
        else if (!strcmp(cmd, "DEL_END"))
        {
            list->del_end();
        }
        else if (!strcmp(cmd, "DEL_ACT"))
        {
            list->del_act();
        }
        else if (!strcmp(cmd, "DEL_VAL"))
        {
            printf("provide value: ");
            scanf("%d", &val);
            list->del_val(val);
        }
        else if (!strcmp(cmd, "PRINT_FORWARD"))
        {
            list->print_forward();
        }
        else if (!strcmp(cmd, "PRINT_BACKWARD"))
        {
            list->print_backward();
        }
        else if (cmd[0] == 'q')
            break;
        else
        {
            printf("unknown command\n");
        }
        printf("\nprovide command: ");
    }

    delete list;
}

int main() {

    print_description();
    demonstration();

    return 0;
}