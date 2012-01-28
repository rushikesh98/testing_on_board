This program is a simple demonstration of how the Virtual Comport can be
used to send and receive data from the PC.  It does not us CircleOS, just
some of the components.  It is a combination of the MODBUS and VirtualComPort
programs found in the Project area of www.stm32circle.com

To use this program, open the project, go to debug then start and then run.
It does not have any of the shutdown features, so it never really turns off. 
I'm still figuring that out.

There are three programs to test. The Virtual Comport, JTAG GPIO Remap, and a
CC25xx programmer(not functioning, it is more an example of FSA parsing).

To use each one open the file in Ride7 and then do a saveas
and save the file as main.c. Then you can do another debug->start then run the
new program without having to worry about all the other files.

A simple Serial Port terminal program is also include that allows you to interface
with the EVO.  You can use Hyperteminal and the letters 'u', 'd', 'l', 'r' and 's'
to change LCD display.
 

The original Virtual Comport program was intended to act as an FTDI serial
to USB converter or bridge.

For those who want to use the USB functionality of the STM32 without having
to write or rewrite software on the PC side using USB or HID, then this 
offers a solution.

You will still have to change your code in the STM32 to use USB.  However,
the examples are simple and there are really only two functions that you
need to know.  Although, you might as well read up on how to implement a 
full USB firmware design.  Simple examples that do a good job at explaining
how everything works are very hard to find.  You can consider this a start...


The functions you will be dealing with are the USB_SIL_READ and USB_SIL_WRITE.
These manage most of the USB leg work.  Once you know what endpoints are being
used for what, ie.  

EP1_IN would be for data going from the STM32 to the PC
EP3_OUT would be used for reading data from the PC through the USB

    // USB_SIL_READ function gets data from the USB that was sent to
    // the EP3_OUT endpoint from the PC and is then put in the msg_buf1

        USB_SIL_Read(EP3_OUT, msg_buf1);

    // USB_SIL_WRITE function stores data in msg_buf1 along with the number
    // number of data bytes to be sent to the PC.  In this case only byte 
    // that was just received is echoed back to hyperterminal

        USB_SIL_Write(EP1_IN, msg_buf1,1);
    
    //  SetEPTxValid function sets the EP_TX_VALID allowing for the data 
    //  and size to be sent to the PC using USB endpoint ENDP1

        SetEPTxValid(ENDP1);

