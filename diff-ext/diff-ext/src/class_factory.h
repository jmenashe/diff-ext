/*
 * Copyright (c) 2003, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */

#ifndef __class_factory_h__
#define __class_factory_h__
 
#include <shlobj.h>
#include <shlguid.h>

class ExtClassFactory : public IClassFactory {
  public:
    ExtClassFactory();
    virtual ~ExtClassFactory();

    //IUnknown members
    STDMETHODIMP QueryInterface(REFIID, void**);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    //IExtClassFactory members
    STDMETHODIMP CreateInstance(IUnknown*, REFIID, void**);
    STDMETHODIMP LockServer(BOOL);
  
  private:
    ULONG _ref_count;
};

#endif //__class_factory_h__
