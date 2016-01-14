#pragma once

#include <map>

template<typename T>
class Sequence {
private:
        T v;
public:
        T current() const {
                return v;
        }
        T next() {
                std::cout <<"inc";
                return ++v;
        }
};

template<typename L, typename R>
class SomewhatBiMap {
protected:
        std::map<L, R> LRmap;
        std::map<R, L> RLmap;

        //Sequence<R> seq;
        R seq{1};
public:
        void insert(L lv, R rv) {
                LRmap[lv] = rv;
                RLmap[rv] = lv;
                //LRmap.insert(lv, rv);
                //RLmap.insert(rv, lv);
        }

        virtual const R& left_get(const L& lv) const {
                return this->LRmap.at(lv);
        }
        virtual std::tuple<bool, R> left_setget(const L& lv, const R& rv) {
                bool was_already_there = false;
                auto it = this->LRmap.find(lv);
                if(it == this->LRmap.end()) {
                        was_already_there = true;
                        this->insert(lv, rv);
                }

                return std::make_tuple(was_already_there, this->LRmap[lv]);
        }
        virtual const L& right_get(const R& rv) const {
                return this->RLmap.at(rv);
        }
        virtual R register_left(const L& lv) {
                auto it = this->LRmap.find(lv);
                if(it == this->LRmap.end()) {
                        this->insert(lv, ++seq); //seq.next());
                        return seq; //seq.current();
                }
                return it->second;
        }
};
