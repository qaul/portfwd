/*
  udp_rcv.c

  gcc -Wall -o udp_rcv udp_rcv.c

  udp_rcv <port>

  $Id: udp_rcv.cc,v 1.1 2001/05/15 00:25:13 evertonm Exp $
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUF_SZ = 8192;

int main(int argc, char *argv[])
{
  int          port;
  int          sd;
  struct       sockaddr_in sa;
  char         buf[BUF_SZ];
  int          rd;
  unsigned int sa_len, cli_sa_len;
  struct sockaddr_in cli_sa;

  cli_sa_len = sizeof(cli_sa);

  if (argc != 2) {
    printf("usage: %s <port>\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);

  sd = socket(AF_INET, SOCK_DGRAM, 17);
  if (sd == -1) {
    fprintf(stderr, "Can't create UDP socket: %s\n", strerror(errno));
    exit(1);
  }
    
  sa.sin_family      = AF_INET;
  sa.sin_port        = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));
  sa_len = sizeof(sa);

  if (bind(sd, (struct sockaddr *) &sa, sa_len)) {
    fprintf(stderr, "Can't bind UDP socket: %s: %s:%d\n", strerror(errno), inet_ntoa(sa.sin_addr), port);
    close(sd);
    exit(1);
  }

  for (;;) { /* forever */
    fprintf(stderr, "Waiting UDP packet on %s:%d\n", inet_ntoa(sa.sin_addr), port);

    rd = recvfrom(sd, buf, BUF_SZ, 0, (struct sockaddr *) &cli_sa, &cli_sa_len);
    if (!rd)
      break;
    if (rd == -1) {
      fprintf(stderr, "Can't receive UDP packet: %s\n", strerror(errno));
      break;
    }

    printf("%s:%d (%d) \"%s\"\n", inet_ntoa(cli_sa.sin_addr), ntohs(cli_sa.sin_port), rd, buf);
  }

  if (close(sd))
    fprintf(stderr, "close on socket failed: %s\n", strerror(errno));

  return 0;
}
