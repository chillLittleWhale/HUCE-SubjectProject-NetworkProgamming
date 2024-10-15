#ifndef POP3CLIENT_H_INCLUDED
#define POP3CLIENT_H_INCLUDED
#include "tcpclient.h"
#include <vector>

class Pop3Client : public TcpClient
{
private:
    string username;
    string password;

public:
    string remote_addr;
    string port;
    bool logedin;  //biến để kiểm tra đã đăng nhập chưa

public:
    Pop3Client();
    ~Pop3Client();

    bool openSession(const string &serverHost, unsigned short port = 110); // cổng mặc định cho pop3
    bool closeSession(); // ket thuc phien va dong ket noi

    void setUsername(const string &user);
    void setPassword(const string &pass);
    string getUsername();
    bool login();

    bool showMailList();
    bool getMail(unsigned short sttMail);
    bool markDelete(unsigned short sttMail);
    bool resetSession();

    bool saveAsEmlFile( string& content);
    string getMailID(unsigned short sttMail);
};

#endif // Pop3CLIENT_H_INCLUDED
