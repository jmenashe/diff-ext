#include <util/menu.h>

MENUITEM::MENUITEM() {
}

MENUITEM::MENUITEM(STRING text, HICON icon) : _text(text), _icon(icon) {
}

MENUITEM::MENUITEM(const MENUITEM& item) {
  _text = item._text;
  _icon = item._icon;
}

MENUITEM::~MENUITEM() {
}

void 
MENUITEM::insert(HMENU menu, UINT id, UINT position, BOOL by_position) {
  MENUITEMINFO item_info;
  
  ZeroMemory(&item_info, sizeof(item_info));
  item_info.cbSize = sizeof(MENUITEMINFO);
  
  if(_icon == 0) {
    item_info.fMask = MIIM_ID | MIIM_TYPE;
    item_info.fType = MFT_STRING;
    item_info.wID = id;
    item_info.dwTypeData = _text;
  } else {
    item_info.fMask = MIIM_ID | MIIM_TYPE | MIIM_DATA;
    item_info.fType = MFT_OWNERDRAW;
    item_info.wID = id;
    item_info.dwItemData = (ULONG_PTR)this;
  }
  
  InsertMenuItem(menu, position, by_position, &item_info);  
}

void 
MENUITEM::measure(MEASUREITEMSTRUCT* mis) {
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
        unsigned int icon_width = 0;
        unsigned int icon_height = 0;
        
        if(_icon != 0) {
          ICONINFO ii;
          GetIconInfo(_icon, &ii);
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
        
        GetTextExtentPoint32(dc, _text, lstrlen(_text), &size);
        LPtoDP(dc, (POINT*)&size, 1);

        SelectObject(dc, old_font);
        DeleteObject(font);
        DeleteDC(dc);

        mis->itemWidth = size.cx + 3 + icon_width; //width of string + width of icon + space between ~icon~text~
        mis->itemHeight = max(size.cy, ncm.iMenuHeight);     
        mis->itemHeight = max(mis->itemHeight, icon_height+2)+1;     
      }
    }
  }
}

void 
MENUITEM::draw(DRAWITEMSTRUCT* dis) {
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
  
  if(_icon != 0) {
    ICONINFO ii;
    GetIconInfo(_icon, &ii);
    HBITMAP hb = ii.hbmColor;
    BITMAP bm;
    
    if(hb == 0) {
      hb = ii.hbmMask;
    }
    
    GetObject(hb, sizeof(bm), &bm);
    
    if(hb == ii.hbmMask) {
      bm.bmHeight /= 2;
    }
    
    DrawIconEx(dis->hDC, dis->rcItem.left+1, dis->rcItem.top+1, _icon, bm.bmWidth, bm.bmHeight, 0, 0, DI_NORMAL);
    dis->rcItem.left += bm.bmWidth+3;
  } else {
    dis->rcItem.left += GetSystemMetrics(SM_CXSMICON)+3;
  }
  
  int y;
  SIZE size = {0, 0};
  
  GetTextExtentPoint32(dis->hDC, _text, lstrlen(_text), &size);
//          LPtoDP(dis->hDC, (POINT*)&size, 1);
  y = ((dis->rcItem.bottom - dis->rcItem.top) - size.cy) / 2;
  y = dis->rcItem.top + (y >= 0 ? y : 0);
  ExtTextOut(dis->hDC, dis->rcItem.left, y, ETO_OPAQUE, &dis->rcItem, _text, lstrlen(_text), 0); 
  
  SelectObject(dis->hDC, font);
  if (dis->itemState & ODS_SELECTED) {
    SetTextColor(dis->hDC, text);
    SetBkColor(dis->hDC, background);
  }
}

MENUITEM&
MENUITEM::operator=(const MENUITEM& item) {
  _text = item._text;
  _icon = item._icon;
  
  return *this;
}

SUBMENU::SUBMENU() {
}

SUBMENU::SUBMENU(HMENU menu, STRING text, HICON icon) : MENUITEM(text, icon), _menu(menu) {
}

SUBMENU::SUBMENU(const SUBMENU& menu) : MENUITEM(menu) {
  _menu = menu._menu;
}

void 
SUBMENU::insert(HMENU menu, UINT id, UINT position, BOOL by_position) {
  MENUITEMINFO item_info;
  
  ZeroMemory(&item_info, sizeof(item_info));
  item_info.cbSize = sizeof(MENUITEMINFO);
  
  if(_icon == 0) {
    item_info.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE;
    item_info.fType = MFT_STRING;
    item_info.wID = id;
    item_info.hSubMenu = _menu;
    item_info.dwTypeData = _text;
  } else {
    item_info.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_DATA;
    item_info.fType = MFT_OWNERDRAW;
    item_info.wID = id;
    item_info.hSubMenu = _menu;
    item_info.dwItemData = (ULONG_PTR)this;
  }
    
  InsertMenuItem(menu, position, by_position, &item_info);  
}

SUBMENU&
SUBMENU::operator=(const SUBMENU& menu) {
  MENUITEM::operator=(menu);
    
  _menu = menu._menu;
  
  return *this;
}
