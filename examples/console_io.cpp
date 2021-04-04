#include "src/SLib.h"
#include "test/core.h"

int main() {
	bool running = true;
    String s_cmd;

    while(running) {
        ConWriteNL(S("=== Console Application ==="));
        ConWrite(S("$: "));
        s_cmd = ConRead();

        if (   s_cmd == S("quit", 0, false)
            OR s_cmd == S("exit", 0, false)
        ) {
            running = false;
            continue;
        }

        if (s_cmd == S("add", 0, false)) {
            int nr1 = ConRead<int>();
            int nr2 = ConRead<int>();
            ConWrite(S("Result: "));
            ConWriteNL(nr1+nr2);
            continue;
        }
    }

    return 0;
}