#include <vector>
#include <string.h>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

long cumulative_sum(long x)
{
    return (x * (x + 1)) / 2;
}

long poscalc(long setsize, long y)
{
    return cumulative_sum(setsize) - cumulative_sum(setsize - y);
}

long Yaccess(long *Y, long setsize, long y, long x)
{
    return Y[poscalc(setsize, y) + x];
}

void Ywrite(long *Y, long setsize, long y, long x, long val)
{
    Y[poscalc(setsize, y) + x] = val;
}

void computeY(long *Y, long *set, long setsize)
{
    memcpy(Y, set, setsize * sizeof(long));
    for (long i = 1; i < setsize; ++i)
    {
        for (long j = 0; j < setsize - i; ++j)
        {
            long sum = Yaccess(Y, setsize, i - 1, j);
            sum += Yaccess(Y, setsize, 0, j + 1);
            Ywrite(Y, setsize, i, j, sum);
        }
    }
}

bool out_of_bounds(long sum, long *Y, long setsize, long subsetsize, long depth, long card, long target)
{
    long row = subsetsize - card - 1;
    long lower_bound = sum + Yaccess(Y, setsize, row, depth);
    long upper_bound = sum + Yaccess(Y, setsize, row, setsize - row - 1);
    return target < lower_bound || target > lower_bound;
}

void subsum(long *set, long setsize, long *subset, long subsetsize, long target)
{
    long Ysize = cumulative_sum(setsize);
    long *Y = new long[Ysize];
    computeY(Y, set, setsize);

    long leaves = 0;
    dynamic_bitset<> status(setsize);
    long depth = 0;
    long sum = 0;
    long card = 0;

    status[0] = true;
    bool go_left = true;

    while (1)
    {
        status <<= 1;
        if (go_left)
        {
            ++depth;
        }
        else
        {
            sum += set[depth];
            ++depth;
            ++card;
            status[0] = true;
        }

        if (sum == target & card == subsetsize)
        {
        }
        else if (card == subsetsize || out_of_bounds(sum, Y, setsize, subsetsize, depth, card, target))
        {
        }
        else
        {
        }
    }
}