#ifndef _NICE_QUEUE_H_
#define _NICE_QUEUE_H_

#include <stdexcept>

#include "NiceStack.h"

// �� ������ ���� ����!

template<class T>
class NiceQueue {
private:
    // ������ ������� NiceQueue �� ���������
    // ������ � ������� ������� ������������ � ����������
    NiceQueue(void) { }

    // ������� ����
    NiceStack<T> inStack;
    // �������� ����
    NiceStack<T> outStack;

public:
    // ������� NiceQueue � ������������ capacity.
    NiceQueue(size_t capacity);

    // ����������� �������
    size_t m_capacity;

    // ���������� ����� ��������� � �������
    size_t size(void) const;

    // ����������. ������ ��� �� ����� ����������?
    // ~NiceQueue(void);

    // �������� ����� ������� � �������
    // ��������� ���������� out_of_range ���� ������� �����
    void enq(T newelement) throw(out_of_range);

    // ������� ������� �� �������
    // ��������� ���������� out_of_range ���� ������� �����
    T deq(void) throw(out_of_range);

    // ���������� ����������� ������� � �������
    T getMinimum(void) throw(out_of_range);
};

#include "NiceQueue.hpp"

#endif

