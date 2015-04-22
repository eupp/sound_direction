#pragma once

#include <list>

#include <boost/iterator/iterator_adaptor.hpp>

#include "audioFilter.h"

namespace trikSound
{

template <typename Iter>
class TRIKSOUNDSHARED_EXPORT AudioPipe
{
public:

    typedef typename AudioFilter<Iter>::FilterPtr FilterPtr;

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
        friend class AudioPipe;

        IteratorHelper(const BaseIter& iter):
            IteratorHelper::iterator_adaptor_(iter)
        {}
    };

    typedef IteratorHelper<typename std::list<FilterPtr>::iterator> Iterator;
    typedef IteratorHelper<typename std::list<FilterPtr>::const_iterator> ConstIterator;

    void handleWindow(Iter first, Iter last);

    Iterator insertFilter(Iterator pos, const FilterPtr& filter) {}
    Iterator insertFilter(ConstIterator pos, const FilterPtr& filter) {}

    Iterator begin() {}
    Iterator end() {}
    ConstIterator begin() const {}
    ConstIterator end() const {}

private:

    std::list<FilterPtr> m_filters;

};

template <typename Iter>
void AudioPipe<Iter>::handleWindow(Iter first, Iter last)
{

}

}
