
#include "usb_cdc.h"
#include <stdio.h>
#include <errno.h>
void cdcacm_init(void);
static int cdcacm_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf,
                                  uint16_t *len, void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
    (void)complete;
    (void)buf;
    (void)usbd_dev;

    switch (req->bRequest)
    {
    case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
    {
        /*
         * This Linux cdc_acm driver requires this to be implemented
         * even though it's optional in the CDC spec, and we don't
         * advertise it in the ACM functional descriptor.
         */
        char local_buf[10];
        struct usb_cdc_notification *notif = (void *)local_buf;

        /* We echo signals back to host as notification. */
        notif->bmRequestType = 0xA1;
        notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
        notif->wValue = 0;
        notif->wIndex = 0;
        notif->wLength = 2;
        local_buf[8] = req->wValue & 3;
        local_buf[9] = 0;
        // usbd_ep_write_packet(0x83, buf, 10);
        return 1;
    }
    case USB_CDC_REQ_SET_LINE_CODING:
        if (*len < sizeof(struct usb_cdc_line_coding))
            return 0;
        return 1;
    }
    return 0;
}

static void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep)
{
   // uint8_t i;
    (void)ep;
    (void)usbd_dev;

   // char buf[64];
    rx_str_len = usbd_ep_read_packet(usbd_dev, 0x01, rx_str, 64);

    if (rx_str_len)   rx_str[rx_str_len] = 0;

}

static void cdcacm_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
    (void)wValue;
    (void)usbd_dev;

    usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb);
    usbd_ep_setup(usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL);
    usbd_ep_setup(usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL);

    usbd_register_control_callback(
        usbd_dev,
        USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
        USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
        cdcacm_control_request);
}
/*
int _write(int file, char *ptr, int len)
{
    int i;

    if (file == 1)
    { while (usbd_ep_write_packet(usbd_dev, 0x82,ptr, len)==0);

        return i;
    }

    errno = EIO;
    return -1;
}*/
void usb_wakeup_isr(void)
{
    usbd_poll(usbd_dev);
}

void usb_lp_can_rx0_isr(void)
{
    usbd_poll(usbd_dev);
}

 void cdcacm_init(void)
{

    gpio_clear(GPIOB, GPIO9);
    usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config, usb_strings, 3, usbd_control_buffer, sizeof(usbd_control_buffer));
    usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);

    nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
    nvic_enable_irq(NVIC_USB_WAKEUP_IRQ);
}
void cdcacm_print_buff(char* str,int len){
      usbd_ep_write_packet(usbd_dev, 0x82,str, len);
}



/*
void cdcacm_send(char *data, int size)*/
int _write(int file, char *ptr, int len)
{
	int i = 0;
     if (file == 1){
	while ((len - (i*64)) > 64) {
		while(usbd_ep_write_packet(usbd_dev,0x82, (ptr+(i*64)), 64) == 0);
		i++;
	}

	while(usbd_ep_write_packet(usbd_dev,0x82, (ptr+(i*64)), len - (i*64)) == 0);
     return 0;
}
errno = EIO;
    return -1;
}

