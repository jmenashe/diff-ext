/*
 * Copyright (c) 2003, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */

#ifndef __trunc_ext_h__
#define __trunc_ext_h__

#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>

// this is the actual OLE Shell context menu handler
class TRUNC_EXT : public IContextMenu, IShellExtInit {
  public:
    TRUNC_EXT();
    virtual ~TRUNC_EXT();

    //IUnknown members
    STDMETHODIMP QueryInterface(REFIID interface_id, void** result);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    //IShell members
    STDMETHODIMP QueryContextMenu(HMENU menu, UINT index, UINT cmd_first, UINT cmd_last, UINT flags);
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO info);
    STDMETHODIMP GetCommandString(UINT cmd, UINT flags, UINT* reserved, LPSTR name, UINT name_length);

    //IShellExtInit methods
    STDMETHODIMP Initialize(LPCITEMIDLIST folder, IDataObject* subj, HKEY key);

  private:
    void truncate();
    void initialize_language();

  private:
    IDataObject* _data;
    HINSTANCE _resource;
    DWORD _language;
    HWND _hwnd;

    ULONG  _ref_count;
};

#endif // __trunc_ext_h__
