
#include "IPrinter.h"

#include "ignis/executor/api/IReadIterator.h"
#include <forward_list>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


template<>
struct ignis::executor::core::io::IPrinterType<bool> {
    inline void operator()(std::ostream &out, const bool &b, int64_t level) { out << b; }
};

template<>
struct ignis::executor::core::io::IPrinterType<int8_t> {
    inline void operator()(std::ostream &out, const int8_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint8_t> {
    inline void operator()(std::ostream &out, const uint8_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<int16_t> {
    inline void operator()(std::ostream &out, const int16_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint16_t> {
    inline void operator()(std::ostream &out, const uint16_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<int32_t> {
    inline void operator()(std::ostream &out, const int32_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint32_t> {
    inline void operator()(std::ostream &out, const uint32_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<int64_t> {
    inline void operator()(std::ostream &out, const int64_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint64_t> {
    inline void operator()(std::ostream &out, const uint64_t &i, int64_t level) { out << i; }
};

template<>
struct ignis::executor::core::io::IPrinterType<float> {
    inline void operator()(std::ostream &out, const float &d, int64_t level) { out << d; }
};

template<>
struct ignis::executor::core::io::IPrinterType<double> {
    inline void operator()(std::ostream &out, const double &d, int64_t level) { out << d; }
};

template<>
struct ignis::executor::core::io::IPrinterType<std::string> {
    inline void operator()(std::ostream &out, const std::string &s, int64_t level) { out << s; }
};


template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::vector<_Tp, _Alloc>> {
    inline void operator()(std::ostream &out, const std::vector<_Tp, _Alloc> &v, int64_t level) {
        auto begin = v.begin();
        auto end = v.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<_Tp>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::list<_Tp, _Alloc>> {
    inline void operator()(std::ostream &out, const std::list<_Tp, _Alloc> &l, int64_t level) {
        auto begin = l.begin();
        auto end = l.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<_Tp>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::forward_list<_Tp, _Alloc>> {
    inline void operator()(std::ostream &out, const std::forward_list<_Tp, _Alloc> &fl, int64_t level) {
        auto begin = fl.begin();
        auto end = fl.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<_Tp>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::set<_Key, _Compare, _Alloc>> {
    inline void operator()(std::ostream &out, const std::set<_Key, _Compare, _Alloc> &s, int64_t level) {
        auto begin = s.begin();
        auto end = s.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<_Key>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline void operator()(std::ostream &out, const std::unordered_set<_Value> &us, int64_t level) {
        auto begin = us.begin();
        auto end = us.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<_Value>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline void operator()(std::ostream &out, const std::map<_Key, _Tp, _Compare, _Alloc> &m, int64_t level) {
        auto begin = m.begin();
        auto end = m.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<std::pair<_Key, _Tp>>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline void operator()(std::ostream &out, const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um,
                           int64_t level) {
        auto begin = um.begin();
        auto end = um.end();
        auto tabs = tab(level);

        while (begin != end) {
            out << tabs;
            IPrinterType<std::pair<_Key, _Tp>>()(out, *begin, level + 1);
            out << '\n';
            begin++;
        }
    }
};


template<typename _T1, typename _T2>
struct ignis::executor::core::io::IPrinterType<std::pair<_T1, _T2>> {
    inline void operator()(std::ostream &out, const std::pair<_T1, _T2> &p, int64_t level) {
        out << tab(level) << "(";
        IPrinterType<_T1>()(out, p.first, level);
        out << ", ";
        IPrinterType<_T2>()(out, p.second, level);
        out << ")";
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IPrinterType<_Tp *> {
    inline void operator()(std::ostream &out, const _Tp *v, int64_t level) { IPrinterType<_Tp>()(out, *v, level); }
};

template<typename _Tp>
struct ignis::executor::core::io::IPrinterType<std::shared_ptr<_Tp>> {
    inline void operator()(std::ostream &out, const std::shared_ptr<_Tp> &v, int64_t level) {
        IPrinterType<_Tp>()(out, *v, level);
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IPrinterType<ignis::executor::api::IReadIterator<_Tp>> {
    inline void operator()(std::ostream &out, const ignis::executor::api::IReadIterator<_Tp> &it, int64_t level) {
        auto &noconst_it = const_cast<ignis::executor::api::IReadIterator<_Tp> &>(it);
        auto tabs = tab(level);

        while (noconst_it.hasNext()) {
            out << tabs;
            IPrinterType<_Tp>()(out, noconst_it.next(), level + 1);
            out << '\n';
        }
    }
};
