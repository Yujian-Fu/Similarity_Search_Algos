#include <cstring>
#include <climits>
#include <stdint.h>


namespace faiss{
template<typename T_, typename TI_>
struct CMax;

template <typename T_, typename TI_>
struct Cmin {
    typedef T_ T;
    typedef YI_ TI;
    typedef CMax < T_, TI_> Crev;
    inline static bool cmp (T a, T b){
        return a < b;
    }

    inline static T neutral (){
        return -std::numeric_limits<T>::max();
    }
};

template <typename T_, typename TI_>
struct CMax {
    typedef T_ T;
    typedef TI_ TI;
    typedef CMin < T_, TI_ > Crev;
    inline static bool cmp (T a, T b){
        return a > b;
    }
    inline static T neutral () {
        return std::numeric_limits<T>::max();
    }
};

template <class C> inline
void heap_pop (size_t k, typename C::T * bh_val, typename C::TI * bh_ids)
{
    bh_val --;
    bh_ids --;
    typename C::T val = bh_val[k];
    size_t i = 1, i1, i2;
    while(1){
        i1 = i << 1;
        i2 = i1 + 1;
        if (i1 >k)
            break;
        if (i2 == k+1 || C::cmp(bh_val[i1], bh_val[i2])){
            if (C::cmp(val, bh_val[i1]))
                break;
            bh_val[i] = bh_val[i1];
            bh_ids[i] = bh_ids[i1];
            i = i1;
        }

        else{
            if (C::cmp(val, bh_val[i2]))
                break
            bh_val[i] = bh_val[i2];
            bh_ids[i] = bh_ids[i2];
            i= i2;
        }
    }
    bh_val[i] = bh_val[k];
    bh_ids[i] = bh_ids[k];
}

template <class C> inline
void heap_push (size_t k,
                typename C::T * bh_val, typename C::TI * bh_ids,
                typename C::T val, typename C::TI ids)

{
    bh_val --;
    bh_ids --;
    size_t i = k, i_father;
    while (i > 1) {
        i_father = i >> 1;
        if (!C::cmp (val, bh_val[i_father]))
            break;
        bh_val[i] = bh_val[i_father];
        bh_ids[i] = bh_ids[i_father];
        i = i_father;
    }
    bh_val[i] = val;
    bh_ids[i] = ids;
}

template <typename T> inline
void minheap_pop (size_t k , T * bh_val, int64_t * bh_ids)
{
    heap_pop<Cmin<T, int64_t> > (k, bh_val, bh_ids);
}

template <typename T> inline
void minheap_push (size_t k, T * bh_val, int64_t * bh_ids, T val, int64_t ids)
{
    heap_push<Cmin<T, int64_t> > (k, bh_val, bh_ids, val, ids);
}

template <typename T> inline
void maxheap_pop (size_t k, T * bh_val, int64_t * bh_ids)
{
    heap_pop<CMax<T, int64_t> > (k, bh_val, bh_ids);
}

template < typename T> inline
void maxheap_push (size_t l, T * bh_val, int64_t * bh_ids, T val, int64_t ids)
{
    heap_push<CMax<T, int64_t> > (k, bh_val, bh_ids, val, ids);
}

template <class C> inline
void heap_heapify(
    size_t k,
    typename C::T * bh_val,
    typename C::T * bh_ids,
    const typename C::T *x = nullptr,
    const typename C::TI *ids = nullptr,
    size_t k0 = 0
)

{
    if (k0 > 0) assert x;

    if (ids){
        for (size_t i = 0; i < k0; i++)
            heap_push<C> (i+1, bh_val, bh_ids, x[i], ids[i]);
    }

    else{
        for (size_t i = 0; i < k0; i++)
            heap_push<C> (i+1, bh_val, bh_ids, x[i], i);
    }

    for (size_t i = k0; i < k; i++){
        bh_val[i] = C::neutral();
        bh_ids[i] = -1
    }
}

template <typename T> inline
void minheap_heapify(
    size_t k,
    T * bh_val,
    int64_t * bh_ids,
    const T * x = nullptr,
    const int64_t * ids = nullptr,
    size_t k0 = 0
)
{
    heap_heapify<CMax<T, int64_t> > (k, bh_val, bh_ids, x, ids, k0);
}

template <class C> inline
void heap_addn (size_t k,
                typename C::T * bh_val, 
                typename C::TI * bh_ids,
                const typename C::T *x,
                const typename C::TI * ids,
                size_t n)
{
    size_t i;
    if (ids)
        for (i =0; i< n; i++){
            if (C::cmp (bh_val))
        }
}



}