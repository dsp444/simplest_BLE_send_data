// Include the BLE package for arduino
#include <ArduinoBLE.h>

// Need to assign GUIDs for the service and also for any data or command that
// gets sent back and forth between Ardunio and the BLE central device.
// Get random GUIDs from a website that makes them.
BLEService testService( "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" );

// Only sending one piece of data from the Ardunio to the central device.
// It will be a random floating point number for this test
BLEFloatCharacteristic valueCharacteristic( "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX", BLERead | BLENotify );

// Set up some booleans to make sure we only give the "waiting" and "connected" messages once
// These are necessary except for debugging purposes
bool connected_message;
bool waiting_message;

// Set up a global variable which is the central device that this peripheral will connect to
BLEDevice central;

// Runs once
void setup() {

// Start serial communication (not necessary for production devices...just for troubleshooting)
// All the Serial.print statements are unnecessary except for debugging purposes
    Serial.begin( 115200 );

// Start the BLE on the Arduino
    if ( !BLE.begin() ) {
        Serial.println( "Failed to start BLE" );
        while(1);
    }
    Serial.println( "BLE startup successful" );
    connected_message = 0;
    waiting_message = 0;

  // Set up the BLE peripheral and advertise its existance
    BLE.setLocalName( "This Is A Test" );
    BLE.setDeviceName( "This Is A Test" );
    BLE.setAdvertisedService( testService );
    testService.addCharacteristic( valueCharacteristic );
    BLE.addService( testService );
    BLE.advertise();
    Serial.println( "BLE device waiting to connect" );
}

// The loop runs repeatedly on the Arduino
void loop() {

// Attemp to connect to a central device
    central = BLE.central();
    
// If connection does not work, just skip the entire loop and keep listening.
    if ( !central ) {
        connected_message = 0;
        if ( !waiting_message ) {
            waiting_message = 1;
            Serial.println( "Waiting for BLE connection..." );
        }
        delay( 100 );

// Connection to central device worked.
// Send a random floating point value every time through the loop.
    } else {
        waiting_message = 0;
        if ( !connected_message ) {
            connected_message = 1;
            Serial.print( "Connected: " );
            Serial.println(  central.address() );
        }
        valueCharacteristic.writeValue( random(100) );
    }
}
