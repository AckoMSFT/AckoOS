#ifndef VECTOR_H_
#define VECTOR_H_

#include "HELPER.h"
#include "PCB.h"

template <class T>
class Vector
{
public:
	int count, size;
	T ** values;
	virtual ~Vector();
	Vector(int __size);
	Vector& add(T * value);
	Vector& remove(ID id);
	T* find(ID id);
};

template <class T>
Vector<T>::Vector(int __size)
{
	count = 0;
	size = __size;
	values = new T*[size];
	for ( int i = 0; i < size; i++ ) values[i] = NULL;
}

template <class T>
Vector<T>::~Vector()
{
	for(int i = 0; i < size; i++)
	{
		if (values[i] != NULL)
		{
			delete values[i];
			values[i] = NULL;
		}
	}
}

template <class T>
Vector<T>& Vector<T>::add(T* value)
{
	for (int i = 0; i < size; i++)
	{
		if (values[i] == NULL)
		{
			values[i] = value;
			count++;
			break;
		}
	}
}

template <class T>
T* Vector<T>::find(ID id)
{
	for (int i = 0; i < size; i++)
	{
		if (values[i] != NULL && values[i] -> id == id) return values[i];
	}
	return NULL;
}

template <class T>
Vector<T>& Vector<T>::remove(ID id)
{
	for (int i = 0; i < size; i++)
	{
		if (values[i] != NULL && values[i] -> id == id)
		{
			delete values[i];
			values[i] = NULL;
		}
	}
}

#endif /* VECTOR_H_ */
