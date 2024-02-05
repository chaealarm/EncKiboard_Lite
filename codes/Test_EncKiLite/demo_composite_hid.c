#include "ch32v003fun.h"
#include <stdio.h>
#include <string.h>
#include "rv003usb.h"
#include "ch32v003_GPIO_branchless.h"

#define bitset(byte,nbit)   ((byte) |=  (1<<(nbit)))
#define bitclear(byte,nbit) ((byte) &= ~(1<<(nbit)))

char tostr[50] = "";
uint8_t tocust = 0;
int i = 0;
unsigned char tonext=0;

char estat = 0; // ���ڴ� ������ġ
char testat = 0;  // ���ڴ� ������ġ
char key = 0; // ������ Ű(0~12)
char tkey = 0; // ���� ������ Ű(0~12)
char mode = 0; // ���
unsigned char kchat = 254; // Ű ä�͸� ������
unsigned char ekchat = 0; 
unsigned char kochange = 0, koflag = 0; // /r /w �� �ڵ� �ѿ���ȯ�� ���� �÷���

char keypad()
{
	char q, j, res = 0, k = 0;
	for (q = 4;  q>= 1; q--)
	{
		GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, q), low);
		for (j = 7; j >= 5; j--)
		{
			k++;
			if (!GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_C, j)))
				res = k;
		}
		GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, q), high);
	}
	return res;
}

int main()
{
	SystemInit();
	usb_setup();
	GPIO_port_enable(GPIO_port_A);
	GPIO_port_enable(GPIO_port_C);
	GPIO_port_enable(GPIO_port_D);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_A, 2), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz); //LED_YELLOW
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 0), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz); //LED_GREEN
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_A, 1), GPIO_pinMode_I_floating, GPIO_Speed_In); //EN_SW
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_D, 0), GPIO_pinMode_I_floating, GPIO_Speed_In); //EN_LE
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_D, 6), GPIO_pinMode_I_floating, GPIO_Speed_In); //EN_RI
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 1), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 2), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 3), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 4), GPIO_pinMode_O_pushPull, GPIO_Speed_50MHz);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 5), GPIO_pinMode_I_floating, GPIO_Speed_In);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 6), GPIO_pinMode_I_floating, GPIO_Speed_In);
	GPIO_pinMode(GPIOv_from_PORT_PIN(GPIO_port_C, 7), GPIO_pinMode_I_floating, GPIO_Speed_In);

    mode = 0;
    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), low);
    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, 0), high);


	while(1)
	{
		/* INPUT TEST
		uint8_t button_is_pressed = !GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_A, 1));
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
		*/
		
		//enc
		estat = GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_D, 6));
		if (estat != testat)
		{
			if (GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_D, 0)) != estat)
			{
				mode = 1;
				GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), high);
				GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, 0), low);
			}
			else
			{
				mode = 0;
				GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), low);
				GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, 0), high);
			}
            testat = estat;
		}

        // button
        if (!kchat) key = keypad();

        if (key != tkey)
        {
            if (key)
            {

                if (koflag && key)
                {
                    while (i);
                    kochange = 1;
                    koflag = 0;
                    Delay_Ms(100);
                }

                if (mode == 0)
                {

                    
                    switch (key) {
                    case 1: strcpy(tostr, "Mode 0 Key 1\n");
                        break;
                    case 2: strcpy(tostr, "Mode 0 Key 2\n");
                        break;
                    case 3: strcpy(tostr, "Mode 0 Key 3\n");
                        break;
                    case 4: strcpy(tostr, "Mode 0 Key 4\n");
                        break;
                    case 5: strcpy(tostr, "Mode 0 Key 5\n");
                        break;
                    case 6: strcpy(tostr, "Mode 0 Key 6\n");
                        break;
                    case 7: strcpy(tostr, "Mode 0 Key 7\n");
                        break;
                    case 8: strcpy(tostr, "Mode 0 Key 8\n");
                        break;
                    case 9: strcpy(tostr, "Mode 0 Key 9\n");
                        break;
                    case 10: strcpy(tostr, "Mode 0 Key 10\n");
                        break;
                    case 11: strcpy(tostr, "Mode 0 Key 11\n");
                        break;
                    case 12: strcpy(tostr, "Mode 0 Key 12\n");
                        break;
                    }

                }
                else if (mode == 1)
                {
                    switch (key)
                    {
                    case 1: strcpy(tostr, "Mode 1 Key 1\n");
                        break;
                    case 2: strcpy(tostr, "Mode 1 Key 2\n");
                        break;
                    case 3: strcpy(tostr, "Mode 1 Key 3\n");
                        break;
                    case 4: strcpy(tostr, "Mode 1 Key 4\n");
                        break;
                    case 5: strcpy(tostr, "Mode 1 Key 5\n");
                        break;
                    case 6: strcpy(tostr, "Mode 1 Key 6\n");
                        break;
                    case 7: strcpy(tostr, "Mode 1 Key 7\n");
                        break;
                    case 8: strcpy(tostr, "Mode 1 Key 8\n");
                        break;
                    case 9: strcpy(tostr, "Mode 1 Key 9\n");
                        break;
                    case 10: strcpy(tostr, "Mode 1 Key 10\n");
                        break;
                    case 11: strcpy(tostr, "Mode 1 Key 11\n");
                        break;
                    case 12: strcpy(tostr, "Mode 1 Key 12\n");
                        break;
                    }
                }
                else if (mode == 3 && key)
                {
                    switch (key)
                    {
                    case 1: strcpy(tostr, "7");
                        break;
                    case 2: strcpy(tostr, "8");
                        break;
                    case 3: strcpy(tostr, "9");
                        break;
                    case 4: strcpy(tostr, "4");
                        break;
                    case 5: strcpy(tostr, "5");
                        break;
                    case 6: strcpy(tostr, "6");
                        break;
                    case 7: strcpy(tostr, "1");
                        break;
                    case 8: strcpy(tostr, "2");
                        break;
                    case 9: strcpy(tostr, "3");
                        break;
                    case 10: tocust = 0x2A;
                        break;
                    case 11: strcpy(tostr, "0");
                        break;
                    case 12: strcpy(tostr, "\n");
                        break;
                    }
                }

            }

            kchat = 254;
            tkey = key;

        }

        if (kchat && keypad() != key) kchat--;

        //ENSW
        if (!GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_A, 1)))
        {
            ekchat = 254;
        }
        if (ekchat && GPIO_digitalRead(GPIOv_from_PORT_PIN(GPIO_port_A, 1)))
        {
            ekchat--;
            if (!ekchat)
            {
                if (mode != 3)
                {
                    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), high);
                    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, 0), high);
                    mode = 3;
                }
                else
                {
                    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_A, 2), low);
                    GPIO_digitalWrite(GPIOv_from_PORT_PIN(GPIO_port_C, 0), high);
                    mode = 0;
                }
            }
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

		if (tostr[i] == 0) //��������
		{
			tsajoystick[0] = 0;
			tsajoystick[2] = 0;
			tostr[i] = 0;
			i = 0;
		}
		if (tonext == 1 || tonext == 2)
		{
			tonext++;
			tsajoystick[0] = 0;
			tsajoystick[2] = 0;
            tocust = 0;
            kochange = 0;
		}
		if (tonext == 3) 
		{
            tsajoystick[0] = 0;
            tsajoystick[2] = 0;
			tonext = 0;
			kochange = 0;
            tocust = 0;
		}
		if (tostr[i] && tonext == 0)
		{
			bitclear(tsajoystick[0], 1);
			if (tostr[i] >= 'a' && tostr[i] <= 'z') //���ĺ��ҹ���
			{
				tsajoystick[2] = 4 + tostr[i] - 'a';
			}
			if (tostr[i] >= 'A' && tostr[i] <= 'Z') //���ĺ��빮��
			{
				tsajoystick[2] = 4 + tostr[i] - 'A';
				bitset(tsajoystick[0], 1);
			}
			else if (tostr[i] >= '1' && tostr[i] <= '9') //����1~9
				tsajoystick[2] = 0x1E + tostr[i] - '1';
			else if (tostr[i] == '0') //����0
				tsajoystick[2] = 0x27;
			else if (tostr[i] == '/') //Ư��/
				tsajoystick[2] = 0x38;
			else if (tostr[i] == ' ') //����
				tsajoystick[2] = 0x2C;
            else if (tostr[i] == '\n') //�ٹٲ�
                tsajoystick[2] = 0x28;
            else if (tostr[i] == '!') //Ư��!
            {
                tsajoystick[2] = 0x1E;
                bitset(tsajoystick[0], 1);
            }
            else if (tostr[i] == '?') //Ư��?
            {
                tsajoystick[2] = 0x38;
                bitset(tsajoystick[0], 1);
            }
            else if (tostr[i] == '~') //Ư��~
            {
                tsajoystick[2] = 0x35;
                bitset(tsajoystick[0], 1);
            }
			tonext = 1;
			tostr[i] = 0;
			i++;
		}
        if (tocust && tonext == 0)
        {
            tsajoystick[2] = tocust;
            tonext = 1;
        }
		if (kochange==1 && tonext == 0)
		{
			bitset(tsajoystick[0], 6);
			tonext = 1;
            kochange = 2;
		}

	}
	else
	{
		// If it's a control transfer, empty it.
		usb_send_empty( sendtok );
	}
}


