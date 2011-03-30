#include <pd/rnd.hpp>
#include <pd/utils.hpp>
#include <mtrand.h>


pd::rnd::rnd()
{
    m_internal_rnd = new MTRand((uint32_t)pd::seconds_since_epoch());
}

pd::rnd::rnd(uint32_t seed)
{
    m_internal_rnd = new MTRand(seed);
}

pd::rnd::~rnd()
{
    delete (MTRand *)m_internal_rnd;
}

void pd::rnd::seed(uint32_t seed)
{
    ((MTRand_int32 *)m_internal_rnd)->seed(seed);
}

uint32_t pd::rnd::next()
{
    return (*(MTRand_int32 *)m_internal_rnd)();
}

double pd::rnd::next_double()
{
    return (double)(next()) * (1.0 / 4294967296.0);
}

float pd::rnd::next_float()
{
    return (float)next_double();
}
