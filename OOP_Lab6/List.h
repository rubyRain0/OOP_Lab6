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
	ValueType get_first()
	{
		return head->next->info;
	}
	ValueType get_size()
	{
		return size;
	}


	void first_node(TInfo data);
	bool empty();
	void add_before(ptrNODE<TInfo> ptr, TInfo data);
	void add_after(ptrNODE<TInfo> ptr, TInfo data);
	void print();
	TInfo& Delete(ptrNODE<TInfo>& ptr);
};


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
DLIST<TInfo>::~DLIST()
{
	while (!empty())
		Delete(head);
}