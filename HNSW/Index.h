#ifndef FAISS_INDEX_H
#define FAISS_INDEX_H


#include <typeinfo>
#include <cstdio>
#include <string>
#include <sstream>


namespace faiss{

enum MetricType {
    METRIC_INNER_PRODUCT = 0,
    METRIC_L2 = 1,
};

struct IDSelector;
struct RangeSearchResult;
struct DistanceComputer;

struct Index {
    using idx_type = int64_t;
    using component_type = float;
    using distance_type = float;

    int d;
    idx_type ntotal;
    bool verbose;

    bool is_trained;

    MetricType metric_type;
    float metric_arg;
    explicit Index (idx_type d = 0, MetricType metric = METRIC_L2):
            d(d),
            ntotal(0),
            is_trained(true),
            metric_type(metric),
            metric_arg(0) {}

    virtual ~Index();

    virtual void train(idx_type, const float * x);

    virtual void add(idx_type, const float *x) = 0;

    virtual void add_with_ids (idx_type n, const float * x, const idx_type *xids);

    virtual void search (idx_type n, const float *x, idx_type k,
                        float * distances, idx_type * labels) const = 0;
    

    virtual void range_search(idx_type n, const float *x, float radius, 
                            RangeSearchResult * result) const;
    
    void assign (idx_type n, const float *x, idx_type * labels, idx_type k = 1);

    virtual void reset() = 0;

    virtual size_t remove_ids (const IDSelector & sel);

    virtual void reconstruct (idx_type key, float * recons) const;

    virtual void reconstruct_n (idx_type i0, idx_type ni, float * recons) const;

    virtual void search_and_reconstruct(idx_type n, const float *x, idx_type k,
                                        float *distances, idx_type * labels,
                                        float *recons) const;


    virtual void comput_residual (const float * x,
                                    float * residual, idx_type key) const;

    virtual DistanceComputer * get_distance_computer() const;

    virtual size_t sa_code_size () const;

    virtual void sa_encode (idx_type n, const float *x,
                            uint8_t *bytes) const;

    virtual void sa_decode (idx_type, const uint8_t *bytes,
                            float *x) const;

};



}

#endif
