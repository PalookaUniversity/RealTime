/*
 * Example to demonstrate thread definition, semaphores, and thread sleep.
 */
#include <FreeRTOS_ARM.h>
// Redefine AVR Flash string macro as nop for ARM
#undef F
#define F(str) str

// The LED is attached to pin 13 on Arduino.
const uint8_t LED_PIN = 13;

// Declare a semaphore handle.
xSemaphoreHandle sem;
// Declare variable to report tick count
portTickType xWakeupTime;

//------------------------------------------------------------------------------
/*
 * Thread 1, turn the LED off when signalled by thread 2.
 */
// Declare the thread function for thread 1.
static void Thread1(void* arg) {
  while (1) {

    // Wait for signal from thread 2.
    xSemaphoreTake(sem, portMAX_DELAY);

    // Report wakeup time
    xWakeupTime = xTaskGetTickCount();
    Serial.println(F(xWakeupTime));
    
    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
    
    Serial.println(F("OFF-X"));
  }
}
//------------------------------------------------------------------------------
/*
 * Thread 2, turn the LED on and signal thread 1 to turn the LED off.
 */
// Declare the thread function for thread 2.
static void Thread2(void* arg) {

  pinMode(LED_PIN, OUTPUT);

  while (1) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);
    Serial.println(F("ON-G"));

    // Sleep for 2000 milliseconds.
    vTaskDelay((2000L * configTICK_RATE_HZ) / 1000L);
    // Report wakeup time
    xWakeupTime = xTaskGetTickCount();
    Serial.println(F("1 - "+ xWakeupTime));    

    // Signal thread 1 to turn LED off.
    xSemaphoreGive(sem);

    // Sleep for 2000 milliseconds.
    vTaskDelay((2000L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
void setup() {
  portBASE_TYPE s1, s2;

  Serial.begin(9600);
  
  // initialize fifoData semaphore to no data available
  sem = xSemaphoreCreateCounting(1, 0);

  // create sensor task at priority two
  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create SD write task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  
  Serial.println(F("About to Start"));
  
  // check for creation errors
  if (sem== NULL || s1 != pdPASS || s2 != pdPASS ) {
    Serial.println(F("Creation problem"));
    while(1);
  }
  // start scheduler
  vTaskStartScheduler();
  Serial.println(F("Insufficient RAM"));
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}
