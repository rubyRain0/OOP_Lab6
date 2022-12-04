#pragma once
#include <iostream>
#include <fstream>

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
		return Iterator(it->next);
	}

	Iterator append(Iterator it, TInfo data)
	{
		if (it->prev)
		{
			add_after(it->prev, data);
		}
		return Iterator(it);
	}

	DLIST()
	{
		head = new NODE<TInfo>(0, nullptr, nullptr);
		tail = head;
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

	void split(ptrNODE<TInfo> head, ptrNODE<TInfo>* a, ptrNODE<TInfo>* b);
	ptrNODE<TInfo> merge(ptrNODE<TInfo> a, ptrNODE<TInfo> b);
	void mergeSort(ptrNODE<TInfo> head);
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
	return data;
}

template<typename TInfo>
void DLIST<TInfo>::split(ptrNODE<TInfo> head, ptrNODE<TInfo>* a, ptrNODE<TInfo>* b)
{
	ptrNODE<TInfo> slow = head;
	ptrNODE<TInfo> fast = head->next;

	// advance `fast` by two nodes, and advance `slow` by a single node
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}

	*b = slow->next;
	slow->next = NULL;
}

template<typename TInfo>
ptrNODE<TInfo> DLIST<TInfo>::merge(ptrNODE<TInfo> a, ptrNODE<TInfo> b)
{
	// base cases
	if (a == NULL) {
		return b;
	}

	if (b == NULL) {
		return a;
	}

	// pick either `a` or `b`, and recur
	if (a->info <= b->info)
	{
		a->next = merge(a->next, b);
		a->next->prev = a;
		a->prev = NULL;
		return a;
	}
	else {
		b->next = merge(a, b->next);
		b->next->prev = b;
		b->prev = NULL;
		return b;
	}
}

template<typename TInfo>
void DLIST<TInfo>::mergeSort(ptrNODE<TInfo> head)
{
	// base case: 0 or 1 node
	if (head == NULL || head->next == NULL) {
		return;
	}

	// split head into `a` and `b` sublists
	ptrNODE<TInfo> a = head, b = NULL;
	split(head, &a, &b);

	// recursively sort the sublists
	mergeSort(a);
	mergeSort(b);

	// merge the two sorted lists
	head = merge(a, b);
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