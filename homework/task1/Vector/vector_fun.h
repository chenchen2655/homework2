#include "vector.h"
#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std;
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] = A[lo++];
}

template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
        return;
    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY;
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
}

template <typename T>
void Vector<T>::shrink()
{
    if (_capacity < DEFAULT_CAPACITY << 1)
        return;
    if (_size << 2 > _capacity)
        return;
    T *oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = oldElem[i];
    delete[] oldElem;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector<T> const &V)
{
    delete[] _elem;
    copyFrom(V._elem, 0, V.size());
    return *this;
}

template <typename T>
T &Vector<T>::operator[](Rank r) { return _elem[r]; }

template <typename T>
void Vector<T>::print()
{
    for (Rank i = 0; i < _size; i++)
        cout << _elem[i] << ' ';
    cout << endl;
}

template <typename T>
Rank Vector<T>::find(T const &e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && (e != _elem[hi]))
        ;
    return hi;
}

template <typename T>
void Vector<T>::unsort()
{
    for (int i = this->size(); i > 0; i--)
        swap(this->_elem[i - 1], this->_elem[rand() % i]);
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const &e)
{
    expand();
    for (int i = _size; i > r; i--)
        _elem[i] = _elem[i - 1];
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T>
Rank Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi)
        return 0;
    while (hi < _size)
        _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
}

template <typename T>
T Vector<T>::remove(Rank r)
{
    T e = _elem[r];
    remove(r, r + 1);
    return e;
}

template <typename T>
Rank Vector<T>::deduplicate()
{
    int oldSize = _size;
    Rank i = 1;
    while (i < _size)
        (find(_elem[i], 0, i) < 0) ? i++ : remove(i);
    return oldSize - _size;
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{ // assert: 0 <= lo < hi <= size
    while (lo < --hi)
        for (Rank i = lo; i < hi; i++)
            if (_elem[i] > _elem[i + 1])
                swap(_elem[i], _elem[i + 1]);
}

template <typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
    bool sorted = true;
    while (++lo < hi)
        if (_elem[lo - 1] > _elem[lo])
        {
            sorted = false;
            swap(_elem[lo - 1], _elem[lo]);
        }
    return sorted;
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    this->bubbleSort(lo, hi);
}

template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    int lb = mi - lo;
    T *B = new T[lb];
    for (Rank i = 0; i < lb; i++)
        B[i] = _elem[lo + i];
    int lc = hi - mi;
    T *C = _elem + mi;
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
    {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
            _elem[lo + i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
            _elem[lo + i++] = C[k++];
    }
    delete[] B;
}

template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
        return;
    int mi = (lo + hi) / 2;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template <typename T>
auto Vector<T>::sort(char x)
{
    auto time = 0;
    switch (x)
    {
    case 'b':
    {
        auto start_b = chrono::steady_clock::now();
        this->bubbleSort(0, _size);
        auto end_b = chrono::steady_clock::now();
        time = chrono::duration_cast<chrono::nanoseconds>(end_b - start_b).count();
        break;
    }
    case 'm':
    {
        auto start_m = chrono::steady_clock::now();
        this->mergeSort(0, _size);
        auto end_m = chrono::steady_clock::now();
        time = chrono::duration_cast<chrono::nanoseconds>(end_m - start_m).count();
        break;
    }
    }
    return time;
}

template <typename T>
void Vector<T>::reverse()
{
    for (Rank i = 0; i < int(_size / 2); i++)
        swap(_elem[i], _elem[_size - i - 1]);
}
