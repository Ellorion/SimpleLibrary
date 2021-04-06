#include "src/SLib.h"
#include "test/core.h"

String ConRead(String s_text) {
    ConWrite(s_text);
    return ConRead();
}

void Storage_Save(File file, String s_data) {
    File_Write(file, s_data);
}

void Storage_Save(File file, Array<String> as_data) {
    FOR_ARRAY_AUTO(as_data, it) {
        Storage_Save(file, *it);
        Storage_Save(file, S("\n"));
    }

    Storage_Save(file, S("\n"));
}

void Storage_Load(File &file, Array<String> *as_data) {
    String s_data;

    while (File_ReadUntil(file, &s_data, S("\n"))) {
        if (String_IsEmpty(&s_data, true))
            break;

        Array_Add(as_data, s_data);
    }
}

int main() {
    Array<String> as_data;

    bool running = true;
    String s_cmd;

    while(running) {
        ConWriteNL(S("=== Data Storage ==="));

        String_Destroy(&s_cmd);
        s_cmd = ConRead(S("$: "));

        if (   s_cmd == S("quit", 0, false)
            OR s_cmd == S("exit", 0, false)
        ) {
            running = false;
            continue;
        }

        if (s_cmd == S("list", 0, false)) {
            ConWriteNL(S("Entries:"));
            Array_PrintList(&as_data);

            continue;
        }

        if (s_cmd == S("add", 0, false)) {
            String s_name = ConRead(S("Entry: "));

            if (Array_Add(&as_data, s_name)) {
                ConWrite(S("Entry added: "));
                ConWriteNL(s_name);
            }

            String_Destroy(&s_name);

            continue;
        }

        if (s_cmd == S("save", 0, false)) {
            File file = File_Open(S("storage.dat"), "w");
            Storage_Save(file, as_data);
            File_Close(file);

            continue;
        }

        if (s_cmd == S("load", 0, false)) {
            File file = File_Open(S("storage.dat"), "rb");
            Storage_Load(file, &as_data);
            File_Close(file);

            continue;
        }
    }

    return 0;
}
