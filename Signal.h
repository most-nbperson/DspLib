#ifndef DSPLIB_SIGNAL_H
#define DSPLIB_SIGNAL_H

#include <iostream>
#include <iomanip>
#include <complex>
#include <string>
#include <vector>
#include <fstream>

#include "Eigen/Dense"

enum SignalDomain {
    FREQUENCY_DOMAIN,
    TIME_DOMAIN,
    BE_PENDING
};

template <class Type>
class Signal {
public:
    Signal();
    Signal(long size, SignalDomain d);
    Signal(const Type* array, long len, SignalDomain d);
    Signal(const std::vector<Type>& v, SignalDomain d);
    Signal(const Eigen::VectorX<Type>& v, SignalDomain d);
    Signal(const Signal<Type>& s);

    long getLength() const;
    SignalDomain getSignalDomain() const;
    void cat(const Signal<Type>& s);
    void insert(long index, const Type& element);
    void insert(long index, const Signal<Type>& s);
    void remove(long index);
    bool isEmpty() const;


    static Signal<Type> zeroSignal(long len, SignalDomain d);
    static Signal<Type> oneSignal(long len, SignalDomain d);
    static Signal<Type> stepSignal(long len, SignalDomain d);
    static Signal<Type> impulseSignals(long len, SignalDomain d);
    static Signal<Type> randomSignal(long len, SignalDomain d);
    static Signal<Type> sinSignal(long len, SignalDomain d);
    static Signal<Type> cosSignal(long len, SignalDomain d);
    //TODO linspace方式生成信号

    Type& operator[](int index);
    Signal<Type> operator+(const Signal<Type>& s);
    Signal<Type> operator-(const Signal<Type>& s);
    Signal<Type> operator*(const Signal<Type>& s);
    Signal<Type> operator*(Type c);
    template<class T> friend Signal<T> operator*(T c, const Signal<T>& s);
    Signal<Type>& operator=(const Signal<Type>& rhs);
    template<class T> friend std::ostream& operator<<(std::ostream& output, const Signal<T>& s);

private:
    Eigen::VectorX<Type> data;
    SignalDomain domain;
};

template <class Type>
Signal<Type>::Signal()
{
    domain = BE_PENDING;
}

template <class Type>
Signal<Type>::Signal(long size, SignalDomain d)
{
    domain = d;
    data.resize(size);
}

template <class Type>
Signal<Type>::Signal(const Type* array, long len, SignalDomain d)
{
    domain = d;
    data = Eigen::Map<const Eigen::VectorX<Type>>(array, len);
}

template <class Type>
Signal<Type>::Signal(const Eigen::VectorX<Type>& v, SignalDomain d)
{
    domain = d;
    data = v;
}

template <class Type>
Signal<Type>::Signal(const std::vector<Type>& v, SignalDomain d)
{
    domain = d;
    data = Eigen::Map<const Eigen::VectorX<Type>>(v.data(), v.size());
}

template <class Type>
Signal<Type>::Signal(const Signal<Type>& s)
{
    domain = s.domain;
    data = s.data;
}

template <class Type>
long Signal<Type>::getLength() const
{
    return data.rows();
}

template <class Type>
SignalDomain Signal<Type>::getSignalDomain() const
{
    return domain;
}

template <class Type>
void Signal<Type>::cat(const Signal<Type>& s)
{
    if(domain != s.domain)
    {
        throw "SIGNAL DOMAIN IS NOT MATCH\n";
    }
    Eigen::VectorX<Type> result{data.rows() + s.getLength()};
    result << data, s.data;
    data = result;
}

template <class Type>
void Signal<Type>::insert(long index, const Type& element)
{
    if (index > data.rows())
    {
        throw "OUT OF RANGE";
    }

    Eigen::VectorX<Type> result{data.rows() + 1};
    for (int i = 0; i < index; ++i)
    {
        result[i] = data[i];
    }
    result[index] = element;
    for (int i = index; i < data.rows(); ++i)
    {
        result[i + 1] = data[i];
    }
    data = result;
}

template <class Type>
void Signal<Type>::insert(long index, const Signal<Type>& s)
{
    if (index > data.rows())
    {
        throw "OUT OF RANGE";
    }
    if (s.domain != domain)
    {
        throw "SIGNAL DOMAIN IS NOT MATCH\n";
    }
    int distance = s.getLength();
    Eigen::VectorX<Type> result{data.rows() + distance};
    for (int i = 0; i < index; ++i)
    {
        result[i] = data[i];
    }
    for (int i = 0; i < distance; ++i)
    {
        result[i + index] = s.data[i];
    }
    for (int i = index; i < data.rows(); ++i)
    {
        result[i + distance] = data[i];
    }
    data = result;
}

template <class Type>
void Signal<Type>::remove(long index)
{
    if (index > data.rows() - 1)
    {
        throw "OUT OF RANGE";
    }
    Eigen::VectorX<Type> result{data.rows() - 1};
    for (int i = 0; i < index; ++i)
    {
        result[i] = data[i];
    }
    for (int i = index + 1; i < data.rows(); ++i)
    {
        result[i - 1] = data[i];
    }
    data = result;
}

template <class Type>
bool Signal<Type>::isEmpty() const
{
    return data.size() == 0;
}

template <class Type>
Signal<Type> Signal<Type>::zeroSignal(long len, SignalDomain d)
{
    return Signal<Type>{Eigen::VectorX<Type>::Zero(len), d};
}

template <class Type>
Signal<Type> Signal<Type>::oneSignal(long len, SignalDomain d)
{
    return Signal<Type>{Eigen::VectorX<Type>::Ones(len), d};
}

template <class Type>
Signal<Type> Signal<Type>::stepSignal(long len, SignalDomain d)
{
    //TODO 补全代码
}

template <class Type>
Signal<Type> Signal<Type>::impulseSignals(long len, SignalDomain d)
{
    //TODO 补全代码
}

template <class Type>
Signal<Type> Signal<Type>::randomSignal(long len, SignalDomain d)
{
    return Signal<Type>{Eigen::VectorX<Type>::Random(len), d};
}

template <class Type>
Signal<Type> Signal<Type>::sinSignal(long len, SignalDomain d)
{
    //TODO 补全代码
}

template <class Type>
Signal<Type> Signal<Type>::cosSignal(long len, SignalDomain d)
{
    //TODO 补全代码
}

template <class Type>
Type& Signal<Type>::operator[](int index)
{
    if (index > data.rows() - 1)
    {
        throw "OUT OF RANGE\n";
    }
    return data[index];
}

template <class Type>
Signal<Type> Signal<Type>::operator+(const Signal<Type>& s)
{
    if (s.getLength() != data.rows())
    {
        throw "SIGNAL LENGTH IS NOT MATCH\n";
    }
    if (s.domain != domain)
    {
        throw "SIGNAL DOMAIN IS NOT MATCH\n";
    }
    Eigen::VectorX<Type> result{data.rows()};
    result.array() = data.array() + s.data.array();
    return Signal<Type>{result, domain};
}

template <class Type>
Signal<Type> Signal<Type>::operator-(const Signal<Type>& s)
{
    if (s.getLength() != data.rows())
    {
        throw "SIGNAL LENGTH IS NOT MATCH\n";
    }
    if (s.domain != domain)
    {
        throw "SIGNAL DOMAIN IS NOT MATCH\n";
    }
    Eigen::VectorX<Type> result{data.rows()};
    result.array() = data.array() - s.data.array();
    return Signal<Type>{result, domain};
}

template <class Type>
Signal<Type> Signal<Type>::operator*(const Signal<Type>& s)
{
    if (s.getLength() != data.rows())
    {
        throw "SIGNAL LENGTH IS NOT MATCH\n";
    }
    if (s.domain != domain)
    {
        throw "SIGNAL DOMAIN IS NOT MATCH\n";
    }
    Eigen::VectorX<Type> result{data.rows()};
    result.array() = data.array() * s.data.array();
    return Signal<Type>{result, domain};
}

template <class Type>
Signal<Type> Signal<Type>::operator*(Type c)
{
    Eigen::VectorX<Type> result{data.rows()};
    result = data * c;
    return Signal<Type>{result, domain};
}

template <class T>
Signal<T> operator*(T c, const Signal<T>& s)
{
    Eigen::VectorX<T> result{s.data.rows()};
    result = s.data * c;
    return Signal<T>{result, s.domain};
}

template <class Type>
Signal<Type>& Signal<Type>::operator=(const Signal<Type>& rhs)
{
    if (this != &rhs)
    {
        this->data = rhs.data;
        this->domain = rhs.domain;
    }
    return *this;
}

template <class T>
std::ostream& operator<<(std::ostream& output, const Signal<T>& s)
{
    if (s.isEmpty())
    {
        output << "[ EMPTY ]" << std::endl;
    }
    else
    {
        output << "["<< s.data.transpose() << "]" << std::endl;
    }
    return output;
}

#endif //DSPLIB_SIGNAL_H
