// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 9000
#define BUF_SIZE 1024

enum GAMEFLAGS
{
    ZERO,
    INITIAL_CONNECTION,
    START,
    WAIT,
    ROCK,
    PAPER,
    SCISSORS,
    WIN,
    LOSE,
    DISCONNECT
};
int sock = 0, valread;

int read_server()
{
    int response = 0;

    if ((valread = read(sock, &response, BUF_SIZE)) == 0)
    {
        puts("recv failed");
        return -1;
    }
    return ntohl(response);
}
int send_to_server(int message)
{
    send(sock, &message, sizeof(message), 0);
};

int get_player_input()
{
    char line[256];
    char ch;
    printf("\n ROCK: r\nPAPER: p\nScissors: s\nType your choice below. \n");
    if (fgets(line, sizeof line, stdin) == NULL)
    {
        printf("Input error.\n");
    }

    ch = line[0];
    printf("Character read: %c\n", ch);
    return ch;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE] = {0};
    char server_reply[BUF_SIZE];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        printf("\nCheck if server is running.");
        return -1;
    }

    // ask server if there is an opening in the game
    send_to_server(START);

    printf("\n Confirming game availability...");

    int response = read_server();

    printf("%d\n", response);

    char input = get_player_input();

    // char char_input = fgetc(stdin);
    // send(char_input, hello, strlen(char_input), 0);
    // printf("'%c' user input sent to server\n", char_input);
    // close(sock);

    return 0;
}