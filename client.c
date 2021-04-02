// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h> //clock

#define PORT 9000
#define BUF_SIZE 1024
#define DEBUG 0
const char *ROCK_ASCII[] = {
    "    _______\n",
    "---'   ____)\n",
    "       (_____)\n",
    "       (_____)\n",
    "       (____)\n",
    "----.__(___)\n"};

const char *PAPER_ASCII[] = {
    "     ________\n",
    "---'    _____)______\n",
    "           _________)\n",
    "          __________)\n",
    "         __________)\n",
    "---._____________)\n"};

const char *SCISSORS_ASCII[] = {
    "    ________\n",
    "---'   _____)_______\n",
    "          __________)\n",
    "       _____________)\n",
    "      (_____)\n",
    "---.__(____)\n"};

const char *GAME_ASCII[] = {
    " _____   ___  ___  ___ _____ \n",
    "|  __ \\ / _ \\ |  \\/  ||  ___|\n",
    "| |  \\// /_\\ \\| .  . || |__  \n",
    "| | __ |  _  || |\\/| ||  __| \n",
    "| |_\\ \\| | | || |  | || |___ \n",
    " \\____/\\_| |_/\\_|  |_/\\____/\n"};

const char *START_ASCII[] = {
    " _____ _____ ___  ______ _____ \n",
    "/  ___|_   _/ _ \\ | ___ \\_   _|\n",
    "\\ `--.  | |/ /_\\ \\| |_/ / | |  \n",
    " `--. \\ | ||  _  ||    /  | |  \n",
    "/\\__/ / | || | | || |\\ \\  | |  \n",
    "\\____/  \\_/\\_| |_/\\_| \\_| \\_/  \n"};
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
    TIE,
    DISCONNECT,
    RESTART,
};
int server_socket = 0, valread;

int read_socket(int socket_fds)
{
    int response = 0;
    int status = 0;

    if ((status = read(socket_fds, &response, BUF_SIZE)) == 0)
    {
        printf("\nFailed to read socket %d\n", socket_fds);
        return -1;
    }
    else
    {
        if (DEBUG)
        {
            printf("\nMessage '%d' received from socket %d\n", ntohl(response), socket_fds);
        }
    }
    return ntohl(response);
}

int send_to_socket(int socket_fds, int raw_message)
{
    int message = htonl(raw_message);

    if (send(socket_fds, &message, sizeof(message), 0) != sizeof(message))
    {
        printf("\nMessage '%d' failed to send to socket %d\n", raw_message, socket_fds);
        return 1;
    }
    else
    {
        if (DEBUG)
        {
            printf("\nMessage '%d' sent to socket %d\n", raw_message, socket_fds);
        }
    };
    return -1;
};

char get_player_input()
{
    char line[256];
    char ch;
    if (fgets(line, sizeof line, stdin) == NULL)
    {
        printf("Input error.\n");
    }

    ch = line[0];
    printf("Character read: %c\n", ch);
    return ch;
}
// Taken from https://c-for-dummies.com/blog/?p=69
void delay(int milliseconds)
{
    long pause;
    clock_t now, then;

    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause)
        now = clock();
}

void print_ascii(const char **art, int length)
{
    for (int i = 0; i < length; i++)
    {
        delay(100);
        printf("%s", art[i]);
    }
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in serv_addr;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

    if (connect(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        printf("\nCheck if server is running.");
        return -1;
    }

    // ask server if there is an opening in the game
    send_to_socket(server_socket, INITIAL_CONNECTION);

    printf("\n LOOKING FOR OPPONENT...\n");

    int response;
    while (response != START)
    {
        response = read_socket(server_socket);
    }

    printf("\n\nPLAYER TWO HAS JOINED\n");

    while (1)
    {
        // Print GAME START ascii
        print_ascii(GAME_ASCII, 6);
        print_ascii(START_ASCII, 6);

        // Get valid input from player
        printf("\nROCK: r\nPAPER: p\nSCISSORS: s\n\nENTER YOUR CHOICE BELOW \n\n");
        char input = 'x';
        input = get_player_input();
        while (input != 'r' && input != 'p' && input != 's')
        {
            printf("\nInvalid input '%c'.\n", input);

            input = get_player_input();
        }

        // Print hand ascii and send message to server
        switch (input)
        {
        case 'r':
            send_to_socket(server_socket, ROCK);
            print_ascii(ROCK_ASCII, 6);

            break;
        case 'p':
            send_to_socket(server_socket, PAPER);
            print_ascii(PAPER_ASCII, 6);

            break;
        case 's':
            send_to_socket(server_socket, SCISSORS);
            print_ascii(SCISSORS_ASCII, 6);
            break;
        }

        // Wait for server results
        printf("\nWAITING FOR PLAYER 2...\n");
        response = read_socket(server_socket);

        //Print game results
        switch (response)
        {
        case WIN:
            switch (input)
            {
            case 'r':
                print_ascii(SCISSORS_ASCII, 6);
                break;
            case 'p':
                print_ascii(ROCK_ASCII, 6);

                break;
            case 's':
                print_ascii(PAPER_ASCII, 6);
                break;
            }
            printf("\nYOU WON!\n");
            break;

        case LOSE:
            switch (input)
            {
            case 'r':
                print_ascii(PAPER_ASCII, 6);
                break;
            case 'p':
                print_ascii(SCISSORS_ASCII, 6);

                break;
            case 's':
                print_ascii(ROCK_ASCII, 6);
                break;
            }
            printf("\nYOU LOST!\n");
            break;
        case TIE:
            switch (input)
            {
            case 'r':
                print_ascii(ROCK_ASCII, 6);
                break;
            case 'p':
                print_ascii(PAPER_ASCII, 6);

                break;
            case 's':
                print_ascii(SCISSORS_ASCII, 6);
                break;
            }
            printf("\nIT'S A TIE!\n");
            break;
        }
        printf("\nPLAY AGAIN: a\nQUIT: q\n\n");

        while (input != 'a' && input != 'q')
        {
            input = get_player_input();
        }
        if (input == 'q')
        {
            printf("\nGOODBYE!\n");
            break;
        }
        else if (input == 'a')
        {
            send_to_socket(server_socket, RESTART);
        }
    }

    return 0;
}