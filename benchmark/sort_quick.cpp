#include "src/SLib.h"
#include "test/core.h"

template<typename T>
bool OnCompareAscending(T one, T two) {
	return (one > two);
}

template<typename T>
bool OnCompareDescending(T one, T two) {
	return (one < two);
}

int main() {
	Array<u64> a_test;

	Array_AddRange(&a_test, 1, 10000);

	MEASURE_START();

	Array_Sort_Descending(&a_test);
	MEASURE_END("Sort-Descending\t\t\t");

	Array_Sort_Descending(&a_test);
	MEASURE_END("Sort-Descending again\t\t");

	Array_Sort_Ascending(&a_test);
	MEASURE_END("Sort-Ascending\t\t\t");

	Array_Sort_Ascending(&a_test);
	MEASURE_END("Sort-Ascending  again\t\t");

	Array_Sort(&a_test, OnCompareDescending);
	MEASURE_END("Sort-Descending (custom)\t");

	Array_Sort(&a_test, OnCompareDescending);
	MEASURE_END("Sort-Descending (custom again)\t");

	Array_Sort(&a_test, OnCompareAscending);
	MEASURE_END("Sort-Ascending  (custom)\t");

	Array_Sort(&a_test, OnCompareAscending);
	MEASURE_END("Sort-Ascending  (custom again)\t");

	return 0;
}
