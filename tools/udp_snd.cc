/*
  udp_snd.c

  gcc -Wall -o udp_snd udp_snd.c

  udp_snd <host> <port>

  $Id: udp_snd.cc,v 1.1 2001/05/15 00:25:13 evertonm Exp $
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
#include <netdb.h>

const int BUF_SZ = 8192;

int main(int argc, char *argv[])
{
  int            port;
  int            sd;
  struct         sockaddr_in sa;
  char           buf[BUF_SZ];
  int            rd, wr;
  const char     *host;
  struct hostent *he;
  unsigned int   sa_len = sizeof(sa);

  if (argc != 3) {
    printf("usage: %s <host> <port>\n", argv[0]);
    exit(1);
  }

  host = argv[1];
  port = atoi(argv[2]);

  he = gethostbyname(host);
  if (!he) {
    fprintf(stderr, "Can't solve hostname: %s\n", host);
    exit(1);
  }

  sd = socket(AF_INET, SOCK_DGRAM, 17);
  if (sd == -1) {
    fprintf(stderr, "Can't create UDP socket: %s\n", strerror(errno));
    exit(1);
  }
    
  sa.sin_family      = AF_INET;
  sa.sin_port        = htons(port);
  sa.sin_addr.s_addr = *((unsigned int *) *he->h_addr_list);
  memset((char *) sa.sin_zero, 0, sizeof(sa.sin_zero));

  for (;;) { /* forever */
    rd = read(0, buf, BUF_SZ);
    if (!rd)
      break;
    if (rd == -1)
      fprintf(stderr, "Can't read from stdin\n");

    wr = sendto(sd, buf, rd, 0, (struct sockaddr *) &sa, sa_len);
    if (rd == -1) {
      fprintf(stderr, "Can't send UDP packet: %s\n", strerror(errno));
      break;
    }
    if (wr < rd) 
      fprintf(stderr, "udp_snd: Partial send\n");

    printf("%s:%d (%d) \"%s\"\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port), wr, buf);
  }

  if (close(sd))
    fprintf(stderr, "close on socket failed: %s\n", strerror(errno));

  return 0;
}
