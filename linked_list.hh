#ifndef KS_LINKED_LIST_HH
#define KS_LINKED_LIST_HH

#include <stddef.h>


template <class T>
struct double_linked_list_node
{
    double_linked_list_node* prev {nullptr};
    double_linked_list_node* next {nullptr};
    T data {};
};

template <class T>
inline T& data(double_linked_list_node<T>* node)
{
    return node->data;
}

template <class T>
inline const T& data(const double_linked_list_node<T>* node)
{
    return node->data;
}

template <class T>
inline double_linked_list_node<T>* next(double_linked_list_node<T>* node)
{
    return node->next;
}

template <class T>
inline double_linked_list_node<T>* prev(double_linked_list_node<T>* node)
{
    return node->prev;
}

template <class T>
inline double_linked_list_node<T>* first(double_linked_list_node<T>* node)
{
    while (node)
    {
        if (!prev(node)) return node;
        node = prev(node);
    }
    return nullptr;
}

template <class T>
inline double_linked_list_node<T>* last(double_linked_list_node<T>* node)
{
    while (node)
    {
        if (!next(node)) return node;
        node = next(node);
    }
    return nullptr;
}

template <class T>
inline size_t size(double_linked_list_node<T>* node)
{
    node = first(node);
    size_t len {};

    for (node = first(node); node != nullptr; node = next(node))
        ++len;

    return len;
}

template <class T>
inline void append(double_linked_list_node<T>* node, double_linked_list_node<T>* new_node)
{
    node = last(node);
    node->next = new_node;
    new_node->prev = node;
}

//template <class T>
//double_linked_list_node<T>* make_double_linked_list_node(const T& data)
//{
//    double_linked_list_node<T>* node = new double_linked_list_node<T>();
//    node->data = data;
//    return node;
//}

template <class T, class U = T>
inline double_linked_list_node<T>* make_double_linked_list_node(const U& data)
{
    double_linked_list_node<T>* node = new double_linked_list_node<T>();
    node->data = dynamic_cast<T>(data);
    return node;
}

//template <class T>
//void append(double_linked_list_node<T>* node, const T& data)
//{
//    double_linked_list_node<T>* new_node = make_double_linked_list_node(data);
//    append(node, new_node);
//}

template <class T, class U = T>
inline void append(double_linked_list_node<T>* node, const U& data)
{
    double_linked_list_node<T>* new_node = make_double_linked_list_node<T>(data);
    append(node, new_node);
}

template <class T>
inline void deallocate(double_linked_list_node<T>* node)
{
    node = first(node);

    while (node)
    {
        double_linked_list_node<T>* temp = node;
        node = next(node);
        delete temp;
    }
}


#endif // KS_LINKED_LIST_HH