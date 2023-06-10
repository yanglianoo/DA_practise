/**
 * @File Name: Vector.hpp
 * @brief  Vector 模板类
 * @Author : Timer email:330070781@qq.com
 * @Version : 1.0
 * @Creat Date : 2023-05-10
 * 
 */
#pragma once 

#include <initializer_list>
#include <iostream>
#include <stdexcept>
namespace my_stl
{

#define DEFAULT_CAPACITY 3 
template <typename T> class vector
{
public:
    vector();
    ~vector();
    vector(const vector<T>& other); //用另外一个vector来构造
    vector(std::initializer_list<T> init); //列表初始化
    vector(const vector<T>& other,int left,int right); //用另外一个vector区间构造
    vector(size_t count, T& value); //初始化为count个 value
public: //成员函数
    vector& operator=(const vector& other);
    vector& operator=( std::initializer_list<T> ilist );

    void assign(size_t count,const T& value);
    void assign(std::initializer_list<T> ilist );
public: //元素访问
    T& at(size_t index);
    T& operator[](size_t index);
    T& front();
    T& back();
    T* data();
public: //容量
    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void reserve(size_t new_cap);
    size_t capacity() const;
    void shrink_to_fit();
public: //迭代器
    class Iterator
    {
        private:
            T * m_pointer;
        public:
            Iterator():m_pointer(nullptr) {}
            Iterator(T * pointer) : m_pointer(pointer) {}
            ~Iterator() {}
        /* 判断两个迭代器是否相等 */
        bool operator == (const Iterator & other)
        {
            return m_pointer == other.m_pointer;   
        } 
        /* 迭代器赋值 */
        Iterator operator = (const Iterator& other)
        {
            m_pointer = other.m_pointer;
        }

        /* 前缀递增*/
        Iterator & operator ++ ()
        {
            m_pointer +=1;
            return *this;
        }

        /* 后缀递增 */
        Iterator operator ++ (int)
        {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        Iterator operator + (int i)
        {
            Iterator it = *this;
            it.m_pointer += i;
            return it;
        }
        Iterator operator += (int i)
        {
            m_pointer += i;
            return *this;
        }

        Iterator operator -= (int i)
        {
            m_pointer -= i;
            return *this;
        }

        Iterator operator - (int i)
        {
            Iterator it = *this;
            it.m_pointer -= i;
            return it;
        }

        int operator - (const Iterator& other) const
        {
            return m_pointer - other.m_pointer;
        }

        T & operator * ()
        {
            return *m_pointer;
        }

        T * operator -> ()
        {
            return m_pointer;
        }
    };
public: //迭代器操作函数
    Iterator begin();
    Iterator end();
public: //修改器
    void clear();
    Iterator insert(const Iterator it ,const T & value);  
    Iterator insert(const Iterator it ,int n,const T & value);
    Iterator erase(const Iterator it);
    Iterator erase(const Iterator first,const Iterator last);
    void push_back(const T & value);
    void pop_back();
    void resize(size_t size);
    void swap(vector & other);

private:
    size_t _size; //数组大小
    size_t _capacity; //数组允许存储的最大长度
    T* _data; //数组头指针
    void expand(); //内存不足时扩容
    void shrink(); //装填因子过小时压缩
    void copyFrom ( T const* A, int lo, int hi );
};

/*构造函数*/
template<typename T>
vector<T>::vector():_data(nullptr),_capacity(0),_size(0)
{

}

/* 析构函数 */
template<typename T>
vector<T>::~vector()
{
    if(_data != nullptr)
    {
        delete [] _data;
        _data = nullptr;
    }
    _size = 0;
    _capacity = 0;
}


template <typename T> 
void vector<T>::expand()
{

    if(_size <= _capacity) return;  //当size 小于等于 capacity 时 不需要扩容
    if(_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //当capacity小于最小大小，更改capacity为最小大小
    /* 反复翻倍，直到 _capacity > _size*/
    while (_capacity < _size)
    {
        _capacity *=2;
    }
    T* old_data = _data;
    _data = new T[_capacity << 1];  //capacity 增大一倍，重新 new 内存
    for(int i=0;i<_size; i++)       //赋值
    {
        _data[i] = old_data[i];
    }

    delete[] old_data;

}


template <typename T> 
void vector<T>::shrink()
{
    if(_capacity < DEFAULT_CAPACITY << 1) return; //不致收缩倒DEFAULT_CAPACITY以下
    if(_size << 2 > _capacity) return;  //以 25% 为边界

    T* old_data = _data;
    _data = new T[_capacity >> 1];
    for(size_t i = 0; i < _size; i++)
    {
        _data[i] = old_data[i];
    }
    delete[] old_data;
}


template <typename T> 
void vector<T>::copyFrom ( T const* A, int left, int right )
{
    if(_data != nullptr)
    {
        delete [] _data;
        _data = nullptr;
    }
    _capacity = (right - left) * 2;
    _size = 0;
    _data = new T[_capacity];

    while (left < right)
    {
        _data[_size] = A[left];
        _size++;
        left++;
    }
}
template <typename T>
vector<T>::vector(const vector<T>& other)
{
    copyFrom(other._data,0,other._size);
}

template <typename T>
vector<T>::vector(const vector<T>& other,int left,int right)
{
    copyFrom(other._data,other.left,other.right);
}

/* c++11 列表初始化 */
template <typename T>
vector<T>::vector(std::initializer_list<T> init)
{
    _size = init.size();
    _capacity = _size * 2;
    int i = 0;
    _data = new T[_capacity];
    for(const auto & elem : init)
    {
        _data[i++] = elem;
    }
}

template <typename T>
vector<T>::vector(size_t count, T& value)
{
    _size = count;
    _capacity = 2 * _size;
    _data = new T[_capacity];
    for(size_t i =0; i<count;i++)
    {
        _data[i] = value;
    }
}

template <typename T>
vector<T>& vector<T>::operator=(const vector& other)
{
    copyFrom(other._data,0,other._size);
    return *this;
}

template <typename T>
vector<T>& vector<T>::operator=( std::initializer_list<T> ilist )
{
    if(_data != nullptr)
    {
        delete [] _data;
        _data = nullptr;
    }
    _size = ilist.size();
    _capacity = _size * 2;
    int i = 0;
    _data = new T[_capacity];
    for(const auto & elem : ilist)
    {
        _data[i++] = elem;
    }
    return *this;
}

template <typename T>
T& vector<T>::at(size_t index)
{
    if(index <0 || index >= _size)
    {
        throw std::logic_error("out of range");
    }
    return _data[index];
}

template <typename T>
T& vector<T>::operator[](size_t index)
{
    return this->at(index);
}

template <typename T>
T& vector<T>::front()
{
    return _data[0];
}


template <typename T>
T& vector<T>::back()
{
    if (_size <= 0)
    {
        throw std::logic_error("vector is empty");
    }
    return _data[_size-1];
}

template <typename T>
T* vector<T>::data()
{
    return _data;
}

template <typename T>
typename vector<T>::Iterator vector<T>::begin()
{
    vector<T>::Iterator it(_data);
    return it;
}

template <typename T>
typename vector<T>::Iterator vector<T>::end()
{
    vector<T>::Iterator it(_data + _size);
    return it;
}

template <typename T>
bool vector<T>::empty() const
{
    return _size==0;
}

template <typename T>
size_t vector<T>::size() const
{
    return _size;
}

template <typename T>
size_t vector<T>::max_size() const
{
    return _capacity;
}


template <typename T>
size_t vector<T>::capacity() const
{
    return _capacity;
}

/* 重置容器大小 */
template <typename T>
void vector<T>::reserve(size_t new_cap)
{
    if(_capacity >= new_cap)
    {
        return;
    }
    else
    {
        _size += new_cap;
        expand();
    }
}

/* 如果 _capacity 过大则缩减*/
template <typename T>
void vector<T>::shrink_to_fit()
{
    shrink();
}

/* clear操作，直接将 _size 清零*/
template <typename T>
void vector<T>::clear()
{
    if (_size <= 0)
    {
        throw std::logic_error("vector is empty");
    }
    _size = 0;
}

/* 在 it 位置插入 1 个元素 value*/
template <typename T>
typename vector<T>::Iterator vector<T>::insert(const Iterator it ,const T & value)
{
    _size+=1;
    expand();
    insert(it,1,value);
}

/* 在 it 的位置插入 n 个 T 元素*/
template <typename T>
typename vector<T>::Iterator vector<T>::insert(const Iterator it ,int n,const T & value)
{
    int count = it - begin();
    _size += n;
    //如果有需要，则扩容
    expand();
    for(size_t i=_size; i>count;i--)
    {
        _data[i+n-1] = _data[i-1];
    }
    for(size_t i=0; i<n;i++)
    {
        _data[count+i] = value;
    }
    return vector<T>::Iterator(_data + size);
}

/* 删除 it 位置的元素 */
template <typename T>
typename vector<T>::Iterator vector<T>::erase(const Iterator it)
{
    if(end() - it == 1)
    {
        _size -= 1;
        return end();
    }
    int count = it - begin();
    for(size_t i = count; i < _size -1 ; i++)
    {
        _data[i] = _data[i+1];
    }
    _size -= 1;

    shrink(); //若有必要则缩容
    return it;
}

/* 区间删除 */
template <typename T>
typename vector<T>::Iterator vector<T>::erase(const Iterator first,const Iterator last)
{
    if( first == last)
    {
        return first;  
    }
    else
    {
        int f = first - begin();
        int l = last - begin();
        while ( l < _size) // [last,_szie) 顺次前移 l - f 个单元
        {
            _data[f++] = _data[l++];
        }
        _size = f;  //更新规模
        shrink(); //若有必要则缩容
        return first;
    }
}

/* 在尾部插入一个元素value */
template <typename T>
void vector<T>::push_back(const T& value)
{
    if(_size < _capacity)
    {
        _data[_size] = value; 
        _size++;
        return;
    }
    else
    {
        _size++;
        expand();
        int index = _size - 1;
        _data[index] = value; 
    }
}

/* 弹出最后一个元素 */
template <typename T>
void vector<T>::pop_back()
{
    if(_size != 0)
    {
        _size--;
    }
}


template <typename T>
void vector<T>::resize(size_t size)
{
    if(_size > size)
    {
        _size = size;
        return;
    }

    if(size <= _capacity)
    {
        for(size_t i= _size; i < size; i++)
        {
            _data[i] = T();
        }
        _size = size;
        return;
    }

    _size = size;
    expand();
}

/* 两个 vector 交换*/
template <typename T>
void vector<T>::swap(vector & other)
{
    T * data = other._data;
    int size = other._size;
    int capacity = other._capacity;

    other._data = _data;
    other._size = _size;
    other._capacity = _capacity;

    _data = data;
    _size = size;
    _capacity = capacity;
}

}