
#include "IndexFlat.h"




namespace faiss
{

IndexFlat::IndexFlat (idx_type d, MetricType metric):
            Index(d, metric)
{
}

void IndexFlat::add (idx_type n, const float *x){
    xb.insert(xb.end(), x, x + n*d);
    ntotal += n;
}

void IndexFlat::reset(){
    xb.clear();
    ntotal = 0;
}

void IndexFlat::search (idx_type n, const float*x, idx_type k,
                    float * distances, idx_type *labels) const
{
    if (metric_type == METRIC_INNER_PRODUCT){
    }
}




};