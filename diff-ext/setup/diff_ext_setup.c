/*
 * Copyright (c) 2003, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string.h>
#include <stdio.h>

#include "layout.h"
#include "page.h"

#include "diff_ext_setup.rh"

typedef struct {
  int x;
  int y;
  int width;
  int height;
} WINDOW_PLACEMENT;

static BOOL CALLBACK DialogFunc(HWND dialog, UINT msg, WPARAM wParam, LPARAM lParam);
extern PAGE* create_options_page(HANDLE resource, HWND parent);
extern PAGE* create_debug_page(HANDLE resource, HWND parent);

static HANDLE resource;
static WINDOW_PLACEMENT* window_placement = 0;
static PAGE* pages[2];

int APIENTRY
WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command_line, int show) {
  WNDCLASS wc;
  HRESULT exit = ID_APPLY;
  HKEY key;
  DWORD language = 0;
  DWORD hlen;
  char language_lib[MAX_PATH];
  HGLOBAL dialog_handle;
  HRSRC resource_handle;
  DLGTEMPLATE* dialog;
  LAYOUT* layout;

  while(exit == ID_APPLY) {
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Z\\diff_ext\\"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
      hlen = sizeof(DWORD);
      RegQueryValueEx(key, TEXT("language"), 0, NULL, (BYTE*)&language, &hlen);
  
      RegCloseKey(key);
  
      sprintf(language_lib, "diff_ext_setup%ld.dll", language);
    }
  
    resource = LoadLibrary(language_lib);
  
    if(resource == NULL) {
      resource = instance;
    }
  
    memset(&wc,0,sizeof(wc));
    wc.lpfnWndProc = DefDlgProc;
    wc.cbWndExtra = DLGWINDOWEXTRA;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(resource, "main_icon");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = "diff-ext-setup";
    UnregisterClass(wc.lpszClassName, instance);
    RegisterClass(&wc);
  
    SetThreadLocale(LOCALE_USER_DEFAULT);
    InitCommonControls();
  
    resource_handle = FindResource(resource, MAKEINTRESOURCE(IDD_MAINDIALOG), RT_DIALOG);
    dialog_handle = LoadResource(resource, resource_handle);
    dialog = (DLGTEMPLATE*)LockResource(dialog_handle);
  
    layout = create_layout(resource, MAKEINTRESOURCE(IDD_MAINDIALOG), MAKEINTRESOURCE(ID_MAINDIALOG_LAYOUT));
  /*  can not do because have to specify hinst as module instance and load dialog from translated resource only dll...
    ret = DialogBox(resource, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC)DialogFunc);
  */  
    exit = DialogBoxIndirectParam(instance, dialog, NULL, (DLGPROC)DialogFunc, (LPARAM)layout);
  
    FreeResource(dialog);
    FreeLibrary(resource);
    free(layout);
  }
  
  return exit;
}

static void
InitializeApp(HWND dialog, WPARAM w_param, LPARAM l_param) {
  HWND tab = GetDlgItem(dialog, ID_TAB);
  RECT rect;
  TCITEM item1;
  TCITEM item2;
  HMODULE uxtheme_library = LoadLibrary(TEXT("uxtheme.dll"));

  pages[0] = create_options_page(resource, dialog);
  pages[1] = create_debug_page(resource, dialog);

  if(uxtheme_library != 0) {
    FARPROC EnableThemeDialogTexture = GetProcAddress(uxtheme_library, "EnableThemeDialogTexture");
    
    if(EnableThemeDialogTexture != 0) {
      int i;
      const int ETDT_DISABLE = 1;
      const int ETDT_ENABLE = 2;
      const int ETDT_USETABTEXTURE = 4;
      const int ETDT_ENABLETAB = ETDT_ENABLE | ETDT_USETABTEXTURE;
      for(i = 0; i < sizeof(pages)/sizeof(pages[0]); i++) {
	(EnableThemeDialogTexture)(pages[i]->page, ETDT_ENABLETAB);
      }
    }
  }

  ZeroMemory(&item1, sizeof(TCITEM));
  ZeroMemory(&item2, sizeof(TCITEM));

  item1.mask = TCIF_TEXT | TCIF_PARAM;
  item1.pszText = "page #1";
  item1.lParam = (LPARAM)0;

  item2.mask = TCIF_TEXT | TCIF_PARAM;
  item2.pszText = "page #2";
  item2.lParam = (LPARAM)0;

  TabCtrl_InsertItem(tab, 1, &item1);
  TabCtrl_InsertItem(tab, 2, &item2);  
  
  SetWindowLongPtr(dialog, DWLP_USER, l_param);
  
  if(window_placement == 0) {
    RECT rect;
    window_placement = (WINDOW_PLACEMENT*)malloc(sizeof(WINDOW_PLACEMENT));
    
    GetWindowRect(dialog, &rect);
    window_placement->x = rect.left;
    window_placement->y = rect.top;
    window_placement->width = rect.right-rect.left;
    window_placement->height = rect.bottom-rect.top;
  }
  else {
    MoveWindow(dialog, window_placement->x, window_placement->y, window_placement->width, window_placement->height, TRUE);
  }
  
  GetClientRect(tab, &rect);
  TabCtrl_AdjustRect(tab, FALSE, &rect);
  MapWindowPoints(tab, dialog, (LPPOINT)&rect, 2);  
  SetWindowPos(pages[0]->page, HWND_TOP, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW);
}

static void
InitializeVersion(HWND dialog, WPARAM w_param, LPARAM l_param) {
  DWORD file_versioninfo_size;
  DWORD version_handle;
  TCHAR path[MAX_PATH];
  
  GetModuleFileName(resource, path, sizeof(path)/sizeof(path[0]));
  file_versioninfo_size = GetFileVersionInfoSize(path, &version_handle);
  
  if(file_versioninfo_size > 0) {
    void* file_versioninfo = malloc(file_versioninfo_size);
    struct {WORD language; WORD codepage;}* translations;
    UINT length = 0;
    TCHAR version_block[] = TEXT("\\StringFileInfo\\12341234\\ProductVersion");
    TCHAR* product_version;
    
    GetFileVersionInfo(path, 0, file_versioninfo_size, file_versioninfo);
    
    VerQueryValue(file_versioninfo, "\\VarFileInfo\\Translation", (void**)&translations, &length);
    
    if(length > 0) {
      wsprintf(version_block, TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"), translations[0].language, translations[0].codepage);
      
      VerQueryValue(file_versioninfo, version_block, (void**)&product_version, &length);
      
      if(length > 0) {
	SetDlgItemText(dialog, ID_VERSION, product_version);
      }
    }
    
    free(file_versioninfo);
  }
}

static BOOL CALLBACK
AboutDialogFunc(HWND dialog, UINT msg, WPARAM wParam, LPARAM lParam) {
  BOOL ret = FALSE;

  switch (msg) {
    case WM_INITDIALOG:
      InitializeVersion(dialog,wParam,lParam);
      ret = TRUE;
      break;
    
    case WM_COMMAND:
      if(LOWORD(wParam) == IDOK) {
	EndDialog(dialog, 1);
	ret = TRUE;
      }
      break;
  }
  
  return ret;
}

static BOOL CALLBACK
DialogFunc(HWND dialog, UINT msg, WPARAM wParam, LPARAM lParam) {
  BOOL ret = FALSE;

  switch(msg) {
    case WM_INITDIALOG:
      InitializeApp(dialog,wParam,lParam);
      ret = TRUE;
      break;

    case WM_COMMAND:
      switch(LOWORD(wParam)) {
	case ID_ABOUT:
	  DialogBox(resource, MAKEINTRESOURCE(DLG_ABOUT), dialog, AboutDialogFunc);
	  break;
	
        case ID_APPLY:
        case IDOK: {
            HKEY key;
	    LRESULT language = 1033;
	    LRESULT old_language = 1033;
	    DWORD hlen;
	    int i;

	    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Z\\diff_ext\\"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
	      hlen = sizeof(DWORD);
	      if(RegQueryValueEx(key, TEXT("language"), 0, NULL, (BYTE*)&old_language, &hlen) != ERROR_SUCCESS) {
		old_language = 1033;
	      }

	      RegCloseKey(key);
	    }
	    
	    for(i = 0; i < sizeof(pages)/sizeof(pages[0]); i++) {
	      pages[i]->apply(pages[i]);
	    }
	    
	    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Z\\diff_ext\\"), 0, KEY_READ, &key) == ERROR_SUCCESS) {
	      hlen = sizeof(DWORD);
	      if(RegQueryValueEx(key, TEXT("language"), 0, NULL, (BYTE*)&language, &hlen) != ERROR_SUCCESS) {
		language = 1033;
	      }

	      RegCloseKey(key);
	    }
	    
	    if((LOWORD(wParam) == IDOK) || (language != old_language)) {
	      EndDialog(dialog, LOWORD(wParam));
	    }
	    
            ret = TRUE;
          }
          break;

        case IDCANCEL:
          EndDialog(dialog, IDCANCEL);
          ret = TRUE;
          break;
      }
      break;

    case WM_GETMINMAXINFO: {
        LAYOUT* layout = (LAYOUT*)GetWindowLongPtr(dialog, DWLP_USER);
        MINMAXINFO* min_max_info = (MINMAXINFO*)lParam;
        RECT client;
        WINDOWINFO info;

        client.top = 0;
        client.bottom = layout->height;
        client.left = 0;
        client.right = layout->width;

        MapDialogRect(dialog, &client);
        GetWindowInfo(dialog, &info);
        AdjustWindowRectEx(&client, info.dwStyle, FALSE, info.dwExStyle);

        min_max_info->ptMinTrackSize.x = client.right-client.left;
        min_max_info->ptMinTrackSize.y = client.bottom-client.top;
        ret = TRUE;
      }
      break;

    case WM_MOVE: {
        RECT rect;
        
        GetWindowRect(dialog, &rect);
        window_placement->x = rect.left;
        window_placement->y = rect.top;
        window_placement->width = rect.right-rect.left;
        window_placement->height = rect.bottom-rect.top;
      }
      break;
      
    case WM_SIZE: {
        RECT rect;
        RECT tab_rect;
        HWND tab = GetDlgItem(dialog, ID_TAB);
        int page = TabCtrl_GetCurSel(tab);
      
	GetClientRect(tab, &tab_rect);

	TabCtrl_AdjustRect(tab, FALSE, &tab_rect);

	MapWindowPoints(tab, dialog, (LPPOINT)&tab_rect, 2);
/**/
        layout(dialog);
      
        MoveWindow(pages[page]->page, tab_rect.left, tab_rect.top, tab_rect.right-tab_rect.left, tab_rect.bottom-tab_rect.top, FALSE);
        layout(pages[page]->page);
/* redraw them all*/
        GetWindowRect(dialog, &rect);
        window_placement->x = rect.left;
        window_placement->y = rect.top;
        window_placement->width = rect.right-rect.left;
        window_placement->height = rect.bottom-rect.top;

/*	RedrawWindow(dialog, 0, 0, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW);*/
        InvalidateRect(dialog, 0, TRUE);

	ret = TRUE;
      }
      break;
      
    case WM_NOTIFY: {
	LPNMHDR data = (LPNMHDR)lParam;

	if(data->code == TCN_SELCHANGE) {
	  if(data->idFrom == ID_TAB) {
	    int page = TabCtrl_GetCurSel(data->hwndFrom);
	    int i;
	    RECT rect;

	    GetClientRect(data->hwndFrom, &rect);
	    TabCtrl_AdjustRect(data->hwndFrom, FALSE, &rect);
	    MapWindowPoints(data->hwndFrom, dialog, (LPPOINT)&rect, 2);
	    
	    for(i = 0; i < sizeof(pages)/sizeof(pages[0]); i++) {
	      if(i == page) {
		SetWindowPos(pages[i]->page, HWND_TOP, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, SWP_SHOWWINDOW);
		layout(pages[i]->page);
		RedrawWindow(pages[i]->page, 0, 0, RDW_ALLCHILDREN | RDW_INVALIDATE);
	      } else {
	        ShowWindow(pages[i]->page, FALSE);
	      }
	    }
	  }
	}
        
        ret = TRUE;
      }
      break;

    case WM_CLOSE:
      EndDialog(dialog,0);
      ret = TRUE;
      break;
  }

  return ret;
}
