#ifndef __LIST_H__
#define __LIST_H__

#include <cstring>

template <class TYPE>
class List{
private:
	TYPE *value;
	int size;
public:
	List();
	~List();

	TYPE &operator[](int index);

	bool empty();

	void resize(int _size);
	void clear();
};


template <class TYPE> List<TYPE>::List(){
	value = NULL;
	size = 0;
}

template <class TYPE> List<TYPE>::~List(){
	delete[] value;
	size = 0;
}

// Operator
template <class TYPE> TYPE &List<TYPE>::operator[](int index){
	return value[index];
}

// Method
template <class TYPE> bool List<TYPE>::empty(){
	return size == 0;
}

template <class TYPE> void List<TYPE>::resize(int _size){
	clear();
	value = new TYPE[_size];
	size = _size;
}

template <class TYPE> void List<TYPE>::clear(){
	delete[] value;
	value = NULL;
	size = 0;
}

#endif
