#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <libusb-1.0/libusb.h>

#define EZUSB_VID 0x04B4
#define EZUSB_PID 0x1987

/* Read N byte data from 2byte address device */
#define R2N 0x21 
/* Write N byte data to 2byte address device */
#define W2N 0x20
/* Read N byte data from 1byte address device */
#define R1N 0x11
/* Write N byte data to 1byte address device */
#define W1N 0x10

/* Package Out/In buffer*/
unsigned char pkg_out[64],pkg_in[64];
/* Data In/Out buffer*/
unsigned char data_in[32],data_out[32];

/**
 * Make CRC
 * @buf, data buffer
 * @len, length of data
 */
unsigned char makecrc(unsigned char *buf, unsigned char len) {

	unsigned char sum,i;
	sum = 0;
	for(i=0; i<len; i++) {
		sum += buf[i];
	}
	return sum;

}

/**
 * Making read/write package
 *
 * @data, data buffer
 * @type, operation type, read or write
 * @i2c_addr, IIC/I2C Device Address
 * @data_addr_h, memory address high byte
 * @data_addr_l, memory address low byte
 * @len, read/write length
 *
 */

void makepackage(unsigned char *data, unsigned char type, unsigned char i2c_addr, 
			unsigned char data_addr_h, unsigned char data_addr_l, unsigned char len) {
	int i;

	memset(pkg_out,0,64);

	if (len>32) {
		printf("Max Write/Read Data Length is 32 bytes!\n");
		len = 32;
	} else if (len<0) {
		printf("Write/Read Length Must > 0 !\n");
		len = 1;
	}

	/* Package Head, see README */
	pkg_out[0] = 0xAA;
	pkg_out[1] = 0xAA;
	pkg_out[2] = 0xAA;
	pkg_out[3] = type;
	pkg_out[4] = i2c_addr;
	pkg_out[5] = data_addr_h;
	pkg_out[6] = data_addr_l;
	pkg_out[7] = len;

	/* Payload */
	for(i=8;i<len+8;i++) {
		pkg_out[i] = data[i-8];
	}

	/* Package Tail, see README */
	pkg_out[60] = makecrc(pkg_out+3,len+5);
	pkg_out[61] = 0xF0;
	pkg_out[62] = 0xF0;
	pkg_out[63] = 0xF0;

}

/**
 * Transfer Data via USB
 *
 * @fd, device handle
 * @data, data buffer
 * @type, write/read operation
 * @i2c_addr, I2C/IIC Device address
 * @data_addr_h, memory address high byte
 * @data_addr_l, memory address low byte
 * @len, length of data to transfer
 */
int xfer(struct libusb_device_handle *handle, unsigned char *data, unsigned char type, unsigned char i2c_addr, \
			unsigned char data_addr_h, unsigned char data_addr_l, unsigned char len) {

	int result;
	int i;
	int transferred;

	makepackage(data,type,i2c_addr,data_addr_h,data_addr_l,len);

	/* Send out the Command Package via USB*/
	result = libusb_interrupt_transfer(handle, 0x01, pkg_out, 64, &transferred, 100); 
	if(result==0) {
		/* Debug 
		printf("Package out success:\n");
		for(i=0;i<64;i++)
		  printf("%02X ",pkg_out[i]);
		printf("\n");
		*/
	}else{
		printf("Package out failed. %d \n", transferred);
		return -1;
	}

	usleep(1000*20);

	/* Receive Package*/
	memset(pkg_in, 0, 64);
	result = libusb_interrupt_transfer(handle, 0x81, pkg_in, 64, &transferred, 100);
	if(result<0) {
		printf("package in failed.\n");
		return -1;
	}else{
		/* Debug
		printf("Package in success:\n");
		for(i=0;i<64;i++)
		  printf("%02X ",pkg_in[i]);
		printf("\n");
		*/
	}

	memset(data_in, 0, 32);

	if((pkg_in[0]==0xAA) &&
	   (pkg_in[1]==0xAA) &&
	   (pkg_in[2]==0xAA) ) {
		
		/* Read data*/
		if (type&0x01)
			memcpy(data_in, pkg_in+8, pkg_in[7]);
	
		return 0;
	}

	return -1;
}

int main(void) {

	struct libusb_device_handle *handle=NULL;
	int result;
	int i;

	/* Initial libusb */
	result = libusb_init(NULL);
	if (result < 0) {
		printf("libusb initial failed\n");
		return 0;
	}

	/* Get Device Handle */
	handle = libusb_open_device_with_vid_pid(NULL, EZUSB_VID, EZUSB_PID);
	if (NULL == handle) {
		printf("Failed to get USB device handle.\n");
		goto end;
	}

	/* Read 8 bytes data from EEPROM(0x51) */
	result=xfer(handle, data_out, R2N, 0x51, 0x00, 0x00, 8);
	if (result >= 0) {
		printf("Read Success.\n");
		for(i=0;i<8;i++) {
			printf("%02X ", data_in[i]);
		}
		printf("\n");
	}

	/* Write IIC */
	data_out[0] = 0xC2;
	data_out[1] = 0x47;
	data_out[2] = 0x05;
	result=xfer(handle, data_out, W2N, 0x51, 0x00, 0x00, 1);
	if (result >= 0) {
		printf("Write Success.\n");
	}


	libusb_close(handle);
	
end:
	libusb_exit(NULL);
	return 0;
}

