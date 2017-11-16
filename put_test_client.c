#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PUT 0
#define FIN 1

#define MAX 1000000

int main(int argc, char ** argv)
{
  int sd;
  struct sockaddr_in addr;
  FILE *fp;
  int packet_num = 1;
  int count = 0;
  
  struct pkt_data {
    int type;
    int key;
    int value;
  };

  struct pkt_data pkt;
  struct pkt_data recv_pkt;

  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(22222);
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  fp = fopen("test.dat","w");
  if(fp==NULL){     
    printf("failed\n");
    return -1;
  }

  while (packet_num < MAX) {
    clock_t t1, t2;

    t1 = clock();
    while (count < packet_num) {
      pkt.type = PUT;
      pkt.key = rand() % 512;
      pkt.value = rand(); 
      if (sendto(sd, &pkt, sizeof(struct pkt_data), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0 ) {
	perror("send");
	return -1;
      }
      count++;
    }
    
    if (recvfrom(sd, &recv_pkt, sizeof(struct pkt_data), 0, NULL, NULL) < 0) {
	perror("recv");
	return -1;
      }
    t2 = clock();
    
    fprintf(fp, "%f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    packet_num *= 10;
  }

  close(sd);
  fclose(fp);
  
  return 0;
}
