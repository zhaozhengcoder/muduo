#include <stdio.h>
#include <string.h>
#include <examples/ace/ttcp/common.h>

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("error \n");
        return -1;
    }
    else
    {
        if(strcmp(argv[1],"recv")==0)
        {
            printf("recv \n");
            Options recv_option;
            recv_option.port = 5002;
            receive(recv_option);
        }
        else if(strcmp(argv[1],"trans")==0)
        {
            printf("trans \n");
            Options trans_option;
            trans_option.port = 5002;
            trans_option.length = 2000;
            trans_option.number = 200;
            trans_option.host = "DESKTOP-0F08SUS";
            transmit(trans_option);
        }
        else
        {
            printf("error \n");
            return -1;
        }

    }
    printf("ok  \n");

    return 0;
}