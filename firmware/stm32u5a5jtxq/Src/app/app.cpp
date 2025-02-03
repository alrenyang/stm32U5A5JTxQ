#include "app.h"

// static void infoCli(uint8_t argc, const char **argv);

void apInit(void)
{
	cliInit();
	logInit();
	microsInit();
	uartInit();

    for (int i=0; i<HW_UART_MAX_CH; i++)
    {
        uartOpen(i, 115200);
    }

	logOpen(HW_LOG_CH, 115200);
	logPrintf("\r\n[ Firmware Begin... ]\r\n");
	logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
	logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);  
	logPrintf("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq()/1000000);
	logPrintf("\n");

	buttonInit();
	cdcInit();
	usbInit();
	usbBegin(USB_CMP_MODE);
 	keyscanInit();

	cliOpen(HW_UART_CH_CLI, 115200);
	logBoot(false);

    // cliAdd("info", infoCli);
}

void apMain(void) 
{
    uint32_t pre_time;

    pre_time = millis();
    while(1)
    {
        if (millis()-pre_time >= 500)
        {
            pre_time = millis();
        }

        cliMain();
    }
}

// void infoCli(uint8_t argc, const char **argv)
// {
//   bool ret = false;


//   if (argc == 1 && cliIsStr(argv[0], "test"))
// 	{
// 		cliPrintf("infoCli run test\r\n");
// 		ret = true;
// 	}

//   if (argc == 2 && cliIsStr(argv[0], "print"))
// 	{
// 	  uint8_t count;

// 	count = (uint8_t)cliGetData(argv[1]);
// 	for (int i=0; i<count; i++)
// 	{
// 	  cliPrintf("print %d/%d\r\n", i+1, count);
// 	}

// 		ret = true;
// 	}

//   if (argc == 1 && cliIsStr(argv[0], "led_on"))
// 	{
// 		// ledOn(_DEF_CH1);
// 		ret = true;
// 	}

//   if (argc == 1 && cliIsStr(argv[0], "led_off"))
// 	{
// 		// ledOff(_DEF_CH1);
// 		ret = true;
// 	}

//   if (argc == 1 && cliIsStr(argv[0], "netinfo"))
// 	{
// 		// print_network_information();
// 		ret = true;
// 	}

//   if (argc == 1 && cliIsStr(argv[0], "mqttrun"))
//   	{
// 	    // MqttRun();
//   		ret = true;
//   	}

//   if (ret == false)
// 	{
// 		cliPrintf("info test\r\n");
// 		cliPrintf("info print 0~10\r\n");
// 		cliPrintf("info led_on\r\n");
// 		cliPrintf("info led_off\r\n");
// 		cliPrintf("info netinfo\r\n");
// 		cliPrintf("info MQTTRUN\r\n");
// 	}
// }

