#include "WProgram.h"

extern "C" int main(void)
{
/*	
#if !defined(ARDUINO)

	// To use Teensy 3.0 without Arduino, simply put your code here.
	// For example:

	pinMode(13, OUTPUT);
	while (1) {
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);
	}
*/
#ifdef TEENSY_FREERTOS
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
	
	xTaskCreate( prvQueueReceiveTask,					/* The function that implements the task. */
				"Rx", 									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				configMINIMAL_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL, 									/* The parameter passed to the task - just to check the functionality. */
				mainQUEUE_RECEIVE_TASK_PRIORITY, 		/* The priority assigned to the task. */
				NULL );									/* The task handle is not required, so NULL is passed. */

	xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, ( void * ) mainQUEUE_SEND_PARAMETER, mainQUEUE_SEND_TASK_PRIORITY, NULL );

#else
	// Arduino's main() function just calls setup() and loop()....
	setup();
	while (1) {
		loop();
		yield();
	}
#endif
}

