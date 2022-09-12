# Protocol

This documentation file is only useful for developers. End users need not be concerned with the inner workings of the communication protocol between the main and worker devices.

### General Info

The main device (typically a computer) communicates with the worker device (Arduino) through serial at 9600 baud.

### Protocol Codes

| Issuer | Code                            | Value  |
|--------|---------------------------------|--------|
| Worker | `STATUS_READY`                  | `0x10` |
| Main   | `COMMAND_READ_UID`              | `0x20` |
| Main   | `COMMAND_READ_ATQA`             | `0x21` |
| Main   | `COMMAND_READ_SAK`              | `0x22` |
| Main   | `COMMAND_READ_BLOCK`            | `0x23` |
| Worker | `STATUS_READ_UID_SUCCESS`       | `0x25` |
| Worker | `STATUS_READ_ATQA_SUCCESS`      | `0x26` |
| Worker | `STATUS_READ_SAK_SUCCESS`       | `0x27` |
| Worker | `STATUS_READ_BLOCK_SUCCESS`     | `0x28` |
| Main   | `COMMAND_WRITE`                 | `0x30` |
| Worker | `STATUS_WRITE_SUCCESS`          | `0x35` |
| Main   | `COMMAND_AUTHENTICATE`          | `0x40` |
| Main   | `COMMAND_AUTHENTICATE_A`        | `0x41` |
| Main   | `COMMAND_AUTHENTICATE_B`        | `0x42` |
| Worker | `STATUS_AUTHENTICATE_SUCCESS`   | `0x45` |
| Worker | `STATUS_AUTHENTICATE_A_SUCCESS` | `0x46` |
| Worker | `STATUS_AUTHENTICATE_B_SUCCESS` | `0x47` |
| Main   | `COMMAND_DETECT_CARD`           | `0x50` |
| Worker | `STATUS_DETECT_CARD_SUCCESS`    | `0x55` |

### Messages

#### Status: Ready

The worker device sends a `STATUS_READY` message to indicate that it is ready to receive a command.

Size: 1 B

Format: `[1 B STATUS_READY]`

#### Command: Read UID

The main device sends a `COMMAND_READ_UID` message to the worker device to get the worker to read and return the UID from the card.

Size: 1 B

Format: `[1 B COMMAND_READ_UID]`

#### Command: Read ATQA

The main device sends a `COMMAND_READ_ATQA` message to the worker device to get the worker to read and return the ATQA from the card.

Size: 1 B

Format: `[1 B COMMAND_READ_ATQA]`

#### Command: Read SAK

The main device sends a `COMMAND_READ_SAK` message to the worker device to get the worker to read and return the SAK from the card.

Size: 1 B

Format: `[1 B COMMAND_READ_SAK]`

#### Command: Read Block

The main device sends a `COMMAND_READ_BLOCK` message and block number to the worker device to get the worker to read the specified block from the card.

Size: 2 B

Format: `[1 B COMMAND_READ_BLOCK][1 B BLOCK NUMBER]`

#### Status: Read UID Success

The worker device sends a `STATUS_READ_UID_SUCCESS` to indicate that the UID was successfully read.

Size: 6 B (4-byte UID) or 9 B (7-byte UID)

Format: `[1 B STATUS_READ_UID_SUCCESS][1 B UID LENGTH n][n B UID]`

#### Status: Read ATQA Success

The worker device sends a `STATUS_READ_ATQA_SUCCESS` to indicate that the ATQA was successfully read.

Size: 3 B

Format: `[1 B STATUS_READ_ATQA_SUCCESS][2 B ATQA]`

#### Status: Read SAK Success

The worker device sends a `STATUS_READ_SAK_SUCCESS` to indicate that the SAK was successfully read.

Size: 2 B

Format: `[1 B STATUS_READ_SAK_SUCCESS][1 B SAK]`

#### Status: Read Block Success

The worker device sends a `STATUS_READ_BLOCK_SUCCESS` to indicate that the block was successfully read.

Size: 18 B

Format: `[1 B READ_BLOCK_SUCCESS][1 B BLOCK NUMBER][16 B BLOCK DATA]`

#### Command: Write

The main device sends a `COMMAND_WRITE` message to the worker to write data to a block on the card.

Size: 18 B

Format: `[1 B COMMAND_WRITE][1 B BLOCK NUMBER][16 B BLOCK DATA]`

#### Status: Write Success

The worker device sends a `STATUS_WRITE_SUCCESS` to indicate that the write was successful.

Size: 2 B

Format: `[1 B STATUS_WRITE_SUCCESS][1 B BLOCK NUMBER]`

#### Command: Authenticate

The main device sends a `COMMAND_AUTHENTICATE` message to the worker with keys A and B to authenticate to a sector given the trailer block number.

Size: 14 B

Format: `[1 B COMMAND_AUTHENTICATE][1 B TRAILER BLOCK NUMBER][6 B KEY A][6 B KEY B]`

#### Command: Authenticate A

The main device sends a `COMMAND_AUTHENTICATE_A` message to the worker with key A to authenticate to a sector given the trailer block number.

Size: 8 B

Format: `[1 B COMMAND_AUTHENTICATE_A][1 B TRAILER BLOCK NUMBER][6 B KEY A]`

#### Command: Authenticate B

The main device sends a `COMMAND_AUTHENTICATE_B` message to the worker with key B to authenticate to a sector given the trailer block number.

Size: 8 B

Format: `[1 B COMMAND_AUTHENTICATE_B][1 B TRAILER BLOCK NUMBER][6 B KEY B]`

#### Status: Authenticate Success

The worker device sends a `STATUS_AUTHENTICATE_SUCCESS` to indicate that authentication was successful.

Size: 1 B

Format: `[1 B STATUS_AUTHENTICATE_SUCCESS]`

#### Status: Authenticate A Success

The worker device sends a `STATUS_AUTHENTICATE_A_SUCCESS` to indicate that authentication was successful.

Size: 1 B

Format: `[1 B STATUS_AUTHENTICATE_A_SUCCESS]`

#### Status: Authenticate B Success

The worker device sends a `STATUS_AUTHENTICATE_B_SUCCESS` to indicate that authentication was successful.

Size: 1 B

Format: `[1 B STATUS_AUTHENTICATE_B_SUCCESS]`

#### Command: Detect Card

The main device sends a `COMMAND_DETECT_CARD` message to the worker to see if a card has been detected.

Size: 1 B

Format: `[1 B COMMAND_DETECT_CARD]`

#### Status: Detect Card Success

The worker device sends a `STATUS_DETECT_CARD_SUCCESS` to indicate that a card has been detected.

Size: 1 B

Format: `[1 B STATUS_DETECT_CARD_SUCCESS]`
