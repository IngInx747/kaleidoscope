#ifndef KS_UTILITY_HH
#define KS_UTILITY_HH


const char* make_c_str(const char*);


template <class T>
struct double_linked_list_node
{
    double_linked_list_node* prev {nullptr};
    double_linked_list_node* next {nullptr};
    T* data {nullptr};
};

template <class T>
double_linked_list_node<T>* next(double_linked_list_node<T>* node)
{
    return node->next;
}

template <class T>
double_linked_list_node<T>* prev(double_linked_list_node<T>* node)
{
    return node->prev;
}

template <class T>
double_linked_list_node<T>* first(double_linked_list_node<T>* node)
{
    while (node)
    {
        if (!prev(node)) return node;
        node = prev(node);
    }
    return nullptr;
}

template <class T>
double_linked_list_node<T>* last(double_linked_list_node<T>* node)
{
    while (node)
    {
        if (!next(node)) return node;
        node = next(node);
    }
    return nullptr;
}

template <class T>
void append(double_linked_list_node<T>* node, double_linked_list_node<T>* new_node)
{
    node = last(node);
    node->next = new_node;
    new_node->prev = node;
}

template <class T>
void deallocate(double_linked_list_node<T>* node)
{
    node = first(node);

    while (node)
    {
        double_linked_list_node<T>* temp = node;
        node = next(node);
        delete temp->data;
        delete temp;
    }
}

template <class T>
double_linked_list_node<T>* make_double_linked_list_node(T* data)
{
    double_linked_list_node<T>* node = new double_linked_list_node<T>();
    node->data = data;
    return node;
}


#endif // KS_UTILITY_HH
