/*
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee�s application development. 
* Fitness and suitability of the example code for any use within Licensee�s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/

/* module includes ********************************************************** */
/**
 * @ingroup APPS_LIST
 *
 * @defgroup EXTBUS_TEMP_IO_BOARD TemperaturaSerial
 * @{
 *	
 * @brief  Extension Temperature IO Board provides the Implementation of Extension Temperature IO Board(MAX31865) prints Temperature Data from PT100/1000 to Serial Console.
 *
 * @details External Temperature Sensor and Digital I/O
 * The XDK External Temperature Sensor and Digital I/O extension board consists of three main elements: the external temperature, two digital inputs and two digital outputs.
 * Please have a look into the extension of the <b> <a href="https://xdk.bosch-connectivity.com/extensions">External Temperature Sensor and Digital I/O Board</a></b>.This web page will go over how to use each element of the board and the maximum rating of each element. 
 *
 * @file TemperaturaSerial.c
 *
 **/

/* own header files */
#include "XDKAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID XDK_APP_MODULE_ID_EXT_TEMPERATURE_IO_BOARD

/*own headers */
#include "TemperaturaSerial.h"

/* system header files */
#include <stdio.h>
#include <stdbool.h>

/* additional interface header files */
#include "BCDS_Assert.h"
#include "BCDS_CmdProcessor.h"
#include "BCDS_Environmental.h"
#include "XdkSensorHandle.h"
#include "BCDS_Max31865.h"
#include "BSP_BoardType.h"
#include "BCDS_BSP_Board.h"
#include "BCDS_BSP_Extension_Gpio.h"
#include "FreeRTOS.h"
#include "timers.h"
/* constant definitions ***************************************************** */
#define ONE_SECOND_DELAY                  UINT32_C(1000)      /**< one second delay is represented by this macro */
#define TIMER_BLOCK_TIME                  UINT32_C(0xffff)    /** Macro used to define block time of a timer*/

/* local variables ********************************************************** */
static xTimerHandle printTimerHandle;/**< variable to store timer handle*/

/* global variables ********************************************************* */
long int numMed = 0;
long int tempLim = 30000;

/*Application Command Processor Instance */
CmdProcessor_T *AppCmdProcessor;

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************** */

/** The function to get and print the Temperature data using printf
 * @brief Gets the data from External Temperature sensor and prints through the USB printf on serial port ,
 * 			If the External Temperature Sensor Not connected it will prints the Internal Temperature Sensor Data through USB using Printf
 *
 */
static void processTempData(void * param1, uint32_t param2)
{
    Retcode_T envSensorRetVal = (Retcode_T) RETCODE_FAILURE;
    Retcode_T maxSensorRetVal = (Retcode_T) RETCODE_FAILURE;
    float externalSensorData =(0);
    float resistance;
    Environmental_Data_T bme280 = { INT32_C(0), UINT32_C(0), UINT32_C(0) };

    BCDS_UNUSED(param1);
    BCDS_UNUSED(param2);

    envSensorRetVal = Environmental_readData(xdkEnvironmental_BME280_Handle, &bme280);
    if (RETCODE_OK != envSensorRetVal)
    {
        printf("Failed to read the BME280 Internal Temperature Sensor Data \n\r");
    }
    maxSensorRetVal = MAX31865_ReadTemperature(&externalSensorData);

    if (RETCODE_OK == maxSensorRetVal)
    {
    	maxSensorRetVal = MAX31865_ReadResistance(&resistance);
    	if(RETCODE_OK == maxSensorRetVal)
    	{
    		printf("External Temperature sensor data obtained %f Degrees on Resistance of %f Ohms\n\r",externalSensorData,resistance);
    	}
    	else
    	{
    		printf("Failed to Read the Resistance \n\r");
    	}
    }
    else
    {
    	if(envSensorRetVal == RETCODE_OK)
    	{
    		// printf("Internal Temperature sensor data obtained: %ld mDeg\n\r", (long int) bme280.temperature);
    		long int tempActual = (long int) bme280.temperature;
    		numMed += 1;
    		if (tempActual < tempLim)
    		{
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_OFF);
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_OFF);
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
    		}
    		else
    		{
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_ON);
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_ON);
    			BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_ON);
    		}
    		printf("%ld %ld %ld\n\r", tempActual, numMed, tempLim);
    	}
    	// printf("Failed to Read the Temperature from External Temperature Sensor \n\r");
        /* Recovery strategy for external sensor */
    	maxSensorRetVal = MAX31865_Disconnect();
        if (RETCODE_OK == maxSensorRetVal)
        {
        	maxSensorRetVal = MAX31865_Connect();
            if (RETCODE_OK != maxSensorRetVal)
            {
                printf("Failed to Connect the Ext MAX31865 Sensor \n\r");
            }
        }
        else
        {
            printf("Failed to DisConnect the Ext MAX31865 Sensor \n\r");
        }

    }

}

/** The function to get and print the Environmental data using printf
 * @brief Gets the temperature data from BME280 Environmental and external sensor and prints
 *        through the USB printf on serial port
 *
 * @param[in] pvParameters Rtos task should be defined with the type void *(as argument)
 */
static void printTempData(void *pvParameters)
{
    BCDS_UNUSED(pvParameters);

    Retcode_T returnValue = RETCODE_OK;
    returnValue = CmdProcessor_Enqueue(AppCmdProcessor, processTempData, NULL, UINT32_C(0));
    if (RETCODE_OK != returnValue)
    {
        printf("Enqueuing for environmental sensor callback failed\n\r");
    }
}

/**
 * @brief The function initializes Temperature sensor, creates and starts a auto reloaded
 * timer task which gets and prints the Environmental and external temperature data
 *
 * @retval  RETCODE_OK
 *                 If Sensor Initialization Success.
 *
 * @retval  RETCODE_FAILURE
 *                  If Sensor Initialization Failed.
 */
Retcode_T TemperatureSensorInit(void)
{
    Retcode_T returnValue = (Retcode_T) RETCODE_FAILURE;
    /* Return value for Timer start */
    int8_t retValPerSwTimer = pdFAIL;

    /*initialize Temperature sensors*/
    returnValue = Environmental_init(xdkEnvironmental_BME280_Handle);

    if (RETCODE_OK == returnValue)
    {
    	/* Set the External Temperature Sensor Type is either PT100 / PT1000 , by default we are configured PT1000 */
    	returnValue =MAX31865_SetSensorType(TEMPERATURE_SENSOR_PT100);
    }

    if (RETCODE_OK == returnValue)
    {
        returnValue = MAX31865_Connect();
    }

    if (RETCODE_OK == returnValue)
    {
        /* External GPIO Temperature Sensor Input A */
        returnValue = BSP_Extension_Gpio_Connect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_IN_A);
    }
    if (RETCODE_OK == returnValue)
    {
        /* External GPIO Temperature Sensor Input B */
        returnValue = BSP_Extension_Gpio_Connect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_IN_B);
    }
    if (RETCODE_OK == returnValue)
    {
        /* External GPIO Temperature Sensor Output A */
        returnValue = BSP_Extension_Gpio_Connect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_OUT_A);
    }
    if (RETCODE_OK == returnValue)
    {
        /* External GPIO Temperature Sensor Output B */
        returnValue = BSP_Extension_Gpio_Connect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_OUT_B);
    }
    if ( RETCODE_OK == returnValue)
    {
        uint32_t Ticks = ONE_SECOND_DELAY;

        if (Ticks != UINT32_MAX) /* Validated for portMAX_DELAY to assist the task to wait Infinitely (without timing out) */
        {
            Ticks /= portTICK_RATE_MS;
        }
        if (UINT32_C(0) == Ticks) /* ticks cannot be 0 in FreeRTOS timer. So ticks is assigned to 1 */
        {
            Ticks = UINT32_C(1);
        }
        /* create timer task to get and print Temperature data every one second automatically*/
        printTimerHandle = xTimerCreate((const char * const ) "printTempData",
                Ticks, pdTRUE, NULL, printTempData);

        /* timer create fail case */
        if (NULL == printTimerHandle)
        {
            /* Assertion Reason : "This software timer was not Created, Due to Insufficient heap memory" */
            printf("Software timer Failed to create Due to insufficient Heap memory\n\r");
            assert(false);
        }
        /*start the created timer*/
        retValPerSwTimer = xTimerStart(printTimerHandle,
                TIMER_BLOCK_TIME);

        /* PED timer start fail case */
        if (pdFAIL == retValPerSwTimer)
        {
            /* Assertion Reason :  "This software timer was not started, Due to Insufficient heap memory" */
            printf("Software timer Failed to Start Due to insufficient Heap memory\n\r");
            assert(false);
        }
        printf("Temperature Sensor initialization Success\n\r");
    }
    else
    {
        printf("Temperature Sensor initialization Failed\n\r");
    }
    return returnValue;

}

/**
 *  @brief API to De initialize the External Temperature Sensor
 *
 *  @retval  RETCODE_OK
 *                 If Sensor DeInitialization Success.
 *
 * @retval  RETCODE_FAILURE
 *                  If Sensor DeInitialization Failed.
 */
Retcode_T TemperatureSensorDeinit(void)
{
    Retcode_T returnValue = (Retcode_T) RETCODE_FAILURE;

    returnValue = Environmental_deInit(xdkEnvironmental_BME280_Handle);
    if (RETCODE_OK != returnValue)
    {
        printf("Environmental sensor De init Failed\n\r");
    }
    else
    {
        returnValue = MAX31865_Disconnect();
        if (RETCODE_OK == returnValue)
        {
            returnValue = BSP_Extension_Gpio_Disconnect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_IN_A);
            if (RETCODE_OK == returnValue)
            {
                returnValue = BSP_Extension_Gpio_Disconnect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_IN_B);
            }
            if (RETCODE_OK == returnValue)
            {
                returnValue = BSP_Extension_Gpio_Disconnect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_OUT_A);
            }
            if (RETCODE_OK == returnValue)
            {
                returnValue = BSP_Extension_Gpio_Disconnect((uint32_t) BSP_GPIO_EXT_TEMPERATURE_OUT_B);
            }
            if (RETCODE_OK == returnValue)
            {
                printf("MAX31865 Temperature sensor De init Success\n\r");
            }
            else
            {
                printf("Extension GPIO Disconnect Failed\n\r");
            }

        }
        else
        {
            printf("MAX31865 Temperature  sensor De init Failed\n\r");
        }
    }
    	/* stop the timer */
    if (pdTRUE != xTimerStop(printTimerHandle, UINT8_C(0)))
    {
        /* Assertion Reason: Failed to stop software timer. Check command queue size of software timer service. */
        printf("Failed to stop software timer \n\r");
        assert(false);
    }
    return returnValue;

}

/**
 * @brief The function initializes Temperature Sensor which calls the individual sensor data print functions
 * The functions prints the data of the Temperature Sensor
 *
 * @retval  RETCODE_OK
 *                 If Temperature Sensor Initialization Success.
 *
 * @retval  RETCODE_FAILURE
 *                  If Temperature Sensor Initialization Failed.
 */
Retcode_T Init(void)
{

    Retcode_T returnValue = RETCODE_OK;
    returnValue = TemperatureSensorInit();
    return returnValue;
}

/* Process the Button1 Data in side the Application context */
static void processButton1Data(void * param1, uint32_t buttonstatus)
{
    BCDS_UNUSED(param1);
    Retcode_T retVal = RETCODE_OK;
    switch (buttonstatus)
    {
    case BSP_XDK_BUTTON_PRESS:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_ON);
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == retVal)
        {
            // printf("PB1 Pressed \n\r");
        	tempLim -= 500;
        }
        else
        {
            printf("PB1 Pressed but setting LED state failed\n\r");
        }
    }
        break;

    case BSP_XDK_BUTTON_RELEASE:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_OFF);
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (RETCODE_OK == retVal)
        {
            // printf("PB1 Released\n\r");
        }
        else
        {
            printf("PB1 Released but setting LED state failed\n\r");
        }
    }
        break;

    default:
        printf("FATAL Error:Unsolicited button event occurred for PB1 \n\r");
        break;

    }

}

/*Callback for Button 1 */
void Button1Callback(uint32_t data)
{
    Retcode_T returnValue = CmdProcessor_EnqueueFromIsr(AppCmdProcessor, processButton1Data, NULL, data);
    if (RETCODE_OK != returnValue)
    {
        printf("Enqueuing for Button 1 callback failed\n\r");
    }
}

/* Process the Button2 Data inside the Application context */
static void processButton2Data(void * param1, uint32_t buttonstatus)
{
    BCDS_UNUSED(param1);
    Retcode_T retVal = RETCODE_OK;
    switch (buttonstatus)
    {
    case BSP_XDK_BUTTON_PRESS:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_ON);
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (RETCODE_OK == retVal)
        {
            // printf("PB2 pressed\n\r");
        	tempLim += 500;
        }
        else
        {
            printf("PB2 Pressed but setting LED state failed\n\r");
        }
    }
        break;

    case BSP_XDK_BUTTON_RELEASE:
        {
        retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_R, (uint32_t) BSP_LED_COMMAND_OFF);
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_O, (uint32_t) BSP_LED_COMMAND_OFF);
        }
        if (RETCODE_OK == retVal)
        {
            retVal = BSP_LED_Switch((uint32_t) BSP_XDK_LED_Y, (uint32_t) BSP_LED_COMMAND_ON);
        }
        if (RETCODE_OK == retVal)
        {
            // printf("PB2 Released\n\r");
        }
        else
        {
            printf("PB2 Released but setting LED state failed\n\r");
        }
    }
        break;

    default:
        printf("FATAL Error:Unsolicited button event occurred for PB2 \n\r");
        break;

    }
}

/*Callback for Button 2 */
void Button2Callback(uint32_t data)
{
    Retcode_T returnValue = CmdProcessor_EnqueueFromIsr(AppCmdProcessor, processButton2Data, NULL, data);
    if (RETCODE_OK != returnValue)
    {
        printf("Enqueuing for Button 2 callback failed\n\r");
    }
}

/* Routine to Initialize the LED */
static Retcode_T LedInitialize(void)
{
    Retcode_T returnVal = RETCODE_OK;
    returnVal = BSP_LED_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_R);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_O);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_LED_Enable((uint32_t) BSP_XDK_LED_Y);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("LED Initialization succeed without error %u \n", (unsigned int) returnVal);
    }
    else
    {
        printf(" Error occurred in LED Initialization routine %u \n", (unsigned int) returnVal);
    }
    return returnVal;
}

/* Routine to Initialize the Button module  */
static Retcode_T ButtonInitialize(void)
{
    Retcode_T returnVal = RETCODE_OK;
    returnVal = BSP_Button_Connect();
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_Button_Enable((uint32_t) BSP_XDK_BUTTON_1, Button1Callback);
    }
    if (RETCODE_OK == returnVal)
    {
        returnVal = BSP_Button_Enable((uint32_t) BSP_XDK_BUTTON_2, Button2Callback);
    }
    if (RETCODE_OK == returnVal)
    {
        printf("BUTTON Initialization success");
    }
    else
    {
        printf(" Error occurred in BUTTON Initialization routine %u \n", (unsigned int) returnVal);
    }

    return returnVal;
}

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
    if (CmdProcessorHandle == NULL)
    {
        printf("Command processor handle is null \n\r");
        assert(false);
    }
    AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
    BCDS_UNUSED(param2);
    Retcode_T returnValue = RETCODE_OK;

    returnValue = Init();
    if (RETCODE_OK == returnValue)
    {
        printf("Sensors initialization succeeded \n\r");
    }
    else
    {
        printf("Sensors initialization failed \n\r");
    }

    Retcode_T returnVal = RETCODE_OK;

        returnVal = LedInitialize();
        if (RETCODE_OK == returnVal)
        {
            returnVal = ButtonInitialize();
        }
        if (RETCODE_OK != returnVal)
        {
            printf("Error in Initializing the LED AND BUTTON APPLICATION \n ");
        }
        else
        {
            printf("Successful Initialize of LED AND BUTTON APPLICATION \n ");
        }
}

/**
 *  @brief API to De initialize the sensor modules
 *
 * @retval  RETCODE_OK
 *                 If Temperature Sensor DeInitialization Success.
 *
 * @retval  RETCODE_FAILURE
 *                  If Temperature Sensor DeInitialization Failed.
 */
Retcode_T deinit(void)
{
    Retcode_T returnValue = RETCODE_OK;
    returnValue = TemperatureSensorDeinit();
    if (RETCODE_OK == returnValue)
    {
        printf("Sensors initialization succeeded \n\r");
    }
    else
    {
        printf("Sensors initialization failed \n\r");
    }
    return returnValue;

}

/**@} */
/** ************************************************************************* */
