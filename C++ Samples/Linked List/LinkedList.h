#include <iostream>

template <typename T>
class LinkedList
{
	struct Node
	{
		T data;
		Node* next;
		Node* prev;
	};

	private:
		
		Node* head;
		Node* tail;
		int mSize;
		Node* CreateNode(T);
	
	public:
		LinkedList(); // Constructor

		T front(); // Get data from the head of the list
		
		T back(); // Get data from the tail of the list

		T at(int); // Get data from a node somewhere in the list

		void set(int, T); // Set a node's data somewhere in the list

		Node* get(int); // Return a node from somewhere in the list
		
		Node* operator[](int); // Overload LinkedList[i] to return a node from somewhere in the list

		void push_front(T); // Add a node to the beginning of the list
		
		void push_back(T); // Add a node to the end of the list
		
		void insert(int, T); // Add a node somewhere in the list
		
		void pop_front(); // Remove the first node in the list

		void pop_back(); // Remove the last node in the list
		
		void erase(int); // Remove a node somewhere in the list

		void iterate(); // Print the list's contents

		int size(); // Return the number of nodes in the list
		
		bool empty(); // Return if the linked list is empty or not
};

template <typename T>
LinkedList<T>::LinkedList()
{
	head = NULL;
	tail = NULL;
	mSize = 0;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::CreateNode(T value)
{
	Node* node = new Node;
	node->data = value;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

template <typename T>
T LinkedList<T>::front()
{
	return head->data;
}

template <typename T>
T LinkedList<T>::back()
{
	return tail->data;
}

template <typename T>
T LinkedList<T>::at(int index)
{
	Node* cur = new Node;
	cur = head;
	for (int i = 0; i < index; i++)
	{
		cur = cur->next;
	}

	return cur->data;
}

template <typename T>
void LinkedList<T>::set(int index, T value)
{
	if (index < 0 || index > mSize)
	{
		return;
	}

	Node* cur = new Node;
	cur = head;
	for (int i = 0; i < index; i++)
	{
		cur = cur->next;
	}

	cur->data = value;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::get(int index)
{
	Node* cur = new Node;
	cur = head;
	for (int i = 0; i < index; i++)
	{
		cur = cur->next;
	}

	return cur;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::operator[](int index)
{
	return get(index);
}

template <typename T>
void LinkedList<T>::push_front(T value)
{
	Node* node = CreateNode(value);
	if (head == NULL)
	{
		head = node;
		mSize++;
		return;
	}

	head->prev = node;
	node->next = head;
	head = node;
	mSize++;
}

template <typename T>
void LinkedList<T>::push_back(T value)
{
	Node* node = CreateNode(value);
	if (head == NULL)
	{
		head = node;
		mSize++;
		return;
	}

	Node* temp = head;
	while (temp->next != NULL) { temp = temp->next; }
	temp->next = node;
	node->prev = temp;
	mSize++;
}

template <typename T>
void LinkedList<T>::insert(int index, T value)
{
	if (index == 0)
	{
		push_front(value);
		return;
	}
	if (index > mSize - 1)
	{
		push_back(value);
		return;
	}

	Node* node = CreateNode(value);
	Node* temp = head;
	for (int i = 0; i < index; i++) { temp = temp->next; }
	temp->next = node;
	node->prev = temp;
	mSize++;
}

template <typename T>
void LinkedList<T>::pop_front()
{
	Node* temp = new Node;
	temp = head;
	head = head->next;
	delete temp;
	mSize--;
}

template <typename T>
void LinkedList<T>::pop_back()
{
	Node* cur = new Node;
	Node* previous = new Node;

	cur = head;
	while (cur->next != NULL)
	{
		previous = cur;
		cur = cur->next;
	}

	tail = previous;
	previous->next = NULL;
	delete cur;
	mSize--;
}

template <typename T>
void LinkedList<T>::erase(int index)
{
	if (head == NULL || index < 0)
	{
		return;
	}
	if (index == 0)
	{
		pop_front();
		return;
	}
	if (index > mSize - 1)
	{
		pop_back();
		return;
	}

	Node* temp = head;
	for (int i = 0; i < index; i++)
	{
		temp = temp->next;
	}

	temp->next->prev = temp->prev;
	temp->prev->next = temp->next;
	delete temp;
	mSize--;
}

template <typename T>
void LinkedList<T>::iterate()
{
	Node* temp = new Node;
	temp = head;
	while (temp != NULL)
	{
		std::cout << temp->data << " ";
		temp = temp->next;
	}
}

template <typename T>
int LinkedList<T>::size()
{
	return mSize;
}

template <typename T>
bool LinkedList<T>::empty()
{
	return mSize == 0;
}
