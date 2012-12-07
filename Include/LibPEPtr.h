#pragma once

#include "LibPEBase.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
class LibPEPtr
{
public:
    LibPEPtr() : p(NULL) {}
    
    LibPEPtr(T *ptr) : p(ptr) {
        if(NULL != p) {
            p->AddRef();
        }
    }

    LibPEPtr(const LibPEPtr<T> &ptr) : p(ptr.p) {
        if(NULL != p) {
            p->AddRef();
        }
    }

    ~LibPEPtr() { Reset(); }

    void Reset()
    {
        if(NULL != p) {
            p->Release();
        }
        p = NULL;
    }

    void Attach(T *ptr)
    {
        if(NULL != p) {
            p->Release();
        }
        p = ptr;
    }

    T * Detach()
    {
        T *ptr = p;
        p = NULL;
        return ptr;
    }

    operator T *() { return p; }
    T & operator *() { return *p; }
    T ** operator &() { return &p; }
    T * operator ->() { return p; }
    bool operator !() const { return (NULL == p); }
    bool operator <(T *ptr) const { return p < ptr; }
    bool operator ==(T *ptr) const { return p == ptr; }
    bool operator !=(T *ptr) const { return p != ptr; }

    T * Copy()
    {
        if(NULL != p) {
            p->AddRef();
        }
        return p;
    }

    error_t CopyTo(T **ppT)
    {
        if(NULL == ppT) {
            return ERR_POINTER;
        }
        *ppT = p;
        if(NULL != p) {
            p->AddRef();
        }
        return S_OK;
    }

    T * operator =(T *ptr)
    {
        if(NULL != ptr) {
            ptr->AddRef();
        }
        if(NULL != p) {
            p->Release();
        }
        p = ptr;
        return p;
    }

    T * operator =(const LibPEPtr<T> &ptr)
    {
        if(p != ptr.p) {
            *this = ptr.p;
        }
        return p;
    }
    
    T *p;
};

LIBPE_NAMESPACE_END