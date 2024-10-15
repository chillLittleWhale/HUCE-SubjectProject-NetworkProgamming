#include <iostream>
#include "Pop3ClientCLI.h"
#include <windows.h>  // thêm thư viện này cho SetConsoleOutputCP
#include <gdiplus.h>

using namespace std;

int main()
{
   // Gdiplus::GdiplusStartupInput gdiplusStartupInput;
   // ULONG_PTR gdiplusToken;
    //Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    SetConsoleOutputCP(65001); //  thiết lập mã hóa xuất ra của cửa sổ console thành UTF-8 (một định dạng mã hóa ký tự Unicode, được sử dụng để hỗ trợ nhiều ngôn ngữ và ký tự trên thế giới)
    Pop3ClientCLI app;
    app.run();

    //Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

