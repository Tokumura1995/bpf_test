#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

void insert(int key, int value);

int data_array[512];

int main(void)
{
  int sd;
  int len;
  struct sockaddr_in addr;
  struct sockaddr_in from_addr;

  len = sizeof(from_addr);
  
  struct pkt {
    int type;
    int key;
    int value;
  };

  struct pkt recv_pkt;
  
  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }
  addr.sin_family = AF_INET;
  addr.sin_port = htons(22222);
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(&recv_pkt, 0, sizeof(recv_pkt));
  
  if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return -1;
  }
  
  
  while (1) {
    if (recvfrom(sd, &recv_pkt, sizeof(struct pkt), 0, (struct sockaddr *)&from_addr, &len) < 0) {
      perror("recv");
      return -1;
    }
    if (recv_pkt.type == 0) {
      
      data_array[recv_pkt.key] = recv_pkt.value;
      
    } else if (recv_pkt.key == 1) {
      if (sendto(sd, &recv_pkt, sizeof(struct pkt), 0, (struct sockaddr *)&from_addr, sizeof(from_addr)) < 0 ) {
	perror("send");
	return -1;
      }
      break;
    }
  }
  
  close(sd);
  
  return 0;
}
