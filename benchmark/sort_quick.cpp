#include "src/SLib.h"
#include "test/core.h"

///
/// @Note: set DEBUG_BENCHMARK to 1
///        in SLib.h to see measuring output
///

int main() {
	Array<u64> a_test;

	FOR(10000, it) {
		Array_Add(&a_test, (u64)rand());
	}

	MEASURE_START();

	Array_Sort_Descending(&a_test);
	MEASURE_END("Sort-Descending\t\t\t");

	Array_Sort_Descending(&a_test);
	MEASURE_END("Sort-Descending again\t\t");

	Array_Sort_Ascending(&a_test);
	MEASURE_END("Sort-Ascending\t\t\t");

	Array_Sort_Ascending(&a_test);
	MEASURE_END("Sort-Ascending  again\t\t");

	Array_Sort(&a_test, Sort_OnCompareDescending);
	MEASURE_END("Sort-Descending (custom)\t");

	Array_Sort(&a_test, Sort_OnCompareDescending);
	MEASURE_END("Sort-Descending (custom again)\t");

	Array_Sort(&a_test, Sort_OnCompareAscending);
	MEASURE_END("Sort-Ascending  (custom)\t");

	Array_Sort(&a_test, Sort_OnCompareAscending);
	MEASURE_END("Sort-Ascending  (custom again)\t");

	return 0;
}
