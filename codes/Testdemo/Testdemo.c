#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"
#include "ch32v003_GPIO_branchless.h"

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))

char tostr[50] = "";
int i = 0;
char tonext=0;

int main()
{
	SystemInit();
	usb_setup();
	GPIO_port_enable(GPIO_port_A);
	GPIO_port_enable(GPIO_port_C);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_A, 2), GPIO_pinMode_O_pushPull, GPIO_Speed_10MHz);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 1), GPIO_pinMode_I_pullUp, GPIO_Speed_In);

	while(1)
	{
/*#if RV003USB_EVENT_DEBUGGING
		uint32_t * ue = GetUEvent();
		if( ue )
		{
			printf( "%lu %lx %lx %lx\n", ue[0], ue[1], ue[2], ue[3] );
		}
#endif */

		uint8_t button_is_pressed = !GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_C, 1));
		if (button_is_pressed)
		{
			GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), high);
			if (i == 0)
				strcpy(tostr, "CH32V003F4P6 USB Keyboard Test 11 22 33 44");
		}
		else
		{
			GPIOA->BSHR = (1 << (16 + 2));
			GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), low);
		}
	}
}

void usb_handle_user_in_request( struct usb_endpoint * e, uint8_t * scratchpad, int endp, uint32_t sendtok, struct rv003usb_internal * ist )
{
	if( endp == 2 )
	{
		// Keyboard (8 bytes)
		
		static uint8_t tsajoystick[8] = { 0x00 };
		usb_send_data( tsajoystick, 8, 0, sendtok );

		

		// Press the 'b' button every second or so.

		if (tostr[i] == 0) //전송종료
		{
			tsajoystick[0] = 0;
			tsajoystick[2] = 0;
			tostr[i] = 0;
			i = 0;
			tonext = 0;
		}
		if (tonext == 1 || tonext == 2)
		{
			tonext++;
			tsajoystick[0] = 0;
			tsajoystick[2] = 0;
		}
		if (tonext == 3) tonext = 0;
		if (tostr[i] && tonext == 0)
		{
			bitclear(tsajoystick[0], 1);
			if (tostr[i] >= 'a' && tostr[i] <= 'z') //알파벳소문자
			{
				tsajoystick[2] = 4 + tostr[i] - 'a';
			}
			if (tostr[i] >= 'A' && tostr[i] <= 'Z') //알파벳대문자
			{
				tsajoystick[2] = 4 + tostr[i] - 'A';
				bitset(tsajoystick[0], 1);
			}
			else if (tostr[i] >= '1' && tostr[i] <= '9') //숫자1~9
				tsajoystick[2] = 0x1E + tostr[i] - '1';
			else if (tostr[i] == '0') //숫자0
				tsajoystick[2] = 0x27;
			else if (tostr[i] == '/') //특문/
				tsajoystick[2] = 0x54;
			else if (tostr[i] == ' ') //띄어쓰기
				tsajoystick[2] = 0x2C;
			tonext = 1;
			tostr[i] = 0;
			i++;
		}

	}
	else
	{
		// If it's a control transfer, empty it.
		usb_send_empty( sendtok );
	}
}


