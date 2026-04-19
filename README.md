📌 Description
A command-line tool written in C for hiding secret text messages within BMP images using the Least Significant Bit (LSB) technique. This project demonstrates low-level bit manipulation and binary file handling by embedding data into the image's pixel bytes without noticeably altering the visual quality.

🚀 Features
Secure Encoding: Hides secret .txt file contents within a .bmp image.
Reliable Decoding: Extracts hidden data back into a readable text file.
Header Protection: Skips the BMP header to ensure the image remains valid and viewable.
Validation: Checks if the source image is large enough to hold the secret message before starting.

🛠️ Technologies Used
C Programming
Bitwise Operations (AND, OR, Shifting)
Binary File I/O
BMP File Structure

📂 Project Structure
Plaintext
LSB_image_Steganography/
├── test_encode.c     Main driver (handles command-line arguments)
├── encode.c          Logic for hiding data in BMP
├── decode.c          Logic for extracting data from BMP
├── encode.h          Function prototypes for encoding
├── decode.h          Function prototypes for decoding
├── common.h          Shared macros and constants
├── types.h           Custom data types (uint, etc.)
├── secret.txt        Example secret message
└── beautiful.bmp     Example source image

💡 Key Concepts
LSB Technique: The last bit of an RGB color byte is replaced with one bit of the secret data.
Image Header: The first 54 bytes (typically) of a BMP are preserved to keep the file format intact.
Bit Manipulation: Uses operations like & 0xFE and | bit to swap data at the bit level.
