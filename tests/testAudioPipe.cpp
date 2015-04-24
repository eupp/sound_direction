#define BOOST_TEST_DYN_LINK

#include <iterator>
#include <algorithm>
#include <array>

#include <boost/test/unit_test.hpp>
#include <boost/generator_iterator.hpp>

#include "trikSound/types.h"
#include "trikSound/audioFilter.h"
#include "trikSound/audioPipe.h"

using namespace std;
using namespace boost;
using namespace trikSound;

template <typename Iter>
class FillFilter: public AudioFilter<Iter>
{
public:

    typedef typename iterator_traits<Iter>::value_type value_type;

    FillFilter(const value_type& val = 0):
        m_val(val)
    {}

private:

    void handleWindowImpl(Iter first, Iter last) override
    {
        fill(first, last, m_val);
    }

    value_type m_val;
};

template <typename Iter>
class PlusFilter: public AudioFilter<Iter>
{
public:

private:

    void handleWindowImpl(Iter first, Iter last) override
    {
        typedef typename iterator_traits<Iter>::value_type value_type;
        transform(first, last, first, [](value_type x) {return x+1; });
    }
};

class AudioPipeFixture
{
public:

    static const size_t size = 8;
    static const sample_type val = 1;

    AudioPipeFixture():
        filter1(std::make_shared<FillFilter<Iter>>(val))
      , filter2(std::make_shared<PlusFilter<Iter>>())
    {
        fill(cont.begin(), cont.end(), 0);
    }

    typedef array<sample_type, size> Container;
    typedef typename Container::iterator Iter;

protected:

    Container cont;
    AudioFilter<Iter>::FilterPtr filter1;
    AudioFilter<Iter>::FilterPtr filter2;
    AudioPipe<Iter> pipe;

};

const sample_type AudioPipeFixture::val;

BOOST_TEST_DONT_PRINT_LOG_VALUE(typename AudioPipe<AudioPipeFixture::Iter>::Iterator)

BOOST_FIXTURE_TEST_SUITE(testAudioPipe, AudioPipeFixture)

BOOST_AUTO_TEST_CASE(testInsertFilter)
{
    auto iter = pipe.insertFilter(pipe.end(), filter2);
    iter = pipe.insertFilter(iter, filter1);

    BOOST_CHECK_EQUAL(*iter, filter1);
    ++iter;
    BOOST_CHECK_EQUAL(*iter, filter2);
    ++iter;
    BOOST_CHECK_EQUAL(iter, pipe.end());
}

BOOST_AUTO_TEST_CASE(testHandleWindow)
{
    pipe.insertFilter(pipe.end(), filter1);
    pipe.handleWindow(cont.begin(), cont.end());

    Container cont1;
    fill(cont1.begin(), cont1.end(), val);
    BOOST_CHECK_EQUAL_COLLECTIONS(cont.begin(), cont.end(), cont1.begin(), cont1.end());

    pipe.insertFilter(pipe.end(), filter2);
    pipe.handleWindow(cont.begin(), cont.end());
    Container cont2;
    fill(cont2.begin(), cont2.end(), val + 1);
    BOOST_CHECK_EQUAL_COLLECTIONS(cont.begin(), cont.end(), cont2.begin(), cont2.end());
}

BOOST_AUTO_TEST_SUITE_END()
