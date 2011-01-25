#include <stdint.h>
#include <unistd.h>

/** Error codes */
enum {
  ERR_SYNTAX = 1, /**< "?" */
  ERR_DATA,	  /**< "DATA ERROR" */
};

/** Protocol version */
typedef enum {
  PROT_KW1281 = 1, /**< KW1281 */
  PROT_KW2000,     /**< KW2000 (aka KW2089) */
} wbh_protocol_t;

/** WBH interface state */
typedef struct {
  int fd;		/**< serial device file descriptor */
  char *name;	/**< serial device file name */
} wbh_interface_t;

/** Baud rates */
typedef enum {
  BAUD_AUTO = 0,
  BAUD_1200,
  BAUD_2400,
  BAUD_4800,
  BAUD_9600,
  BAUD_10400,
} wbh_baudrate_t;

/** WBH diagnostic device state */
typedef struct {
  uint8_t id;			/**< device ID */
  wbh_interface_t *iface;
  wbh_protocol_t protocol;	/**< protocol ID (KW1281 or KW2000) */
  wbh_baudrate_t baudrate;
  const char *specs;		/**< raw specification data as sent by
                                     the device on connect */
} wbh_device_t;

/** read analog value pin 0..5
    @param iface WBH interface handle
    @param pin pin number
    @return analog value or negative error code
 */
int wbh_get_analog(wbh_interface_t *iface, uint8_t pin);

/** read block delay time
    @param iface WBH interface handle
    @return block delay time in ms or negative error code
 */
int wbh_get_bdt(wbh_interface_t *iface);
/** set block delay time
    @param iface WBH interface handle
    @param bdt block delay time (ms)
    @return zero or negative error code
 */
int wbh_set_bdt(wbh_interface_t *iface, uint8_t bdt);

/** read inter-byte time
    @param iface WBH interface handle
    @return inter-byte delay in ms or negative error code
 */
int wbh_get_ibt(wbh_interface_t *iface);
/** set inter-byte time
    @param iface WBH interface handle
    @param ibt inter-byte delay time (ms)
    @return zero or negative error code
 */
int wbh_set_ibt(wbh_interface_t *iface, uint8_t ibt);

/** initialize WBH interface
    @param tty serial device name
    @return WBH interface handle or NULL on error
 */
wbh_interface_t *wbh_init(const char *tty);
/** shut down WBH interface
    @param iface WBH interface handle
    @return zero or negative error code
 */
int wbh_shutdown(wbh_interface_t* iface);

/** connect to diagnostic device
    @param iface WBH interface handle
    @param device device ID
    @return WBH device handle or NULL on error
 */
wbh_device_t *wbh_connect(wbh_interface_t *iface, uint8_t device);
/** disconnect from diagnostic device
    @param dev WBH device handle
    @return zero or negative error code
 */
int wbh_disconnect(wbh_device_t *dev);

/** force a specific baud rate for diagnostic device connection
    @param iface WBH interface handle
    @param baudrate baud rate
    @return zero or negative error code
 */
int wbh_force_baud_rate(wbh_interface_t *iface, wbh_baudrate_t baudrate);

/** reset interface
    @param iface WBH interface handle
    @return zero or negative error code
 */
int wbh_reset(wbh_interface_t *iface);

/** send a custom command to the diagnostic device
    @param dev diagnostic device handle
    @param cmd command string
    @param data response buffer
    @param data_size size of response buffer
    @param timeout time to wait for data
    @return bytes read or negative error code
 */
int wbh_send_command(wbh_device_t *dev, char *cmd, char *data,
                     size_t data_size, int timeout);