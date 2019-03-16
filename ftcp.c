#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

int dial(FILE **in, FILE **out, char *ip, char *port)
{
	int con;
	sockaddr_in addr;

	if ((con = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		return -1;
	memset(&addr, 0, sizeof (addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip? inet_addr(ip) : htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(atoi(port));
	if (connect(con, (sockaddr*) &addr, sizeof (addr)) < 0)
		return -2;
	*in = fdopen(dup(con), "r");
	*out = fdopen(con, "w");
	return 0;
}

int host(char *port)
{
	int fd;
	sockaddr_in server;

	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		return -1;
	memset(&server, 0, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(port));
	if (bind(fd, (sockaddr*) &server, sizeof (server)) < 0)
		return -2;
	listen(fd, 5);
	return fd;
}

int pickup(FILE **in, FILE **out, int fd)
{
	static socklen_t clen = sizeof(sockaddr_in);
	int con;
	sockaddr_in client;

	if ((con = accept(fd, (sockaddr*) &client, &clen)) < 0)
		return -1;
	*in = fdopen(dup(con), "r");
	*out = fdopen(con, "w");
	return 0;
}
