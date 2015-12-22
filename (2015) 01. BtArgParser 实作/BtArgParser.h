/*!
 * \file BtArgParser.h
 * \date 2015/12/22 16:33
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of BtArgParser 
*/

#pragma once

template<class Tuple, std::size_t N>
void BtAssignTupleElement(Tuple& t, const std::string& arg, bool tolerateErrors)
{
    using type = std::tuple_element<N, Tuple>::type;

    std::istringstream convert(arg);
    if (!(convert >> std::get<N>(t)))
    {
        if (tolerateErrors)
        {
            // it's possible to tolerate the error in this case, is because int/float could be constructed with '0', and std::string-streaming-to-itself would never fail.
            std::get<N>(t) = type(0);  
        }
        else
        {
            std::string errInfo = tfm::format("type cast of 'arg #%d' failed. (from: 'std::string', to: '%s')", N, typeid(type).name());
            throw std::bad_cast(errInfo.c_str());
        }
    }
}

template<class Tuple, std::size_t N>
struct BtTupleElemGetter
{
    static void fetch_elements(Tuple& t, const std::vector<std::string>& args, bool tolerateErrors)
    {
        BtTupleElemGetter<Tuple, N - 1>::fetch_elements(t, args, tolerateErrors);
        BtAssignTupleElement<Tuple, N - 1>(t, args[N - 1], tolerateErrors);
    }
};

template<class Tuple>
struct BtTupleElemGetter<Tuple, 1>
{
    static void fetch_elements(Tuple& t, const std::vector<std::string>& args, bool tolerateErrors)
    {
        BtAssignTupleElement<Tuple, 0>(t, args[0], tolerateErrors);
    }
};

class BtArgParser 
{
public: 
    BtArgParser(const std::string& args);
    ~BtArgParser() {}

    template<class... Args>
    std::tuple<Args...> parse_tuple()
    {
        std::tuple<Args...> ret;

        const int size = std::tuple_size<decltype(ret)>::value;
        if (size != m_args.size())
            throw std::logic_error(tfm::format("argument number mismatched. (%d expected, %d got)", size, m_args.size()));

        BtTupleElemGetter<decltype(ret), size>::fetch_elements(ret, m_args, false);
        return ret;
    }

    template<class... Args>
    bool parse_tuple_no_throw(std::tuple<Args...>* out_tuple)
    {
        BT_ASSERT_RET_VAL(out_tuple, "invalid output tuple object.", false);

        try
        {
            *out_tuple = parse_tuple<Args...>(); 
            return true;
        }
        catch (std::exception& e)
        {
            BT_ERROR("%s: failed to parse tuple object. (%s)", typeid(e).name(), e.what());
            return false;
        }
    }

    template<class... Args>
    std::tuple<Args...> parse_tuple_tolerated()
    {
        std::tuple<Args...> ret;
        const int size = std::tuple_size<std::tuple<Args...>>::value;
        if (size != m_args.size())
        {
            // two cases here:
            //  1. if 'm_args' has more elements, the redundant would be discarded
            //  2. if 'm_args' doesn't provide enough elements, resize() would complement empty strings to the tail
            // both cases would generate valid tuple finally, which makes this function tolerable against errors 
            m_args.resize(size);
        }
        BtTupleElemGetter<decltype(ret), size>::fetch_elements(ret, m_args, true);
        return ret;
    }

private:
    std::vector<std::string> m_args;
};

