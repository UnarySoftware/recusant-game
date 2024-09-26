#pragma once

#include <map>
#include <vector>
#include <algorithm>

namespace recusant
{
    template<typename T>
    class TopologicalSorter
    {

    private:

        T _error_key;
        T _error_dep;
        bool _error = false;
        std::vector<T> _seen;
        std::map<T, std::vector<T>> _deps;

        void resolve(T const& p_dep, std::vector<T>& p_sorted)
        {
            _seen.push_back(p_dep);
            for (auto const& nd : _deps[p_dep])
            {
                if (std::find(p_sorted.begin(), p_sorted.end(), nd) != p_sorted.end())
                {
                    continue;
                }
                else if (std::find(_seen.begin(), _seen.end(), nd) == _seen.end())
                {
                    resolve(nd, p_sorted);
                }
                else
                {
                    _error = true;
                    _error_key = p_dep;
                    _error_dep = nd;
                    break;
                }
            }
            p_sorted.push_back(p_dep);
        }

    public:

        bool get_error()
        {
            return _error;
        }

        const T& get_error_key()
        {
            return _error_key;
        }

        const T& get_error_dep()
        {
            return _error_dep;
        }

        void clear()
        {
            _error = false;
            _seen.clear();
            _deps.clear();
        }

        void add(T const& a)
        {
            _deps[a];
        }

        void add(T const& p_key, T const& p_dep)
        {
            _deps[p_key].push_back(p_dep);
        }

        std::vector<T> sort()
        {
            _error = false;
            std::vector<T> sorted;
            for (auto const& d : _deps)
            {
                if (std::find(sorted.begin(), sorted.end(), d.first) == sorted.end())
                {
                    resolve(d.first, sorted);
                }
            }
            return sorted;
        }

    };
}
