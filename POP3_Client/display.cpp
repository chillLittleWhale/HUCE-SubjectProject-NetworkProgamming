#include "display.h"
#include <iomanip>
using namespace std;

void Display::home_screen()
{
    system("cls");
    SetConsoleTextAttribute(console, COLOR_DIRECT);

    cout << "           _______  ________  _______  _______     _______  ___      ___   _______  __    _  _______ " << endl;
    cout << "          |    _  ||        ||    _  ||       |   |       ||   |    |   | |       ||  |  | ||       |" << endl;
    cout << "          |   |_| ||   __   ||   |_| ||___    |   |    ___||   |    |   | |    ___||   |_| ||_     _|" << endl;
    cout << "          |       ||  |  |  ||       | ___|   |   |   |    |   |    |   | |   |___ |       |  |   |" << endl;
    cout << "          |    ___||  |__|  ||    ___||___    |   |   |___ |   |___ |   | |    ___||  _    |  |   |" << endl;
    cout << "          |   |    |        ||   |     ___|   |   |       ||       ||   | |   |___ | | |   |  |   |" << endl;
    cout << "          |_ _|    |________||___|    |_______|   |_______||_______||___| |_______||_|  |__|  |___| v2.0" << endl;
    cout << "" << endl<<endl;

    SetConsoleTextAttribute(console, COLOR_DEFAULT);

    cout << "Các câu lệnh được hỗ trợ trong chương trình:" << endl;
    cout << "1.  open <host> <port>             Mở kết nối đến host a, port x" << endl;                     // Huyền
    cout << "2.  close                          Đóng kết nối với server hiện tại" << endl;                  // Huyền
    cout << "3.  login                          Đăng nhập tài khoản"<<endl;                                 // Linh
    cout << "4.  help                           Hiển thị các lệnh được hỗ trợ và mô tả của chúng" << endl;  // Linh
    cout << "5.  list                           Hiển thị danh sách thư trong hòm thư"<<endl;                // Tâm
    cout << "6.  download                       Tải mail về thư mục Inbox"<<endl;                           // Tâm
    cout << "7.  delete                         Đánh dấu xóa mail"<<endl;                                   // Hoàng
    cout << "8.  reset                          Bỏ đánh dấu toàn bộ các mail định xóa"<<endl;               // Hoàng
    cout << "9.  quit                           Kết thúc chương trình"<<endl<<endl;                         // Thầy =))
}
