#include <iostream>
#include <stdexcept>
template <typename T>
class Stack
{
    T *array = nullptr;
    unsigned int m_size = 0;
    unsigned int n_capacity = 0;

    void increase_capacity()
    {
        T *new_array = nullptr;
        unsigned int new_capacity = (n_capacity == 0) ? 10 : 2 * n_capacity;
        unsigned int old_size = this->m_size;
        new_array = static_cast<T *>(std::malloc(new_capacity * sizeof(T)));
        if (new_array == nullptr)
        {
            throw std::bad_alloc();
        }
        unsigned int new_size = 0;
        try
        {
            for (int i = 0; i < old_size; i++)
            {
                new (&(new_array[i])) T(std::move(this->array[i]));
                ++new_size;
            }
        }
        catch (...)
        {
            for (unsigned int i = 0; i < new_size; i++)
            {
                new_array[i].~T();
            }
            std::free(new_array);
            throw;
        }
        this->clear();
        this->array = new_array;
        this->m_size = new_size;
        this->n_capacity = new_capacity;
    }
    void swap(Stack &other)
    {
        T *temp_array = this->array;
        unsigned int temp_size = this->m_size;
        unsigned int temp_capacity = this->n_capacity;

        this->array = other.array;
        this->m_size = other.m_size;
        this->n_capacity = other.n_capacity;

        other.array = temp_array;
        other.m_size = temp_size;
        other.n_capacity = temp_capacity;
    }

public:
    Stack() {}
    Stack(const Stack &other)
    {
        if (other.n_capacity == 0)
        {
            return;
        }

        Stack<T> temporary;
        temporary.n_capacity = other.n_capacity;
        temporary.array = static_cast<T *>(std::malloc(other.n_capacity * sizeof(T)));
        if (temporary.array == nullptr)
        {
            throw std::bad_alloc();
        }
        for (int i = 0; i < other.m_size; i++)
        {
            new (&(temporary.array[i])) T(other.array[i]);
            ++temporary.m_size;
        }
        swap(temporary);
    }
    Stack(Stack &&other)
    {
        swap(other);
    }

    Stack &operator=(const Stack &other)
    {
        if (this != &other)
        {
            Stack temporary(other);
            swap(temporary);
        }
        return *this;
    }
    Stack &operator=(Stack &&other)
    {
        if (this != &other)
        {
            Stack temporary(other);
            swap(temporary);
        }
        return *this;
    }

    bool is_in_stack(const T &value) const
    {
        if (is_empty())
        {
            return false;
        }
        for (int i = 0; i < m_size; i++)
        {
            if (array[i] == value)
            {
                return true;
            }
            return false;
        }
    }
    bool is_empty() const
    {
        return m_size == 0;
    }
    unsigned int size() const
    {
        return m_size;
    }

    void pop()
    {
        if (is_empty())
        {
            throw std::out_of_range("Stack is empty.");
        }

        array[m_size - 1].~T();
        --m_size;
    }
    void push(const T &value)
    {
        T temporary = value;
        if (m_size == n_capacity)
        {
            increase_capacity();
        }
        new (&(array[m_size])) T(temporary);
        ++m_size;
    }
    void push(T &&value)
    {
        T temporary = std::move(value);
        if (m_size == n_capacity)
        {
            increase_capacity();
        }
        new (&(array[m_size])) T(std::move(temporary));
        ++m_size;
    }

    const T &top() const
    {
        if (is_empty())
        {
            throw std::out_of_range("Stack is empty.");
        }
        return array[m_size - 1];
    }
    T &top()
    {
        if (is_empty())
        {
            throw std::out_of_range("Stack is empty.");
        }
        return array[m_size - 1];
    }
    void clear()
    {
        if (n_capacity == 0)
        {
            return; // no need to do sth else
        }
        for (int i = 0; i < m_size; i++)
        {
            array[i].~T();
        }
        std::free(array);
        array = nullptr;

        m_size = 0;
        n_capacity = 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const Stack &stack)
    {
        if (stack.is_empty())
        {
            os << "[]";
            return os;
        }
        os << "[";
        for (int i = 0; i < stack.m_size; i++)
        {
            os << stack.array[i];
            if (i < stack.m_size - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
    ~Stack()
    {
        clear();
    }
};