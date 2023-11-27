#include "usb_cdc_acm.h"

#include <stdbool.h>

#include "delay.h"

#include "usbd_core.h"
#include "usbd_cdc.h"

/*!< endpoint address */
#define CDC_IN_EP 0x81
#define CDC_OUT_EP 0x01
#define CDC_INT_EP 0x82

#define USBD_VID 0xFFFF
#define USBD_PID 0xFFFF
#define USBD_MAX_POWER 100
#define USBD_LANGID_STRING 1033

/*!< config descriptor size */
#define USB_CONFIG_SIZE (9 + CDC_ACM_DESCRIPTOR_LEN)

#define CDC_MAX_MPS 64

/*!< global descriptor */
static const uint8_t cdc_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0xEF, 0x02, 0x01, USBD_VID, USBD_PID, 0x0100, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x02, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    CDC_ACM_DESCRIPTOR_INIT(0x00, CDC_INT_EP, CDC_OUT_EP, CDC_IN_EP, CDC_MAX_MPS, 0x02),
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'C', 0x00,                  /* wcChar10 */
    'D', 0x00,                  /* wcChar11 */
    'C', 0x00,                  /* wcChar12 */
    ' ', 0x00,                  /* wcChar13 */
    'D', 0x00,                  /* wcChar14 */
    'E', 0x00,                  /* wcChar15 */
    'M', 0x00,                  /* wcChar16 */
    'O', 0x00,                  /* wcChar17 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
    0x00};

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[2048];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t write_buffer[2048];

volatile bool ep_tx_busy_flag = false;

void usbd_event_handler(uint8_t event) {
  switch (event) {
    case USBD_EVENT_RESET:
      break;
    case USBD_EVENT_CONNECTED:
      break;
    case USBD_EVENT_DISCONNECTED:
      break;
    case USBD_EVENT_RESUME:
      break;
    case USBD_EVENT_SUSPEND:
      break;
    case USBD_EVENT_CONFIGURED:
      /* setup first out ep read transfer */
      usbd_ep_start_read(CDC_OUT_EP, read_buffer, 2048);
      break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
      break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
      break;

    default:
      break;
  }
}

void usbd_cdc_acm_bulk_out(uint8_t ep, uint32_t nbytes) {
  USB_LOG_RAW("actual out len:%d\r\n", nbytes);
  /* setup next out ep read transfer */
  usbd_ep_start_read(CDC_OUT_EP, read_buffer, 2048);
}

void usbd_cdc_acm_bulk_in(uint8_t ep, uint32_t nbytes) {
  USB_LOG_RAW("actual in len:%d\r\n", nbytes);
  if ((nbytes % CDC_MAX_MPS) == 0 && nbytes) {
    /* send zlp */
    usbd_ep_start_write(CDC_IN_EP, NULL, 0);
  } else {
		ep_tx_busy_flag = false;
	}
}

/*!< endpoint call back */
struct usbd_endpoint cdc_out_ep = {.ep_addr = CDC_OUT_EP,
                                   .ep_cb = usbd_cdc_acm_bulk_out};

struct usbd_endpoint cdc_in_ep = {.ep_addr = CDC_IN_EP,
                                  .ep_cb = usbd_cdc_acm_bulk_in};

static struct usbd_interface intf0;
static struct usbd_interface intf1;

void cdc_acm_init(void) {
  int i;
  memset(&write_buffer[0], 0, 2048);
  for (i = 0; i < 1; i++) {
    memset(&write_buffer[i * 64], 'x', 62);
		write_buffer[i*64+62] = '\r';
		write_buffer[i*64+63] = '\n';
  }

  usbd_desc_register(cdc_descriptor);
  usbd_add_interface(usbd_cdc_acm_init_intf(&intf0));
  usbd_add_interface(usbd_cdc_acm_init_intf(&intf1));
  usbd_add_endpoint(&cdc_out_ep);
  usbd_add_endpoint(&cdc_in_ep);
  usbd_initialize();
}

bool dtr_flag = false;
bool rts_flag = false;

void usbd_cdc_acm_set_dtr(uint8_t intf, bool dtr) {
	dtr_flag = dtr;
	printf("dtr %s\r\n", dtr ? "set" : "reset");
}

void usbd_cdc_acm_set_rts(uint8_t intf, bool rts) {
	rts_flag = rts;
	printf("rts %s\r\n", rts ? "set" : "reset");
}

void cdc_acm_data_send_test(void) {
  if (dtr_flag) {
    ep_tx_busy_flag = true;
    usbd_ep_start_write(CDC_IN_EP, write_buffer,
                        strlen((const char *)write_buffer));
    while (ep_tx_busy_flag) {
    }
  }
}

static struct cdc_line_coding m_line_coding = {0};

void usbd_cdc_acm_set_line_coding(uint8_t intf,
                                  struct cdc_line_coding *line_coding) {
  m_line_coding.bCharFormat = line_coding->bCharFormat;
  m_line_coding.bDataBits = line_coding->bDataBits;
  m_line_coding.bParityType = line_coding->bParityType;
  m_line_coding.dwDTERate = line_coding->dwDTERate;
}
void usbd_cdc_acm_get_line_coding(uint8_t intf,
                                  struct cdc_line_coding *line_coding) {
  line_coding->bCharFormat = m_line_coding.bCharFormat;
  line_coding->bDataBits = m_line_coding.bDataBits;
  line_coding->bParityType = m_line_coding.bParityType;
  line_coding->dwDTERate = m_line_coding.dwDTERate;
}
