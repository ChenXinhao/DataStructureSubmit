#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include <climits>
#include <cstddef>
#include <stdlib.h>
#include "exceptions.hpp"

namespace sjtu 
{
template<typename T>
class vector 
{
private:
	T *data;
	size_t currentLength, maxSize;
	void doubleSpace()
	{
		T* tem = data;
		maxSize *= 2;
		data = (T*)operator new(maxSize * sizeof(T));
		memset(data, 0, maxSize * sizeof(T));
		for(size_t i = 0; i < currentLength; ++i) new(&data[i]) T(tem[i]);
		for(size_t i = 0; i < currentLength; ++i) tem[i].~T();
		operator delete(tem);
	}
public:
	class iterator;
	class const_iterator;
	
	class iterator
	{
		friend class vector;
	private:
		vector<T>* it;
		size_t position;
		iterator(const vector<T>* temIt, size_t temPosition)
		{
			it = (vector<T>*) temIt;
			position = temPosition;
		}
	public:
		iterator operator+(const size_t &n) { return iterator(this -> it, (this -> position) + n); }
		iterator operator-(const size_t &n) { return iterator(this -> it, (this -> position) - n); }
		
		size_t operator-(const iterator &rhs) 
		{ 
			if(it != rhs.it) throw invaild_iterator("vector :: iterator"); 
			return position - rhs.position; 
		}
		
		iterator operator+=(const size_t &n) { position += n; return *this; }
		iterator operator-=(const size_t &n) { position -= n; return *this; }
	
		iterator operator++(int) { ++position; return iterator(it, position - 1); }
		iterator operator--(int) { --position; return iterator(it, position + 1); }
		iterator& operator++() { ++position; return *this; }
		iterator& operator--() { --position; return *this; }
		
		T& operator*() const
		{
			if(position < 0 || position >= it -> currentLength)  throw invaild_iterator("vector :: iterator");
			return it -> data[position];
		}
		
		bool operator==(const iterator &rhs) { return it == rhs.it && position == rhs.position; }
		bool operator!=(const iterator &rhs) { return it != rhs.it || position != rhs.position; }
		bool operator==(const const_iterator &rhs) { return it == rhs.it && position == rhs.position; }
		bool operator!=(const const_iterator &rhs) { return it != rhs.it || position != rhs.position; }
	};
	
	class const_iterator
	{
		friend class vector;
	private:
		vector<T>* it;
		size_t position;
		const_iterator(const vector<T>* temIt, size_t temPosition)
		{
			it = (vector<T>*)temIt;
			position = temPosition;
		}
	public:
		const_iterator operator+(const size_t &n) { return const_iterator(this -> it, (this -> position) + n); }
		const_iterator operator-(const size_t &n) { return const_iterator(this -> it, (this -> position) - n); }
		
		size_t operator-(const const_iterator &rhs) 
		{ 
			if(it != rhs.it) throw invaild_iterator("vector :: iterator"); 
			return position - rhs.position; 
		} 
		
		const_iterator operator+=(const size_t &n) { position += n; return *this; }
		const_iterator operator-=(const size_t &n) { position -= n; return *this; }
		
		const_iterator operator++(int) { ++position; return const_iterator(it, position - 1); }
		const_iterator operator--(int) { --position; return const_iterator(it, position + 1); }
		const_iterator& operator++() { ++position; return *this; }
		const_iterator& operator--() { --position; return *this; }
		
		const T& operator*() const
		{
			if(position < 0 || position >= it -> currentLength)  throw invaild_iterator("vector :: iterator");
			return it -> data[position];
		}
		
		bool operator==(const iterator &rhs) { return it == rhs.it && position == rhs.position; }
		bool operator!=(const iterator &rhs) { return it != rhs.it || position != rhs.position; }
		bool operator==(const const_iterator &rhs) { return it == rhs.it && position == rhs.position; }
		bool operator!=(const const_iterator &rhs) { return it != rhs.it || position != rhs.position; }
	};
	
	vector()
	{
		currentLength = 0, maxSize = 20;
		data = (T*) operator new(maxSize * sizeof(T));
		memset(data, 0, maxSize * sizeof(T));
	}
	
	vector(const vector<T> &other)
	{
		if(&other == this) return ;
		currentLength = other.currentLength;
		maxSize = other.maxSize;
		data = (T*)operator new(maxSize * sizeof(T));
		memset(data, 0, maxSize * sizeof(T));
		for(size_t i = 0; i < currentLength; ++i) new(&data[i]) T(other.data[i]);
	}
	
	vector<T>& operator=(const vector<T> &other) 
	{
		if(&other == this) return *this;
		for(int i = 0; i < currentLength; ++i) data[i].~T();
		operator delete(data);;
		maxSize = other.maxSize, currentLength = other.currentLength;
		data = (T*)operator new(maxSize * sizeof(T));
		memset(data, 0, maxSize * sizeof(T));
		for(size_t i = 0; i < currentLength; ++i) new(&data[i]) T(other.data[i]);
		return *this;
	}
	vector<T>(vector<T>&& other)
	{
		if(this == &other) return ;
		currentLength = other.currentLength;
		maxSize = other.maxSize;
		other.currentLength = 0; 
		data = other.data;
		other.data = NULL;
	}
	vector<T>& operator=(vector<T>&& other)
    {
		if(this == &other) return *this;
		for(int i = 0; i < currentLength; ++i) data[i].~T();
		operator delete(data);
		currentLength = other.currentLength;
		maxSize = other.maxSize;
		other.currentLength = 0;
		data = other.data;
		other.data = NULL;
    	return *this;
	}
	~vector() 
	{ 
		for(int i = 0; i < currentLength; ++i) data[i].~T();
		operator delete(data);
	}
	T& at(const size_t &pos)
	{
		if(pos < 0 || pos >= currentLength)  throw index_out_of_bound("vector");
		return data[pos];
	}
	const T& at(const size_t &pos) const
	{
		if(pos < 0 || pos >= currentLength)  throw index_out_of_bound("vector");
		return data[pos];
	}
	T& operator[](const size_t &pos) 
	{
		if(pos < 0 || pos >= currentLength)  throw index_out_of_bound("vector");
		return data[pos];
	}
	const T& operator[](const size_t &pos) const 
	{
		if(pos < 0 || pos >= currentLength)  throw index_out_of_bound("vector");
		return data[pos];
	}
	const T& front() const 
	{
		if(currentLength == 0) throw container_is_empty("vector");
		return data[0];
	}
	const T& back() const 
	{
		if(currentLength == 0) throw container_is_empty("vector");
		return data[currentLength - 1];
	}
	
	iterator begin() { return iterator(this, 0); }
	iterator end() { return iterator(this, currentLength); }
	
	const_iterator cbegin() const { return const_iterator(this, 0); }
	const_iterator cend() const { return const_iterator(this, currentLength); }

	bool empty() const { return currentLength == 0; }
	
	size_t size() const { return currentLength; }
	
	size_t capacity() const { return maxSize; }
	
	void clear() { currentLength = 0;}
	
	iterator insert(iterator pos, const T &value) 
	{
		if(pos.it != this) throw invaild_iterator("vector :: iterator");
		if(pos.position < 0 || pos.position > currentLength) throw index_out_of_bound("vector");
		if(currentLength == maxSize) doubleSpace();
		for(size_t i = currentLength; i > pos.position; --i) data[i] = data[i - 1];
		data[pos.position] = value;
		++currentLength;
		return iterator(this, pos.position);
	}
	
	iterator insert(const size_t& ind, const T& value) 
	{
		if(ind < 0 || ind > currentLength) throw index_out_of_bound("vector");
		if(currentLength == maxSize) doubleSpace();
		for(size_t i = currentLength; i > ind; --i) data[i] = data[i - 1];
		data[ind] = value;
		++currentLength;
		return iterator(this, ind);
	}
	
	iterator erase(iterator pos) 
	{
		if(pos.it != this) throw invaild_iterator("vector :: iterator");
		if(pos.position < 0 || pos.position >= currentLength) throw index_out_of_bound("vector");
		--currentLength;
		for(size_t i = pos.position; i < currentLength; ++i) data[i] = data[i + 1];
		data[currentLength].~T();
		return iterator(this, pos.position);
	}
	
	iterator erase(const size_t& ind)
	{
		if(ind < 0 || ind >= currentLength) throw index_out_of_bound("vector");
		--currentLength;
		for(size_t i = ind; i < currentLength; ++i) data[i] = data[i + 1];
		data[currentLength].~T();
		return iterator(this, ind);
	}
	
	void push_back(const T& value) 
	{
		if(currentLength == maxSize) doubleSpace();
		data[currentLength++] = value;
	}
	
	void pop_back() 
	{
		if(currentLength == 0) throw container_is_empty("vector");
		--currentLength;
	}
};
}
#endif
