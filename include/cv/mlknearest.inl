
#include "_ml.h"

/****************************************************************************************\
*                          K-Nearest Neighbors Classifier                                *
\****************************************************************************************/

// k Nearest Neighbors
CvKNearest::CvKNearest()
{
    samples = 0;
    clear();
}


CvKNearest::~CvKNearest()
{
    clear();
}


CvKNearest::CvKNearest(const img_t* _train_data, const img_t* _responses,
                        const img_t* _sample_idx, bool _is_regression, int _max_k)
{
    samples = 0;
    train(_train_data, _responses, _sample_idx, _is_regression, _max_k, false);
}


void CvKNearest::clear()
{
    while(samples)
    {
        CvVectors* next_samples = samples->next;
        cFree(&samples->tt.fl);
        cFree(&samples);
        samples = next_samples;
    }
    var_count = 0;
    total = 0;
    max_k = 0;
}


int CvKNearest::get_max_k() const { return max_k; }

int CvKNearest::get_var_count() const { return var_count; }

bool CvKNearest::is_regression() const { return regression; }

int CvKNearest::get_sample_count() const { return total; }

bool CvKNearest::train(const img_t* _train_data, const img_t* _responses,
                        const img_t* _sample_idx, bool _is_regression,
                        int _max_k, bool _update_base)
{
    bool ok = false;
    img_t* responses = 0;

    CC_FUNCNAME("CvKNearest::train");

    __BEGIN__;

    CvVectors* _samples;
    float** _data;
    int _count, _dims, _dims_all, _rsize;
    
    if(!_update_base)
        clear();

    // Prepare training data and related parameters.
    // Treat categorical responses as ordered - to prevent class label compression and
    // to enable entering new classes in the updates
    CC_CALL(cvPrepareTrainData("CvKNearest::train", _train_data, CC_ROW_SAMPLE,
        _responses, CC_VAR_ORDERED, 0, _sample_idx, true, (const float***)&_data,
        &_count, &_dims, &_dims_all, &responses, 0, 0));

    if(_update_base && _dims != var_count)
        CC_ERROR(CC_StsBadArg, "The newly added data have different dimensionality");

    if(!_update_base)
    {
        if(_max_k < 1)
            CC_ERROR(CC_StsOutOfRange, "max_k must be a positive number");
        
        regression = _is_regression;
        var_count = _dims;
        max_k = _max_k;
    }

    _rsize = _count*sizeof(float);
    CC_CALL(_samples = (CvVectors*)cAlloc(sizeof(*_samples) + _rsize));
    _samples->next = samples;
    _samples->tid = CC_32F;
    _samples->tt.fl = _data;
    _samples->count = _count;
    total += _count;

    samples = _samples;
    memcpy(_samples + 1, responses->tt.fl, _rsize);

    ok = true;

    __END__;

    return ok;
}



void CvKNearest::find_neighbors_direct(const img_t* _samples, int k, int start, int end,
                    float* neighbor_responses, const float** neighbors, float* dist) const
{
    int i, j, count = end - start, k1 = 0, k2 = 0, d = var_count;
    CvVectors* s = samples;

    for(; s != 0; s = s->next)
    {
        int n = s->count;
        for(j = 0; j < n; j++)
        {
            for(i = 0; i < count; i++)
            {
                double sum = 0;
                suf32_t si;
                const float* v = s->tt.fl[j];
                const float* u = (float*)(_samples->tt.data + _samples->step*(start + i));
                suf32_t* dd = (suf32_t*)(dist + i*k);
                float* nr;
                const float** nn;
                int t, ii, ii1;

                for(t = 0; t <= d - 4; t += 4)
                {
                    double t0 = u[t] - v[t], t1 = u[t+1] - v[t+1];
                    double t2 = u[t+2] - v[t+2], t3 = u[t+3] - v[t+3];
                    sum += t0*t0 + t1*t1 + t2*t2 + t3*t3;
                }

                for(; t < d; t++)
                {
                    double t0 = u[t] - v[t];
                    sum += t0*t0;
                }

                si.f = (float)sum;
                for(ii = k1-1; ii >= 0; ii--)
                    if(si.i > dd[ii].i)
                        break;
                if(ii >= k-1)
                    continue;

                nr = neighbor_responses + i*k;
                nn = neighbors ? neighbors + (start + i)*k : 0;
                for(ii1 = k2 - 1; ii1 > ii; ii1--)
                {
                    dd[ii1+1].i = dd[ii1].i;
                    nr[ii1+1] = nr[ii1];
                    if(nn) nn[ii1+1] = nn[ii1];
                }
                dd[ii+1].i = si.i;
                nr[ii+1] = ((float*)(s + 1))[j];
                if(nn)
                    nn[ii+1] = v;
            }
            k1 = MIN(k1+1, k);
            k2 = MIN(k1, k-1);
        }
    }
}


float CvKNearest::write_results(int k, int k1, int start, int end,
    const float* neighbor_responses, const float* dist,
    img_t* _results, img_t* _neighbor_responses,
    img_t* _dist, suf32_t* sort_buf) const
{
    float result = 0.f;
    int i, j, j1, count = end - start;
    double inv_scale = 1./k1;
    int rstep = _results && !CC_IS_MAT_CONT(_results->tid) ? _results->step/sizeof(result) : 1;

    for(i = 0; i < count; i++)
    {
        const suf32_t* nr = (const suf32_t*)(neighbor_responses + i*k);
        float* dst;
        float r;
        if(_results || start+i == 0)
        {
            if(regression)
            {
                double s = 0;
                for(j = 0; j < k1; j++)
                    s += nr[j].f;
                r = (float)(s*inv_scale);
            }
            else
            {
                int prev_start = 0, best_count = 0, cur_count;
                suf32_t best_val;
            
                for(j = 0; j < k1; j++)
                    sort_buf[j].i = nr[j].i;

                for(j = k1-1; j > 0; j--)
                {
                    bool swap_fl = false;
                    for(j1 = 0; j1 < j; j1++)
                        if(sort_buf[j1].i > sort_buf[j1+1].i)
                        {
                            int t;
                            CC_SWAP(sort_buf[j1].i, sort_buf[j1+1].i, t);
                            swap_fl = true;
                        }
                    if(!swap_fl)
                        break;
                }
            
                best_val.i = 0;
                for(j = 1; j <= k1; j++)
                    if(j == k1 || sort_buf[j].i != sort_buf[j-1].i)
                    {
                        cur_count = j - prev_start;
                        if(best_count < cur_count)
                        {
                            best_count = cur_count;
                            best_val.i = sort_buf[j-1].i;
                        }
                        prev_start = j;
                    }
                r = best_val.f;
            }
            
            if(start+i == 0)
                result = r;

            if(_results)
                _results->tt.fl[(start + i)*rstep] = r;
        }

        if(_neighbor_responses)
        {
            dst = (float*)(_neighbor_responses->tt.data +
                (start + i)*_neighbor_responses->step);
            for(j = 0; j < k1; j++)
                dst[j] = nr[j].f;
            for(; j < k; j++)
                dst[j] = 0.f;
        }

        if(_dist)
        {
            dst = (float*)(_dist->tt.data + (start + i)*_dist->step);
            for(j = 0; j < k1; j++)
                dst[j] = dist[j + i*k];
            for(; j < k; j++)
                dst[j] = 0.f;
        }
    }

    return result;
}
    


float CvKNearest::find_nearest(const img_t* _samples, int k, img_t* _results,
    const float** _neighbors, img_t* _neighbor_responses, img_t* _dist) const
{
    float result = 0.f;
    bool local_alloc = false;
    float* buf = 0;
    const int max_blk_count = 128, max_buf_sz = 1 << 12;

    CC_FUNCNAME("CvKNearest::find_nearest");

    __BEGIN__;

    int i, count, count_scale, blk_count0, blk_count = 0, buf_sz, k1;

    if(!samples)
        CC_ERROR(CC_StsError, "The search tree must be constructed first using train method");

    if(!CC_IS_MAT(_samples) ||
        CC_MAT_TYPE(_samples->tid) != CC_32F ||
        _samples->cols != var_count)
        CC_ERROR(CC_StsBadArg, "Input samples must be floating-point matrix (<num_samples>x<var_count>)");

    if(_results && (!CC_IS_MAT(_results) ||
        _results->cols != 1 && _results->rows != 1 ||
        _results->cols + _results->rows - 1 != _samples->rows))
        CC_ERROR(CC_StsBadArg,
        "The results must be 1d vector containing as much elements as the number of samples");

    if(_results && CC_MAT_TYPE(_results->tid) != CC_32F &&
        (CC_MAT_TYPE(_results->tid) != CC_32SC1 || regression))
        CC_ERROR(CC_StsUnsupportedFormat,
        "The results must be floating-point or integer (in case of classification) vector");

    if(k < 1 || k > max_k)
        CC_ERROR(CC_StsOutOfRange, "k must be within 1..max_k range");

    if(_neighbor_responses)
    {
        if(!CC_IS_MAT(_neighbor_responses) || CC_MAT_TYPE(_neighbor_responses->tid) != CC_32F ||
            _neighbor_responses->rows != _samples->rows || _neighbor_responses->cols != k)
            CC_ERROR(CC_StsBadArg,
            "The neighbor responses (if present) must be floating-point matrix of <num_samples> x <k> size");
    }

    if(_dist)
    {
        if(!CC_IS_MAT(_dist) || CC_MAT_TYPE(_dist->tid) != CC_32F ||
            _dist->rows != _samples->rows || _dist->cols != k)
            CC_ERROR(CC_StsBadArg,
            "The distances from the neighbors (if present) must be floating-point matrix of <num_samples> x <k> size");
    }

    count = _samples->rows;
    count_scale = k*2*sizeof(float);
    blk_count0 = MIN(count, max_blk_count);
    buf_sz = MIN(blk_count0 * count_scale, max_buf_sz);
    blk_count0 = MAX(buf_sz/count_scale, 1);
    blk_count0 += blk_count0 % 2;
    blk_count0 = MIN(blk_count0, count);
    buf_sz = blk_count0 * count_scale + k*sizeof(float);
    k1 = get_sample_count();
    k1 = MIN(k1, k);

    if(buf_sz <= CC_MAX_LOCAL_SIZE)
    {
        buf = (float*)cvStackAlloc(buf_sz);
        local_alloc = true;
    }
    else
        CC_CALL(buf = (float*)cAlloc(buf_sz));

    for(i = 0; i < count; i += blk_count)
    {
        blk_count = MIN(count - i, blk_count0);
        float* neighbor_responses = buf;
        float* dist = buf + blk_count*k;
        suf32_t* sort_buf = (suf32_t*)(dist + blk_count*k);

        find_neighbors_direct(_samples, k, i, i + blk_count,
                    neighbor_responses, _neighbors, dist);

        float r = write_results(k, k1, i, i + blk_count, neighbor_responses, dist,
                                 _results, _neighbor_responses, _dist, sort_buf);
        if(i == 0)
            result = r;
    }

    __END__;

    if(!local_alloc)
        cFree(&buf);

    return result;
}

/* End of file */

