/**
 * @file w5500_task.c
 * @brief Файл с определением структуры и функций задания tcp-сервера на базе чипа w5500
 */
#include "w5500_task.h"
#include "cmsis_os.h"
#include <string.h>
/**
 * @brief Получение кода статуса сокета. Используется только внутри w5500_task_loop().
 * @return Код статуса сокета
 * @see w5500_task_loop() Sn_SR
 */
static uint8_t _status();
/**
 * @brief Открытие сокета. Используется только внутри w5500_task_loop().
 * @retval 1 Успех
 * @retval 0 Ошибка
 * @see w5500_task_loop()
 */
static int8_t _socket();
/**
 * @brief Прослушивание сокета. Используется только внутри w5500_task_loop().
 * @retval 1 Успех
 * @retval 0 Ошибка
 * @see w5500_task_loop()
 */
static int8_t _listen();
/**
 * @brief Отключение от сокета. Используется только внутри w5500_task_loop().
 * @retval 1 Успех. Единственное возвращаемое значение
 * @see w5500_task_loop()
 */
static int8_t _disconnect();
/**
 * @brief Закрытие сокета. Используется только внутри w5500_task_loop().
 * @retval 1 Успех. Единственное возвращаемое значение
 * @see w5500_task_loop()
 */
static int8_t _close();
/**
 * @brief Задержка. Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static void _delay();
/**
 * @brief Устанавливает длинную задержку в конце данного цикла.
 * Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static void _set_long_delay();
/**
 * @brief Устанавливает короткую задержку в конце данного цикла.
 * Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static void _set_short_delay();
/**
 * @brief !!! Функция решила проблему стабильности работы клиент-серверного обмена на этапе разработки.
 * Конкретное назначение до конца не прояснено. При решении удалить ее вызов необходима повторная тщательная отладка.
 * Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static void _check_connected();
/**
 * @brief Прием сообщения от клиента. Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static int _receive();
/**
 * @brief Отправка сообщения клиенту. Используется только внутри w5500_task_loop().
 * @see w5500_task_loop()
 */
static int _send();
/**
 * @brief Выбор IP и порта для инициализации w5500. Используется только внутри w5500_init().
 * @see w5500_init()
 */
static void _select_ip_and_port();
/**
 * @brief Структура задания tcp-сервера
 */
typedef struct
{
	uint8_t ip[4]; ///< Буфер для хранения выбранного IP
	uint16_t port; ///< Поле для хранения выбранного порта
	uint16_t socket; ///< Номер сокета, по умолчанию 0
	TickType_t xLastWakeTime; ///< Хранит время пробуждения цикла после последней задержки, мс
	TickType_t xDelayMs; ///< Величина выбранной задержки, мс
	uint16_t shortDelayMs; ///< Величина коротой задержки, мс
	uint16_t longDelayMs; ///< Величина длинной задержки, мс
	uint8_t connected; ///< Флаг наличия соединения с клиентом
} w5500_task_t;
/**
 * @brief Единственный экземпляр структуры w5500_task_t.
 * Работа всех определенных в файле функций происходит только с ней
 */
static w5500_task_t data;

void w5500_task_init()
{
	memset(&data, 0, sizeof(data));
	_select_ip_and_port();
	data.socket = 0;
	W5500_Init(data.ip[0], data.ip[1], data.ip[2], data.ip[3]);
	data.shortDelayMs = 50;
	data.longDelayMs  = 200;
	tcp_server_rx_init();
	tcp_server_tx_init();
}

void w5500_task_loop()
{
	_set_long_delay();
	data.connected = 0;
	switch(_status()) {
	case SOCK_ESTABLISHED:
		 _check_connected();
		 int recv = _receive();
		 data.connected = (recv > 0);
		 if(recv < 0)
		 {
			 _disconnect();
			 _close();
		 }
		 else if (recv > 0)
		 {
			tcp_server_rx_handle();
			tcp_server_tx_update();
			data.connected = (_send() > 0);
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
		break;
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

int w5500_task_get_port()
{
	return data.port;
}

uint8_t w5500_task_is_connected()
{
	return data.connected;
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

static void _select_ip_and_port()
{
	uint8_t _ip[] = {192, 168, 1, 12};
	data.port = 22250;
	data.socket = 0;
	memcpy(data.ip, _ip, 4);
}
