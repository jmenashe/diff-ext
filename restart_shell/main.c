#include <windows.h>

int APIENTRY
WinMain(HINSTANCE __not_used_1__, HINSTANCE __not_used_2__, LPSTR __not_used_3__, int __not_used_4__) {
  int ret;
  BOOL post_result;
  
  HWND hwnd = FindWindow("Progman", NULL);
  if(PostMessage(hwnd, WM_QUIT, 0, 0) == 0) {
    LPTSTR message;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);
    MessageBox(0, message, TEXT("PostMessage error"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
  }
  
  Sleep(1000);
  
  ret = (int)ShellExecute(NULL, NULL, "explorer.exe", NULL, NULL, SW_SHOW);
  if(ret <= 32) {
    LPTSTR message;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0,
      GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &message, 0, 0);
    MessageBox(0, message, TEXT("ShellExecute error"), MB_OK | MB_ICONINFORMATION);

    LocalFree(message);
  }
  
  return 0;
}
