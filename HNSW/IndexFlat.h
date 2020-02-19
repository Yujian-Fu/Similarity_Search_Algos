
#ifndef FAISS_INDEX_IVF_H
#define FAISS_INDEX_IVF_H

#include <vector>


#include <Index.h>


namespace faiss{


struct IndexFlat: Index{
    std::vector<float> xb;

    explicit IndexFlat (idx_type d, MetricType metric = METRIC_L2);

    void add(idx_type, const float* x) override;

    void reset() override;

    void search(
        idx_type n,
        const float* x,
        idx_type k,
        float* distances,
        idx_type* labels
    ) const override;

   void range_search(
       idx_type n,
       const float* x,
       float radius,
       RangeSearchResult* result) const override;
   
    void reconstruct(idx_type key, float* recons) const override;

    void compute_distance_subset(
        idx_type n,
        const float* x,
        idx_type k,
        float *distances,
        const idx_type *labels 
    )const;

    size_t remove_ids(const IDSelector & sel) override;

    size_t sa_code_size() const override;

    void sa_encode (idx_type n, const float *x, 
                    uint8_t *bytes) const override;

    void sa_decode (idx_type n, const uint8_t *bytes,
                    float *x) const override;

};


struct IndexFlatIP:IndexFlat {
    explicit IndexFlatIP (idx_type d): IndexFlat (d, METRIC_INNER_PRODUCT) {}
};

struct IndexFlatL2:IndexFlat{
    explicit IndexFlatL2 (idx_type n): IndexFlat (d, METRIC_L2) {}
};

struct IndexFlatL2BaseShift: IndexFlatL2{
    std::vector<float> shift;

    IndexFlatL2BaseShift (idx_type, size_t nshift, const float *shift);

    void search(
        idx_type n,
        const float *x,
        idx_type k,
        float* distances,
        idx_type* labels
    ) const override;
};

struct IndexRefineFlat: Index{

    IndexFlat refine_index;

    Index *base_index;
    bool own_fields;

    float k_factor;
    explicit IndexRefineFlat (Index *base_index);

    IndexRefineFlat ();

    void train(idx_type n, const float *x) override;

    void add(idx_type n, const float *x) override;

    void reset() override;

    void search(
        idx_type n,
        const float * x,
        idx_type k,
        float* distances,
        idx_type* labels) const override;

    ~IndexRefineFlat() override;

};

struct  IndexFlat1D:IndexFlatL2
{
    bool continuous_update;

    std::vector<idx_type> perm;

    explicit IndexFlat1D (bool continuous_update = true);

    void update_permutation();

    void add(idx_type, const float* x) override;

    void reset() override;

    void search(
        idx_type n,
        const float* x,
        idx_type k,
        float* distances,
        idx_type* labels
    )const override;
};

}


#endif