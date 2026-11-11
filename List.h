#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class List
{
private:
    struct Node
    {
        T info;
        Node *next;

        // створюємо конструктор: Node()
        Node(const T &value, Node *n = nullptr) : info(value), next(n) {} // конструктор через список ініціалізації(створює обʼєкт одразу з наданим значенням)
    };
    Node *head;
    Node *tail; // to simiplify addition in the end of the list
    int m_size;

public:
    class Iterator
    {
    private:
        Node *current; // це просто тимчасовий обʼєкт на стеці

    public:
        Iterator(Node *node) : current(node) {} // ми не створюємо контейнер, а лише новий покажчик на стеці

        T &operator*() //  повертає значення елементу списку, тобто ми зе й можемо його модифікувати, тому що ми повертаємо посилання на нього
        {
            return current->info;
        }
        const T &operator*() const //  повертає значення елементу списку, тобто ми зе й можемо його модифікувати, тому що ми повертаємо посилання на нього
        {
            return current->info;
        }
        Iterator &operator++() // перезаписуємо в ту ж комірку памʼяті, нове значення current, тобто ми використовуємо 8 байтів
        // незважаючи скільки ми ітераторів не створили, значення єдиного, що ми свторили просто затирається і перезаписується новим
        {
            current = current->next; // ми змінили на що вказує current
            return *this;            // розіменовуємо this, який містить в собі лише адресу Iterator, тобто ми повертаємо сам Iterator, як обʼєкт
        } // через те, що ми повертаємо  Iterator &, ми не копіюємо, а передаємо той самий обʼєкт на якого вже була виділена памʼять
        bool operator!=(const Iterator &other) const
        {
            return this->current != other.current; // якщо вони вказують на різні адреси повертається тру, потрібно для перевірки кінця списку
        }
    };
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    class Const_iterator
    {
    private:
        const Node *current;

    public:
        Const_iterator(const Node *node) : current(node) {}
        Const_iterator &operator++()
        {
            this->current = this->current->next;
            return *this;
        }
        const T &operator*() const
        {
            return this->current->info;
        }
        bool operator!=(const Const_iterator &other) const
        {
            return this->current != other.current;
        }
    };
    Const_iterator begin() const
    {
        return Const_iterator(head);
    } // цей ітератор робить всю роботу
    Const_iterator end() const
    {
        return Const_iterator(nullptr);
    } // його ми використовуємо для того, щоб знати коли зупинитись(поки не дійдемо до кінця списку)

    List() : head(nullptr), tail(nullptr), m_size(0) {} // конструюємо пустий список
    List(const List &other) : head(nullptr), tail(nullptr), m_size(0)
    {
        for (const auto &element : other)
        {
            this->push_back(element);
        }
    }
    List(List &&other) noexcept : head(nullptr), tail(nullptr), m_size(0) // move-constructor: "краде" посилання на голову і розмір у іншого списка, після чого викликається deconstructor ~List()
    {
        std::swap(this->head, other.head);
        std::swap(this->tail, other.tail);
        std::swap(this->m_size, other.m_size);

        // List<int> garbage_list;
        // List<int> r_value_garbage_list(static_cast <List<int>&&>(garbage_list));
    }
    template <typename U>                                                // castinmg constructor, i.e. from U to T of our list
    List(const List<U> &other) : head(nullptr), tail(nullptr), m_size(0) // if I need to use static_cast<>(), I need to write a specific constructor fot this purpose     // we're using an initializr list to construct the basic
    {
        // typename List<U>::Iterator it = other.begin(); //without 'typename' it won't be working!
        // typename List<U>::Iterator it_end = other.end(); //typename tells compiler that it should assume that what follows it is a type and not a variable

        // //However we can also use auto, since the compiler knows what type of a variable it is since C++ :
        // // auto it = other.begin;
        // // auto it_end = other.end(); //the compiler will know the data type of it_end by looking at the returning type of end()

        // while (it != it_end)
        // {
        //     this->push_back(static_cast<T>(*it));
        //     ++it;
        // }

        // Overall, this one is way more better to use:
        for (const auto &element : other)
        { // if other is List<int>, then element is const int&
            this->push_back(static_cast<T>(element));
        }
    }

    List &operator=(const List &other) // окей для l-value assignment, але не дуже ефективно для роботи з тимчасовими r-value objects: a = b + c;, where (b + c) - r-value
    {
        if (this != &other) // this holds the address
        {                   // whether they aren't pointing to one and the same address
            List copy_other(other);
            // міняємо місцями на що вони вказують
            std::swap(this->head, copy_other.head);
            std::swap(this->tail, copy_other.tail);
            std::swap(this->m_size, copy_other.m_size);
        }
        return *this;
    }
    List &operator=(List &&other) noexcept
    { // move-assignement for r-value assignment
        if (this != &other)
        {
            this->clear(); // deletion all memoty stored in this

            this->head = other.head;
            this->tail = other.tail;
            this->m_size = other.m_size;

            other.head = nullptr;
            other.tail = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    bool is_empty() const
    {
        if (head == nullptr)
        {
            return true;
        }
        return false;
    }
    bool contains(const T &value) const
    {
        if (is_empty())
        {
            return false;
        }

        for (const T &info : *this)
        {
            if (info == value)
            {
                return true;
            }
        }
        return false;
    }
    int get_size() const
    {
        return m_size;
    }
    T &front()
    {
        if (!is_empty())
        {
            return head->info;
        }
        throw std::out_of_range("List is empty!");
    }
    T &back()
    {
        if (!is_empty())
        {
            return tail->info;
        }
        throw std::out_of_range("List is empty!");
    }
    const T &front() const
    {
        if (!is_empty())
        {
            return head->info;
        }
        throw std::out_of_range("List is empty!");
    }
    const T &back() const
    {
        if (!is_empty())
        {
            return tail->info;
        }
        throw std::out_of_range("List is empty!");
    }
    void push_front(const T &info)
    {
        bool was_empty = is_empty();
        Node *new_node = new Node(info, head); // binding head->next  with new_node(via Node(), which is a constructor)
        // to clear this up:
        // when creating a new Node we're assigning to new_node->info = info; and new_node->next = head; -- now the new_node
        // is conected to the rest of the list, afterwards we're reeassigning head to new_node
        head = new_node;

        // now we need to know if it was the very first element of the list, because if so, tail must assign to the same address as head
        if (was_empty)
        {
            tail = head;
        }
        m_size++;
    }
    void push_back(const T &info)
    {
        if (is_empty())
        {
            head = new Node(info);
            tail = head;
        }
        else
        {
            Node *new_node = new Node(info);
            tail->next = new_node;
            tail = new_node;
        }
        m_size++;
    }

    template <typename Predicate>
    void remove_if(Predicate predicate) // erasing all nodes uising Predicate
    {
        if (is_empty())
        {
            return;
        }
        Node *prev_node = nullptr;
        Node *curr_node = head;
        while (curr_node != nullptr)
        {
            if (predicate(curr_node->info))
            {
                if (curr_node == head)
                {
                    if (head->next == nullptr)
                    {
                        Node *node_to_delete = curr_node;
                        head = nullptr;
                        tail = nullptr;
                        delete node_to_delete;
                        --m_size;

                        return;
                    }
                    else
                    {
                        Node *node_to_delete = curr_node;
                        head = curr_node->next;
                        delete node_to_delete;

                        curr_node = head;
                        --m_size;
                    }
                }
                else
                {
                    if (curr_node == tail)
                    {
                        tail = prev_node;
                    }

                    Node *node_to_delete = curr_node;
                    prev_node->next = curr_node->next;
                    curr_node = curr_node->next;

                    delete node_to_delete;

                    --m_size;
                }
            }
            else
            {
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
        }
    }
    void remove(const T &value) // erasing all nodes whose info is equal to value
    {
        remove_if(
            [&value](const T &element)
            {
                return element == value;
            });
    }

    void erase(const T &info) // finds the first node with a such info value
    {
        if (is_empty())
            return;             // a list is empty
        if (head->info == info) // if it's in the head's node
        {
            Node *node_to_delete = head;
            head = head->next; // even if there's no node after head, there'll be nullptr assigned to head(head->next == nullptr)
            delete node_to_delete;
            m_size--;

            if (head == nullptr)
            {
                tail = head;
            }
            return;
        }

        Node *prev_node = head;
        Node *curr_node = head->next;
        while (curr_node != nullptr)
        {
            if (curr_node->info == info)
            {
                Node *node_to_delete = curr_node;
                prev_node->next = curr_node->next;
                if (curr_node == tail)
                {
                    tail = prev_node;
                }
                delete node_to_delete;
                m_size--;
                return;
            }
            prev_node = curr_node;
            curr_node = curr_node->next;
        }
    }
    void clear()
    {
        Node *curr_node = head;
        while (curr_node != nullptr)
        {
            Node *node_to_delete = curr_node;
            curr_node = curr_node->next;
            delete node_to_delete;
        }
        head = nullptr;
        tail = nullptr;
        m_size = 0;
    }

    List operator+(const List<T> &other) const // в мене оператор додавання, оюʼєднує два списка, тобто в мене це конкатенація
    {
        List result(*this); // using counstructor to crearte the first part of a needed list

        for (const auto &element : other)
        {
            result.push_back(element); // adding the rest elements from OTHER array
        }
        // it's the same as:
        //  List<T>::Iterator it = other.begin();  // nopw it holds the head
        //  List<T>::Iterator it_end = other.end();

        // while(it != it_end){ //until we don't go to the end of a list
        //     result.push_back(*it);
        //     ++it;
        // }

        return result;
    }
    T &operator[](int index) // не дуже оптимізований, якщо використовувати в циклі: O(n^2)
    {
        if (index < 0 || index >= m_size)
        { // even if index is equal to the list m_size, it's out of the list's range, since its scope is [o, m_size - 1]
            throw std::out_of_range("Index out of range!");
        }

        Node *curr_node = head;
        for (int i = 0; i < index; i++)
        {
            curr_node = curr_node->next;
        }
        return curr_node->info;
    }
    const T &operator[](int index) const
    {
        if (index < 0 || index >= m_size)
        { // even if index is equal to the list m_size, it's out of the list's range, since its scope is [o, m_size - 1]
            throw std::out_of_range("Index out of range!");
        }

        Node *curr_node = head;
        for (int i = 0; i < index; i++)
        {
            curr_node = curr_node->next;
        }
        return curr_node->info;
    }
    bool operator==(const List<T> &other) const
    {
        if (this->m_size != other.m_size)
        { // if they don;t have the same number of nodes they can't be the same
            return false;
        }
        if (this == &other)
        { // if it's one and the same list, it's equal to itself
            return true;
        }
        typename List<T>::Const_iterator it = this->begin();
        for (const auto &other_it : other)
        {
            if (*it != other_it)
            {
                return false;
            }
            ++it;
        }
        return true;
    }
    bool operator!=(const List<T> &other) const
    {
        return !(*this == other);
    }
    friend std::ostream &operator<<(std::ostream &os, const List<T> &list) // ця ф-я не належить класу, але має жоступ до його лементів
    {
        if (list.is_empty())
        {
            os << "{}";
            return os;
        }
        os << "{";
        Node *curr_node = list.head;
        while (curr_node != nullptr)
        {
            os << curr_node->info;
            if (curr_node->next != nullptr)
            {
                os << ", ";
            }

            curr_node = curr_node->next;
        }
        os << "}";
        return os;
    }

    ~List()
    {
        clear();
    }
};
