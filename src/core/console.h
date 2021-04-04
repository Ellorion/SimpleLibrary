#pragma once

void ConWriteNL(String s_text) {
    String_PrintLine(s_text);
}

template <class T>
void ConWriteNL(T value) {
    std::cout << value << std::endl;
}

void ConWrite(String s_text) {
    String_Print(s_text);
}

template <class T>
void ConWrite(T value) {
    std::cout << value;
}

template<class T>
T ConRead() {
    T value;
    std::cin >> value;
    return value;
}

/// @todo: temporary global storage for strings?
String ConRead() {
    char buffer[1024];
    std::cin.getline(buffer, 1024);

    return String_Copy(buffer);
}
