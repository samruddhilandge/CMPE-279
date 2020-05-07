#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[],char const *envp[]) 
{   
    int sent_socket, valread;               //This is the child process
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    uid_t nobody_uid = 65534;

    sent_socket = atoi(argv[1]);

    if (mkdir("/temp") < 0){        //Creating a new directory
        perror("Error creating a new directory using mkdir");
        exit(EXIT_FAILURE);
    }


    chdir("/temp");
    if(chroot("/temp")!=0)      //performing the chroot 
    {
        perror("Failed to change the root directory using chroot /temp");
        return 1;
    }

    if(setuid(nobody_uid)<0){           //dropping the privileges to the nobody user uid= 65534
        printf("Error in set uid\n");
        exit(EXIT_FAILURE);
    }

    valread = read( sent_socket , buffer, 1024); 
    printf("%s\n",buffer ); 
    send(sent_socket , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n");         //child process sending the message to the client
    rmdir("/temp");                     //removing the temp dir
    return 0;
}
