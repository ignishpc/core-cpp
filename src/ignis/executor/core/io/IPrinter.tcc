
#include "IPrinter.h"

#include <string>
#include <list>
#include <forward_list>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <memory>


template<>
struct ignis::executor::core::io::IPrinterType<bool> {
    inline void operator()(const bool &b, std::ostream &out, size_t level) {
        out << b;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<int8_t> {
    inline void operator()(const int8_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint8_t> {
    inline void operator()(const uint8_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<int16_t> {
    inline void operator()(const int16_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint16_t> {
    inline void operator()(const uint16_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<int32_t> {
    inline void operator()(const int32_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint32_t> {
    inline void operator()(const uint32_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<int64_t> {
    inline void operator()(const int64_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<uint64_t> {
    inline void operator()(const uint64_t &i, std::ostream &out, size_t level) {
        out << i;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<float> {
    inline void operator()(const float &d, std::ostream &out, size_t level) {
        out << d;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<double> {
    inline void operator()(const double &d, std::ostream &out, size_t level) {
        out << d;
    }
};

template<>
struct ignis::executor::core::io::IPrinterType<std::string> {
    inline void operator()(const std::string &s, std::ostream &out, size_t level) {
        out << s;
    }
};


template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::vector<_Tp, _Alloc>> {
    inline void operator()(const std::vector<_Tp, _Alloc> &v, std::ostream &out, size_t level) {
        auto begin = v.begin();
        auto end = v.end();
        if (begin != end) {
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::list<_Tp, _Alloc>> {
    inline void operator()(const std::list<_Tp, _Alloc> &l, std::ostream &out, size_t level) {
        auto begin = l.begin();
        auto end = l.end();
        if (begin != end) {
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }
    }
};

template<typename _Tp, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::forward_list<_Tp, _Alloc>> {
    inline void operator()(const std::forward_list<_Tp, _Alloc> &fl, std::ostream &out, size_t level) {
        auto begin = fl.begin();
        auto end = fl.end();
        if (begin != end) {
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<_Tp>()(*begin, out, level + 1);
            begin++;
        }
    }
};

template<typename _Key, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::set<_Key, _Compare, _Alloc>> {
    inline void operator()(const std::set<_Key, _Compare, _Alloc> &s, std::ostream &out, size_t level) {
        auto begin = s.begin();
        auto end = s.end();
        if (begin != end) {
            IPrinterType<_Key>()(*begin, out, level + 1);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<_Key>()(*begin, out, level + 1);
            begin++;
        }
    }
};

template<typename _Value, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::unordered_set<_Value, _Hash, _Pred, _Alloc>> {
    inline void operator()(const std::unordered_set<_Value> &us, std::ostream &out, size_t level) {
        auto begin = us.begin();
        auto end = us.end();
        if (begin != end) {
            IPrinterType<_Value>()(*begin, out, level + 1);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<_Value>()(*begin, out, level + 1);
            begin++;
        }
    }
};

template<typename _Key, typename _Tp, typename _Compare, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::map<_Key, _Tp, _Compare, _Alloc>> {
    inline void
    operator()(const std::map<_Key, _Tp, _Compare, _Alloc> &m, std::ostream &out, size_t level) {
        auto begin = m.begin();
        auto end = m.end();
        if (begin != end) {
            IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
            begin++;
        }
    }
};

template<typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct ignis::executor::core::io::IPrinterType<std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> {
    inline void operator()(const std::unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc> &um, std::ostream &out,
                           size_t level) {
        auto begin = um.begin();
        auto end = um.end();
        if (begin != end) {
            IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
            begin++;
        }

        while (begin != end) {
            out << std::endl;
            IPrinterType<std::pair<_Key, _Tp>>()(*begin, out, level);
            begin++;
        }
    }
};


template<typename _T1, typename _T2>
struct ignis::executor::core::io::IPrinterType<std::pair<_T1, _T2>> {
    inline void operator()(const std::pair<_T1, _T2> &p, std::ostream &out, size_t level) {
        out << tab(level) << "(";
        IPrinterType<_T1>()(p.first, out, level + 1);
        out << ", ";
        IPrinterType<_T2>()(p.second, out, level + 1);
        out << ")";
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IPrinterType<_Tp *> {
    inline void operator()(const _Tp *&v, std::ostream &out, size_t level) {
        IPrinterType<_Tp *>()(*v, out, level);
    }
};

template<typename _Tp>
struct ignis::executor::core::io::IPrinterType<std::shared_ptr<_Tp>> {
    inline void operator()(const std::shared_ptr<_Tp> &v, std::ostream &out, size_t level) {
        IPrinterType<_Tp>()(*v, out, level);
    }
};