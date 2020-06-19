#include <array>

template <typename T, int cap, typename Container = std::array<T, cap>>
class RingBuffer
{
  public:
    using value_type = T;

    RingBuffer() 
        : head_(0), tail_(0), size_(0), current_(0), capacity_(cap)
    {}

    T& head() { return c_.at(head_); }
    T &tail() { return c_.at(tail_); }

    T const &head() const { return c_.at(head_); }
    T const &tail() const { return c_.at(tail_); }

    void push_back(T val) noexcept
    {
        if (current_ >= capacity_)
        {
            current_ = 0;
        }

        c_.at(current_++) = val;
        tail_ = current_ - 1;
        if (size_++ >= capacity_)
        {
            size_ = capacity_;
            ++head_;
            if (head_ >= capacity_)
            {
                head_ = 0;
            }
        }
    }

    void place_back(T val)
    {
        if (full())
        {
            throw std::overflow_error("place_back(): full buffer");
        }
        push_back(val);
    }

    void pop()
    {
         ++head_;
         if (head_ >= capacity_)
         {
             head_ = 0;
         }

         --size_;
    }

    std::size_t size() const noexcept {  return size_; }
    std::size_t capacity() const noexcept { return capacity_; }

    bool empty() const noexcept { return size_ <= 0; }
    bool full() const noexcept { return size_ >= capacity_; }

    /**
     * Create a ring buffer iterator
     */
    template<typename Buffer, typename Iterator>
    class RingBufferIterator 
    {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename Buffer::value_type;
        using difference_type = std::ptrdiff_t;
        using pointer = typename Buffer::value_type*;
        using reference = typename Buffer::value_type&;

        RingBufferIterator() : done_(true) {}

        RingBufferIterator(const Buffer& buf, Iterator begin) 
            : buf_(buf), begin_(begin), cursor_(begin), done_(false) {}

        RingBufferIterator(const Buffer& buf, Iterator begin, bool done) 
            : buf_(buf), begin_(begin), cursor_(begin), done_(done) {}

        reference operator*() const 
        {
            return *cursor_;
        }

        pointer operator->() const 
        {
            return cursor_;
        }

        // pre-increment
        RingBufferIterator& operator++()
        {
            ++cursor_;
            if (cursor_ == buf_.end())
            {
                cursor_ = const_cast<Iterator>(buf_.begin());
            }
            done_ = cursor_ == begin_;

            return *this;
        }

        // post-increment
        RingBufferIterator operator++(int)
        {
            iterator tmp(*this);
            ++cursor_;
            if (cursor_ == end(buf_))
            {
                cursor_ = const_cast<Iterator>(begin(buf_));
            }
            done_ = cursor_ == begin(buf_);

            return tmp;
        }

        bool operator==(const RingBufferIterator& it) const
        {
            if (done_ && it.done_)
            {
                return true;
            }
            else if (!done_ && !it.done_)
            {
                return (this->cursor_ == it.cursor_);
            }
            else
            {
                return false;
            }
        }

        bool operator!=(const RingBufferIterator& it) const
        {
            return !(*this == it);
        }

      private:
        const Buffer& buf_;
        const Iterator begin_;
        Iterator cursor_;
        bool done_;
    };

    typedef RingBufferIterator<Container, typename Container::iterator> iterator;

    iterator begin()
    {
        unsigned int offset = head_ % capacity_;
        return RingBuffer::iterator(c_, c_.begin() + offset);
    }

    iterator end()
    {
        unsigned int offset = tail_ + 1 % capacity_;
        return RingBuffer::iterator(c_, c_.begin() + offset, full());
    }

    friend std::ostream& operator<<(std::ostream& os, const RingBuffer& buf) {
        return (os 
            << "RingBuffer: {" << "\n"
            << "  head: " << buf.head_ << ",\n"
            << "  tail: " << buf.tail_ << ",\n"
            << "  current: " << buf.current_ << ",\n"
            << "  capacity: " << buf.capacity_ << ",\n"
            << "  size: " << buf.size() << "\n}");
    }

  private:
    Container c_;
    int head_;
    int tail_;
    int size_;
    int current_;
    int capacity_;
};