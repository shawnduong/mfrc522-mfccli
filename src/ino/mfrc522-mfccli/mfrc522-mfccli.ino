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
#define STATUS_AUTHENTICATE_A_FAILURE   0x48
#define STATUS_AUTHENTICATE_B_FAILURE   0x49
#define COMMAND_DETECT_CARD             0x50
#define STATUS_DETECT_CARD_SUCCESS      0x55

#define WAIT_FOR_CARD() \
	while (!(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())) \
		delay(100);

/* Setup the library. */
MFRC522 mfrc522(10, 9);
MFRC522::MIFARE_Key keyA;
MFRC522::MIFARE_Key keyB;

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

	/* Stop reading. In the future, this should become their own command. */
	mfrc522.PICC_HaltA();
	mfrc522.PCD_StopCrypto1();

	switch (Serial.read())
	{
		case COMMAND_READ_UID:
			read_uid();
			break;

		case COMMAND_READ_ATQA:
			read_atqa();
			break;

		case COMMAND_READ_SAK:
			read_sak();
			break;

		case COMMAND_AUTHENTICATE:
			authenticate();
			break;

		case COMMAND_DETECT_CARD:
			detect_card();
			break;
	}
}

void read_uid()
{
	byte buffer[12];

	WAIT_FOR_CARD();
	buffer[0] = STATUS_READ_UID_SUCCESS;
	memcpy(buffer+1, &(mfrc522.uid), 1+mfrc522.uid.size);

	Serial.write(buffer, 2+mfrc522.uid.size);
}

void read_atqa()
{
	byte buffer[3];
	byte atqaSize = 2;

	WAIT_FOR_CARD();
	buffer[0] = STATUS_READ_ATQA_SUCCESS;
	mfrc522.PICC_WakeupA(buffer+1, &atqaSize);

	/* (atqa & 0xFF00u) >> 8 */
	buffer[2] = buffer[1];
	buffer[1] = 0;

	Serial.write(buffer, 3);
}

void read_sak()
{
	WAIT_FOR_CARD();
	Serial.write(STATUS_READ_SAK_SUCCESS);
	Serial.write(mfrc522.uid.sak);
}

void authenticate()
{
	byte trailerBlock;

	WAIT_FOR_CARD();
	trailerBlock = Serial.read();
	Serial.readBytes(keyA.keyByte, 6);
	Serial.readBytes(keyB.keyByte, 6);

	/* Auth with key A. */
	if ((MFRC522::StatusCode) mfrc522.PCD_Authenticate(
		MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &keyA, &(mfrc522.uid)) != MFRC522::STATUS_OK)
	{
		Serial.write(STATUS_AUTHENTICATE_A_FAILURE);
		return;
	}

	/* Auth with key B. */
	if ((MFRC522::StatusCode) mfrc522.PCD_Authenticate(
		MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &keyB, &(mfrc522.uid)) != MFRC522::STATUS_OK)
	{
		Serial.write(STATUS_AUTHENTICATE_B_FAILURE);
		return;
	}

	Serial.write(STATUS_AUTHENTICATE_SUCCESS);
}

void detect_card()
{
	WAIT_FOR_CARD();
	Serial.write(STATUS_DETECT_CARD_SUCCESS);
}
