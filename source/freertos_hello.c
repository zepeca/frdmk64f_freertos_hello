/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define TASK_ADMIN_PRIORITY (configMAX_PRIORITIES - 2)
#define TASK_CMD_PRIORITY   (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void task_admin();
static void task_cmd_left();
static void task_cmd_right();
static void task_cmd_forwrd();
static void task_cmd_bckwrd();

/*******************************************************************************
 * Type def
 ******************************************************************************/
typedef enum{
    CMD_LEFT = 0,
    CMD_RIGHT,
    CMD_FORWRD,
    CMD_BCKWRD
}COMMAND_en;


/*******************************************************************************
 * Global var
 ******************************************************************************/
static uint8_t cmd_rutine_array [] = {CMD_RIGHT,CMD_RIGHT, CMD_LEFT, CMD_LEFT, CMD_FORWRD, CMD_FORWRD, CMD_BCKWRD, CMD_BCKWRD};

static TaskHandle_t id_task_admin;
static TaskHandle_t id_task_cmd_left;
static TaskHandle_t id_task_cmd_right;
static TaskHandle_t id_task_cmd_forwrd;
static TaskHandle_t id_task_cmd_bckwrd;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*carlosa admin task creation*/
    if (xTaskCreate(task_admin, "task_admin", configMINIMAL_STACK_SIZE + 100, NULL, TASK_ADMIN_PRIORITY, &id_task_admin) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    /*carlosa LEFT task creation*/
    if (xTaskCreate(task_cmd_left, "task_cmd_left", configMINIMAL_STACK_SIZE + 100, NULL, TASK_CMD_PRIORITY, &id_task_cmd_left) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    /*carlosa RIGHT task creation*/
    if (xTaskCreate(task_cmd_right, "task_cmd_right", configMINIMAL_STACK_SIZE + 100, NULL, TASK_CMD_PRIORITY, &id_task_cmd_right) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    /*carlosa FOREWARD task creation*/
    if (xTaskCreate(task_cmd_forwrd, "task_cmd_forwrd", configMINIMAL_STACK_SIZE + 100, NULL, TASK_CMD_PRIORITY, &id_task_cmd_forwrd) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

        /*carlosa BACKWARD task creation*/
    if (xTaskCreate(task_cmd_bckwrd, "task_cmd_bckwrd", configMINIMAL_STACK_SIZE + 100, NULL, TASK_CMD_PRIORITY, &id_task_cmd_bckwrd) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    vTaskStartScheduler();

    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */

static void task_admin( void ){
    
    for (uint8_t cmd_rutine_id = 0; cmd_rutine_id < 0x07/*sizeof(cmd_rutine_array)*/; cmd_rutine_id++){

        switch(cmd_rutine_array[cmd_rutine_id]){

            case CMD_LEFT:
            vTaskResume( id_task_cmd_left );
            break;

            case CMD_RIGHT:
            vTaskResume( id_task_cmd_right );
            break;

            case CMD_FORWRD:
            vTaskResume( id_task_cmd_forwrd );
            break;

            case CMD_BCKWRD:
            vTaskResume( id_task_cmd_bckwrd );
            break;

            default:
            break;
        }
    }

    for(;;){
    	vTaskSuspend(NULL);
    }
};

static void task_cmd_left(){
    vTaskSuspend(NULL);
    PRINTF("Left");
};

static void task_cmd_right(){
    vTaskSuspend(NULL);
    PRINTF("Right");
};

static void task_cmd_forwrd(){
    vTaskSuspend(NULL);
    PRINTF("Foreward");
};

static void task_cmd_bckwrd(){
    vTaskSuspend(NULL);
    PRINTF("Backward");
};
