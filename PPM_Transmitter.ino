#include <Arduino.h>
#include "driver/rmt.h"  // ESP32 Remote Control (RMT) module

//                                 TRANSMITTER SETUP 
//         Standard Mode of transmitter is PWM not PPM
// you have to configure the transmitter to use PPM for this project with the following steps:
// 1. Switch on the transmitter
// 2. Hold the OK button for two seconds 
// 3. Choose System
// 4. Go down and choose "RX setup"
// 5. Go down and choose "OUTPUT MODE"
// 6. Choose "PPM" and HOLD the CANCEL button for two seconds to save your choice  
//    BOOOOOM !!! Done ✅



// SO let's first try to decode why we use PPM instead of PWM
// Consider a two-channel example: you want to receive information about the throttle and the pitch input
// These inputs are independent of each other and require two separate signal cables if you use PWM
// When receiving two PWM signals in the ESP32, you will need two different analogRead() commands
// Since they cannot be processed simultaneously, either the throttle or the pitch values will be sampled first, and then the other follows directly
// Besides requiring two signal cables, this also increases calculation time for the ESP32
//
// The advantage of PPM: 
// - Only the position of the signals changes (not their width)
// - With one signal cable, information from multiple signals can be 'transported'  
// - We measure the time from each rising signal value to keep track of the original values  

 
// Channel 1 of the receiver has "PPM" written on it, meaning you can use this connector to send PPM signals to your microcontroller
// - Use a Jumper to connect the second connector of the receiver to any GPIO of the ESP32
// - Connect the third and fourth connectors of the receiver to 5V and ground
// - This ensures the receiver is powered by the processor
// - We're now ready to start coding!  
#define RECEIVER_PIN GPIO_NUM_14  // GPIO14 for PPM input 
#define MAX_CHANNELS 8  // Number of PPM channels expected
volatile uint16_t ReceiverValue[MAX_CHANNELS] = {0};  // Stores channel values (initialized to zero)
//    channel 1 --------->>>  "ROLL"
//    channel 2 --------->>>  "PITCH"
//    channel 3 --------->>>  "YAW"
//    channel 4 --------->>>  "THROTTLE"
//  As you know in C++, array[0] corresponds to the first element
// ReceiverValue[0] = Roll

rmt_channel_t rmtChannel = RMT_CHANNEL_0;  // Define the RMT channel for receiving PPM signals

rmt_item32_t *items = NULL;  // PPM data buffer
RingbufHandle_t rb = NULL;   // Ring buffer handle for storing received PPM data


// This function will continuously read and process the PPM signal values
void parsePPM(void *pvParameters) {  // Fixed function signature for FreeRTOS compatibility
    size_t rx_size = 0;

    while (1) {  // Infinite loop for continuous reading
        items = (rmt_item32_t *)xRingbufferReceive(rb, &rx_size, 1000 / portTICK_PERIOD_MS);
        
        if (items) {  // If data is received
            if (rx_size / sizeof(rmt_item32_t) >= MAX_CHANNELS + 1) {  // Ensure enough data is received
                for (int i = 0; i < MAX_CHANNELS; i++) {
                    ReceiverValue[i] = items[i + 1].duration0;  // Read channel pulse widths
                }
            }
            vRingbufferReturnItem(rb, (void *)items);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);  // Delay to prevent CPU overload
    }
}

void setupPPMReceiver() {
    rmt_config_t rmt_rx = {};  // Initialize structure

    rmt_rx.channel = rmtChannel;
    rmt_rx.gpio_num = RECEIVER_PIN;  // Set receiver pin
    rmt_rx.clk_div = 80;  // Clock divider (80MHz / 80 = 1MHz -> 1µs resolution)
    rmt_rx.mem_block_num = 1;  // Number of memory blocks
    rmt_rx.rmt_mode = RMT_MODE_RX;  // Set to receive mode
    rmt_rx.rx_config.filter_en = true;  // Enable noise filter
    rmt_rx.rx_config.filter_ticks_thresh = 100;  // Filter noise under 100µs
    rmt_rx.rx_config.idle_threshold = 5000;  // Consider signal idle if pulse > 5000µs

    rmt_config(&rmt_rx);  // Apply configuration
    rmt_driver_install(rmtChannel, 1000, 0);  // Install RMT driver

    rmt_get_ringbuf_handle(rmtChannel, &rb);  // Get ring buffer handle
    rmt_rx_start(rmtChannel, 1);  // Start receiving data

    xTaskCreatePinnedToCore(parsePPM, "parsePPM", 2048, NULL, 1, NULL, 1);  // Create FreeRTOS task
}


void setup() {
    Serial.begin(115200);  // Initialize serial communication
    pinMode(2, OUTPUT);    // ESP32 LED
    digitalWrite(2, HIGH);

    setupPPMReceiver();  // Call function to setup PPM receiver
}


void loop() {
    Serial.print("Number of channels: ");
    Serial.print(MAX_CHANNELS);  // Print number of channels
    
    Serial.print(" Roll: "); Serial.print(ReceiverValue[0]); Serial.print(" μs | ");
    Serial.print(" Pitch: "); Serial.print(ReceiverValue[1]); Serial.print(" μs | ");
    Serial.print(" Throttle: "); Serial.print(ReceiverValue[2]); Serial.print(" μs | ");
    Serial.print(" Yaw: "); Serial.println(ReceiverValue[3]);  

    delay(50);  // Small delay to allow data processing
}
