#include <lwip/api.h>

#define UDP_ECHO_PORT   7

void udpecho_entry(void *parameter)
{
	struct netconn *conn;
	struct netbuf *buf;
	ip_addr_t *addr;
	unsigned short port;

	conn = netconn_new(NETCONN_UDP);
	netconn_bind(conn, IP_ADDR_ANY, 7);

	while(1)
	{
        /* received data to buffer */
		netconn_recv(conn, &buf);

		addr = netbuf_fromaddr(buf);
		port = netbuf_fromport(buf);

        /* send the data to buffer */
		netconn_connect(conn, addr, port);

		/* reset address, and send to client */
		buf->addr = *IP_ADDR_ANY;
		netconn_send(conn, buf);

        /* release buffer */
		netbuf_delete(buf);
	}
}

static rt_thread_t echo_tid = RT_NULL;
void udpecho(void)
{
	if (echo_tid == RT_NULL)
	{
		echo_tid = rt_thread_create("uecho",
									udpecho_entry, RT_NULL,
									512, 30, 20);
		if (echo_tid != RT_NULL)
			rt_thread_startup(echo_tid);
	}
	else
	{
		rt_thread_delete(echo_tid); /* delete thread */
		echo_tid = RT_NULL;
	}
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(udpecho, startup or stop UDP echo server);
MSH_CMD_EXPORT(udpecho, startup or stop UDP echo server);
#endif
