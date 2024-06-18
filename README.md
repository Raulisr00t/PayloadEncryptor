# Shellcode Encryption and UTF-16 Encoding
## Introduction
This project demonstrates a simple method of encrypting and encoding shellcode. The encryption is performed using XOR encryption, and the encoded shellcode is then converted to a UTF-16 encoded string for further use. The project is implemented in C and is designed to run on Windows.

## Project Structure
shellcode: An array containing the raw shellcode to be encrypted and encoded.
encrypt: A function to XOR-encrypt the shellcode with a given key.
utf16_encode: A function to convert the encrypted shellcode to a UTF-16 encoded string.
main: The main function that orchestrates the encryption and encoding process.

## Files
main.c: Contains the implementation of the shellcode encryption and UTF-16 encoding.
readme.md: This file, providing an overview and explanation of the project.

## Functions
### encrypt
```c
void encrypt(unsigned char* data, int data_len, unsigned char key) {
    for (int i = 0; i < data_len; i++) {
        data[i] ^= key;
    }
}
```
This function performs XOR encryption on the given data using the specified key.

### utf16_encode
```c
wchar_t* utf16_encode(const unsigned char* data, int data_len) {
    wchar_t* utf16_string = (wchar_t*)malloc((data_len + 1) * sizeof(wchar_t));
    for (int i = 0; i < data_len; i++) {
        utf16_string[i] = (wchar_t)data[i];
    }
    utf16_string[data_len] = L'\0'; 
    return utf16_string;
}
```
This function converts the encrypted shellcode into a UTF-16 encoded string.

### main
```c
int main() {
    unsigned char key = 0xAA;
    int shell_len = sizeof(shellcode) / sizeof(shellcode[0]);

    encrypt(shellcode, shell_len, key);
    encrypt(shellcode, shell_len, key);

    wchar_t* utf16_encoded_shell = utf16_encode(shellcode, shell_len);

    wprintf(L"UTF-16 Encoded Shellcode: ");
    for (int i = 0; i < shell_len; i++) {
        wprintf(L"\\x%04x", utf16_encoded_shell[i]);
    }
    wprintf(L"\n");
    free(utf16_encoded_shell);

    return 0;
}
```

The main function performs the following steps:

Defines the encryption key.
Calculates the length of the shellcode.
Encrypts the shellcode twice using the defined key.
Converts the encrypted shellcode into a UTF-16 encoded string.
Prints the UTF-16 encoded shellcode to the console.

## Compilation
gcc encryptor.c -o encryptor.exe

## Usage
Run the compiled program to see the UTF-16 encoded shellcode:
```cmd
encrytor
```
## License
This project is licensed under the MIT License. See the LICENSE file for more details.


