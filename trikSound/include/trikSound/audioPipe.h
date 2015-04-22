#pragma once

#include <list>

#include <boost/iterator/iterator_adaptor.hpp>

#include "audioFilter.h"
#include "stereoAudioFilter.h"

namespace trikSound
{

template <typename T, typename Filter>
class TRIKSOUNDSHARED_EXPORT AudioPipeBase
{
public:

    typedef typename Filter::FilterPtr FilterPtr;

    template <typename BaseIter>
    class IteratorHelper: public boost::iterator_adaptor<
              IteratorHelper<BaseIter>
            , BaseIter
            , boost::use_default
            , boost::bidirectional_traversal_tag
            >
    {
    public:

        IteratorHelper():
            IteratorHelper::iterator_adaptor_()
        {}

        IteratorHelper(const IteratorHelper& other):
            IteratorHelper::iterator_adaptor_(other.base_reference())
        {}

    private:

        friend class boost::iterator_core_access;
        friend class AudioPipeBase;

        IteratorHelper(const BaseIter& iter):
            IteratorHelper::iterator_adaptor_(iter)
        {}
    };

    typedef IteratorHelper<typename std::list<FilterPtr>::iterator> Iterator;
    typedef IteratorHelper<typename std::list<FilterPtr>::const_iterator> ConstIterator;

    void handleWindow(T windowSpec1, T windowSpec2);

    Iterator insertFilter(Iterator pos, const FilterPtr& filter);
    Iterator insertFilter(ConstIterator pos, const FilterPtr& filter);

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

private:

    std::list<FilterPtr> m_filters;

};

template <typename Iter>
using AudioPipe = AudioPipeBase<Iter, AudioFilter<Iter>>;

template <typename Iter>
using StereoAudioPipe = AudioPipeBase<typename StereoAudioFilter<Iter>::range_type, StereoAudioFilter<Iter>>;


template <typename T, typename Filter>
void AudioPipeBase<T, Filter>::handleWindow(T windowSpec1, T windowSpec2)
{
    for (auto& filt: m_filters) {
        filt->handleWindow(windowSpec1, windowSpec2);
    }
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::Iterator AudioPipeBase<T, Filter>::insertFilter(Iterator pos, const FilterPtr& filter)
{
    return Iterator(m_filters.insert(pos.base(), filter));
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::Iterator AudioPipeBase<T, Filter>::insertFilter(ConstIterator pos, const FilterPtr& filter)
{
    return Iterator(m_filters.insert(pos.base(), filter));
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::Iterator AudioPipeBase<T, Filter>::begin()
{
    return Iterator(m_filters.begin());
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::Iterator AudioPipeBase<T, Filter>::end()
{
    return Iterator(m_filters.end());
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::ConstIterator AudioPipeBase<T, Filter>::begin() const
{
    return Iterator(m_filters.begin());
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::ConstIterator AudioPipeBase<T, Filter>::end() const
{
    return Iterator(m_filters.end());
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::ConstIterator AudioPipeBase<T, Filter>::cbegin() const
{
    return Iterator(m_filters.cbegin());
}

template <typename T, typename Filter>
typename AudioPipeBase<T, Filter>::ConstIterator AudioPipeBase<T, Filter>::cend() const
{
    return Iterator(m_filters.cend());
}


}
