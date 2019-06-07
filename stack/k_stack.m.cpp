#include <array>
#include <iostream>

template <typename T, std::size_t N, std::size_t K>
class KStack
{
public:
    KStack();
    ~KStack();

    bool push(std::size_t sn, T t);
    bool pop(std::size_t sn);
    const T& front(std::size_t sn) const;
    bool is_full() const;
    bool is_empty(std::size_t sn) const;
    void print(std::size_t sn, std::ostream& out) const;

private:
    int _free;
    T* _array;
    std::array<int, K> _top;
    std::array<int, N> _next;
};


template <typename T, std::size_t N, std::size_t K>
KStack<T,N,K>::KStack(): _free(0), _array((T*)std::malloc(N*sizeof(T)))
{
    for (auto & i : _top) i = -1;

    for (int i = 0; i != N-1; ++i) 
    {
        _next[i] = i + 1;
    }
    _next[N-1] = -1;
}
template <typename T, std::size_t N, std::size_t K>
KStack<T,N,K>::~KStack()
{
    std::free(_array);
}

template <typename T, std::size_t N, std::size_t K>
bool KStack<T,N,K>::is_full() const
{
    return -1 == _free;
}

template <typename T, std::size_t N, std::size_t K>
bool KStack<T,N,K>::push(std::size_t sn, T t)
{
    if (is_full()) return false;

    auto index = _free;
    _free = _next[index];       // update head to the next in the free list
    _next[index] = _top[sn];    // update next for stack sn to point to the current top of stack sn
    _top[sn] = index;           // update the top of stack sn to index

    new (_array+index) T(std::move(t));
    return true; 
}

template <typename T, std::size_t N, std::size_t K>
bool KStack<T,N,K>::pop(std::size_t sn)
{
    if (is_empty(sn)) return false;
    
    auto index = _top[sn];      // get the index of top of the stack sn
    _top[sn] = _next[index];    // update top of the stack sn to its next item of the stack
    _next[index] = _free;       // link the free list to the current head
    _free = index;              // update the current head

    _array[index].~T();
    return true;
}


template <typename T, std::size_t N, std::size_t K>
const T& KStack<T,N,K>::front(std::size_t sn) const
{
    return _array[_top[sn]];
}

template <typename T, std::size_t N, std::size_t K>
bool KStack<T,N,K>::is_empty(std::size_t sn) const
{
    return _top[sn] == -1;
}

template <typename T, std::size_t N, std::size_t K>
void KStack<T,N,K>::print(std::size_t sn, std::ostream& out) const
{
    if (is_empty(sn)) 
    {
        out << "<empty>\n";
        return;
    }

    auto index = _top[sn];
    while (index != -1)
    {
        out << _array[index] << " ";
        index = _next[index];
    }
    out << '\n';
}

int main()
{
    KStack<int, 10, 3> stack;
    stack.push(1,10);
    stack.push(1,11);
    stack.push(1,12);

    stack.push(0,1);
    stack.push(0,2);
    stack.push(0,3);
    stack.push(0,4);

    stack.push(2,4);
    stack.push(2,3);
    stack.push(2,2);
    stack.push(2,1);
    stack.push(2,0);

    stack.print(0, std::cout);
    stack.print(1, std::cout);
    stack.print(2, std::cout);

    stack.pop(0);
    stack.pop(1);
    stack.pop(2);

    stack.print(0, std::cout);
    stack.print(1, std::cout);
    stack.print(2, std::cout);

}
