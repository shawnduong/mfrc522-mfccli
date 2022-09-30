#include <SPI.h>
#include <MFRC522.h>   // MFRC522 library by GithubCommunity.

/* Serial protocol codes. */
#define STATUS_READY                    0x10
#define COMMAND_READ_UID                0x20
#define COMMAND_READ_ATQA               0x21
#define COMMAND_READ_SAK                0x22
#define COMMAND_READ_BLOCK              0x23
#define STATUS_READ_UID_SUCCESS         0x25
#define STATUS_READ_ATQA_SUCCESS        0x26
#define STATUS_READ_SAK_SUCCESS         0x27
#define STATUS_READ_BLOCK_SUCCESS       0x28
#define COMMAND_WRITE                   0x30
#define STATUS_WRITE_SUCCESS            0x35
#define COMMAND_AUTHENTICATE            0x40
#define COMMAND_AUTHENTICATE_A          0x41
#define COMMAND_AUTHENTICATE_B          0x42
#define STATUS_AUTHENTICATE_SUCCESS     0x45
#define STATUS_AUTHENTICATE_A_SUCCESS   0x46
#define STATUS_AUTHENTICATE_B_SUCCESS   0x47
#define COMMAND_DETECT_CARD             0x50
#define STATUS_DETECT_CARD_SUCCESS      0x55

/* Setup the library. */
MFRC522 mfrc522(10, 9);

void setup()
{
	Serial.begin(9600);
	SPI.begin();
	mfrc522.PCD_Init();
}

void loop()
{
	Serial.write(STATUS_READY);
	while (Serial.available() == 0);

	switch (Serial.read())
	{
		case COMMAND_DETECT_CARD:
			detect_card();
			break;
	}
}

void detect_card()
{
	while (!(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()))
		delay(100);

	Serial.write(STATUS_DETECT_CARD_SUCCESS);
}
