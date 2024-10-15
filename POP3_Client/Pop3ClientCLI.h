#ifndef POP3CLIENTCLI_H
#define POP3CLIENTCLI_H

#include "cli.h"
#include "tcpsocket.h"
#include "Pop3Client.h"

#define cmd_default "Pop3>"
#define cmd_mark_colon_tilde ":~"
#define cmd_mark_greater ">"
#define cmd_mark_at "@"

class Pop3ClientCLI : public CmdLineInterface
{
    private:
        Pop3Client client;
    public:
        string cmd_prompt_default;
        Pop3ClientCLI();
        void initCmd();

    private:
        void doOpen(string cmd_argv[], int cmd_argc);
        void doClose(string cmd_argv[], int cmd_argc);
        void doHelp(string cmd_argv[], int cmd_argc);

        void doLogIn(string cmd_argv[], int cmd_argc);
        void doShowMailList(string cmd_argv[], int cmd_argc);
        void doDownload(string cmd_argv[], int cmd_argc);
        void doDelete(string cmd_argv[], int cmd_argc);
        void doReset(string cmd_argv[], int cmd_argc);
};

#endif
