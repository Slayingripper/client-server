#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int fortune_cookie();
int encryption_decryption();
int main_menu(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    system("clear");
    
    while(1) {
        main_menu(argc, argv);
    }

    return 1;
}

// Show fortune cookie menu
int fortune_cookie() {
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    //puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    //keep communicating with server
    while(1)
    {
        printf("\n----------------------------------------------------\n");    
        printf(" How many fortune cookies do you need?");
        printf("\n----------------------------------------------------\n\n");
        
        scanf("%s" , message);
        
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            quick_exit();
        }

        printf("Server reply :");
        
        char *pch = strstr(server_reply, "#");
        pch++; //Remove the first char

        int server_reply_len = atoi(pch);
        printf(" %.*s\n", server_reply_len, server_reply);
    }

    return 0;
}

// Show encryption decryption menu
int encryption_decryption() {
    char e_or_d, ed_mode;

    int done = 0;
    while(!done){
        printf("\n----------------------------------------------------\n");
        printf("e) Encryption\nd) Decryption\nc) Close");
        printf("\n----------------------------------------------------\n\n");    

        scanf(" %c", &e_or_d);

        switch (tolower(e_or_d)) {
            case 'e':
            printf("\n(---Encryption---)\n");
            ed_mode = 'e';
            break;
            case 'd':
            printf("\n(---Decryption---)\n");
            ed_mode = 'd';
            break;
            case 'c':
            done = 1;
            break;
            default :
            printf("\nPlease enter either a or b or c.\n");
            break;
        }

        if (e_or_d == 'e' || e_or_d == 'd') {

            int sock;
            struct sockaddr_in server;
            char message[1000] , server_reply_ed[1000];

            //Create socket
            sock = socket(AF_INET , SOCK_STREAM , 0);
            if (sock == -1)
            {
                printf("Could not create socket");
            }
            puts("Socket created");

            server.sin_addr.s_addr = inet_addr("127.0.0.1");
            server.sin_family = AF_INET;
            server.sin_port = htons(8888);

            //Connect to remote server
            if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
            {
                perror("connect failed. Error");
                return 1;
            }

            puts("Connected\n");

            printf("Enter message : ");
            scanf(" %[^\n]s",message);
            
            //Add mode char ( e for encryption and d for decryption)
            message[strlen(message)] = ed_mode;

            //Send some data
            if( send(sock , message , strlen(message) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }

            //Receive a reply from the server
            if( recv(sock , server_reply_ed , 1000 , 0) < 0)
            {
                puts("recv failed");
                break;
            }

            printf("Server reply :");
            int msg_len = (int) strlen(message) - 1;
            printf(" %.*s\n", msg_len, server_reply_ed);

            close(sock);
        }
    }

    return 0;
}
// Show Main Menu
int main_menu(int argc, char *argv[]) {
    char main_menu_char;
    printf("\n----------------------------------------------------\n");
    printf(" What do you want to do?");
    printf("\n----------------------------------------------------\n");
    printf("\na) interact with the Fortune Cookie Server\nb) interact with the encryption/decryption server\nc) quit\n");
    printf("\n----------------------------------------------------\n\n");

    scanf("%s", &main_menu_char);
    
    getchar();
    
    switch (tolower(main_menu_char)) {
        case 'a':
        fortune_cookie();
        break;
        case 'b':
        encryption_decryption();
        break;
        case 'c':
        quick_exit();
        break;
        default :
        printf("\nPlease enter either a or b or c.\n");
        break;
    }
    
    return 1;
}