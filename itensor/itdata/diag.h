//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_DIAG_H
#define __ITENSOR_DIAG_H

#include "itensor/itdata/dense.h"

namespace itensor {

template<typename T>
class Diag;

using DiagReal = Diag<Real>;
using DiagCplx = Diag<Cplx>;

template<typename T>
class Diag
    {
    public:
    using value_type = stdx::decay_t<T>;
    using storage_type = std::vector<value_type>;
    using size_type = typename storage_type::size_type;
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    storage_type store;
    T val = 0;
    size_type length = 0;

    Diag() { }

    //Special "all same" case, no
    //storage allocated since elements
    //all the same (== val)
    Diag(size_t length_, T val_)
      : val(val_),
        length(length_)
        { }

    Diag(size_t size)
      : store(size,0),
        length(size)
        { }

    template<typename InputIterator>
    Diag(InputIterator b, InputIterator e)
      : store(b,e),
        length(store.size())
        { }

    explicit
    Diag(storage_type&& data)
      : store(std::move(data)),
        length(store.size())
        { }

    template<typename V>
    explicit
    Diag(Diag<V> const& D)
      : store(D.begin(),D.end()),
        val(D.val),
        length(D.length)
        {
        }

    bool
    allSame() const { return store.empty(); }

    size_type
    size() const { return store.size(); }

    bool
    empty() const { return store.empty(); }

    value_type*
    data() { return store.data(); }

    value_type const*
    data() const { return store.data(); }
    
    const_iterator
    begin() const { return store.begin(); }

    const_iterator
    end() const { return store.end(); }

    iterator
    begin() { return store.begin(); }

    iterator
    end() { return store.end(); }
    };

template<typename T>
bool constexpr
isReal(Diag<T> const& D) { return isReal<T>(); }

template<typename T>
bool constexpr
isCplx(Diag<T> const& D) { return isCplx<T>(); }

Data inline
realData(DiagReal & d) { return Data(d.data(),d.size()); }

Datac inline
realData(DiagReal const& d) { return Datac(d.data(),d.size()); }

Data inline
realData(DiagCplx & d) { return Data(reinterpret_cast<Real*>(d.data()),2*d.size()); }

Datac inline
realData(DiagCplx const& d) { return Datac(reinterpret_cast<const Real*>(d.data()),2*d.size()); }

template<typename T>
void
read(std::istream& s, Diag<T>& dat)
    {
    itensor::read(s,dat.val);
    itensor::read(s,dat.length);
    itensor::read(s,dat.store);
    }

template<typename T>
void
write(std::ostream& s, const Diag<T>& dat)
    {
    itensor::write(s,dat.val);
    itensor::write(s,dat.length);
    itensor::write(s,dat.store);
    }

template <typename F, typename T,
          class = stdx::require<std::is_same<T,stdx::result_of_t<F(T)>>> >
void
doTask(ApplyIT<F>& A, Diag<T>& d) 
    { 
    //if(d.allSame()) 
    //    {
    //    d.val = detail::call<T>(A.f,d.val);
    //    }
    //else
    //    {
    //    for(auto& elt : d.store) elt = detail::call<T>(A.f,elt);
    //    }
    if(d.allSame()) 
        {
        A(d.val);
        }
    else
        {
        for(auto& el : d.store) A(el);
        }
    }

template <typename T>
Cplx
doTask(GetElt<Index> const& g, Diag<T> const& d);

template<typename T1, typename T2>
void
doTask(Contract<Index> & C,
       Dense<T1>  const& t,
       Diag<T2>   const& d,
       ManageStore     & m);

template<typename T1, typename T2>
void
doTask(Contract<Index> & C,
       Diag<T1>   const& d,
       Dense<T2>  const& t,
       ManageStore     & m);

void
doTask(PlusEQ<Index> const& P,
       DiagReal           & D1,
       DiagReal      const& D2);

template<typename N, typename T>
void
doTask(Fill<N> const& f, Diag<T> const& d, ManageStore& m);

template<typename T1, typename T2>
void
doTask(Mult<T1> const& M, Diag<T2> & D);

void
doTask(Mult<Cplx> const& M, DiagReal const& D, ManageStore & m);

template<typename T>
Real
doTask(NormNoScale, Diag<T> const& d);

void
doTask(Conj, DiagReal const& d);

void
doTask(Conj, DiagCplx & d);

void
doTask(TakeReal, DiagReal const& D);

void
doTask(TakeReal, DiagCplx const& D, ManageStore& m);

void
doTask(TakeImag, DiagReal & D);

void
doTask(TakeImag, DiagCplx const& D, ManageStore& m);

template<typename T>
void
doTask(PrintIT<Index> & P, Diag<T> const& d);

template<typename T>
bool
doTask(CheckComplex, Diag<T> const& d) { return isCplx<T>(); }

template <class T>
Cplx
doTask(SumEls<Index> S, Diag<T> const& d);

auto inline
doTask(StorageType const& S, DiagReal const& d) ->StorageType::Type { return StorageType::DiagReal; }

auto inline
doTask(StorageType const& S, DiagCplx const& d) ->StorageType::Type { return StorageType::DiagCplx; }


} //namespace itensor

#endif

