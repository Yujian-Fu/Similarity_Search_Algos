#ifndef FAISS_Heap_h
#define FAISS_Heap_h

#include <cstring>
#include <climits>
#include <stdint.h>


namespace faiss{
template<typename T_, typename TI_>
struct CMax;

template <typename T_, typename TI_>
struct CMin {
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
    heap_heapify<CMin<T, int64_t> > (k, bh_val, bh_ids, x, ids, k0);
}


template <typename T> inline
void maxheap_heapify (
    size_t k,
    T * bh_val,
    int64_t * bh_ids,
    const T * x = nullptr,
    const int64_t * ids = nullptr,
    size_t k0 = 0
)
{
    heap_heapify<CMax <T, int64_t> > (k, bh_val, bh_ids, x, ids, k0);
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
            if (C::cmp (bh_val[0], x[i])){

                heap_pop<C> (k, bh_val, bh_ids);
                heap_push<C> (k, bh_val, bh_ids, x[i], i);
            }
        }
}

template <typename T> inline
void minheap_addn (size_t k, T * bh_val, int64_t * bh_ids,
                    const T * x, const int64_t * ids, size_t n)
                    {
                        heap_addn<CMin<T, int64_t> > (k, bh_val, bh_ids, x, ids, n);
                    } 


template <typename T> inline
void maxheap_addn ( size_t k, T *bh_val, int64_t * bh_ids, 
                    const T * x, const int64_t * ids, size_t n)
                    {
                        heap_addn<CMax<T, int64_t> > (k, bh_val, bh_ids, x, ids, n);
                    }

template <typename C> inline
size_t heap_recorder (size_t k, typename C:: T * bh_val, typename C::TI * bh_ids)
{
    size_t i, ii;

    for (i = 0, ii = 0; i < k; i++){
        typename C:: T val = bh_val[0];
        typename C:: TI id = bh_ids[0];

        heap_pop <C> (k-i, bh_val, bh_ids);
        bh_val[k - ii -1] = val;
        bh_ids[k - ii - 1] = id;
        if(id != -1) ii ++;
    }

    size_t nel = ii;

    memmove (bh_val, bh_val+k-ii, ii*sizeof(*bh_val));
    memmove (bh_ids, bh_ids+k-ii, ii*sizeof(*bh_ids));

    for (; ii < k; ii++){
        bh_val[ii] = C::neutral();
        bh_ids[ii] = -1;
    }
    return nel;
}


template <typename T> inline
size_t minheap_recorder (size_t k, T * bh_val, int64_t * bh_ids)
{
    return heap_recorder < CMin < T, int64_t> > (k, bh_val, bh_ids);
}

template <typename T> inline
size_t maxheap_recorder (size_t k, T * bh_val, int64_t * bh_ids)
{
    return heap_recorder < CMax<T, int64_t> > (k, bh_val, bh_ids);
}


template <typename C>
struct  HeapArray
{
    typedef typename C:: TI TI;
    typedef typename C:: T T;

    size_t nh;
    size_t k;
    TI * ids;
    T * val;

    T * get_val (size_t key) {return val + key * k;}

    TI * get_ids (size_t key) {return ids + key * k;}

    void heapify ();

    void addn (size_t nj, const T * vin, tTI j0 = 0, size_t i0 = 0, int64_t ni = -1);

    void addn_with_ids (
        size_t nj, const T * vin, const TI * id_in = nullptr,
        int64_t id_stride = 0, size_t i0 = 0, int64_t ni = -1
    );

    void recorder();

    void per_line_extrema (T * vals_out, TI * idx_out) const;

};

typedef HeapArray<CMin <float, int64_t> > float_minheap_array_t;
typedef HeapArray<CMin<int, int64_t> > int_minheap_array_t;

typedef HeapArray<CMax<float, int64_t>> float_maxheap_array_t;
typedef HeapArray<CMax<int, int64_t> > int_maxheap_array_t;


template <class C>
inline 
void indirect_heap_pop(
    size_t k, const typename C:: T * bh_val,
    typename C::TI * bh_ids
)
{
    bh_ids -- ;
    typename C:: T val = bh_val[bh_ids[k]];
    size_t i = 1;
    while(1) {
        size_t i1 = i << 1;
        size_t i2 = i1 + 1;
        if (i1 > k)
            break;
        typename C:: TI id1 = bh_ids[i1], id2 = bh_ids[i2];
        if (i2 == k+1 || C::cmp(bh_val[id1], bh_val[id2])){
            if (C::cmp(val, bh_val[id1]))
                break;
            bh_ids[i] = id1;
            i = i1;
        } else{
            if (C::cmp(val, bh_val[id2]))
                break;
            bh_ids[i] = id2;
            i = i2;
        }
    }
    bh_ids[i] = bh_ids[k];
}

template <class C>
inline 
void indirect_heap_push (size_t k, const typename C::T * bh_val,
                        typename C::TI * bh_ids, typename C:: TI id)
{
    bh_ids --;
    typename C::T val = bh_val[id];
    size_t i = k;
    while (i > 1){
        size_t i_father = i >> 1;
        if (!C::cmp (val, bh_val[bh_ids[i_father]]))
            break;
        bh_ids[i] = bh_ids[i_father];
        i = i_father;
    }
    bh_ids[i] = id;
}

}
#endif