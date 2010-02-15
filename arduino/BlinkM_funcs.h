/*
 */

#define DEBUG  1

#include <WiServer.h>

#define WIRELESS_MODE_INFRA 1
#define WIRELESS_MODE_ADHOC 2

#include "Wire.h"
#include "BlinkM_funcs.h"

// ---------------------------------------------------------------------------------
// BlinkM configuration parameters -------------------------------------------------

// set this if you're plugging a BlinkM directly into an Arduino,
// into the standard position on analog in pins 2,3,4,5
// otherwise you can set it to false or just leave it alone
const boolean BLINKM_ARDUINO_POWERED = true;

int blinkm_addr = 0x09;

byte bri_val = 0;
byte hue_val = 0;

// End of BlinkM configuration parameters ------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// Wireless configuration parameters -----------------------------------------------
unsigned char local_ip[] = {192,168,1,222}; // IP address of WiShield
unsigned char gateway_ip[] = {192,168,1,1}; // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0};	// subnet mask for the local network
const prog_char ssid[] PROGMEM = {"NETGEARS"}; // {"ovnguest"}; // max 32 bytes

unsigned char security_type = 1;	// 0 - open; 1 - WEP; 2 - WPA; 3 - WPA2

// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {""};	// max 64 characters

// WEP 128-bit keys
// user this site to generate from password, unless password is already in hex!
//	  http://www.wepkey.com/
// edit g2100.c keyLen to 5 or 13 (bytes) for 64- or 128-bit, respectively
// enter 128 bit as many characters as there are.
prog_uchar wep_keys[] PROGMEM = { 
0x12, 0x34, 0x5a, 0xbc, 0xde, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 1
0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 2
0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Key 3
};

// setup the wireless mode
// infrastructure - connect to AP
// adhoc - connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;
// End of wireless configuration parameters ----------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ProcrasDoCoderRing configuration parameters --------------------------------------

int ping_pin =	7;
int switch_pin = 6;
int switch_state = LOW;

// IP Address for procrasdonate.com 
uint8 ip[] = {72,14,185,232};
// A request that gets procrasdonate data
GETrequest pinger(ip, 80, "procrasdonate.com", "/procrasdocoder/");

// ping update time in milliseconds
// int updateTime = 8000

char* partial_packet;

// End of ProcrasDoCoderRing configuration parameters ------------------------------
// ---------------------------------------------------------------------------------

void process_procrasdocoderring(char* data, int len) {
	// Note that the data is not null-terminated, may be broken up into smaller packets, and 
	// includes the HTTP header. 
	Serial.print("\nprocess update of length: ");
	Serial.println(len);
	while (len-- > 0) {
		Serial.print(*(data++));
	}
	Serial.println("");
}

boolean url_dispatch(char* URL) {
	// Check if the requested URL matches "/"
	Serial.println("--->process...");
	if (strcmp(URL, "/") == 0) {
		// Use WiServer's print and println functions to write out the page content
		WiServer.print("<html>");
		WiServer.print("I am wifly");
		WiServer.print("</html>");
		
		digitalWrite(ping_pin, HIGH);
		delay(1000);
		digitalWrite(ping_pin, LOW);
		
		// URL was recognized
		return true;
	}
	// URL not found
	return false;
}

void setup() {
	WiServer.init(url_dispatch);
	
	// Enable Serial output and ask WiServer to generate log messages (optional)
	Serial.begin(57600);
	WiServer.enableVerboseMode(true);
	
	// Have the processData function called when data is returned by the server
	pinger.setReturnFunc(process_procrasdocoderring);
	
	pinMode(ping_pin, OUTPUT);
	pinMode(switch_pin, INPUT);
	
	Serial.println("WiFly setup complete");
	
	if( BLINKM_ARDUINO_POWERED ) {
		BlinkM_beginWithPower();
	} else {
		BlinkM_begin();
	}
	delay(1000);  // some startup light is good
	
	BlinkM_stopScript(blinkm_addr);  // turn off startup script
	
	Serial.println("BlinkM setup complete");
}

void loop(){
	// Check if it's time to get an update
	/*
	 if (millis() >= updateTime) {
	 pinger.submit();
	 // Get another update....x seconds from now
	 updateTime += sec*10;
	 
	 digitalWrite(ping_pin, HIGH);
	 delay(sec);
	 digitalWrite(ping_pin, LOW);
	 }
	 */
	
	// Run WiServer
	WiServer.server_task();
	
	// Check if switch is pushed
	int switch_val = digitalRead(switch_pin);
	if (switch_val == HIGH && switch_state == LOW ) {
		pinger.submit();
		BlinkM_fadeToHSB( blinkm_addr, 111, 255, 255 );
		delay(500);
		BlinkM_fadeToHSB( blinkm_addr, 200, 255, 255 );
		delay(500);
		BlinkM_fadeToHSB( blinkm_addr, 111, 255, 255 );
		delay(500);
	}
	switch_state = switch_val;
	
	
	//bri_val++;
	//hue_val++;
	
	if (bri_val > 255) bri_val = 0;
	if (hue_val > 255) hue_val = 0;
	
	// set blinkms with hue & bri, saturation is max
	BlinkM_fadeToHSB( blinkm_addr, hue_val, 255, bri_val );
	
	delay(10);
}

