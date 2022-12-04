#include "List.h"
int main()
{
	DLIST<int> list1;
	ptrNODE<int> p1 = list1.get_head();
	list1.add_after(p1, 1);
	list1.add_after(p1, 2);
	list1.add_after(p1, 3);
	list1.add_after(p1, 7);

	DLIST<int> list2;
	ptrNODE<int> p2 = list2.get_head();
	list2.add_after(p2, 53);
	list2.add_after(p2, 6);

	DLIST<int>::Iterator it;
	std::cout << "List1: ";
	for (it = list1.begin(); it != list1.end(); it++)
	{
		std::cout << *it << " ";
	}

	/*for (auto it : list1)
		std::cout << it << " ";*/

	for (it = list1.begin(); it != list1.end();)
	{	
		if (*it == 1)
		{
			std::cout << "Deleted: " << *it << " ";
			it = list1.erase(it);
		}
		else
		{
			it++;
		}
		
	}

	for (it = list1.begin(); it != list1.end(); it++)
	{	
		if (*it == 2)
		{
			int data = 5;
			std::cout << "Added: " << data;
			it = list1.append(it, data);
		}
	}

	std::cout << "\nList1: ";
	for (it = list1.begin(); it != list1.end(); it++)
	{
		std::cout << *it << " ";
	}
	
	std::cout << "\nList2: ";
	for (it = list2.begin(); it != list2.end(); it++)
	{
		std::cout << *it << " ";
	}

	//Merge List1 & List2 into List3
	DLIST<int> listResult;
	listResult.mergeLists(list1.begin(), list1.end(), list2.begin(), list2.end());
	std::cout << "\nList3 (Merged): ";
	for (it = listResult.begin(); it != listResult.end(); it++)
	{
		std::cout << *it << " ";
	}

	std::cout << "\nMerge Sort List3: ";
	listResult.mergeSort(listResult.get_head());
	for (it = listResult.begin(); it != listResult.end(); it++)
	{
		std::cout << *it << " ";
	}
}