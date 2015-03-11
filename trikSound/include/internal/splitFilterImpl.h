#pragma once

#include <vector>
#include <iterator>
#include <utility>

#include <boost/shared_container_iterator.hpp>
#include <boost/shared_ptr.hpp>

#include "../trikSound/angleFilter.h"
#include "../trikSound/utils.h"

namespace trikSound {

template <typename Iter>
class SplitFilterImpl
{
public:

    SplitFilterImpl(const AudioFilter::ptrFilter& channelFilter = ptrFilter());

    AudioFilter::range_type handleWindowImpl(Iter first, Iter last);

private:
    AudioFilter::ptrFilter mChannelFilter;
};

template <typename Iter>
AudioFilter<Iter>::range_type SplitFilterImpl<Iter>::handleWindowImpl(Iter first, Iter last)
{
    typedef typename std::iterator_traits<Iter>::value_type value_type;

    const int size = std::distance(first, last);
    boost::shared_ptr<std::vector<value_type>> shared_vector(new std::vector(size));

    auto dst = shared_vector->begin();
    extractChannel<2, 0>(first, last, dst);
    std::advance(dst, size / 2);
    extractChannel<2, 1>(first, last, dst);

    auto filteredChl1 = mChannelFilter->handleWindowImpl(shared_vector->begin(),
                                                         shared_vector->begin() + size / 2);
    auto filteredChl2 = mChannelFilter->handleWindowImpl(shared_vector->begin() + size / 2,
                                                         shared_vector->end());

    return boost::make_shared_container_range(shared_vector);
}

}
