#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include "exceptions.hpp"

namespace sjtu 
{
template<typename T>
class list 
{
private :
	class node
	{
		friend list;
		T* data;
		node *prev, *next; 
		node() : data(NULL),prev(NULL),next(NULL) {}
		node(T* temData, node* temPrev, node* temNext) : data(temData), prev(temPrev), next(temNext) {}
	};
	
	node *head, *tail;
	size_t currentLength;
	
public:
	class iterator;
	class const_iterator;
	
	class iterator 
	{
		friend class list;
	private:
		node* position;
	public:
		iterator(node* temPosition): position(temPosition) {}
		iterator& operator++() { position = position -> next; return *this; }
		iterator& operator--() { position = position -> prev; return *this; }
		iterator operator++(int) { position = position -> next; return iterator(position -> prev); }
		iterator operator--(int) { position = position -> prev; return iterator(position -> next); }
		T& operator*() const{ return *(position -> data); }
		bool operator==(const iterator &rhs) { return position == rhs.position; }
		bool operator!=(const iterator &rhs) { return position != rhs.position; }
		bool operator==(const const_iterator &rhs) { return position == rhs.position; }
		bool operator!=(const const_iterator &rhs) { return position != rhs.position; }
	};
	
	class const_iterator 
	{
		friend class list;
	private:
		node* position;
	public:
		const_iterator(node* temPosition): position(temPosition) {}
		const_iterator& operator++() { position = position -> next; return *this; }
		const_iterator& operator--() { position = position -> prev; return *this; }
		const_iterator operator++(int) { position = position -> next; return const_iterator(position -> prev); }
		const_iterator operator--(int) { position = position -> prev; return const_iterator(position -> next); }
		const T& operator*() const{ return *(position -> data); }
		bool operator==(const iterator &rhs) { return position == rhs.position; }
		bool operator!=(const iterator &rhs) { return position != rhs.position; }
		bool operator==(const const_iterator &rhs) { return position == rhs.position; }
		bool operator!=(const const_iterator &rhs) { return position != rhs.position; }
	};
	
	list()
	{
		head = new node;
		tail = new node;
		head -> next = tail;
		tail -> prev = head;
		currentLength = 0;
	}
	
	list(const list<T> &other) 
	{
		head = new node;
		tail = new node;
		head -> next = tail;
		tail -> prev = head;
		if(&other == this) return;
		for(node* i = other.head -> next; i != other.tail; i = i -> next)
		{
			node* tem = new node(new T(*(i -> data)), tail -> prev, tail);
			tail -> prev -> next = tem;
			tail -> prev = tem;
		}
		currentLength = other.currentLength;
	}
	
	inline void clear() 
	{
		for(node* i = head -> next; i != tail;)
		{
			delete i -> data;
			i = i -> next;
			delete i -> prev;
		}
		head -> next = tail;
		tail -> prev = head;
		currentLength = 0;
	}
	
	list<T>& operator=(const list<T> &other) 
	{
		if(this == &other) return *this;
		for(node* i = head -> next; i != tail;)
		{
			delete i -> data;
			i = i -> next;
			delete i -> prev;
		}
		head -> next = tail;
		tail -> prev = head;
		for(node* i = other.head -> next; i != other.tail; i = i -> next)
		{
			node* tem = new node(new T(*(i -> data)), tail -> prev, tail);
			tail -> prev -> next = tem;
			tail -> prev = tem;
		}
		currentLength = other.currentLength;
		return *this;
	}
	
	~list() 
	{
		for(node* i = head -> next; i != tail;)
		{
			delete i -> data;
			i = i -> next;
			delete i -> prev;
		}
		delete head;
		delete tail;
	}
	
	const T& front() const 
	{ 
		if(currentLength == 0) throw container_is_empty("list");
		return *(head -> next -> data); 
	}
	const T& back() const 
	{ 
		if(currentLength == 0) throw container_is_empty("list");
		return *(tail -> prev -> data); 
	}
	
	iterator begin() { return iterator(head -> next); }
	iterator end() { return iterator(tail); }
	
	const_iterator cbegin() const { return const_iterator(head -> next); }
	const_iterator cend() const { return const_iterator(tail); }
	
	bool empty() const { return currentLength == 0; }
	
	size_t size() const { return currentLength; }
	
	inline iterator insert(iterator pos, const T &value) 
	{
		node* tem = new node(new T(value), pos.position -> prev, pos.position);
		++currentLength;
		pos.position -> prev -> next = tem;
		pos.position -> prev = tem;
		return iterator(tem);
	}
	
	inline iterator erase(iterator pos) 
	{
		node* tem = pos.position;
		iterator i(tem -> next);
		tem -> prev -> next = tem -> next;
		tem -> next -> prev = tem -> prev;
		delete tem -> data;
		delete tem;
		--currentLength;
		return i;
	}
	
	iterator erase(iterator first, iterator last) 
	{
		node* temHead = first.position -> prev;
		node* temTail = last.position;
		for(node* i = temHead -> next; i != temTail;)
		{
			delete i -> data;
			i = i -> next;
			delete i -> prev;
			--currentLength;
		}
		temHead -> next = temTail;
		temTail -> prev = temHead;
		return last;
	}
	
	void push_back(const T &value) { insert(tail, value); }
	void push_front(const T &value) { insert(head -> next, value); }
	
	void pop_back()
	{
		if(currentLength == 0) throw container_is_empty("list");
		--currentLength;
		node* tem = tail -> prev -> prev;
		delete tail -> prev -> data;
		delete tail -> prev;
		tem -> next = tail;
		tail -> prev = tem;
	}
	
	void pop_front()
	{
		if(currentLength == 0) throw container_is_empty("list");
		--currentLength;
		node* tem = head -> next -> next;
		delete head -> next -> data;
		delete head -> next;
		tem -> prev = head;
		head -> next = tem;
	}
};
}
#endif
