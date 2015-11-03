#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>

char *getFortune(int index);
int getRand(int low, int high);
void encrypt(char *str, char *q);
void decrypt(char *str, char *q);
void *connection_handler(void *);
int fortune_ids[55];

int main(int argc , char *argv[])
{
    pid_t  pid;

    pid = fork();
    if (pid == 0) 
        fortune_server();
    else 
        encdec_server();

    return 0;
}

/*
 * 
 * Fortune Server
 * 
 */
 int fortune_server(){
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2];
    
    srand(time(0));
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Fortune Server : Could not create socket");
    }
    puts("Fortune Server : Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Fortune Server : bind failed. Error");
        return 1;
    }
    puts("Fortune Server : bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Fortune Server : Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Fortune Server : accept failed");
        return 1;
    }
    puts("Fortune Server : Connection accepted");

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2 , 0)) > 0 )
    {

        int num_fortune_cookie = atoi(client_message);
        int fc_len = 0;
        int i;
        for (i = 0; i < num_fortune_cookie; ++i)
        {
            //Send the message back to client
            if (i < num_fortune_cookie)
            {
                //Send the message back to client
                int rnd = getRand(0,55);
                char *fortunes = getFortune(rnd);

                //Get the size  of the fortune cookie
                fc_len += (int)strlen(fortunes);

                write(client_sock , fortunes , strlen(fortunes));
            }
        }

        char fc_len_str[15];
        sprintf(fc_len_str, "#%d", fc_len);
        write(client_sock , fc_len_str , strlen(fc_len_str));
    }

    if(read_size == 0)
    {
        puts("Fortune Server : Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("Fortune Server : recv failed");
    }

    return 0;
}

/*
 * 
 * Array of fortunes
 * 
 */
 char *getFortune(int index) {

    char *fortunes[] = {
        "Adventure can be real happiness.\n",
        "Advice is like kissing. It costs nothing and is a pleasant thing to do.\n",
        "Advice, when most needed, is least heeded.\n",
        "All your hard work will soon pay off.\n",
        "Allow compassion to guide your decisions.\n",
        "An agreeable romance might begin to take on the appearance.\n",
        "An important person will offer you support.\n",
        "An inch of time is an inch of gold.\n",
        "Any decision you have to make tomorrow is a good decision.\n",
        "At the touch of love, everyone becomes a poet.\n",
        "Be careful or you could fall for some tricks today.\n",
        "Beauty in its various forms appeals to you.\n",
        "Because you demand more from yourself, others respect you deeply.\n",
        "Believe in yourself and others will too.\n",
        "Believe it can be done.\n",
        "Better ask twice than lose yourself once.\n",
        "Carve your name on your heart and not on marble.\n",
        "Change is happening in your life, so go with the flow!\n",
        "Competence like yours is underrated.\n",
        "Congratulations! You are on your way.\n",
        "Could I get some directions to your heart?\n",
        "Courtesy begins in the home.\n",
        "Courtesy is contagious.\n",
        "Don’t confuse recklessness with confidence.\n",
        "Don’t just spend time. Invest it.\n",
        "Don’t just think, act!\n",
        "Don’t let friends impose on you, work calmly and silently.\n",
        "Don’t let your limitations overshadow your talents.\n",
        "Don’t worry; prosperity will knock on your door soon.\n",
        "Each day, compel yourself to do something you would rather not do.\n",
        "Education is the ability to meet life’s situations.\n",
        "Emulate what you admire in your parents.\n",
        "Emulate what you respect in your friends.\n",
        "Every flower blooms in its own sweet time.\n",
        "Everyday in your life is a special occasion.\n",
        "Failure is the chance to do better next time.\n",
        "Fear and desire — two sides of the same coin.\n",
        "Feeding a cow with roses does not get extra appreciation.\n",
        "For hate is never conquered by hate. Hate is conquered by love.\n",
        "Fortune Not Found: Abort, Retry, Ignore?\n",
        "From listening comes wisdom and from speaking repentance.\n",
        "From now on your kindness will lead you to success.\n",
        "Get your mind set — confidence will lead you on.\n",
        "Get your mind set…confidence will lead you on.\n",
        "Go take a rest; you deserve it.\n",
        "Good news will be brought to you by mail.\n",
        "Good news will come to you by mail.\n",
        "Good to begin well, better to end well.\n",
        "Happiness begins with facing life with a smile and a wink.\n",
        "Happiness will bring you good luck.\n",
        "Happy life is just in front of you.\n",
        "Hard words break no bones, fine words butter no parsnips.\n",
        "Have a beautiful day."                                                                 
        "Show everyone what you can do.\n", 
        "Respect your elders. You could inherit a large sum of money.\n", 
        "Move quickly. Now is the time to make progress.\n" };

        return fortunes[index];
    }

    int checkRand(int _rnd)
    {
        int i;
        for (i = 0; i < 55; ++i)
        {
            if (fortune_ids[i] == _rnd)
            {
                return 1;
            }
        }
        return 0;
    }

    int getRand(int low, int high)
    {
        int rnd;
        while(1){
            rnd = rand() % (high - low + 1) + low;
            if (checkRand(rnd) == 0) {
                fortune_ids[rnd] = rnd;
                break;
            }
        }
        return rnd;
    }

/*
 * 
 * Encryption\Decryption Server
 * 
 */
 int encdec_server()
 {
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Enc\\Dec Server : Could not create socket");
    }
    puts("Enc\\Dec Server : Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Enc\\Dec Server : bind failed. Error");
        return 1;
    }
    puts("Enc\\Dec Server : bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Enc\\Dec Server : Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);


    //Accept and incoming connection
    puts("Enc\\Dec Server : Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Enc\\Dec Server : Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("Enc\\Dec Server : could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Enc\\Dec Server : Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("Enc\\Dec Server : accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
 void *connection_handler(void *socket_desc)
 {
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Encrypt/Decrypt the message
        char new_client_message[strlen(client_message)];
        int mode_index = strlen(client_message) - 1;
        if (client_message[mode_index] == 'e')
        {
            client_message[mode_index] = 0; //Remove the mode char
            encrypt(client_message, new_client_message);    
        } 
        else if (client_message[mode_index] == 'd')
        {
            client_message[mode_index] = 0; //Remove the mode char
            decrypt(client_message, new_client_message);    
        }
        
        //Send the message back to the client
        write(sock , new_client_message , strlen(new_client_message));
    }
    
    //
    if(read_size == 0)
    {
        puts("Enc\\Dec Server : The client has disconnected!");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("Enc\\Dec Server : Receive failed!!!");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

/*
 * 
 * Encryption
 * 
 */
 void encrypt(char *str, char *q) 
 {
    int n=0;
    char *p=str;

    while(*p)
    {
     if(islower(*p))
     {
         if((*p>='a')&&(*p<'x'))
             q[n]=toupper(*p + (char)3);
         else if(*p=='x')
             q[n]='A';
         else if(*p=='y')
             q[n]='B';
         else
             q[n]='C';
     }
     else
     {
         q[n]=*p;
     }
     n++; p++;
 }
 q[n++]='\0';
 puts(q);
}

/*
 * 
 * Decryption
 * 
 */
 void decrypt(char *str, char *q)
 {
    int   n=0;
    char *p=str;

    while(*p)
    {
     if(isupper(*p))
     {
         if((*p>='D')&&(*p<='Z'))
             q[n]=tolower(*p - (char)3);
         else if(*p=='A')
             q[n]='x';
         else if(*p=='B')
             q[n]='y';
         else
             q[n]='z';
     }
     else
     {
         q[n]=*p;
     }
     n++; p++;
 }
 q[n++]='\0';
}