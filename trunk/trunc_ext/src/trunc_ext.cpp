/*
 * Copyright (c) 2003, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */

#include <assert.h>
#include <stdio.h>

#include "trunc_ext.h"
#include "server.h"
#include "trunc_ext.rh"

const UINT IDM_TRUNCATE=10;

TRUNC_EXT::TRUNC_EXT() : _data(0), _language(1033), _ref_count(0L) {
  _resource = SERVER::instance()->handle();
  
  SERVER::instance()->lock();
}

TRUNC_EXT::~TRUNC_EXT() {
  if(_resource != SERVER::instance()->handle()) {
    FreeLibrary(_resource);
  }
  
  if(_data != 0) {
    _data->Release();
  }
  
  SERVER::instance()->release();
}

STDMETHODIMP
TRUNC_EXT::QueryInterface(REFIID refiid, void** ppv) {
  HRESULT ret = E_NOINTERFACE;
  *ppv = 0;

  if(IsEqualIID(refiid, IID_IShellExtInit) || IsEqualIID(refiid, IID_IUnknown)) {
    *ppv = static_cast<IShellExtInit*>(this);
  } else if (IsEqualIID(refiid, IID_IContextMenu)) {
    *ppv = static_cast<IContextMenu*>(this);
  }

  if(*ppv != 0) {
    AddRef();

    ret = NOERROR;
  }

  return ret;
}

STDMETHODIMP_(ULONG)
TRUNC_EXT::AddRef() {
  return InterlockedIncrement((LPLONG)&_ref_count);
}

STDMETHODIMP_(ULONG)
TRUNC_EXT::Release() {
  ULONG ret = 0L;
  
  if(InterlockedDecrement((LPLONG)&_ref_count) != 0)
    ret = _ref_count;
  else
    delete this;

  return ret;
}

void
TRUNC_EXT::initialize_language() {
  HKEY key;
  DWORD language = 0;
  DWORD len;
  TCHAR language_lib[MAX_PATH];

  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Z\\TRUNC_EXT\\"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
    len = sizeof(DWORD);
    RegQueryValueEx(key, TEXT("language"), 0, NULL, (BYTE*)&language, &len);

    RegCloseKey(key);

    if(language != _language) {
      _language = language;
      
      if(_resource != SERVER::instance()->handle()) {
	FreeLibrary(_resource);
      }
      
      if(_language != 1033) {
	GetModuleFileName(SERVER::instance()->handle(), language_lib, sizeof(language_lib)/sizeof(language_lib[0]));
	
	wsprintf(language_lib+strlen(language_lib)-4, "%ld.dll", language);
	
	_resource = LoadLibrary(language_lib);

	if(_resource == 0) {
	  _resource = SERVER::instance()->handle();
	  _language = 1033;
 	}
      }
      else {
	_resource = SERVER::instance()->handle();
      }
    }
  }
}

STDMETHODIMP
TRUNC_EXT::Initialize(LPCITEMIDLIST /*folder not used*/, IDataObject* data, HKEY /*key not used*/) {
  HRESULT ret = S_OK;

  if(_data != 0) {
    _data->Release();
  }
  
  _data = data;
  _data->AddRef();
  
  return ret;
}

STDMETHODIMP
TRUNC_EXT::QueryContextMenu(HMENU menu, UINT position, UINT first_cmd, UINT /*last_cmd not used*/, UINT flags) {
  TCHAR resource_string[256];
  int resource_string_length;
  HRESULT ret = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

  if(!(flags & CMF_DEFAULTONLY)) {
    MENUITEMINFO item_info;

    ZeroMemory(&item_info, sizeof(item_info));
    item_info.cbSize = sizeof(MENUITEMINFO);
    item_info.fMask = MIIM_TYPE;
    item_info.fType = MFT_SEPARATOR;
    item_info.dwTypeData = 0;
    InsertMenuItem(menu, position, TRUE, &item_info);
    position++;

    resource_string_length = LoadString(_resource, TRUNC_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
    
    if(resource_string_length == 0) {
      resource_string_length = LoadString(SERVER::instance()->handle(), TRUNC_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
      
      if(resource_string_length == 0) {
	lstrcpy(resource_string, TEXT("truncate"));
	MessageBox(0, TEXT("Can not load 'TRUNC_STR' string resource"), TEXT("ERROR"), MB_OK);
      }
    }
    
    ZeroMemory(&item_info, sizeof(item_info));
    item_info.cbSize = sizeof(MENUITEMINFO);
    item_info.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
    item_info.fType = MFT_STRING;
    item_info.fState = MFS_ENABLED;
    item_info.wID = first_cmd + IDM_TRUNCATE;
    item_info.dwTypeData = resource_string;
    InsertMenuItem(menu, position, TRUE, &item_info);
    position++;

    ZeroMemory(&item_info, sizeof(item_info));
    item_info.cbSize = sizeof(MENUITEMINFO);
    item_info.fMask = MIIM_TYPE;
    item_info.fType = MFT_SEPARATOR;
    item_info.dwTypeData = 0;
    InsertMenuItem(menu, position, TRUE, &item_info);
    position++;

    ret = MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_TRUNCATE+1));
  }

  return ret;
}

STDMETHODIMP
TRUNC_EXT::InvokeCommand(LPCMINVOKECOMMANDINFO ici) {
  HRESULT ret = NOERROR;

  _hwnd = ici->hwnd;

  if(HIWORD(ici->lpVerb) == 0) {
//~ char buf[256];
//~ sprintf(buf, "id: %d", LOWORD(ici->lpVerb));
//~ MessageBox(0, buf, "InvokeCommand", MB_OK);
    if(LOWORD(ici->lpVerb) == IDM_TRUNCATE) {
      truncate();
    } else {
      ret = E_INVALIDARG;
      assert(0);
    }
  }

  return ret;
}

STDMETHODIMP
TRUNC_EXT::GetCommandString(UINT idCmd, UINT uFlags, UINT*, LPSTR pszName, UINT cchMax) {
  HRESULT ret = NOERROR;
  TCHAR resource_string[256];
  int resource_string_length;
  
  if(uFlags == GCS_HELPTEXT) {
    if(idCmd == IDM_TRUNCATE) {
      resource_string_length = LoadString(_resource, TRUNC_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
      
      if(resource_string_length == 0) {
	resource_string_length = LoadString(SERVER::instance()->handle(), TRUNC_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
	
	if(resource_string_length == 0) {
	  lstrcpy(resource_string, TEXT("Compare selected files"));
	  MessageBox(0, TEXT("Can not load 'TRUNC_HINT' string resource"), TEXT("ERROR"), MB_OK);
	}
      }
      
      lstrcpyn(pszName, resource_string, cchMax);
    }
  }

  return ret;
}

void
TRUNC_EXT::truncate() {
  TCHAR message[32768];
  TCHAR tmp[MAX_PATH];
  FORMATETC format = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
  STGMEDIUM medium;
  medium.tymed = TYMED_HGLOBAL;
  UINT n_files;

  ZeroMemory(message, sizeof(message));
  
  if(_data->GetData(&format, &medium) == S_OK) {
    HDROP drop = (HDROP)medium.hGlobal;
    n_files = DragQueryFile(drop, 0xFFFFFFFF, 0, 0);
    
    if(n_files > 5) { // TODO: should be configurable
      if(MessageBox(0, TEXT("Do you really want to truncate selected files"), TEXT("WARNING"), MB_YESNO) == IDNO) {
        n_files = 0;
      }
    }
    
    for(unsigned int i = 0; i < n_files; i++) {
      DragQueryFile(drop, i, tmp, MAX_PATH);
      
      DWORD attrs = GetFileAttributes(tmp); 
      if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
/*        
        FILE* file = fopen(tmp, "wb");
        
        if(file == 0) {
          sprintf(message, "Couldnot truncate '%s'; error: %d", tmp, GetLastError());
          MessageBox(0, message, TEXT("WARNING"), MB_OK);
        }
        
        fclose(file);
*/        
        HANDLE file = CreateFile(tmp, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        
        if(file == INVALID_HANDLE_VALUE) {
          sprintf(message, "Could not truncate '%s'", tmp);
          MessageBox(0, message, TEXT("WARNING"), MB_OK);
        }
        
        CloseHandle(file);
      }
    }
  }  
  
  _data->Release();
}
