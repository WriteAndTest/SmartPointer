#ifndef __SMARTPOINTER__
#define __SMARTPOINTER__

#include <cstddef>              // for std::size_t
#include <iostream>


//--- 内部数据 类型萃取 ----
template <class T>
struct sp_element // 这里为什么不用接 <T> ??  可能是为了统一的风格 因为很多模板类型都有一个type表示类型。 还有一个考虑是 直接用T 是没办法用的，需要typedef 定义一个别名 间接使用
{
	typedef T type;
};

//#if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION )
template <class T>
struct sp_element<T[]> // T[] & T[N] 区别在哪里?? T[]表示T类型的指针 T[N]表示N个T类型元素的数组类型
{
	typedef T type;
};

//#if !defined( __BORLANDC__ ) || !BOOST_WORKAROUND( __BORLANDC__, < 0x600 )  这是什么鬼??  先不管define 这些逻辑 看看我们关注的业务代码呢 我也没跳进去看
template <class T, std::size_t N>
struct sp_element<T[N]>
{
	typedef T type;
};


//--- 内部解引用 类型萃取 ----
template <class T>
struct sp_dereference
{
	typedef T& type;  // T&  ?? T& T类型的引用类型 
};
template <> // class T  ??
struct sp_dereference<void>
{
	typedef void type;
};

//#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS) ??
template <> // 这里没有 class T  ?? 这里是模板偏特化， 里面的 class T 省略的毕竟后面用不到这个T 对吧
struct sp_dereference<void const>
{
	typedef void type;
};

template <>
struct sp_dereference<void volatile>
{
	typedef void type; //怎么不是 void volatile ?? 可以认为void是一种特殊的类型 volatile是一种额外修饰类型的描述符号 告诉编译器 不需要对这个变量做优化 每次从内存里取值 不用缓存 等等。。。 void volatile 是两种语义结合一起了 
};

template <>
struct sp_dereference<void const volatile>
{
	typedef void type; //怎么不是 void const volatile ?? 同上 加了一个const语句
};

// 下面两个情况不用讨论么
// void* ??
// const void const ??


//if !defined( BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION ) ??
template <class T>
struct sp_dereference<T[]>
{
	typedef void type;  // 这里为什么是 void, 不是 void* 或者 T& T* ??
};

//if !defined( __BORLANDC__ ) || !BOOST_WORKAROUND( __BORLANDC__, < 0x600 )
template <class T, std::size_t N>
struct sp_dereference<T[N]>
{
	typedef void type;  // 这里为什么是 void ??
};


//--- 内部成员访问 类型萃取 ----
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


//--- 数组成员下标访问 类型萃取 ----
template <class T>
struct sp_array_access
{
	typedef void type; // 对比 sp_member_access，为什么不是 T* 或 T&  ??
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


//--- 下标访问的下标上限 类型萃取 ----
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

// T[] 的情况不用讨论吗，为什么 ??



template <class T>
class SmartPointer
{
public:
	SmartPointer(T* p = nullptr)
	: m_p(p)
		, m_ref(new int(1))
	{
		std::cout << "构造! p=" << m_p << std::endl;
	}
	~SmartPointer()
	{
		int ref = *m_ref - 1;
		if (-- *m_ref == 0)
		{
			std::cout << "析构! p=" << m_p << std::endl;
			delete m_p;
			delete m_ref;
			m_p = nullptr;
			m_ref = nullptr;
		}
		std::cout << "析构!Ref = " << ref << std::endl;
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
		std::cout << "拷贝构造! p=" << m_p << std::endl;
	}
	SmartPointer& operator=(const SmartPointer& p)
	{
		std::cout << "赋值! p1=" << m_p << ", p2=" << p.m_p << std::endl;
		p.addRef();

		if (decRef() == 0)
		{
			std::cout << "ref降为0! 删除自身" << std::endl;
			delete m_p;
			delete m_ref;
		}

		m_p = p.m_p;
		m_ref = p.m_ref;

		return *this;
	}
	T& operator[] (int indx)
	{
		std::cout << "取值! p" << indx << std::endl;

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