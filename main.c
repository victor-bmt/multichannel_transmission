/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP430F5529LP:  simpleUsbBackchannel example
//
//   Description: 	Demonstrates simple sending over USB, as well as the F5529's
//                  backchannel UART.
//
//   Texas Instruments Inc.
//   August 2013
//******************************************************************************

// Basic MSP430 and driverLib #includes
#include "msp430.h"
#include "driverlib/MSP430F5xx_6xx/wdt_a.h"
#include "driverlib/MSP430F5xx_6xx/ucs.h"
#include "driverlib/MSP430F5xx_6xx/pmm.h"
#include "driverlib/MSP430F5xx_6xx/sfr.h"

// USB API #includes
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"

#include "USB_app/usbConstructs.h"

// Application #includes
#include "hal.h"              // Modify hal.h to select your hardware


/* You have a choice between implementing this as a CDC USB device, or a HID-
 * Datapipe device.  With CDC, the USB device presents a COM port on the host;
 * you interact with it with a terminal application, like Hyperterminal or
 * Docklight.  With HID-Datapipe, you interact with it using the Java HID Demo
 * App available within the MSP430 USB Developers Package.
 *
 * By default, this app uses CDC.  The HID calls are included, but commented
 * out.
 *
 * See the F5529 LaunchPad User's Guide for simple instructions to convert
 * this demo to HID-Datapipe.  For deeper information on CDC and HID-Datapipe,
 * see the USB API Programmer's Guide in the USB Developers Package.
 */

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Halt the dog

    // MSP430 USB requires a Vcore setting of at least 2.  2 is high enough
	// for 8MHz MCLK, below.
    PMM_setVCore(PMM_CORE_LEVEL_2);

    initPorts();           // Config all the GPIOS for low-power (output low)
    initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    initButtons();
    //bcUartInit();          // Init the back-channel UART
    USB_setup(TRUE,TRUE);  // Init USB; if a USB host (PC) is present, connect
    initTimerA();

    /*while(1)
    {
       // Look for rcv'ed bytes on the backchannel UART. If any, send over USB.
       rxByteCount = bcUartReceiveBytesInBuffer(buf_bcuartToUsb);
       if(rxByteCount)
       {
           cdcSendDataInBackground(buf_bcuartToUsb, rxByteCount, CDC0_INTFNUM, 1000);
           //hidSendDataInBackground(buf_bcuartToUsb, rxByteCount, HID0_INTFNUM, 1000);
       }

       // Look for received bytes over USB. If any, send over backchannel UART.
       rxByteCount = cdcReceiveDataInBuffer(buf_usbToBcuart, sizeof(buf_usbToBcuart), CDC0_INTFNUM);
       //rxByteCount = hidReceiveDataInBuffer(buf_usbToBcuart, sizeof(buf_usbToBcuart), HID0_INTFNUM);
       if(rxByteCount)
       {
           bcUartSend(buf_usbToBcuart, rxByteCount);
       }
    }*/

    BYTE buf1[][8] = {
    { (BYTE)1, (BYTE)1, (BYTE)128, (BYTE)255, (BYTE)9, (BYTE)4, (BYTE)21, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)141, (BYTE)254, (BYTE)17, (BYTE)9, (BYTE)21, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)154, (BYTE)252, (BYTE)26, (BYTE)13, (BYTE)21, (BYTE)52 },
    { (BYTE)1, (BYTE)1, (BYTE)167, (BYTE)249, (BYTE)34, (BYTE)17, (BYTE)21, (BYTE)58 },
    { (BYTE)1, (BYTE)1, (BYTE)180, (BYTE)244, (BYTE)43, (BYTE)21, (BYTE)21, (BYTE)61 },
    { (BYTE)1, (BYTE)1, (BYTE)192, (BYTE)238, (BYTE)51, (BYTE)26, (BYTE)43, (BYTE)63 },
    { (BYTE)1, (BYTE)1, (BYTE)203, (BYTE)231, (BYTE)60, (BYTE)30, (BYTE)43, (BYTE)62 },
    { (BYTE)1, (BYTE)1, (BYTE)213, (BYTE)222, (BYTE)68, (BYTE)34, (BYTE)43, (BYTE)58 },
    { (BYTE)1, (BYTE)1, (BYTE)222, (BYTE)213, (BYTE)77, (BYTE)38, (BYTE)43, (BYTE)53 },
    { (BYTE)1, (BYTE)1, (BYTE)231, (BYTE)203, (BYTE)85, (BYTE)43, (BYTE)43, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)238, (BYTE)192, (BYTE)94, (BYTE)47, (BYTE)64, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)244, (BYTE)180, (BYTE)102, (BYTE)51, (BYTE)64, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)249, (BYTE)167, (BYTE)111, (BYTE)55, (BYTE)64, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)252, (BYTE)154, (BYTE)119, (BYTE)60, (BYTE)64, (BYTE)50 },
    { (BYTE)1, (BYTE)1, (BYTE)254, (BYTE)141, (BYTE)128, (BYTE)64, (BYTE)64, (BYTE)43 },
    { (BYTE)1, (BYTE)128, (BYTE)255, (BYTE)128, (BYTE)136, (BYTE)68, (BYTE)85, (BYTE)32 },
    { (BYTE)1, (BYTE)128, (BYTE)254, (BYTE)115, (BYTE)145, (BYTE)72, (BYTE)85, (BYTE)154 },
    { (BYTE)1, (BYTE)128, (BYTE)252, (BYTE)102, (BYTE)153, (BYTE)77, (BYTE)85, (BYTE)220 },
    { (BYTE)1, (BYTE)128, (BYTE)249, (BYTE)89, (BYTE)162, (BYTE)81, (BYTE)85, (BYTE)157 },
    { (BYTE)1, (BYTE)128, (BYTE)244, (BYTE)76, (BYTE)170, (BYTE)85, (BYTE)85, (BYTE)98 },
        { (BYTE)1, (BYTE)128, (BYTE)238, (BYTE)65, (BYTE)179, (BYTE)90, (BYTE)106, (BYTE)43 },
        { (BYTE)1, (BYTE)128, (BYTE)231, (BYTE)53, (BYTE)187, (BYTE)94, (BYTE)106, (BYTE)27 },
        { (BYTE)1, (BYTE)128, (BYTE)222, (BYTE)43, (BYTE)196, (BYTE)98, (BYTE)106, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)213, (BYTE)34, (BYTE)204, (BYTE)102, (BYTE)106, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)203, (BYTE)25, (BYTE)213, (BYTE)106, (BYTE)106, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)192, (BYTE)18, (BYTE)221, (BYTE)111, (BYTE)128, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)180, (BYTE)12, (BYTE)230, (BYTE)115, (BYTE)128, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)167, (BYTE)7, (BYTE)238, (BYTE)119, (BYTE)128, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)154, (BYTE)4, (BYTE)247, (BYTE)123, (BYTE)128, (BYTE)51 },
        { (BYTE)1, (BYTE)128, (BYTE)141, (BYTE)2, (BYTE)255, (BYTE)128, (BYTE)128, (BYTE)51 },
        { (BYTE)255, (BYTE)255, (BYTE)128, (BYTE)1, (BYTE)247, (BYTE)132, (BYTE)149, (BYTE)54 },
        { (BYTE)255, (BYTE)255, (BYTE)115, (BYTE)2, (BYTE)238, (BYTE)136, (BYTE)149, (BYTE)57 },
        { (BYTE)255, (BYTE)255, (BYTE)102, (BYTE)4, (BYTE)230, (BYTE)140, (BYTE)149, (BYTE)62 },
        { (BYTE)255, (BYTE)255, (BYTE)89, (BYTE)7, (BYTE)221, (BYTE)145, (BYTE)149, (BYTE)68 },
        { (BYTE)255, (BYTE)255, (BYTE)76, (BYTE)12, (BYTE)213, (BYTE)149, (BYTE)149, (BYTE)73 },
        { (BYTE)255, (BYTE)255, (BYTE)65, (BYTE)18, (BYTE)204, (BYTE)153, (BYTE)170, (BYTE)79 },
        { (BYTE)255, (BYTE)255, (BYTE)53, (BYTE)25, (BYTE)196, (BYTE)157, (BYTE)170, (BYTE)83 },
        { (BYTE)255, (BYTE)255, (BYTE)43, (BYTE)34, (BYTE)187, (BYTE)162, (BYTE)170, (BYTE)86 },
        { (BYTE)255, (BYTE)255, (BYTE)34, (BYTE)43, (BYTE)179, (BYTE)166, (BYTE)170, (BYTE)87 },
        { (BYTE)255, (BYTE)255, (BYTE)25, (BYTE)53, (BYTE)170, (BYTE)170, (BYTE)170, (BYTE)86 },
            { (BYTE)255, (BYTE)255, (BYTE)18, (BYTE)65, (BYTE)162, (BYTE)174, (BYTE)191, (BYTE)82 },
            { (BYTE)255, (BYTE)255, (BYTE)12, (BYTE)76, (BYTE)153, (BYTE)179, (BYTE)191, (BYTE)76 },
            { (BYTE)255, (BYTE)255, (BYTE)7, (BYTE)89, (BYTE)145, (BYTE)183, (BYTE)191, (BYTE)65 },
            { (BYTE)255, (BYTE)255, (BYTE)4, (BYTE)102, (BYTE)136, (BYTE)187, (BYTE)191, (BYTE)57 },
            { (BYTE)255, (BYTE)255, (BYTE)2, (BYTE)115, (BYTE)128, (BYTE)191, (BYTE)191, (BYTE)54 },
            { (BYTE)255, (BYTE)128, (BYTE)1, (BYTE)128, (BYTE)119, (BYTE)196, (BYTE)213, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)2, (BYTE)142, (BYTE)111, (BYTE)200, (BYTE)213, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)4, (BYTE)154, (BYTE)102, (BYTE)204, (BYTE)213, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)7, (BYTE)167, (BYTE)94, (BYTE)208, (BYTE)213, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)12, (BYTE)180, (BYTE)85, (BYTE)213, (BYTE)213, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)18, (BYTE)192, (BYTE)77, (BYTE)217, (BYTE)234, (BYTE)52 },
            { (BYTE)255, (BYTE)128, (BYTE)25, (BYTE)203, (BYTE)68, (BYTE)221, (BYTE)234, (BYTE)54 },
            { (BYTE)255, (BYTE)128, (BYTE)34, (BYTE)213, (BYTE)60, (BYTE)225, (BYTE)234, (BYTE)56 },
            { (BYTE)255, (BYTE)128, (BYTE)43, (BYTE)222, (BYTE)51, (BYTE)230, (BYTE)234, (BYTE)58 },
            { (BYTE)255, (BYTE)128, (BYTE)53, (BYTE)231, (BYTE)43, (BYTE)234, (BYTE)234, (BYTE)60 },
            { (BYTE)255, (BYTE)128, (BYTE)65, (BYTE)238, (BYTE)34, (BYTE)238, (BYTE)255, (BYTE)61 },
            { (BYTE)255, (BYTE)128, (BYTE)76, (BYTE)244, (BYTE)26, (BYTE)242, (BYTE)255, (BYTE)61 },
            { (BYTE)255, (BYTE)128, (BYTE)89, (BYTE)249, (BYTE)17, (BYTE)247, (BYTE)255, (BYTE)59 },
            { (BYTE)255, (BYTE)128, (BYTE)102, (BYTE)252, (BYTE)9, (BYTE)251, (BYTE)255, (BYTE)54 },
            { (BYTE)255, (BYTE)128, (BYTE)115, (BYTE)254, (BYTE)0, (BYTE)255, (BYTE)255, (BYTE)52 }
    };

    __bis_SR_register(GIE);

    buttonFlag = FALSE;

    int row_number = 0;

    while(1)
    	while(buttonFlag)
    	{
    		//bcUartSend(buf1, sizeof(buf1));
    		cdcSendDataInBackground(buf1[row_number], sizeof(buf1[row_number]), CDC0_INTFNUM, 1000);

    		if (++row_number == 60)
    			row_number = 0;

    		//start the timer
    		TA0CTL |= 1 << 4;

    		// enter LPM
    		__bis_SR_register(LPM0_bits);
    	}
}
