/*
 * Copyright (c) 2003-2006, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */
#include <tchar.h>
#include <assert.h>
#include <stdio.h>

#include <log/log.h>
#include <log/log_message.h>
#include <debug/trace.h>

#include "diff_ext.h"
#include "server.h"
#include "resource.h"

const UINT IDM_DIFF=1;
const UINT IDM_DIFF3=2;
const UINT IDM_DIFF_WITH=3;
const UINT IDM_DIFF_LATER=4;
const UINT IDM_CLEAR=5;
const UINT IDM_DIFF_WITH_BASE=6;

typedef struct tag_menu_item_data {
  STRING text;
  HICON icon;
} MENU_ITEM_DATA;

HBITMAP
icon2bitmap(HANDLE icon) {
  ICONINFO icon_info;
  HBITMAP result = 0;
  LONG d = GetMenuCheckMarkDimensions();
  LONG dx = HIWORD(d);
  LONG dy = LOWORD(d);
  HDC bitmap_dc = CreateCompatibleDC(0);
  HDC icon_dc = CreateCompatibleDC(0);
  BITMAP bmp;
  
  GetIconInfo((HICON)icon, &icon_info);
  GetObject(icon_info.hbmColor, sizeof(BITMAP), &bmp);
  
  result = CreateBitmap(bmp.bmWidth, bmp.bmWidth, bmp.bmPlanes, bmp.bmBitsPixel, 0);
//  result = CreateCompatibleBitmap(bitmap_dc, d, d);
  if(result == 0) {
    MessageBox(0, TEXT(""), TEXT("No bitmap :("), MB_OK);
  }
  SelectObject(bitmap_dc, result);
  
  SelectObject(icon_dc, icon_info.hbmMask);
  BitBlt(bitmap_dc, 0, 0, dx, dy, 
             icon_dc, 0, 0, SRCAND);

  SelectObject(icon_dc, icon_info.hbmColor);
  BitBlt(bitmap_dc, 0, 0, dx, dy, 
             icon_dc, 0, 0, SRCINVERT);
  
  DeleteDC(bitmap_dc);
  DeleteDC(icon_dc);
  
  return result;
}

DIFF_EXT::DIFF_EXT() : _n_files(0), _selection(0), _language(1033), _ref_count(0L) {
//  TRACE trace(TEXT("DIFF_EXT::DIFF_EXT()"), TEXT(__FILE__), __LINE__);
  
  _recent_files = SERVER::instance()->recent_files();

  _resource = SERVER::instance()->handle();
  
  SERVER::instance()->lock();
  
  HICON icon = (HICON)LoadImage(_resource, MAKEINTRESOURCE(100), IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTCOLOR);
  
  _diff_icon = icon;
  _diff3_icon = icon;
  _diff_later_icon = icon;
  _diff_with_icon = icon;
  _diff3_with_icon = icon;
  _clear_icon = icon;
}

DIFF_EXT::~DIFF_EXT() {
//  TRACE trace(TEXT(__f__), TEXT(__FILE__), __LINE__);
  delete[] _selection;
  
  if(_resource != SERVER::instance()->handle()) {
    FreeLibrary(_resource);
  }
  
  SERVER::instance()->release();
}

STDMETHODIMP
DIFF_EXT::QueryInterface(REFIID refiid, void** ppv) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  HRESULT ret = E_NOINTERFACE;
  *ppv = 0;

  if(IsEqualIID(refiid, IID_IShellExtInit) || IsEqualIID(refiid, IID_IUnknown)) {
    *ppv = static_cast<IShellExtInit*>(this);
  } else if(IsEqualIID(refiid, IID_IContextMenu)) {
    *ppv = static_cast<IContextMenu*>(this);
  } else if(IsEqualIID(refiid, IID_IContextMenu2)) {
    *ppv = static_cast<IContextMenu2*>(this);
  } else if(IsEqualIID(refiid, IID_IContextMenu3)) {
    *ppv = static_cast<IContextMenu3*>(this);
  }

  if(*ppv != 0) {
    AddRef();

    ret = NOERROR;
  }

  return ret;
}

STDMETHODIMP_(ULONG)
DIFF_EXT::AddRef() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  return InterlockedIncrement((LPLONG)&_ref_count);
}

STDMETHODIMP_(ULONG)
DIFF_EXT::Release() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  ULONG ret = 0L;
  
  if(InterlockedDecrement((LPLONG)&_ref_count) != 0) {
    ret = _ref_count;
  } else {
    delete this;
  }

  return ret;
}

void
DIFF_EXT::initialize_language() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  HKEY key;
  DWORD language = 0;
  DWORD len;

  if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Z\\diff-ext"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    len = sizeof(DWORD);
    RegQueryValueEx(key, TEXT("language"), 0, NULL, (BYTE*)&language, &len);

    RegCloseKey(key);

    if(language != _language) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 5);
      _language = language;
      
      if(_resource != SERVER::instance()->handle()) {
	FreeLibrary(_resource);
      }
      
      if(_language != 1033) {
//        TRACE trace(__FUNCTION__, __FILE__, __LINE__, 6);
        TCHAR language_lib[MAX_PATH];
        TCHAR* dot;
	
        GetModuleFileName(SERVER::instance()->handle(), language_lib, sizeof(language_lib)/sizeof(language_lib[0]));
        dot = _tcsrchr(language_lib, TEXT('.'));
        _stprintf(dot, TEXT("%lu.dll"), language);
	
	_resource = LoadLibrary(language_lib);

	if(_resource == 0) {
          LPTSTR message;
          FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
            GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &message, 0, 0);
          MessageBox(0, message, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);
      
          LocalFree(message);
          
          GetModuleFileName(SERVER::instance()->handle(), language_lib, sizeof(language_lib)/sizeof(language_lib[0]));
          MessageBox(0, language_lib, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);
          
	  _resource = SERVER::instance()->handle();
	  _language = 1033;
 	}
      } else {
	_resource = SERVER::instance()->handle();
      }
    }
  }
}

STDMETHODIMP
DIFF_EXT::Initialize(LPCITEMIDLIST /*folder not used*/, IDataObject* data, HKEY /*key not used*/) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  FORMATETC format = {CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
  STGMEDIUM medium;
  medium.tymed = TYMED_HGLOBAL;
  HRESULT ret = S_OK/*E_INVALIDARG*/;

  if(data->GetData(&format, &medium) == S_OK) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__);
    HDROP drop = (HDROP)medium.hGlobal;
    _n_files = DragQueryFile(drop, 0xFFFFFFFF, 0, 0);

    TCHAR tmp[MAX_PATH];
    
    initialize_language();
    
    _selection = new STRING[_n_files+1];
    for(unsigned int i = 0; i < _n_files; i++) {
      DragQueryFile(drop, i, tmp, MAX_PATH);
      _selection[i+1] = STRING(tmp);
    }
  }

  return ret;
}

void
DIFF_EXT::load_resource_string(UINT string_id, TCHAR* string, int length, TCHAR* default_value) {
  int resource_string_length;
  
  resource_string_length = LoadString(_resource, string_id, string, length);
  
  if(resource_string_length == 0) {
    resource_string_length = LoadString(SERVER::instance()->handle(), string_id, string, length);
    
    if(resource_string_length == 0) {
      TCHAR message[256];
      
      wsprintf(message, TEXT("Can not load string resource %d"), string_id);
      
      lstrcpy(string, default_value);
      MessageBox(0, message, TEXT("ERROR"), MB_OK);
    }
  }
}

void
insert_menu(HMENU menu, UINT position, UINT id, UINT state, TCHAR* string, HICON icon) {
  MENUITEMINFO item_info;
  MENU_ITEM_DATA* data = new MENU_ITEM_DATA;
  
  data->text = string;
  data->icon = icon;
  
  ZeroMemory(&item_info, sizeof(item_info));
  item_info.cbSize = sizeof(MENUITEMINFO);
  item_info.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
  item_info.fType = MFT_OWNERDRAW;
  item_info.fState = state;
  item_info.wID = id;
  item_info.dwItemData = (ULONG_PTR)data;
  item_info.dwTypeData = (LPTSTR)data;
  InsertMenuItem(menu, position, TRUE, &item_info);
}

STDMETHODIMP 
DIFF_EXT::HandleMenuMsg(UINT msg, WPARAM w_param, LPARAM l_param) {
  LRESULT result;
  
  MessageBox(0, TEXT(""), TEXT("HandleMenuMsg"), MB_OK);
  return HandleMenuMsg2(msg, w_param, l_param, &result);
}

STDMETHODIMP 
DIFF_EXT::HandleMenuMsg2(UINT msg, WPARAM w_param, LPARAM l_param, LRESULT *result) {
  LRESULT local;
  
  if(result == 0) {
    result = &local;
  }
  
  *result = NOERROR;
  
//  MessageBox(0, TEXT(""), TEXT("HandleMenuMsg2"), MB_OK);
  if(msg == WM_MEASUREITEM) {
    MEASUREITEMSTRUCT* mis = (MEASUREITEMSTRUCT*)l_param;
    
//    MessageBox(0, TEXT(""), TEXT("WM_MEASUREITEM"), MB_OK);
    if(mis != 0) {    
      MENU_ITEM_DATA* data = (MENU_ITEM_DATA*)mis->itemData;
      
      *result = TRUE;
            
      NONCLIENTMETRICS ncm;

      mis->itemWidth = 0;
      mis->itemHeight = 0;
      
      ncm.cbSize = sizeof(NONCLIENTMETRICS);
      
      if(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0) != 0) {
        HDC dc = CreateCompatibleDC(0);
        
        if(dc != 0) {
          HFONT font = CreateFontIndirect(&ncm.lfMenuFont);
          
          if(font != 0) {
            SIZE size;
            HFONT old_font = (HFONT)SelectObject(dc, font);
            
            if(data != 0) {
              unsigned int icon_width = 0;
              unsigned int icon_height = 0;
              if(data->icon != 0) {
                ICONINFO ii;
                GetIconInfo(data->icon, &ii);
                HBITMAP hb = ii.hbmColor;
                BITMAP bm;
                if(hb == 0) {
                  hb = ii.hbmMask;
                }
                GetObject(hb, sizeof(bm), &bm);
                if(hb == ii.hbmMask) {
                  bm.bmHeight /= 2;
                }
                
                icon_width = bm.bmWidth;
                icon_height = bm.bmHeight;
              }
              GetTextExtentPoint32(dc, data->text, lstrlen(data->text), &size);
              LPtoDP(dc, (POINT*)&size, 1);

              SelectObject(dc, old_font);
              DeleteObject(font);
              DeleteDC(dc);

              mis->itemWidth = size.cx + 3 + icon_width; //width of string + width of icon + space between ~icon~text~
              mis->itemHeight = max(size.cy, ncm.iMenuHeight);     
              mis->itemHeight = max(mis->itemHeight, icon_height+2)+1;     
              
//              MessageBox(0, TEXT("Ok"), TEXT("WM_MEASUREITEM"), MB_OK);
            }
          }
        }
      }
    }
  } else if(msg == WM_DRAWITEM) {
    DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)l_param;
    
//    MessageBox(0, TEXT(""), TEXT("WM_DRAWITEM"), MB_OK);
    
    if(dis != 0) {
      if(dis->CtlType == ODT_MENU) {
        MENU_ITEM_DATA* data = (MENU_ITEM_DATA*)dis->itemData;
        
        if(data != 0) {
          NONCLIENTMETRICS ncm;
          COLORREF text;
          COLORREF background;
          HFONT font;
          HFONT menu_font;
          
          ncm.cbSize = sizeof(NONCLIENTMETRICS);
          SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
          menu_font = CreateFontIndirect(&ncm.lfMenuFont);
          
          if (dis->itemState & ODS_SELECTED) {
            if (dis->itemState & ODS_GRAYED) {
              text = SetTextColor(dis->hDC, GetSysColor(COLOR_GRAYTEXT));
            } else {
              text = SetTextColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
            }
            
            background = SetBkColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHT));
          } else { 
            text = SetTextColor(dis->hDC, GetSysColor(COLOR_MENUTEXT)); 
            background = SetBkColor(dis->hDC, GetSysColor(COLOR_MENU)); 
          } 
    
          font = (HFONT)SelectObject(dis->hDC, menu_font);
          
          ExtTextOut(dis->hDC, dis->rcItem.left, dis->rcItem.top, ETO_CLIPPED | ETO_OPAQUE, &dis->rcItem, 0, 0, 0); 
          if(data->icon != 0) {
            ICONINFO ii;
            GetIconInfo(data->icon, &ii);
            HBITMAP hb = ii.hbmColor;
            BITMAP bm;
            if(hb == 0) {
              hb = ii.hbmMask;
            }
            GetObject(hb, sizeof(bm), &bm);
            if(hb == ii.hbmMask) {
              bm.bmHeight /= 2;
            }
            DrawIconEx(dis->hDC, dis->rcItem.left+1, dis->rcItem.top+1, data->icon, bm.bmWidth, bm.bmHeight, 0, 0, DI_NORMAL);
            dis->rcItem.left += bm.bmWidth+3;
          } else {
            dis->rcItem.left += GetSystemMetrics(SM_CXSMICON);
          }
          int y;
          SIZE size = {0, 0};
          GetTextExtentPoint32(dis->hDC, data->text, lstrlen(data->text), &size);
//          LPtoDP(dis->hDC, (POINT*)&size, 1);
          y = ((dis->rcItem.bottom - dis->rcItem.top) - size.cy) / 2;
          y = dis->rcItem.top + (y >= 0 ? y : 0);
          ExtTextOut(dis->hDC, dis->rcItem.left, y, ETO_OPAQUE, &dis->rcItem, data->text, lstrlen(data->text), 0); 
          
          SelectObject(dis->hDC, font);
          if (dis->itemState & ODS_SELECTED) {
            SetTextColor(dis->hDC, text);
            SetBkColor(dis->hDC, background);
          }
          
          *result = TRUE;
//          MessageBox(0, TEXT("Ok"), TEXT("WM_DRAWITEM"), MB_OK);
        }
      }
    }
  }
  
  return NOERROR;
}

STDMETHODIMP
DIFF_EXT::QueryContextMenu(HMENU menu, UINT position, UINT first_cmd, UINT /*last_cmd not used*/, UINT flags) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);  
  TCHAR resource_string[256];
  HRESULT ret = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

  if(!(flags & CMF_DEFAULTONLY)) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__);
    UINT id = first_cmd;
    LPTSTR c_str;
    
    if(_n_files > 0) {
      MENUITEMINFO item_info;

      ZeroMemory(&item_info, sizeof(item_info));
      item_info.cbSize = sizeof(MENUITEMINFO);
      item_info.fMask = MIIM_TYPE;
      item_info.fType = MFT_SEPARATOR;
      item_info.dwTypeData = 0;
      InsertMenuItem(menu, position, TRUE, &item_info);
      position++;
      
      if(_n_files == 1) {
        if(_recent_files->count() > 0) {
          DLIST<STRING>::ITERATOR i = _recent_files->head();
          STRING str  = cut_to_length((*i)->data());
          void* args[] = {(void*)str};
          
          load_resource_string(DIFF_WITH_FILE_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("compare to '%1'"));
          FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, resource_string, 0, 0, (LPTSTR)&c_str, 0, (char**)args);
          id = first_cmd + IDM_DIFF_WITH;
          insert_menu(menu, position, id, MFS_ENABLED, c_str, _diff_with_icon);
          position++;
        }        
      } else if(_n_files == 2) {
        if(SERVER::instance()->tree_way_compare_supported()) {
          if(_recent_files->count() > 0) {
            DLIST<STRING>::ITERATOR i = _recent_files->head();
            STRING str1  = cut_to_length((*i)->data());
            void* args[] = {(void*)str1};
            
            load_resource_string(DIFF3_WITH_FILE_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3-way compare '%1'"));
            FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, resource_string, 0, 0, (LPTSTR)&c_str, 0, (char**)args);
            id = first_cmd + IDM_DIFF_WITH;
            insert_menu(menu, position, id, MFS_ENABLED, c_str, _diff3_with_icon);
            position++;
          }
        }
        
        load_resource_string(DIFF_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("compare"));
        
        id = first_cmd + IDM_DIFF;
        insert_menu(menu, position, id, MFS_ENABLED, resource_string, _diff_icon);
        position++;
      } else if(_n_files == 3) {
        if(SERVER::instance()->tree_way_compare_supported()) {
          load_resource_string(DIFF3_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3way compare"));
          
          id = first_cmd + IDM_DIFF3;
          insert_menu(menu, position, id, MFS_ENABLED, resource_string, _diff3_icon);
          position++;
        }
      }

      load_resource_string(DIFF_LATER_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("compare later"));
      
      id = first_cmd + IDM_DIFF_LATER;
      insert_menu(menu, position, id, MFS_ENABLED, resource_string, _diff_later_icon);
      position++;
      
      c_str = 0;
      if(_n_files == 1) {
        load_resource_string(DIFF_WITH_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("compare to '%1'"));
        c_str = resource_string;
      } else if(_n_files == 2) {
        if(SERVER::instance()->tree_way_compare_supported()) {
          load_resource_string(DIFF3_WITH_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3-way compare to"));
          c_str = resource_string;
        }
      }
      
      if(c_str != 0) {
        if(_recent_files->count() > 0) {
          HMENU file_list = CreateMenu();
          DLIST<STRING>::ITERATOR i = _recent_files->head();

          id = first_cmd+IDM_DIFF_WITH_BASE;
          int n = 0;
          while(!i.done()) {
            STRING str;
            SHFILEINFO file_info;
            
            str = cut_to_length((*i)->data());
            c_str = str;

            SHGetFileInfo((*i)->data(), 0, &file_info, sizeof(file_info), SHGFI_ICON | SHGFI_SMALLICON);
            insert_menu(file_list, n, id, MFS_ENABLED, c_str, file_info.hIcon);
            id++;
            i++;
            n++;
          }
          
          ZeroMemory(&item_info, sizeof(item_info));
          item_info.cbSize = sizeof(MENUITEMINFO);
          item_info.fMask = MIIM_TYPE;
          item_info.fType = MFT_SEPARATOR;
          item_info.dwTypeData = 0;
          InsertMenuItem(file_list, n, TRUE, &item_info);
          n++;

          load_resource_string(CLEAR_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Clear"));
          insert_menu(file_list, n, first_cmd + IDM_CLEAR, MFS_ENABLED, resource_string, _clear_icon);

          load_resource_string(DIFF_WITH_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("compare to '%1'"));
          
          ZeroMemory(&item_info, sizeof(item_info));
          item_info.cbSize = sizeof(MENUITEMINFO);
          item_info.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_ID;
          item_info.wID = first_cmd + IDM_DIFF_WITH;
          item_info.hSubMenu = file_list;
          item_info.dwTypeData = resource_string;
          InsertMenuItem(menu, position, TRUE, &item_info);
          position++;
        }
      }
      
      ZeroMemory(&item_info, sizeof(item_info));
      item_info.cbSize = sizeof(MENUITEMINFO);
      item_info.fMask = MIIM_TYPE;
      item_info.fType = MFT_SEPARATOR;
      item_info.dwTypeData = 0;
      InsertMenuItem(menu, position, TRUE, &item_info);
      position++;
    }    
    
    ret = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, id-first_cmd+1);
  }

  return ret;
}

STDMETHODIMP
DIFF_EXT::InvokeCommand(LPCMINVOKECOMMANDINFO ici) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  HRESULT ret = NOERROR;

  _hwnd = ici->hwnd;

  if(HIWORD(ici->lpVerb) == 0) {
    if(LOWORD(ici->lpVerb) == IDM_DIFF) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      _selection[0] = _selection[2];
      diff();
    } else if(LOWORD(ici->lpVerb) == IDM_DIFF3) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      _selection[0] = _selection[3];
      diff3();
    } else if(LOWORD(ici->lpVerb) == IDM_DIFF_WITH) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      if(_n_files == 1) {
        diff_with(0);
      } else if((_n_files == 2) && (SERVER::instance()->tree_way_compare_supported())) {
        diff3_with(0);
      }        
    } else if(LOWORD(ici->lpVerb) == IDM_DIFF_LATER) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      diff_later();
    } else if((LOWORD(ici->lpVerb) >= IDM_DIFF_WITH_BASE) && (LOWORD(ici->lpVerb) < IDM_DIFF_WITH_BASE+_recent_files->count())) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      if(_n_files == 1) {
        diff_with(LOWORD(ici->lpVerb)-IDM_DIFF_WITH_BASE);
      } else if((_n_files == 2) && (SERVER::instance()->tree_way_compare_supported())) {
        diff3_with(LOWORD(ici->lpVerb)-IDM_DIFF_WITH_BASE);
      }
    } else if(LOWORD(ici->lpVerb) == IDM_CLEAR) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      _recent_files->clear();
    } else {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      ret = E_INVALIDARG;
//      assert(0);
      TCHAR verb[80];
      
      _stprintf(verb, TEXT("%d"), LOWORD(ici->lpVerb));
      MessageBox(0, verb, TEXT("Command id"), MB_OK);
    }
  }

  return ret;
}

STDMETHODIMP
DIFF_EXT::GetCommandString(UINT_PTR idCmd, UINT uFlags, UINT*, LPSTR pszName, UINT cchMax) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  HRESULT ret = NOERROR;
  TCHAR resource_string[256];
  
  if(uFlags == GCS_HELPTEXT) {
    if(idCmd == IDM_DIFF) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      load_resource_string(DIFF_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Compare selected files."));
      
      lstrcpyn((LPTSTR)pszName, resource_string, cchMax);
    } else if(idCmd == IDM_DIFF3) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      load_resource_string(DIFF3_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3-way compare selected files."));
      
      lstrcpyn((LPTSTR)pszName, resource_string, cchMax);
    } else if(idCmd == IDM_DIFF_WITH) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      if(!_recent_files->empty()) {
        DLIST<STRING>::ITERATOR i = _recent_files->head();
        LPTSTR message;
        STRING fn = (*i)->data();
        LPTSTR file_name = fn;
        void* args[] = {(void*)file_name};
        
        if(_n_files == 1) {
  	  load_resource_string(DIFF_WITH_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Compare ''%1''."));	
        } else if(_n_files == 2) {
  	  load_resource_string(DIFF3_WITH_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3-way compare to ''%1''."));	
        } else {
          MessageBox(0, TEXT("This is bad"), TEXT(""), MB_OK);
        }

	FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, resource_string, 0, 0, (LPTSTR)&message, 0, (char**)args);
	lstrcpyn((LPTSTR)pszName, message, cchMax);
	LocalFree(message);
      } else {
	lstrcpyn((LPTSTR)pszName, TEXT(""), cchMax);
      }
    } else if(idCmd == IDM_DIFF_LATER) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      load_resource_string(DIFF_LATER_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Save selected file(s) for later comparison."));
      
      lstrcpyn((LPTSTR)pszName, resource_string, cchMax);
    } else if(idCmd == IDM_CLEAR) {
      load_resource_string(CLEAR_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Clear selected files list."));
      
      lstrcpyn((LPTSTR)pszName, resource_string, cchMax);
    } else if((idCmd >= IDM_DIFF_WITH_BASE) && (idCmd < IDM_DIFF_WITH_BASE+_recent_files->count())) {
//      TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
      if(!_recent_files->empty()) {
	UINT_PTR num = idCmd-IDM_DIFF_WITH_BASE;
	DLIST<STRING>::ITERATOR i = _recent_files->head();
	for(unsigned int j = 0; j < num; j++) {
	  i++;
        }
      
	LPTSTR message;
        STRING fn = (*i)->data();
        LPTSTR file_name = fn;
        void* args[] = {(void*)file_name};
        
        if(_n_files == 1) {
	  load_resource_string(DIFF_WITH_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("Compare to ''%1''."));
        } else if(_n_files == 2) {
	  load_resource_string(DIFF3_WITH_HINT, resource_string, sizeof(resource_string)/sizeof(resource_string[0]), TEXT("3-way compare to ''%1''."));
        }

	FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, resource_string, 0, 0, (LPTSTR)&message, 0, (char**)args);
	lstrcpyn((LPTSTR)pszName, message, cchMax);
	LocalFree(message);
      } else {
//        TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
	lstrcpyn((LPTSTR)pszName, TEXT(""), cchMax);
      }
    }
  }

  return ret;
}

void
DIFF_EXT::diff() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  //~ FILE* f = fopen("d:/DIFF_EXT.log", "a");
  //~ MessageBox(_hwnd, "diff", "command", MB_OK);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  HKEY key;
  DWORD length = MAX_PATH;
  TCHAR command_template[MAX_PATH*4 + 8]; // path_to_diff+options(MAX_PATH)+2*path_to_files+qoutes&spaces
  LPTSTR command;
  void* args[] = {_selection[1], _selection[0]};

  ZeroMemory(command_template, sizeof(command_template));

  if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Z\\diff-ext"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    if (RegQueryValueEx(key, TEXT("diff"), 0, 0, (BYTE*)command_template, &length) != ERROR_SUCCESS) {
      command_template[0] = TEXT('\0');
    }

    RegCloseKey(key);
  } else {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    LPTSTR message;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);

    MessageBox(0, message, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
  }
  
  FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, 
    command_template, 0, 0, (LPTSTR)&command, 0, (char**)args);
  
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);

  if (CreateProcess(0, command, 0, 0, FALSE, 0, 0, 0, &si, &pi) == 0) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    TCHAR resource_string[1024];
    TCHAR error_string[256];
    int string_length;
    
    string_length = LoadString(_resource, CREATE_PROCESS_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
  
    if(string_length == 0) {
      string_length = LoadString(SERVER::instance()->handle(), CREATE_PROCESS_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
      
      if(string_length == 0) {
	lstrcpy(resource_string, TEXT("Could not start diff command. Please run diff_ext setup program and verify your configuration."));
	MessageBox(0, TEXT("Can not load 'CREATE_PROCESS_STR' string resource"), TEXT("ERROR"), MB_OK);
      }
    }
    
    string_length = LoadString(_resource, ERROR_STR, error_string, sizeof(error_string)/sizeof(error_string[0]));
  
    if(string_length == 0) {
      string_length = LoadString(SERVER::instance()->handle(), ERROR_STR, error_string, sizeof(error_string)/sizeof(error_string[0]));
      
      if(string_length == 0) {
	lstrcpy(error_string, TEXT("Error"));
	MessageBox(0, TEXT("Can not load 'ERROR_STR' string resource"), TEXT("ERROR"), MB_OK);
      }
    }

    MessageBox(_hwnd, resource_string, error_string, MB_OK);
  } else {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
/*    
    LPTSTR message;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);
    MessageBox(0, message, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
*/
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
  }
  
  LocalFree(command);

  //~ fclose(f);
}

void
DIFF_EXT::diff3() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  //~ FILE* f = fopen("d:/DIFF_EXT.log", "a");
  //~ MessageBox(_hwnd, "diff", "command", MB_OK);
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  HKEY key;
  DWORD length = MAX_PATH;
  TCHAR command_template[MAX_PATH*5 + 11]; // path_to_diff+options(MAX_PATH)+3*path_to_files+qoutes&spaces
  LPTSTR command;
  void* args[] = {_selection[1], _selection[2], _selection[0]};

  ZeroMemory(command_template, sizeof(command_template));

  if(RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Z\\diff-ext"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    if (RegQueryValueEx(key, TEXT("diff3"), 0, 0, (BYTE*)command_template, &length) != ERROR_SUCCESS) {
      command_template[0] = TEXT('\0');
    }

    RegCloseKey(key);
  } else {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    LPTSTR message;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);

    MessageBox(0, message, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
  }
  FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_ARGUMENT_ARRAY, 
    command_template, 0, 0, (LPTSTR)&command, 0, (char**)args);
  
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);

  if (CreateProcess(0, command, 0, 0, FALSE, 0, 0, 0, &si, &pi) == 0) {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
    TCHAR resource_string[1024];
    TCHAR error_string[256];
    int string_length;
    
    string_length = LoadString(_resource, CREATE_PROCESS_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
  
    if(string_length == 0) {
      string_length = LoadString(SERVER::instance()->handle(), CREATE_PROCESS_STR, resource_string, sizeof(resource_string)/sizeof(resource_string[0]));
      
      if(string_length == 0) {
	lstrcpy(resource_string, TEXT("Could not start diff command. Please run diff_ext setup program and verify your configuration."));
	MessageBox(0, TEXT("Can not load 'CREATE_PROCESS_STR' string resource"), TEXT("ERROR"), MB_OK);
      }
    }
    
    string_length = LoadString(_resource, ERROR_STR, error_string, sizeof(error_string)/sizeof(error_string[0]));
  
    if(string_length == 0) {
      string_length = LoadString(SERVER::instance()->handle(), ERROR_STR, error_string, sizeof(error_string)/sizeof(error_string[0]));
      
      if(string_length == 0) {
	lstrcpy(error_string, TEXT("Error"));
	MessageBox(0, TEXT("Can not load 'ERROR_STR' string resource"), TEXT("ERROR"), MB_OK);
      }
    }

    MessageBox(_hwnd, resource_string, error_string, MB_OK);
  } else {
//    TRACE trace(__FUNCTION__, __FILE__, __LINE__, 4);
/*    
    LPTSTR message;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);
    MessageBox(0, message, TEXT("GetLastError"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
*/
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
  }

  LocalFree(command);
  //~ fclose(f);
}

void
DIFF_EXT::diff_with(unsigned int num) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  //~ STRING str = "diff "+_selection[0]+" and "+_recent_files->at(num);
  //~ MessageBox(_hwnd, str, "command", MB_OK);
  DLIST<STRING>::ITERATOR i = _recent_files->head();
  for(unsigned int j = 0; j < num; j++) {
    i++;
  }

  _selection[0] = (*i)->data();

  diff();
}

void
DIFF_EXT::diff3_with(unsigned int num) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  //~ STRING str = "diff "+_selection[0]+" and "+_recent_files->at(num);
  //~ MessageBox(_hwnd, str, "command", MB_OK);
  DLIST<STRING>::ITERATOR i = _recent_files->head();
  for(unsigned int j = 0; j < num; j++) {
    i++;
  }

  _selection[0] = (*i)->data();

  diff3();
}

void
DIFF_EXT::diff_later() {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);
  //~ FILE* f = fopen("d:/DIFF_EXT.log", "a");
  //~ MessageBox(_hwnd, "diff later", "command", MB_OK);
//  unsigned int start = max(min(SERVER::instance()->history_size(), _n_files)-1, 0);
  for(int i = _n_files; i > 0; i--) {
    bool found = false;
    DLIST<STRING>::ITERATOR it = _recent_files->head();
    DLIST<STRING>::NODE* node = 0;
    
    while(!it.done() && !found) {
      if((*it)->data() == _selection[i]/*_file_name1*/) {
        found = true;
        node = *it;
        _recent_files->unlink(it);
      } else {
        it++;
      }
    }

    if(!found) {
      if(_recent_files->count() == SERVER::instance()->history_size()) {
        DLIST<STRING>::ITERATOR t = _recent_files->tail();
        _recent_files->remove(t);
      }
      _recent_files->prepend(_selection[i]);
    } else {
      _recent_files->prepend(node);
    }
  }
  //~ fprintf(f, "added file %s; new size: %d\n", _selection[0], _recent_files->size());

  //~ fclose(f);
}

STRING
DIFF_EXT::cut_to_length(STRING in, int max_len) {
//  TRACE trace(__FUNCTION__, __FILE__, __LINE__);

  STRING ret;
  int length = in.length();
  
  if(length > max_len) {
    ret = in.substr(0, (max_len-3)/2);
    ret += TEXT("...");
    ret += in.substr(length-(max_len-3)/2, STRING::end);
  }
  else {
    ret = in;
  }
  
  return ret;
}
