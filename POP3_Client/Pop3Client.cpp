#include "Pop3Client.h"
#include "display.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <cstring>
#include <algorithm>    // Để sử dụng replace
#include <fstream>
#include <windows.h> // Hàm CreateDirectory

using namespace std;

Pop3Client::Pop3Client()
{
    logedin = false;
}

Pop3Client::~Pop3Client()
{
}

void Pop3Client::setUsername(const string &name)
{
    this->username = name;
}

string Pop3Client::getUsername()
{
    return this->username;
}
void Pop3Client::setPassword(const string &pass)
{
    this->password = pass;
}

bool Pop3Client::login()
{
    Display dp;
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    try
    {// cac buoc trong giao thuc de xac thuc tai khoan
        string request;
        string response;
        char respBuffer[256];

        // Send USER
        request = "USER " + username + "\r\n";
        sendStringRequest(request);
        // kiểm tra phản hồi
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            cout << "Server response: " + response << endl;

            // kiem tra ma phan hoi trong response
            if (response.find("+OK") != 0)
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERORR: Pop3Client/ login()/ send(USER): userName bạn nhập không tồn tại" << endl;
                return false;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Pop3Client/ login()/ send(USER): không nhận được phản hồi" << endl;
            return false;
        }

        // Send PASSWORD
        request = "PASS " + password + "\r\n";
        sendStringRequest(request);
        // Kiểm tra phản hồi
        byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            cout << "Server response: " + response << endl;
            // kiem tra ma phan hoi trong response
            if (response.find("+OK") != 0)
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERORR: Pop3Client/ login()/ send(PASS): mật khẩu không chính xác" << endl;
                return false;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Pop3Client/ login()/ send(PASS): không nhận được phản hồi" << endl;
            return false;
        }
    }
    catch (SocketException &e)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Pop3Client/ login():  catch SocketException " << endl;
        return false;
    }

    logedin = true;
    return true;

}

bool Pop3Client::openSession(const string &serverHost, unsigned short port)
{
    Display dp;
    string response;
    bool sessionOpen;
    // Mo ket noi den Server
    bool connected = open(serverHost, port);
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Pop3Client.cpp/ openSession: open() failed" << endl;
        return false;
    }

    // nhan response trang thai cua Server
    char respBuffer[256];
    try
    {
        int byte_recv = recvGetLine(respBuffer, 256);

        if (byte_recv > 0){
            response = respBuffer;
            cout << "Server response: "; cout << response << endl;

            if (response.find("+OK") == 0){ // xuất hiện ở ngay vị trí đầu
                remote_addr = serverHost;
                sessionOpen = true;
            }
            else
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERORR: Pop3Client.cpp/ openSession: server hết cứu" << endl;
                sessionOpen = false;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Pop3Client.cpp/ openSession: không nhận được phản hồi " << endl;
            sessionOpen = false;
        }
    }

    catch (SocketException &e)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Pop3Client.cpp/ openSession: catch SocketException" << endl;
        return false;
    }

    return sessionOpen;
}

bool Pop3Client::closeSession()
{
    Display dp;
    if (!connected){
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    bool sessionClose;
    string response;
    try
    {
        sendStringRequest("QUIT\r\n");
        char respBuffer[256];

        // nhan response tu Server
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            cout << "Server response: " + response << endl;
            // kiem tra ma phan hoi trong response
            if (response.find("+OK") == 0)
            {
                sessionClose = true;
            }
            else
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERORR: Pop3Client.cpp/ closeSession: server bất ổn" << endl;
                sessionClose = false;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Pop3Client/ closeSession: không nhận được phản hồi" << endl;
            sessionClose = false;
        }
    }
    catch (SocketException &e)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Pop3Client.cpp/ closeSession: catch SocketException" << endl;
        return false;
    }
    return sessionClose;
}

bool Pop3Client::showMailList()
{
    Display dp;
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }
        if (!logedin)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not login to any account yet!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    string response;
    int mailCount = 0;
    int octetCount = 0;
    bool showMail;

    try
    {
        sendStringRequest("STAT\r\n");
        char respBuffer[256];
        int byte_recv = recvGetLine(respBuffer, 256);
        if (byte_recv > 0)
        {
            response = respBuffer;
            //cout << "Server response: " + response << endl;
            if (response.find("+OK") == 0)
            {
                size_t spacePos = response.find(' ', 4); // Bắt đầu tìm từ vị trí thứ 4 (sau "+OK ")
                if (spacePos != string::npos) {
                    string mailCountStr = response.substr(4, spacePos - 4);     // Lấy ra phần số lượng thư
                    mailCount = stoi(mailCountStr);

                    string octetCountStr = response.substr(spacePos + 1);       // Lấy ra phần tổng số octet
                    octetCount = stoi(octetCountStr);

                }
            }
            else
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERORR: Pop3Client.cpp/ showMailList: server bất ổn" << endl;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Pop3Client/ showMailList: không nhận được phản hồi" << endl;
        }
    }
    catch (SocketException &e)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Pop3Client.cpp/ showMailList: catch SocketException" << endl;
        return false;
    }


    try
    {
        sendStringRequest("LIST\r\n");
        // Nhận dữ liệu từ server
        char respBuffer[1024]; // Tăng kích thước buffer để đảm bảo nhận đủ dữ liệu
        int totalBytesReceived = 0;
        int bytesReceived;

        do {
            bytesReceived = recvStringBuffer(respBuffer + totalBytesReceived, sizeof(respBuffer) - totalBytesReceived);
            if (bytesReceived > 0) {
                totalBytesReceived += bytesReceived;
            }
        } while ((strchr(respBuffer, '\n.') == nullptr)); // còn chưa thấy chấm hết thì còn nhận buffer

        if (totalBytesReceived > 0)
        {
            respBuffer[totalBytesReceived] = '\0'; // Đảm bảo kết thúc chuỗi
            response = respBuffer;

            //cout << "Server response: " << response << endl;

            // Kiểm tra mã phản hồi trong response
            if (response.find("+OK") == 0)
            {   // xử lý để bỏ dòng +ok đầu tiên và dòng . cuối cùng
                size_t pos = response.find("\n");             // Tìm vị trí của kí tự xuống dòng '\n' đầu tiên
                if (pos != string::npos) {
                    response = response.substr(pos + 1);      // Bỏ qua dòng "+OK"
                }
                else {
                    SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                    cout << "ERROR: Pop3Client.cpp/ showMailList: Dòng chứa danh sách mail không hợp lệ" << endl;
                    showMail = false;
                }

                response.erase(response.find_last_of('.') ); // xóa hết tất cả ở sau, bắt đầu từ dấu chấm cuối
                //cout <<"phản hổi sau khi được xử lý: " << response << endl;
/*
                // Xử lý danh sách mail, lấy tổng của số mail, số octet
                istringstream iss(response);
                string line;
                while (getline(iss, line))
                {
                    size_t firstSpace = line.find_first_of(' ');      //cout << "vị trí khoảng trắng đầu:" << firstSpace << endl;
                    int octet = stoi(line.substr(firstSpace + 1));

                    mailCount++;
                    mailOctet += octet;
                }
*/
                // Hiển thị danh sách mail
                cout<<"\nTổng số mail: " << mailCount<< ", dung lượng: " << octetCount <<endl;
                istringstream iss2(response);
                string line2;

                while (getline(iss2, line2))
                {
                    size_t firstSpace = line2.find_first_of(' ');
                    int stt   = stoi(line2.substr(0, firstSpace));
                    int octet = stoi(line2.substr(firstSpace + 1));
                    cout << "Mail " << stt << ": ";
                    cout << octet << " octets   ";
                    cout << "ID: " << getMailID(stt)<< endl;

                }

                showMail = true;
            }
            else
            {
                SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
                cout << "ERROR: Pop3Client.cpp/showMailList: server không ổn định" << endl;
                showMail = false;
            }
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERROR: Pop3Client.cpp/showMailList: không nhận được phản hồi" << endl;
            showMail = false;
        }
    }
    catch (SocketException &e)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERROR: Pop3Client.cpp/showMailList: catch SocketException" << endl;
        return false;
    }

    return showMail;
}

bool Pop3Client::getMail(unsigned short sttMail)
{
    Display dp;
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }
        if (!logedin)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not login to any account yet!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    string response;
    bool gotMail;
    int tryTime = 2;
    int maloi = 0;

    do{          // phải lấy response 2 lần vì lần 1 thường bị sót response từ các câu lệnh trước
        maloi = 0;
        tryTime --;
        try
        {
            string request = "RETR " + to_string(sttMail) +"\r\n";
            sendStringRequest(request);
            // Nhận dữ liệu từ server
            char respBuffer[3072];                              // Tăng kích thước buffer để đảm bảo nhận đủ dữ liệu
            int totalBytesReceived = 0;
            int bytesReceived;

            do {
                bytesReceived = recvStringBuffer(respBuffer + totalBytesReceived, sizeof(respBuffer) - totalBytesReceived);
                if (bytesReceived > 0) {
                    totalBytesReceived += bytesReceived;
                }
            } while ((strchr(respBuffer, '\n.') == nullptr));         // còn chưa thấy chấm hết thì còn nhận buffer

            if (totalBytesReceived > 0)
            {
                respBuffer[totalBytesReceived] = '\0';                     // Đảm bảo kết thúc chuỗi
                response = respBuffer;
                //cout << "Server response: " << response << endl;

                // Kiểm tra mã phản hồi trong response
                if (response.find("+OK") == 0)
                {
                    cout << "Server response: " << response << endl;

                    if( saveAsEmlFile(response) ){               // Tạo và lưu nội dung thư vào tệp .eml
                        gotMail = true;

                    }
                    else{
                        maloi = 1;   // lỗi tạo và lưu file eml
                        gotMail = false;
                    }
                }
                else
                {
                    maloi = 2;    // lỗi response không +OK
                    gotMail = false;
                }
            }
            else
            {
                maloi = 3;      // lỗi không nhận được phản hồi của server
                gotMail = false;
            }
        }
        catch (SocketException &e)
        {
            maloi = 4;    // gặp SocketException
            gotMail = false;
        }
    }while((tryTime >=0) && (gotMail == false) );

    SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
    switch(maloi)
    {
    case 1:
        cout << "ERROR: Pop3Client.cpp/ getMail: lỗi khi tạo và lưu file" << endl; break;
    case 2:
        cout << "ERROR: Pop3Client.cpp/ getMail: Server không ổn định" << endl; break;
    case 3:
        cout << "ERROR: Pop3Client.cpp/ getMail: không nhận được phản hồi từ phía Server" << endl; break;
    case 4:
        cout << "ERROR: Pop3Client.cpp/ getMail: catch SocketException" << endl; break;
    default:
        break;
    }
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);

    return gotMail;
}

bool Pop3Client::markDelete(unsigned short sttMail)
{
    Display dp;
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }
        if (!logedin)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not login to any account yet!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    string response;
    bool markMail;
    int tryTime = 200; // cần nhiều lần vì nếu k may mail trước nhiều dòng thì lặp k hết
    int maloi = 0;

    do{          // phải lấy response vài lần vì lần 1 thường bị sót response từ các câu lệnh trước
        maloi = 0;
        tryTime --;

        try
        {
            string request = "DELE " + std::to_string(sttMail) +"\r\n";
            sendStringRequest(request);
            // Nhận dữ liệu từ server
            char respBuffer[256];
            int byte_recv = recvGetLine(respBuffer, 256);

            if (byte_recv > 0)
            {
                response = respBuffer;
                //cout << "Server response: " << response << endl;

                // Kiểm tra mã phản hồi trong response
                if (response.find("+OK Message number") == 0)
                {
                    cout << "Server response: " << response << endl;
                    markMail = true;
                }
                else
                {
                    maloi = 1;
                    markMail = false;
                }
            }
            else
            {
                maloi = 2;
                markMail = false;
            }
        }
        catch (SocketException &e)
        {
            maloi = 3;
            return false;
        }
    }
    while((tryTime >=0) && (markMail == false) );

    SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
    switch(maloi)
    {
    case 1:
        cout << "ERROR: Pop3Client.cpp/ markDelete: server không ổn định" << endl; break;
    case 2:
        cout << "ERROR: Pop3Client.cpp/ getMail: Không nhận được phản hồi từ phía Server" << endl; break;
    case 3:
        cout << "ERROR: Pop3Client.cpp/ markDelete: catch SocketException" << endl; break;
    default:
        break;
    }
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);

    return markMail;
}

bool Pop3Client::resetSession()
{
    Display dp;
    if (!connected)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not connected to server!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }
        if (!logedin)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        print("INFO: Not login to any account yet!");
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    string response;
    bool resetDone;
    int tryTime = 20;
    int maloi = 0;

    do{          // phải lấy response vài lần vì lần 1 thường bị sót response từ các câu lệnh trước
        maloi = 0;
        tryTime --;

        try
        {
            sendStringRequest("RSET\r\n");
            // Nhận dữ liệu từ server
            char respBuffer[256];
            int byte_recv = recvGetLine(respBuffer, 256);

            if (byte_recv > 0)
            {
                response = respBuffer;
                //cout << "Server response: " << response << endl;

                // Kiểm tra mã phản hồi trong response
                if (response == "+OK") // cái phải hồi này chỉ có "+OK" mới là chính xác, k có phần theo sau
                {
                    cout << "Server response: " << response << endl;
                    resetDone = true;
                }
                else
                {
                    maloi = 1;
                    resetDone = false;
                }
            }
            else
            {
                maloi = 2;
                resetDone = false;
            }
        }
        catch (SocketException &e)
        {
            maloi = 3;
            return false;
        }
    }
    while((tryTime >=0) && (resetDone == false) );

    SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
    switch(maloi)
    {
    case 1:
        cout << "ERROR: Pop3Client.cpp/ resetSession: server không ổn định" << endl;
    case 2:
        cout << "ERROR: Pop3Client.cpp/ resetSession: không nhận được phản hồi" << endl; break;
    case 3:
        cout << "ERROR: Pop3Client.cpp/ resetSession: catch SocketException" << endl; break;
    default:
        break;
    }
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);

    return resetDone;
}

bool Pop3Client::saveAsEmlFile( string &response)
{
    Display dp;
    size_t firstDate = response.find("Date");
    //cout << "\nVị trí của date: "<<firstDate;
    response = response.substr(firstDate + 1 +5);                 // lấy từ ngày tháng đổ đi
    //cout<<"\nRespon từ ngày tháng đổ đi: " << response;
    size_t firstNN = response.find("\n");            // đoạn xuống dòng giữa date và nội dung thư
    string date =  response.substr(0, firstNN -1);                //
    //cout<<"\nNgày tháng: " << date;
    string content = response.substr(firstNN + 1 +2);    // thêm 2 cho phần \n
    //cout<<"\nNội dung: " << content;

    string fileName = "Mail-" +getUsername() +"-" +date +".eml";            // tạo tên thư mục
    replace(fileName.begin(), fileName.end(), ' ', '_');                // Thay thế khoảng trắng bằng dấu gạch dưới
    replace(fileName.begin(), fileName.end(), ':', '-');                // Thay thế dấu ":" bằng dấu "-"

    // Tạo thư mục nếu chưa tồn tại
    string inboxPath = "C:\\Pop3_Inbox\\";                              // Đường dẫn đầy đủ cho thư mục Inbox
    if (CreateDirectory(inboxPath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        //cout << "Directory created: " << inboxPath << endl;
    } else {
        cerr << "Error creating directory: " << inboxPath << endl;
        return false;
    }

    string fullPath = inboxPath + fileName;                                 // Đường dẫn đầy đủ cho tệp .eml
    //cout << "\nFull Path: " << fullPath << endl;

    SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
    std::ofstream file(fullPath);        // Mở tệp .eml để ghi
    if (!file.is_open()) {
        cerr << "Error when opening file: " << fullPath << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    file << content;                    // Ghi nội dung thư vào tệp
    if (file.fail()) {
        cerr << "Error when writing to file: " << fullPath << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    file.close();                      // Đóng tệp
    if (file.fail()) {
        cerr << "Error when closing file: " << fullPath << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return false;
    }

    return true;
}

string Pop3Client::getMailID(unsigned short sttMail)
{
    Display dp;
    string response;
    string id ="Lỗi khi lấy Id";
    int tryTime = 2;
    do{ tryTime --;
        try
        {
            string request = "RETR " + to_string(sttMail) +"\r\n";
            sendStringRequest(request);
            char respBuffer[2048];
            int totalBytesReceived = 0;
            int bytesReceived;

            do {
                bytesReceived = recvStringBuffer(respBuffer + totalBytesReceived, sizeof(respBuffer) - totalBytesReceived);
                if (bytesReceived > 0) {
                    totalBytesReceived += bytesReceived;
                }
            } while ((strchr(respBuffer, '\n.') == nullptr));

            if (totalBytesReceived > 0)
            {
                respBuffer[totalBytesReceived] = '\0';
                response = respBuffer;
                if (response.find("+OK") == 0)
                {
                    size_t firstPos = response.find('<');
                    id = response.substr(firstPos);
                    size_t lastPos  = id.find('>');
                    id = id.substr(0, lastPos +1);
                }
            }
        }
        catch (SocketException &e){}
    } while((tryTime >=0) && (id =="Lỗi khi lấy Id"));

    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    return id;
}


