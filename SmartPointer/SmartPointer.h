#ifndef __SMARTPOINTER__
#define __SMARTPOINTER__

#include <iostream>

template <class T>
class SmartPointer
{
public:
	SmartPointer(T* p = nullptr)
	: m_p(p)
		, m_ref(new int(1))
	{
		std::cout << "����! p=" << m_p << std::endl;
	}
	~SmartPointer()
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

	void reset() noexcept
	{
		m_p = nullptr;
		*m_ref = 0;
	}
	int addRef() const
	{
		++*m_ref;
		std::cout << "p= " << m_p << "Ref = " << *m_ref << std::endl;
		return *m_ref;
	}
	int decRef() const
	{
		--*m_ref;
		std::cout << "p= " << m_p << "Ref = " << *m_ref << std::endl;
		return *m_ref;
	}

	SmartPointer(const SmartPointer& sp)
		: m_p(sp.m_p)
		, m_ref(sp.m_ref)
	{
		addRef();
		std::cout << "��������! p=" << m_p << std::endl;
	}
	SmartPointer& operator=(const SmartPointer& p)
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
	T& operator[] (int indx)
	{
		std::cout << "ȡֵ! p" << indx << std::endl;

		return m_p[indx];
	}
	T* operator->()
	{
		return m_p;
	}

private:
	T* m_p;
	int* m_ref;
};


//template<typename T, typename A, typename Arg1, typename Arg2, typename Arg3>
//SmartPointer<T> allocate_shared(A const & a, Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3)
//{
//	return SmartPointer<T[]>
//}




#endif // !__SMARTPOINTER__