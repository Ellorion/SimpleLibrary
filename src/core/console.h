#pragma once

instant void
ConWriteNL(
    const String &s_text
) {
    String_PrintLine(s_text);
}

template <class T>
instant void
ConWriteNL(
    T value
) {
    std::cout << value << std::endl;
}

instant void
ConWrite(
    String s_text
) {
    String_Print(s_text);
}

template <class T>
instant void
ConWrite(
    T value
) {
    std::cout << value;
}

template<class T>
instant T
ConRead() {
    T value;
    std::cin >> value;
    return value;
}

/// @todo: temporary global storage for strings?
instant String
ConRead() {
    char buffer[1024];
    std::cin.getline(buffer, 1024);

    return String_Copy(buffer);
}
