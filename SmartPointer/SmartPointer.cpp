
#ifdef CPP
#include "SmartPointer.h"

#include <iostream>

template <class T>
SmartPointer<T>::SmartPointer(T* p)
	: m_p(p)
	, m_ref(new int(1))
{
	std::cout << "����! p=" << m_p << std::endl;
}

template <class T>
SmartPointer<T>::~SmartPointer()
{
	int ref = *m_ref - 1;
	if (-- *m_ref == 0)
	{
		std::cout << "����! p=" << m_p << std::endl;
		delete m_p;
		delete m_ref;
		m_p = nullptr;
		m_ref = nullptr;
	}
	std::cout << "����!Ref = " << ref << std::endl;
}

template <class T>
void SmartPointer<T>::reset() noexcept
{
	m_p = nullptr;
	*m_ref = 0;
}

template <class T>
int SmartPointer<T>::addRef() const
{
	++*m_ref;
	std::cout << "p= " << m_p << "Ref = " << *m_ref << std::endl;
	return *m_ref;
}

template <class T>
int SmartPointer<T>::decRef() const
{
	--*m_ref;
	std::cout << "p= " << m_p << "Ref = " << *m_ref << std::endl;
	return *m_ref;
}

template <class T>
SmartPointer<T>::SmartPointer(const SmartPointer& sp)
	: m_p(sp.m_p)
	, m_ref(sp.m_ref)
{
	addRef();
	std::cout << "��������! p=" << m_p << std::endl;
}

template <class T>
SmartPointer<T>& SmartPointer<T>::operator=(const SmartPointer& p)
{
	std::cout << "��ֵ! p1=" << m_p << ", p2=" << p.m_p << std::endl;
	p.addRef();

	if (decRef() == 0)
	{
		std::cout << "ref��Ϊ0! ɾ������" << std::endl;
		delete m_p;
		delete m_ref;
	}

	m_p = p.m_p;
	m_ref = p.m_ref;

	return *this;
}

template <class T>
T& SmartPointer<T>::operator[](int indx)
{
	std::cout << "ȡֵ! p" << indx << std::endl;

	return m_p[indx];
}

template <class T>
T* SmartPointer<T>::operator->()
{
	return m_p;
}

#endif