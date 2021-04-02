#include <stdio.h>
#include <string.h> //sizeof
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 9000
#define BUF_SIZE 1024
#define DEBUG 1

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
    RESTART

};

int client_socket[2];
int player_hands[2];

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

int determine_winner(int player_1_hand, int player_2_hand)
{
    int result_flag = -1; //0 = p1 lost, 1 = p1 won, 2 = p1 tied
    switch (player_1_hand)
    {
    case ROCK:
        switch (player_2_hand)
        {
        case ROCK:
            result_flag = 2;
            break;
        case PAPER:
            result_flag = 0;
            break;
        case SCISSORS:
            result_flag = 1;
            break;
        default:
            printf("\nUnknown player 2 hand.");
            break;
        }
        break;

    case PAPER:
        switch (player_2_hand)
        {
        case ROCK:
            result_flag = 1;
            break;
        case PAPER:
            result_flag = 2;
            break;
        case SCISSORS:
            result_flag = 0;
            break;
        default:
            printf("\nUnknown player 2 hand.");
            break;
        }
        break;
    case SCISSORS:
        switch (player_2_hand)
        {
        case ROCK:
            result_flag = 0;
            break;
        case PAPER:
            result_flag = 1;
            break;
        case SCISSORS:
            result_flag = 2;
            break;
        default:
            printf("\nUnknown player 2 hand.");
            break;
        }
        break;
    default:
        printf("\nUnknown player 1 hand: %d", player_1_hand);
        break;
    }

    return result_flag;
}
void reset_game()
{
    for (int i = 0; i < 2; i++)
    {
        player_hands[i] = 0;
    }
}
int main(int argc, char *argv[])
{

    int opt = TRUE;
    int master_socket, addrlen, new_socket,
        max_clients = 2, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;
    char buffer[BUF_SIZE]; //data buffer of 1K
    int player_hands[2];

    //set of socket descriptors
    fd_set readfds;

    //initialise all client_socket[] to 0 so not checked and player_hand to be 0 so unplayed
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
        player_hands[i] = 0;
    }

    //create a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //bind the socket to localhost port 9000
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 2 pending connections for the master socket
    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while (TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for (i = 0; i < max_clients; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)

            if (FD_ISSET(client_socket[i], &readfds))
            {
                int response = read_socket(client_socket[i]);

                printf("\nPlayer 1 hand: %d \nPlayer 2 hand: %d ", player_hands[0], player_hands[1]);

                switch (response)
                {
                case ZERO:

                    printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));
                    // //Close the socket and mark as 0 in list for reuse
                    close(client_socket[i]);
                    client_socket[i] = 0;
                    break;
                case INITIAL_CONNECTION:
                    printf("Player %d has joined the game", i);

                    //If two players have joined, send each client the start flag
                    if (i == 1)
                    {
                        printf("Game starting... sending start flags to each player");

                        send_to_socket(client_socket[0], START);
                        send_to_socket(client_socket[1], START);
                    }
                    else //If only one player has joined, send wait flag
                    {
                        printf("Waiting for second player join, sending wait flag to player %d", i);
                        send_to_socket(i, WAIT);
                    }
                    break;
                case ROCK:
                case PAPER:
                case SCISSORS:
                    player_hands[i] = response;
                    if (player_hands[(i + 1) % 2] != 0)
                    {
                        int game_result = determine_winner(player_hands[0], player_hands[1]);
                        switch (game_result)
                        {
                        case 0:
                            send_to_socket(client_socket[0], LOSE);
                            send_to_socket(client_socket[1], WIN);
                            break;
                        case 1:
                            send_to_socket(client_socket[0], WIN);
                            send_to_socket(client_socket[1], LOSE);
                            break;
                        case 2:
                            send_to_socket(client_socket[0], TIE);
                            send_to_socket(client_socket[1], TIE);
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case RESTART:
                    reset_game();
                    break;
                default:
                    printf("\nUnknown response '%d' received from player %d", response, i);
                    break;
                }
            }
    }

    return 0;
}
