#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "text.h"

#include "freertos_hello.h"
#include "max7219.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_BASEADDR         SPI0
#define DSPI_MASTER_CLK_SRC                  DSPI0_CLK_SRC
#define DSPI_MASTER_CLK_FREQ                 CLOCK_GetFreq(DSPI0_CLK_SRC)
#define EXAMPLE_DSPI_MASTER_PCS_FOR_INIT     kDSPI_Pcs0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0
#define EXAMPLE_DSPI_DEALY_COUNT             0xfffffU

#define TRANSFER_SIZE     1U     /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter  = 20U;

uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

uint8_t max7219_num;
uint8_t *__max7219_buffer;
uint8_t __max7219_buffer_size;

const uint8_t max7219_initseq[]  =
{
			0x09, 0x00,	// Decode-Mode Register, 00 = No decode
			0x0A, 0x01,	// Intensity Register, 0x00 .. 0x0f
			0x0B, 0x07,	// Scan-Limit Register, 0x07 to show all lines
			0x0C, 0x01,	// Shutdown Register, 0x01 = Normal Operation
			0x0F, 0x00,	// Display-Test Register, 0x01, 0x00 = Normal
};

uint8_t pattern_smileyface[] = { // Smile face (:
		0b00111100,
		0b01000010,
		0b10100101,
		0b10000001,
		0b10100101,
		0b10011001,
		0b01000010,
		0b00111100,
};

uint8_t pattern_cmd_left[] = {
        0b00010000,
        0b00110000,
        0b01110000,
        0b11111111,
        0b11111111,
        0b01110000,
        0b00110000,
        0b00010000,
};

uint8_t pattern_cmd_right[] = {
        0b00001000,
        0b00001100,
        0b00001110,
        0b11111111,
        0b11111111,
        0b00001110,
        0b00001100,
        0b00001000,
};

uint8_t pattern_cmd_forwrd[] = {
        0b00011000,
        0b00111100,
        0b01111110,
        0b11111111,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
};

uint8_t pattern_cmd_bckwrd[] = {
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00011000,
};

uint8_t letterG[]={
		0b00011100,
		0b00100010,
		0b01000000,
		0b01000000,
		0b01001110,
		0b01000010,
		0b00100010,
		0b00011100,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

void vApplicationTickHook(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void delay_ms(uint16_t miliseconds)
{
	/* Delay to wait slave is ready */
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}
	/* Delay 100 ms */
	g_systickCounter = miliseconds;
	while (g_systickCounter != 0U)
	{
	}
}
/*Address - register where the data will the written*/
/*data - value to send to the matrix */
/*num - number of led matrixes*/
void max7219_word(uint8_t address, uint8_t data, uint8_t num)
{
	dspi_transfer_t masterXfer;
	uint8_t n;
	uint8_t masterTxData[2] = {0U};

	masterTxData[0] = data;
	masterTxData[1] = address;

	for (n = num; n != 0; n--) // Send multiple times for cascaded matrices
	{
		masterXfer.txData      = masterTxData;
		masterXfer.rxData      = NULL;
		masterXfer.dataSize    = sizeof(masterTxData);
		masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

		//send via SPI
		DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);
    }
}

/*Address - register where the data will the written*/
/*ptrdata - pointer to buffer value to send to the matrix */

void max7219_doubleLong(uint8_t address, uint8_t *ptrtodata)
{
	dspi_transfer_t masterXfer;
	uint8_t i;
	uint8_t masterTxData[8] = {0U};

	if(1)
	{
		masterTxData[0] = 0x00;
		masterTxData[1] = address;
		masterTxData[2] = *ptrtodata;
		masterTxData[3] = address;
		masterTxData[4] = 0x00;
		masterTxData[5] = address;
		masterTxData[6] = *ptrtodata;
		masterTxData[7] = address;
	}
	else
	{
		//Fill the SPI array to send.
		for (i=0; i<=6; i+=2)
		{
			masterTxData[i] = *ptrtodata;
			masterTxData[i+1] = address;
			ptrtodata++;
		}
	}

	masterXfer.txData      = masterTxData;
	masterXfer.rxData      = NULL;
	masterXfer.dataSize    = sizeof(masterTxData);
	masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;

	//send via SPI
	DSPI_MasterTransferBlocking(EXAMPLE_DSPI_MASTER_BASEADDR, &masterXfer);
}

void max7219_outputbuffer(void)
{
	uint8_t *bufferPtr = NULL;
    uint8_t row;
    uint8_t buffer_seg;

    bufferPtr = max7219_buffer;
    for (row = 1; row <= 8; row++)
    {
    	max7219_doubleLong(row, max7219_buffer);
		bufferPtr++;
		delay_ms(100);
    }
}

void max7219_init(uint8_t num, uint8_t *buffer, uint8_t buffer_size)
{
    uint8_t opcode;
    uint8_t opdata;
    uint8_t i;

    max7219_num = num;
    for (i = 0; i < sizeof (max7219_initseq);)
    {
		opcode = max7219_initseq[i++];
		opdata = max7219_initseq[i++];
		max7219_word(opcode, opdata, max7219_num);
    }

    //Clean the matrix
	for (i = 1; i <= 8; i++)
	{
		max7219_word(i, 0x00, max7219_num);
	}

	__max7219_buffer = buffer;
	__max7219_buffer_size = buffer_size;
}

void max7219_row(uint8_t address, uint8_t data)
{
    if (address >= 1 && address <= 8) max7219_word(address, data, max7219_num);
}

void max7219b_col(uint8_t x, uint8_t data)
{
	if (x < __max7219_buffer_size) __max7219_buffer[x] = data;
}

void max7219b_set(uint8_t x, uint8_t y)
{
    if (x < __max7219_buffer_size) __max7219_buffer[x] |= (1 << y);
}

void max7219b_clr(uint8_t x, uint8_t y)
{
    if (x < __max7219_buffer_size) __max7219_buffer[x] &= ~(1 << y);
}

uint8_t max7219b_get(uint8_t x)
{
    return __max7219_buffer[x];
}

void max7219b_left(void)
{
    memcpy(__max7219_buffer, __max7219_buffer + 1, __max7219_buffer_size - 1);
}

void SPI_init(void)
{
    dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;

	/* Master config */
    masterConfig.whichCtar                                = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame                  = 16U*4U; //Creo que tienes que multiplicar este por 4 (el numero de matrix que tienes)
    masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;

    masterConfig.whichPcs           = EXAMPLE_DSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK        = false;
    masterConfig.enableRxFifoOverWrite      = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint                = kDSPI_SckToSin0Clock;

    srcClock_Hz = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);
}

void drawFace(void)
{
	int row;
	uint8_t *facePtr = pattern_smileyface;
	for (row = 1; row <= 8; row++)
	{
		//Draw a face
		//max7219_word(row, pattern_smileyface[row-1], 4);
		//max7219_word(row, *facePtr, 4);
		max7219_doubleLong(row, facePtr);
		facePtr++;
		delay_ms(100);
	}
	return;
}

void draw_cmd(uint8_t cmd)
{
	int row;
    uint8_t *facePtr;

    switch (cmd)
    {
    case CMD_LEFT:
        facePtr = pattern_cmd_left;
        break;
    
    case CMD_RIGHT:
        facePtr = pattern_cmd_right;
        break;

    case CMD_FORWRD:
        facePtr = pattern_cmd_forwrd;
        break;
    
    case CMD_BCKWRD:
        facePtr = pattern_cmd_bckwrd;
        break;

    default:
        // Handle unknown command or add an error indicator
        break;
    }
	
	
    for (row = 1; row <= 8; row++)
	{
		max7219_doubleLong(row, facePtr);
		facePtr++;
		delay_ms(100);
	}
	return;
}

void drawText(void)
{
	int row;
	uint8_t *charPtr = letterG;
	for (row = 1; row <= 8; row++)
	{
		//Draw a face
		//max7219_word(row, pattern_smileyface[row-1], 4);
		//max7219_word(row, *facePtr, 4);
		max7219_doubleLong(row, charPtr);
		charPtr++;
		delay_ms(100);
	}
	return;
}

void cleanMatrix(void)
{
    uint8_t i;
    uint8_t r;
    uint8_t d;

	for (r = 1; r <= 8; r++)
	{
		d = 1;
		for (i = 9; i > 0; i--)
		{
			max7219_row(r, d);
			//PRINTF("Data: 0b%b\n\r",d);
			d = d << 1;
			delay_ms(50);
		}
	}
}
