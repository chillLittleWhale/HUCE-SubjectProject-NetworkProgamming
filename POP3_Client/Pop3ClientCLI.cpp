#include "Pop3ClientCLI.h"
#include "display.h"
#include <iostream>
#include <cmath>

Pop3ClientCLI::Pop3ClientCLI():CmdLineInterface("Pop3>")
{
    Display show;
    show.home_screen();
    cmdDefaulID = 0;
    initCmd();
}

void Pop3ClientCLI::initCmd()
{
    addCmd("open",CLI_CAST(&Pop3ClientCLI::doOpen));
    addCmd("close",CLI_CAST(&Pop3ClientCLI::doClose));
    addCmd("login",CLI_CAST(&Pop3ClientCLI::doLogIn));
    addCmd("help",CLI_CAST(&Pop3ClientCLI::doHelp));

    addCmd("list",CLI_CAST(&Pop3ClientCLI::doShowMailList));
    addCmd("download",CLI_CAST(&Pop3ClientCLI::doDownload));
    addCmd("delete",CLI_CAST(&Pop3ClientCLI::doDelete));
    addCmd("reset",CLI_CAST(&Pop3ClientCLI::doReset));
}

void Pop3ClientCLI::doOpen(string cmd_argv[], int cmd_argc){
    Display dp;

    if (cmd_argc == 3)  // open localhost 110
    {
        int port = stoi(cmd_argv[2]);
        bool ok = client.openSession(cmd_argv[1], port);
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Connect to server failed." << endl << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
            return;
        }
        this->cmd_prompt_default = cmd_argv[1];                               //   đặt promt ở đây
    }

    else if (cmd_argc == 2) // open localhost
    {
        int defaultPort = 110;
        bool ok = client.openSession(cmd_argv[1], defaultPort);                   //    đặt cổng mặc định là 110
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Connect to server failed." <<  endl << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
            return;
        }
            this->cmd_prompt_default = cmd_argv[1];                               //   đặt promt ở đây
    }

    else if (cmd_argc == 1)
    {
        int defaultPort = 110;
        string defaultHost = "localhost";
        bool ok = client.openSession(defaultHost, defaultPort);                   //    đặt cổng mặc định là 110
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
            cout << "ERORR: Connect to server failed." <<  endl << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
            return;
        }
        this->cmd_prompt_default = defaultHost;                               //   đặt promt ở đây
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: Command without parameters" << endl << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        return;
    }

    this->setCmdPrompt(this->cmd_prompt_default + cmd_mark_greater);

    SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
    cout << "SUCCESS: Connect to server succeed." << endl << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doClose(string cmd_argv[], int cmd_argc){
    Display dp;
    // dong phien
    bool ok = client.closeSession();
    if (ok)
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
        cout << "SUCCESS: Ket thuc phien POP3 thanh cong" << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_ERROR);
        cout << "ERORR: Ket thuc phien POP3, Server thong bao co loi xay ra" << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    client.close(); // dong localsocket.
    this->setCmdPrompt(cmd_default);
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doLogIn(string cmd_argv[], int cmd_argc){
    Display dp;
    if (cmd_argc == 1)
    {
        string username, password;

        cout << "Username:";
        getline(cin, username);
        client.setUsername(username);

        cout << "Password:";
        getline(cin, password);
        client.setPassword(password);

        bool ok = client.login();
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
            cout << "FAIL: login failed." << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
        else
        {
            this->cmd_prompt_default = username + cmd_mark_at + this->cmd_prompt_default + cmd_mark_colon_tilde;
            //string cmd_prompt = this->cmd_prompt_default + client.working_directory + cmd_mark_greater;
            string cmd_prompt = this->cmd_prompt_default + cmd_mark_greater;
            this->setCmdPrompt(cmd_prompt);

            SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
            cout << "SUCCESS: Đăng nhập thành công" << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: Command without parameters" << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doHelp(string cmd_argv[], int cmd_argc){
    cout << "Các câu lệnh được hỗ trợ trong chương trình:" << endl;
    cout << "1.  open <host> <port>             Mở kết nối đến host a, port x" << endl;
    cout << "2.  close                          Đóng kết nối với server hiện tại" << endl;
    cout << "3.  login                          Đăng nhập tài khoản"<<endl;
    cout << "4.  help                           Hiển thị các lệnh được hỗ trợ và mô tả của chúng" << endl;
    cout << "5.  list                           Hiển thị danh sách thư trong hòm thư"<<endl;
    cout << "6.  download                       Tải mail về thư mục Inbox"<<endl;
    cout << "7.  delete                         Đánh dấu xóa mail"<<endl;
    cout << "8.  reset                          Bỏ đánh dấu toàn bộ các mail định xóa"<<endl;
    cout << "9.  quit                           Kết thúc chương trình"<<endl<<endl;
}

void Pop3ClientCLI::doShowMailList(string cmd_argv[], int cmd_argc){
    Display dp;
    if (cmd_argc == 1)
    {
        bool ok = client.showMailList();
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
            cout << "FAIL: show mail list failed." << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
            cout << "SUCCESS: show mail list thành công" << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: Command without parameters" << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doDownload(string cmd_argv[], int cmd_argc){
    Display dp;
    if (cmd_argc == 2)
    {
        int sttMail = stoi(cmd_argv[1]);
        bool ok = client.getMail(sttMail);
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
            cout << "FAIL: download mail number "<<sttMail<<" failed." << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
            cout << "SUCCESS: download mail number "<<sttMail<<" thành công" << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: form lệnh: down <mail số> " << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doDelete(string cmd_argv[], int cmd_argc){
    Display dp;
    if (cmd_argc == 2)
    {
        int sttMail = stoi(cmd_argv[1]);
        bool ok = client.markDelete(sttMail);
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
            cout << "FAIL: mark mail number "<<sttMail<<" failed." << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
            cout << "SUCCESS: mark mail number "<<sttMail<<" thành công" << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: form lệnh: dele <mail số> " << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

void Pop3ClientCLI::doReset(string cmd_argv[], int cmd_argc){
    Display dp;
    if (cmd_argc == 1)
    {
        bool ok = client.resetSession();
        if (!ok)
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
            cout << "FAIL: reset phiên thất bại." << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
        else
        {
            SetConsoleTextAttribute(dp.console, dp.COLOR_DIRECT);
            cout << "SUCCESS: reset phiên thành công" << endl;
            SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
        }
    }
    else
    {
        SetConsoleTextAttribute(dp.console, dp.COLOR_INFO);
        cout << "INFO: form lệnh: reset> " << endl;
        SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
    }
    cout << endl;
    SetConsoleTextAttribute(dp.console, dp.COLOR_DEFAULT);
}

