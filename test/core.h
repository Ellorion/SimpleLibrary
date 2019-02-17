#pragma once

/// Usage: include after SLib.h and run the desired tests.

#include "string.h"
#include "array.h"
#include "files.h"
#include "parser.h"

instant void
Test_Run(
) {
	Test_Strings();
	Test_Arrays();
	Test_Files();
	Test_Parser();
}
