/*
 * Copyright (c) 2006-2008, Sergey Zorin. All rights reserved.
 *
 * This software is distributable under the BSD license. See the terms
 * of the BSD license in the LICENSE file provided with this software.
 *
 */

#include <util/menu.h>

MENUITEM::MENUITEM() : _text(TEXT("")), _icon(0), _id(0) {
}

MENUITEM::MENUITEM(UINT id, STRING text, HICON icon) : _text(text), _icon(icon), _id(id) {
}

MENUITEM::MENUITEM(UINT id, STRING text, ICON icon) : _text(text), _icon(icon), _id(id) {
}

MENUITEM::MENUITEM(const MENUITEM& item) : _text(item._text), _icon(item._icon), _id(item._id) {
}

MENUITEM::~MENUITEM() {
}

MENUITEMINFO* 
MENUITEM::item_info() {
  MENUITEMINFO* result = new MENUITEMINFO;
  
  ZeroMemory(result, sizeof(MENUITEMINFO));
  result->cbSize = sizeof(MENUITEMINFO);
  
  if(_icon == 0) {
    result->fMask = MIIM_ID | MIIM_TYPE;
    result->fType = MFT_STRING;
    result->wID = _id;
    result->dwTypeData = _text;
  } else {
    result->fMask = MIIM_ID | MIIM_FTYPE | MIIM_DATA | MIIM_BITMAP | MIIM_STRING;
    result->fType = MFT_STRING;
    result->wID = _id;
    result->dwTypeData = _text;
    result->cch = _tcslen(_text);
    result->hbmpItem = HBMMENU_CALLBACK;
    result->dwItemData = (ULONG_PTR)this;
  }

  return  result;
}

void 
MENUITEM::measure(MEASUREITEMSTRUCT* mis) {
  int icon_width;
  int icon_height;
  
  mis->itemWidth += 10;
  
  ICONINFO ii;
  if(GetIconInfo(_icon, &ii) != 0) {
    HBITMAP hb = ii.hbmColor;
    BITMAP bm;
    
    if(hb == 0) {
      hb = ii.hbmMask;
    }
    
    if(GetObject(hb, sizeof(bm), &bm) != 0) {            
      if(hb == ii.hbmMask) {
        bm.bmHeight /= 2;
      }
      
      icon_width = bm.bmWidth;
      icon_height = bm.bmHeight;
    }
      
    if(ii.hbmColor != 0) {
      DeleteObject(ii.hbmColor);
    }
    
    if(ii.hbmMask != 0) {
      DeleteObject(ii.hbmMask);
    }
  }
  
  if (mis->itemHeight < icon_height) {
    mis->itemHeight = icon_height;
  }
}

void 
MENUITEM::draw(DRAWITEMSTRUCT* dis) {
  int icon_width;
  int icon_height;
    
  ICONINFO ii;
  if(GetIconInfo(_icon, &ii) != 0) {
    HBITMAP hb = ii.hbmColor;
    BITMAP bm;
    
    if(hb == 0) {
      hb = ii.hbmMask;
    }
    
    if(GetObject(hb, sizeof(bm), &bm) != 0) {            
      if(hb == ii.hbmMask) {
        bm.bmHeight /= 2;
      }
      
      icon_width = bm.bmWidth;
      icon_height = bm.bmHeight;
    }
      
    if(ii.hbmColor != 0) {
      DeleteObject(ii.hbmColor);
    }
    
    if(ii.hbmMask != 0) {
      DeleteObject(ii.hbmMask);
    }
  }
  
  //~ DrawIcon(dis->hDC, 
    //~ dis->rcItem.left - icon_width + 5, dis->rcItem.top + (dis->rcItem.bottom - dis->rcItem.top - icon_height) / 2,
    //~ _icon);
  DrawIconEx(dis->hDC, 
    dis->rcItem.left - icon_width + 5, dis->rcItem.top + (dis->rcItem.bottom - dis->rcItem.top - icon_height) / 2,
    _icon, icon_width, icon_height,
    0, NULL, DI_NORMAL);
}

MENUITEM&
MENUITEM::operator=(const MENUITEM& item) {
  _text = item._text;
  _icon = item._icon;
  _id = item._id;
  
  return *this;
}

SUBMENU::SUBMENU() : _own_menu(true) {
  _menu = CreateMenu();
}

SUBMENU::SUBMENU(HMENU menu, UINT id, STRING text, HICON icon) : MENUITEM(id, text, icon), _menu(menu), _own_menu(false) {
  if(menu == 0) {
    _menu = CreateMenu();
    _own_menu = true;
  }
}

SUBMENU::SUBMENU(HMENU menu, UINT id, STRING text, ICON icon) : MENUITEM(id, text, icon), _menu(menu), _own_menu(false) {
  if(menu == 0) {
    _menu = CreateMenu();
    _own_menu = true;
  }
}

SUBMENU::SUBMENU(const SUBMENU& menu) : MENUITEM(menu) {
  _menu = menu._menu;
  _own_menu = menu._own_menu;
}

SUBMENU::~SUBMENU() {
  if(_own_menu) {
    DestroyMenu(_menu);
  }
}

void 
SUBMENU::insert(MENUITEM& item, UINT position) {
  MENUITEMINFO* info = item.item_info();
  
  if(info != 0) {
    InsertMenuItem(_menu, position, TRUE, info);
    
    delete info;
  }
}

void 
SUBMENU::append(MENUITEM& item, UINT id) {
  MENUITEMINFO* info = item.item_info();
  
  if(info != 0) {
    InsertMenuItem(_menu, id, FALSE, info);
    
    delete info;
  }
}

SUBMENU&
SUBMENU::operator=(const SUBMENU& menu) {
  MENUITEM::operator=(menu);
    
  _menu = menu._menu;
  _own_menu = menu._own_menu;
  
  return *this;
}

MENUITEMINFO* 
SUBMENU::item_info() {
  MENUITEMINFO* result = MENUITEM::item_info();
  
  if(result != 0) {
    result->fMask |= MIIM_SUBMENU;
    result->hSubMenu = _menu;
  }
  
  return result;
}

extern "C" {
void* 
create_menu_item(UINT id, LPTSTR text, HICON icon) {
  return new MENUITEM(id, text, icon);
}

void* 
create_submenu(HMENU menu, UINT id, LPTSTR text, HICON icon) {
  return new SUBMENU(menu, id, text, icon);
}

void
delete_menu_item(void* item) {
  delete (MENUITEM*)item;
}
  
void
insert(void* menu, void* item/*MENUITEM or SUBMENU handle*/, UINT position) {
  ((SUBMENU*)menu)->insert(*(MENUITEM*)item, position);
}

void
append(void* menu, void* item/*MENUITEM or SUBMENU handle*/, UINT id) {
  ((SUBMENU*)menu)->append(*(MENUITEM*)item, id);
}

void
measure(void* menuitem, MEASUREITEMSTRUCT* mis) {
  ((MENUITEM*)menuitem)->measure(mis);
}

void
draw(void* menuitem, DRAWITEMSTRUCT* dis) {
  ((MENUITEM*)menuitem)->draw(dis);
}
}
