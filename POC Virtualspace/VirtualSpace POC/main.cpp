#include <windows.h>
#include <stdio.h>
#include <vector>
#include "../VirtualSpace POC/minhook-master/include/MinHook.h"

#pragma comment(lib, "MinHook.x64.lib")

typedef BOOL(WINAPI* pWriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
pWriteFile TrueWriteFile = nullptr;
bool g_allowWrites = false;

struct WriteOperation {
    HANDLE hFile;
    std::vector<BYTE> data;
};

// Simple XOR for in-memory data
void XorEncryptDecrypt(std::vector<BYTE>& buf) {
    const BYTE key = 0xAA;
    for (auto& b : buf) {
        b ^= key;
    }
}

std::vector<WriteOperation> g_ops;

BOOL WINAPI HookedWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nBytes, LPDWORD lpWritten, LPOVERLAPPED lpOverlapped)
{
    if (!g_allowWrites) {
        WriteOperation wo;
        wo.hFile = hFile;
        wo.data.resize(nBytes);
        memcpy(wo.data.data(), lpBuffer, nBytes);
        XorEncryptDecrypt(wo.data); // encrypt in memory
        g_ops.push_back(wo);
        if (lpWritten) *lpWritten = nBytes;
        return TRUE;
    }
    return TrueWriteFile(hFile, lpBuffer, nBytes, lpWritten, lpOverlapped);
}

// Commit everything
void CommitAll() {
    for (auto& op : g_ops) {
        XorEncryptDecrypt(op.data); // decrypt before writing
        DWORD written = 0;
        TrueWriteFile(op.hFile, op.data.data(), (DWORD)op.data.size(), &written, nullptr);
        XorEncryptDecrypt(op.data); // re-encrypt in case we want further tricks
    }
    g_ops.clear();
}

// Commit only the first N queued writes
void CommitPartial(size_t n) {
    if (n > g_ops.size()) n = g_ops.size();
    for (size_t i = 0; i < n; i++) {
        XorEncryptDecrypt(g_ops[i].data);
        DWORD written = 0;
        TrueWriteFile(g_ops[i].hFile, g_ops[i].data.data(), (DWORD)g_ops[i].data.size(), &written, nullptr);
    }
    // remove committed ops from vector
    g_ops.erase(g_ops.begin(), g_ops.begin() + n);
}

// Discard everything
void DiscardAll() {
    g_ops.clear();
}

int main() {
    MH_Initialize();
    MH_CreateHook(&WriteFile, &HookedWriteFile, reinterpret_cast<LPVOID*>(&TrueWriteFile));
    MH_EnableHook(&WriteFile);

    HANDLE hFile = CreateFileA("testfile.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open testfile.txt\n");
        return 1;
    }

    // Queue some sample writes
    const char* msg1 = "Encrypted in memory, not yet on disk.\n";
    DWORD bw = 0;
    WriteFile(hFile, msg1, (DWORD)strlen(msg1), &bw, nullptr);

    const char* msg2 = "Try partial commits or discarding me.\n";
    WriteFile(hFile, msg2, (DWORD)strlen(msg2), &bw, nullptr);

    printf("Queued writes: %zu\n", g_ops.size());
    printf("[C] Commit all, [Pn] Commit partial n, [D] Discard all, [Q] Quit\n");

    char input[64];
    fgets(input, 64, stdin);

    if (input[0] == 'C' || input[0] == 'c') {
        g_allowWrites = true;
        CommitAll();
        printf("All queued writes committed.\n");
    }
    else if (input[0] == 'P' || input[0] == 'p') {
        // extract number after 'P'
        size_t partialCount = strtoul(input + 1, nullptr, 10);
        g_allowWrites = true;
        CommitPartial(partialCount);
        printf("Committed partial writes: %zu.\n", partialCount);
    }
    else if (input[0] == 'D' || input[0] == 'd') {
        DiscardAll();
        printf("All queued writes discarded.\n");
    }
    else {
        printf("No action taken. Closing.\n");
    }

    CloseHandle(hFile);
    MH_DisableHook(&WriteFile);
    MH_Uninitialize();
    printf("Done. Check testfile.txt for results.\n");
    Sleep(5000);
    return 0;
}
