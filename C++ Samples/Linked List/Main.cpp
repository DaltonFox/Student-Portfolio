// Lab 2: Linked List ~ Dalton Fox

#include "LinkedList.h"

int main(void)
{
	// Initialize a LinkedList;
	LinkedList<int> intList;

	// Demonstrates LinkedList.empty()
	if (intList.empty())
	{
		// Demonstrates LinkedList.push_front()
		for (int i = 0; i < 4; i++)
		{
			intList.push_front(1);
		}

		// Demonstrates LinkedList.pop_front()
		intList.pop_front();

		// Demonstrates LinkedList.pop_back()
		intList.pop_back();

		// Demonstrates LinkedList.erase()
		intList.erase(3);

		// Demonstrates LinkedList.insert(), LinkedList.at()
		for (int i = 0; i < 10; i++)
		{
			intList.insert(i + 1, intList.at(i) + 2);
		}

		// Demonstrates LinkedList.push_back(), LinkedList[], LinkedList.size(), LinkedList.get(), Node->prev
		intList.push_back(intList[intList.size() - 1]->data + intList.get(intList.size() - 1)->prev->data);

		// Demonstrates LinkedList.set()
		intList.push_back(intList.size() - 1);
		intList.set(intList.size() - 1, 0);
	}

	// Demonstrates LinkedList.iterate()
	intList.iterate();

	return 0;
}