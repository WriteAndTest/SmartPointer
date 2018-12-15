#ifndef __SMARTPOINTER__
#define __SMARTPOINTER__

#include <cstddef>              // for std::size_t
#include <iostream>


//--- �ڲ����� ������ȡ ----
template <class T>
struct sp_element // ����Ϊʲô���ý� <T> ??
{
	typedef T type;
};

//#if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
template <class T>
struct sp_element<T[]> // T[] & T[N] ����������??
{
	typedef T type;
};

//#if !defined( __BORLANDC__ ) || !BOOST_WORKAROUND( __BORLANDC__, < 0x600 )  ����ʲô��??
template <class T, std::size_t N>
struct sp_element<T[N]>
{
	typedef T type;
};


//--- �ڲ������� ������ȡ ----
template <class T>
struct sp_dereference
{
	typedef T& type;  // T&  ??
};
template <> // class T  ??
struct sp_dereference<void>
{
	typedef void type;
};

//#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS) ??
template <> // ����û�� class T  ??
struct sp_dereference<void const>
{
	typedef void type;
};

template <>
struct sp_dereference<void volatile>
{
	typedef void type; //��ô���� void volatile ??
};

template <>
struct sp_dereference<void const volatile>
{
	typedef void type; //��ô���� void const volatile ??
};

// �������������������ô
// void* ??
// const void const ??


//if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION ) ??
template <class T>
struct sp_dereference<T[]>
{
	typedef void type;  // ����Ϊʲô�� void, ���� void* ���� T& T* ??
};

//if !defined( __BORLANDC__ ) || !BOOST_WORKAROUND( __BORLANDC__, < 0x600 )
template <class T, std::size_t N>
struct sp_dereference<T[N]>
{
	typedef void type;  // ����Ϊʲô�� void ??
};


//--- �ڲ���Ա���� ������ȡ ----
template <class T>
struct sp_member_access
{
	typedef T* type;
};

//if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
template <class T>
struct sp_member_access<T[]>
{
	typedef void type;
};

//if !defined( __BORLANDC__ ) || !BOOST_WORKAROUND( __BORLANDC__, < 0x600 )
template <class T, std::size_t N>
struct sp_member_access <T[N]>
{
	typedef void type;
};


//--- �����Ա�±���� ������ȡ ----
template <class T>
struct sp_array_access
{
	typedef void type; // �Ա� sp_member_access��Ϊʲô���� T* �� T&  ??
};

template <class T>
struct sp_array_access<T[]>
{
	typedef T& type;
};

template <class T, std::size_t N>
struct sp_array_access<T[N]>
{
	typedef T& type;
};


//--- �±���ʵ��±����� ������ȡ ----
template <class T>
struct sp_extent
{
	enum _vt{ value = 0 };
};

//if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
template <class T, std::size_t N>
struct sp_extent<T[N]>
{
	enum _vt{ value = N };
};

// T[] ���������������Ϊʲô ??



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