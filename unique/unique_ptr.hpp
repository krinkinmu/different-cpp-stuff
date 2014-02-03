template <typename T>
struct default_deleter
{
    void operator()(T * t) const
    { delete t; }
};

template <typename T>
struct array_deleter
{
    void operator()(T * t) const
    { delete[] t; }
};

template <typename T, typename D = default_deleter<T> >
struct unique_ptr_ref
{
    unique_ptr_ref(T * ptr, D const & d) : data(ptr), disposer(d) {}

    T * data;
    D disposer;
};

template <typename T, typename D = default_deleter<T> >
class unique_ptr
{
    public:
        unique_ptr(T * ptr = 0, D const & d = D())
            : data(ptr)
            , disposer(d)
        {}

        unique_ptr(unique_ptr & u)
            : data(u.release())
            , disposer(u.d)
        {}

        template <typename U, typename V>
        unique_ptr(unique_ptr<U, V> & u)
            : data(u.release())
            , disposer(u.d)
        {}

        unique_ptr & operator=(unique_ptr & u)
        {
            reset(u.release());
            disposer = u.disposer;
            return *this;
        }

        template <typename U, typename V>
        unique_ptr & operator=(unique_ptr<U, V> & u)
        {
            reset(u.release());
            disposer = u.disposer;
            return *this;
        }

        ~unique_ptr()
        { reset(NULL); }
        
        T & operator*() const
        { return *data; }

        T * operator->() const
        { return data; }

        T * release()
        {
            T * ptr = data;
            data = NULL;
            return ptr;
        }

        void reset(T * ptr)
        {
            if (data && data != ptr)
                disposer(data);
            data = ptr;
        }

        unique_ptr(unique_ptr_ref<T, D> const & ref)
            : data(ref.data)
            , disposer(ref.disposer)
        {}

        unique_ptr<T, D> & operator=(unique_ptr_ref<T, D> const & u)
        {
            reset(u.data);
            disposer = u.disposer;
            return *this;
        }

        operator const unique_ptr_ref<T, D>()
        { return unique_ptr_ref<T, D>(release(), disposer); }

        template <typename U, typename V>
        operator const unique_ptr_ref<U, V>()
        { return unique_ptr_ref<U, V>(release(), disposer); }

    private:
        T * data;
        D disposer;
};
