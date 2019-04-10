/**
 * Matthew Skipworth and Bruce Baker
 * Microprocessors Course Project
 * I2C implementation
 */

/**
 * @author Matthew Skipworth
 * @version 11 December, 2018
 */

/**
 * Included libraries/header files
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "sensorlib/i2cm_drv.h"

/**
 * Global variable definitions.
 */

#define SLAVE_ADDRESS_EXT 0x68   /*                                              // The rest are unsused.
#define NUM_OF_I2CBYTES 255
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define CLOCK_GPION 0x00001000 // value write to enable PortNclkctrl
#define GPIO_PORTN_AFSEL_R      (*((volatile unsigned long *)0x40064420))
#define GPIO_PORTN_PCTL_R       (*((volatile unsigned long *)0x4006452C))
#define GPIO_PORTN_DEN_R        (*((volatile unsigned long *)0x4006451C))
#define GPIO_PORTN_ODR_R        (*((volatile unsigned long *)0x4006450C))
#define I2C2_MTPR_R             (*((volatile unsigned long *)0x4002200C))
#define I2C2_MCR_R              (*((volatile unsigned long *)0x40022020))
#define I2C2_MSA_R              (*((volatile unsigned long *)0x40022000))
#define I2C2_MCS_R              (*((volatile unsigned long *)0x40022004))
#define I2C2_MDR_R              (*((volatile unsigned long *)0x40022008))
#define I2C2_MTPR_R             (*((volatile unsigned long *)0x4002200C))
#define I2C2_MCR_R              (*((volatile unsigned long *)0x40022020))
*/
/**
 * Global Variables
 */
unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;
uint32_t ui32SysClkFreq;
/**
 * Function Prototypes
 */
unsigned char get_data();
unsigned char BCD_conversion(unsigned int);

/**
 * main function, drives the program
 */
int main(void) {

/**
 * clock and I2C module initialization
 */


    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_16MHZ |SYSCTL_OSC_MAIN |SYSCTL_USE_OSC), 16000000); //set the system clock frequency
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2); //enable the I2C2 module

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2)) {;} //wait until the module is ready before assigning it pins.

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); //enable GPIO port N


    GPIOPinConfigure(GPIO_PN4_I2C2SDA); //set up PN4 to be SDA, PN5 to be SCL
    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);

    GPIOPinConfigure(GPIO_PN5_I2C2SCL);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);


    I2CMasterInitExpClk(I2C2_BASE, ui32SysClkFreq, false); //set the transmission speed to 100Kb/s


    I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDRESS_EXT, false); //initiating a write to the slave...

    I2CMasterDataPut(I2C2_BASE, 0x00); //select the zeroth register
    I2CMasterControl(I2C2_BASE, 0x03); //start and transmit the slave register address
    while(I2CMasterBusy(I2C2_BASE)) {;}

    I2CMasterDataPut(I2C2_BASE, 0); //write a 0 to the clock halting bit to start the RTC oscillator
    I2CMasterControl(I2C2_BASE, 0x07); //transmit
    while(I2CMasterBusy(I2C2_BASE)) {;}


    while(1){

        //This code should work, however I didn't get to test is without an RTC module.


        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x00); // get seconds
        while(I2CMasterBusy(I2C2_BASE)) {;}
        seconds = BCD_conversion(I2CMasterDataGet(I2C2_BASE)); // assign seconds variable to the current MDR value

        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x01); //get minutes
        while(I2CMasterBusy(I2C2_BASE)) {;}
        minutes = BCD_conversion(I2CMasterDataGet(I2C2_BASE)); // assign minutes variable to the current MDR value

        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x02); //get hours
        while(I2CMasterBusy(I2C2_BASE)) {;}
        hours = BCD_conversion(I2CMasterDataGet(I2C2_BASE));   //assign hours variable to the current MDR value

        /*
        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x00); // get seconds
        while(I2CMasterBusy(I2C2_BASE)) {;}
        seconds = I2CMasterDataGet(I2C2_BASE); // assign seconds variable to the current MDR value

        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x01); //get minutes
        while(I2CMasterBusy(I2C2_BASE)) {;}
        minutes = I2CMasterDataGet(I2C2_BASE); // assign minutes variable to the current MDR value

        while(I2CMasterBusy(I2C2_BASE)) {;}
        get_data(0x02); //get hours
        while(I2CMasterBusy(I2C2_BASE)) {;}
        hours = I2CMasterDataGet(I2C2_BASE);   //assign hours variable to the current MDR value
*/

    }
    return 0;
}

/**
 * This function reads the register specified by the parameter slave_reg. slave_reg
 * a register on the RTC module. If a 2 passed in for instance, the Function would
 * get data from the hours register.
 */
 unsigned char get_data(unsigned long slave_reg) {

    unsigned char result = 0xFF;

  // while(I2CMasterBusy(I2C2_BASE)) {;}
   I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDRESS_EXT, false); //set the bus up for a write
   I2CMasterDataPut(I2C2_BASE, slave_reg); // select the register extension to read from
   I2CMasterControl(I2C2_BASE, 0x07); //write the address extension and go back to idle state.
   while(I2CMasterBusy(I2C2_BASE)) {;}


   while(I2CMasterBusy(I2C2_BASE)) {;}
   I2CMasterSlaveAddrSet(I2C2_BASE, SLAVE_ADDRESS_EXT, true); //get ready for read.
   while(I2CMasterBusy(I2C2_BASE)) {;}
   I2CMasterControl(I2C2_BASE, 0x07); //read data and go back to idle state.
   while(I2CMasterBusy(I2C2_BASE)) {;}
   if(!(I2CMasterErr(I2C2_BASE))) {

        result = I2CMasterDataGet(I2C2_BASE); //put read data in the Master Data Register
    }
    return result;
}
/**
 * BCD_conversion function used to convert a BCD number into a decimal number.
 * data represents a binary coded decimal number being passed in.
 * the function returns the number in decimal format
 */

 unsigned char BCD_conversion(unsigned int data) {
     unsigned char result = 0, num1 = 0, num2 = 0;
     num1 = num2 = data;
     num1 &= 9;//mask all bits but 0, 1, 2, and 3.
     num2 &= 240; //mask all bits but the second set of 4, 5, 6, and 7.
     num2 = num2 >> 4; //move num2 to the 10's place.
     num2 *= 10;
     result = num1+num2; // add them together.

     return result;
 }



