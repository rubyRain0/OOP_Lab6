#pragma once
#include <iostream>
#include <fstream>
#define INT_MIN   (-INT_MAX - 1)  
template<typename TInfo>
struct NODE
{
	TInfo info;
	NODE* next, * prev;
	NODE(TInfo info, NODE* next = nullptr, NODE* prev = nullptr) :info(info), next(next), prev(prev) {}
	~NODE()
	{
		next = nullptr;
		prev = nullptr;
	}
};

template<typename TInfo>
using ptrNODE = NODE<TInfo>*;

template<typename DLIST>
class ListIterator
{
public:
	using ValueType = typename DLIST::ValueType;
	using PointerType = ptrNODE<ValueType>;
	using ReferenceType = ValueType&;
public:
	ListIterator() : ptr(nullptr) {};
	ListIterator(PointerType _ptr) : ptr(_ptr) {};
	ListIterator& operator++()
	{
		ptr = ptr->next;
		return *this;
	}

	ListIterator operator++(int)
	{
		ListIterator iterator = *this;
		ptr = ptr->next;
		return iterator;
	}

	ListIterator& operator--()
	{
		ptr--;
		return *this;
	}

	ListIterator operator--(int)
	{
		ListIterator iterator = *this;
		--(*this);
		return iterator;
	}

	ReferenceType operator[](int index)
	{
		for (size_t i = 0; i < index; i++)
		{
			ptr = ptr->next;
		}
		return ptr->info;
	}

	ReferenceType operator*()
	{
		if(ptr!=nullptr)
			return ptr->info;
	}

	PointerType operator->()
	{
		return ptr;
	}

	PointerType getPtr()
	{
		return ptr;
	}

	bool operator==(const ListIterator& other) const
	{
		return ptr == other.ptr;
	}

	bool operator!=(const ListIterator& other) const
	{
		return ptr != other.ptr;
	}
private:
	PointerType ptr;
};


template<typename TInfo>
class DLIST
{
public:
	using ValueType = TInfo;
	using Iterator = ListIterator<DLIST<TInfo>>;
private:
	ptrNODE<ValueType> head, tail;
	size_t size;
public:

	Iterator erase(Iterator it)
	{	
		it = it->prev;
		if(it->next)
		{
		ptrNODE<ValueType>& p = it->next;
		Delete(p);
		}
		size--;
		return Iterator(it->next);
	}

	Iterator append(Iterator it, TInfo data)
	{
		if (it->prev)
		{
			add_after(it->prev, data);
		}
		size++;
		return Iterator(it);
	}

	DLIST()
	{
		head = new NODE<TInfo>(INT_MIN, nullptr, nullptr);
		tail = head;
		size = 0;
	}
	DLIST(const DLIST<TInfo>& other);
	~DLIST();

	Iterator begin()
	{
		return Iterator(head->next);
	}
		
	Iterator end()
	{
		return Iterator(tail->next);
	}

	ptrNODE<ValueType> get_head()
	{
		return head;
	}
	ptrNODE<ValueType> get_tail()
	{
		return tail;
	}
	ValueType get_first()
	{
		return head->next->info;
	}
	ValueType get_size()
	{
		return size;
	}

	#pragma warning (4:4596) 
	DLIST<TInfo>& DLIST<TInfo>::mergeLists(DLIST<TInfo>::Iterator it1Begin, DLIST<TInfo>::Iterator it1End,
											DLIST<TInfo>::Iterator it2Begin, DLIST<TInfo>::Iterator it2End);



	bool empty();
	void add_before(ptrNODE<TInfo> ptr, TInfo data);
	void add_after(ptrNODE<TInfo> ptr, TInfo data);
	void print();
	TInfo& Delete(ptrNODE<TInfo>& ptr);

	ptrNODE<TInfo> SortedMerge(ptrNODE<TInfo> a, ptrNODE<TInfo> b);
	void FrontBackSplit(ptrNODE<TInfo> source, ptrNODE<TInfo>* frontRef, ptrNODE<TInfo>* backRef);
	void MergeSort(ptrNODE<TInfo>* headRef);
};


template<typename TInfo>
DLIST<TInfo>& DLIST<TInfo>::mergeLists(DLIST<TInfo>::Iterator it1Begin, DLIST<TInfo>::Iterator it1End,
	DLIST<TInfo>::Iterator it2Begin, DLIST<TInfo>::Iterator it2End)
{
	while (it1Begin != it1End)
	{
		(*this).add_after((*this).get_tail(), *it1Begin);
		it1Begin++;
	}
	while (it2Begin != it2End)
	{
		(*this).add_after((*this).get_tail(), *it2Begin);
		it2Begin++;
	}
	return *this;
}

template<typename TInfo>
bool DLIST<TInfo>::empty()
{
	return head == nullptr;
}

template<typename TInfo>
void DLIST<TInfo>::add_after(ptrNODE<TInfo> ptr, TInfo data)
{	
	ptrNODE<TInfo> p = new NODE<TInfo>(data, ptr->next, ptr);
	if (ptr == tail)
		tail = p;
	else
		ptr->next->prev = p;
	ptr->next = p;
	size++;
}

template<typename TInfo>
void DLIST<TInfo>::add_before(ptrNODE<TInfo> ptr, TInfo data)
{
	ptrNODE<TInfo> p = new NODE<TInfo>(data, ptr, ptr->prev);
	if (ptr == head)
		head = p;
	else
		ptr->prev->next = p;
	ptr->prev = p;
	size++;
}

template<typename TInfo>
void DLIST<TInfo>::print()
{
	ptrNODE<TInfo> ptr = head;
	while (ptr)
	{
		ptr->info.print();
		ptr = ptr->next;
	}
}

template<typename TInfo>
TInfo& DLIST<TInfo>::Delete(ptrNODE<TInfo>& ptr)
{
	ptrNODE<TInfo> p = ptr;
	if (ptr == head) 
	{
		head = p->next;
		ptr = head;
		if (p->next)
			p->next->prev = nullptr;
	}
	else
		if (ptr == tail) 
		{
			tail = p->prev;
			ptr = tail;
			if (p->prev)
				p->prev->next = nullptr;
		}
		else 
		{
			ptr = ptr->next;
			p->next->prev = p->prev;
			p->prev->next = p->next;
		}
	static TInfo data;
	data = p->info;
	delete p;
	size--;
	return data;
}

template<typename TInfo>
ptrNODE<TInfo> DLIST<TInfo>::SortedMerge(ptrNODE<TInfo> a, ptrNODE<TInfo> b)
{
	ptrNODE<TInfo> result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->info <= b->info) {
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else {
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return (result);
}

template<typename TInfo>
void DLIST<TInfo>::FrontBackSplit(ptrNODE<TInfo> source, ptrNODE<TInfo>* frontRef, ptrNODE<TInfo>* backRef)
{
	ptrNODE<TInfo> fast;
	ptrNODE<TInfo> slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

template<typename TInfo>
void DLIST<TInfo>::MergeSort(ptrNODE<TInfo>* headRef)
{
	ptrNODE<TInfo> head = *headRef;
	ptrNODE<TInfo> a;
	ptrNODE<TInfo> b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	MergeSort(&a);
	MergeSort(&b);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b);
}


template<typename TInfo>
DLIST<TInfo>::DLIST(const DLIST<TInfo>& other)
{
	head = new NODE<TInfo>(0, nullptr, nullptr);
	head = other.head;
	tail = other.tail;
}

template<typename TInfo>
DLIST<TInfo>::~DLIST()
{
	while (!empty())
		Delete(head);
}