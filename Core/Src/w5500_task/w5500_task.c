/*
 * w5500_task.c
 *
 *  Created on: Dec 23, 2025
 *      Author: Kirill
 */

// 4) client


#include "w5500_task.h"
#include "cmsis_os.h"
#include <string.h>

static uint8_t _status();
static int8_t _socket();
static int8_t _listen();
static int8_t _disconnect();
static int8_t _close();
static void _delay();
static void _set_long_delay();
static void _set_short_delay();

static void _check_connected();
static int _receive();
static int _send();

typedef struct
{
	uint8_t ip[4];
	uint16_t port;
	uint16_t socket;
	uint8_t status;

	TickType_t xLastWakeTime;
	TickType_t xDelayMs;

	uint16_t shortDelayMs;
	uint16_t longDelayMs;

	// debug
	uint8_t rx_buff[64];
	uint8_t tx_buff[64];
} w5500_task_t;

static w5500_task_t data;

void w5500_task_init()
{
	memset(&data, 0, sizeof(data));

	// set ip
	uint8_t _ip[] = {192, 168, 1, 12};
	data.port = 22250;
	data.socket = 0;
	memcpy(data.ip, _ip, 4);
	W5500_Init(_ip[0], _ip[1], _ip[2], _ip[3]);

	data.shortDelayMs = 50;
	data.longDelayMs  = 200;

	tcp_server_rx_init();
	tcp_server_tx_init();
}

void w5500_task_loop()
{
	_set_long_delay();
	switch(_status()) {
	case SOCK_ESTABLISHED:
		 _check_connected();
		 int recv = _receive();
		 if(recv < 0)
		 {
			 _disconnect();
			 _close();
		 }
		 else if (recv > 0) // size
		 {
			tcp_server_rx_handle();
			tcp_server_tx_update();
			 _send();
		 }
		 _set_short_delay();
		break;
	case SOCK_CLOSE_WAIT:
		_disconnect();
		_close();
		break;
	case SOCK_SYNRECV:
		_disconnect();
		_close();
		break;
	case SOCK_SYNSENT:
		_disconnect();
		_close();
		break;
	case SOCK_CLOSED:
		if(_socket()) _listen();
		break;
	case SOCK_INIT:
		_listen();
	default:
		break;
	}
	_delay();
}

// other
int w5500_task_get_ip()
{
	int ret;
	memcpy(&ret, data.ip, 4);
	return ret;
}

uint8_t w5500_get_socket_status()
{
	return _status();
}

static uint8_t _status()
{
	return getSn_SR(data.socket);
}

static int8_t _socket()
{
	return socket(data.socket, Sn_MR_TCP, data.port, SF_IO_NONBLOCK) == data.socket;
}

static int8_t _listen()
{
	return listen(data.socket) == SOCK_OK;
}

static int8_t _disconnect()
{
	return disconnect(data.socket);
}

static int8_t _close()
{
	return close(data.socket);
}

static void _delay()
{
	vTaskDelayUntil(&data.xLastWakeTime, data.xDelayMs);
}

static void _set_long_delay()
{
	data.xDelayMs = pdMS_TO_TICKS(data.longDelayMs);
}

static void _set_short_delay()
{
	data.xDelayMs = pdMS_TO_TICKS(data.shortDelayMs);
}

static void _check_connected()
{
    if (getSn_IR(data.socket) & Sn_IR_CON) {
        setSn_IR(data.socket, Sn_IR_CON);
    }
}

static int _receive()
{
	return recv(data.socket, tcp_server_rx_get(), tcp_server_rx_size());
}

static int _send()
{
	return send(data.socket, tcp_server_tx_get(), tcp_server_tx_size());
}


