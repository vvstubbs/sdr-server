#include <stdlib.h>
#include <check.h>
#include "../src/xlating.h"
#include "../src/lpf.h"

#include <stdio.h>

xlating *filter = NULL;
int8_t *input = NULL;

void setup_input_data(size_t input_offset, size_t len) {
	input = malloc(sizeof(int8_t) * len);
	ck_assert_ptr_nonnull(input);
	for (size_t i = 0; i < len; i++) {
		// don't care about the loss of data
		input[i] = (int8_t) (input_offset + i);
	}
}

void assert_complex(const float expected[], size_t expected_size, float complex *actual, size_t actual_size) {
	ck_assert_int_eq(expected_size, actual_size);
	for (size_t i = 0, j = 0; i < expected_size * 2; i += 2, j++) {
		printf("index %zu\n",i);
		ck_assert_float_eq_tol(expected[i], crealf(actual[j]), 0.00001f);
		ck_assert_float_eq_tol(expected[i + 1], cimagf(actual[j]), 0.00001f);
	}
}

void setup_filter(size_t input_offset, size_t input_len, size_t max_input) {
	double sampling_freq = 48000;
	float *taps = NULL;
	size_t len;
	int code = create_low_pass_filter(1.0, sampling_freq, 9600, 2000, &taps, &len);
	ck_assert_int_eq(code, 0);
	code = create_frequency_xlating_filter(6, taps, len, -12000, sampling_freq, max_input, &filter);
	ck_assert_int_eq(code, 0);
	setup_input_data(input_offset, input_len);
}

START_TEST (test_max_input_buffer_size) {
	size_t input_len = 2000;
	setup_filter(0, input_len, input_len);
	float complex *output;
	size_t output_len = 0;
	process(input, input_len, &output, &output_len, filter);
	const float expected[] = { 5.32239E-4, 5.2806456E-4, -0.005198962, -0.0038215446, 0.004434482, -0.019613884, 0.04317172, 0.0114030065, 0.048882127, 0.16873941, 0.11678756, 0.09947322, -0.008784744, 0.07265453, -0.021184905, 0.0057728905, -5.2483374E-4, -0.00775056, 5.0187466E-4, -9.116491E-4, 5.035849E-4, 7.555388E-4, -3.8371302E-4, -6.3558214E-4, 2.6386292E-4, 5.155849E-4, -1.4400974E-4, -3.956068E-4, 2.416639E-5, 2.7563027E-4, 9.569182E-5, -1.5565171E-4, -2.155456E-4, 3.5673398E-5, 3.3540037E-4, 8.430634E-5, -4.5526054E-4, -2.0428277E-4, 5.7509384E-4, 3.2428277E-4,
			-6.949515E-4, -4.4422157E-4, 8.1484247E-4, 5.642409E-4, 0.004282254, 0.0068293647, -0.018195812, 0.015338528, -0.050322544, -0.06689663, 0.058113288, -0.19945177, 0.19628277, -0.99594504, 0.10873028, -0.14883728, 0.046853814, 0.030281652, -0.008243934, 0.02528844, -0.0036841996, -0.001135408, -5.4352795E-4, -7.95548E-4, 4.2367497E-4, 6.7555055E-4, -3.0382856E-4, -5.5558345E-4, 1.8396229E-4, 4.3560637E-4, -6.411341E-5, -3.1561792E-4, -5.574157E-5, 1.9564696E-4, 1.7559499E-4, -7.5667274E-5, -2.9544212E-4, -4.431846E-5, 4.1531282E-4, 1.6429575E-4, -5.351746E-4,
			-2.8426736E-4, 6.5500516E-4, 4.042555E-4, -7.7487E-4, -5.242393E-4, -0.001156186, -0.0037032967, 0.015904473, 0.0011798486, 0.013242431, 0.06385327, -0.10258264, 0.06251575, -0.39252713, -0.40717676, -0.19943306, 0.058134876, -0.028847475, -0.0884115, 0.023895692, -0.026713058, 0.004362696, 0.00670901, 5.8350596E-4, 8.3553954E-4, -4.6361735E-4, -7.1555376E-4, 3.4375233E-4, 5.9556344E-4, -2.2391645E-4, -4.7559696E-4, 1.0406845E-4, 3.5562148E-4, 1.5788304E-5, -2.3564363E-4, -1.3564635E-4, 1.15660296E-4, 2.5549787E-4, 4.316647E-6, -3.7536025E-4, -1.242851E-4,
			4.951959E-4, 2.4428123E-4, -6.1505765E-4, -3.6424698E-4, 7.3491834E-4, 4.8423238E-4, 2.138969E-4, 4.644367E-4, -0.009756218, -0.0074080825, 0.010140638, -0.04047023, 0.0884305, 0.028868632, 0.08625167, 0.34381726, 0.22234057, 0.2076883, -0.023880864, 0.14121968, -0.040489215, 0.01011961, -0.0012008179, -0.01592356, 0.0015935013, -9.5518114E-4, 5.0358375E-4, 7.555396E-4, -3.8371206E-4, -6.355827E-4, 2.6386214E-4, 5.155853E-4, -1.4400913E-4, -3.95607E-4, 2.4165969E-5, 2.7563033E-4, 9.5692056E-5, -1.5565156E-4, -2.1554566E-4, 3.567307E-5, 3.3540023E-4, 8.430685E-5,
			-4.5526022E-4, -2.0428347E-4, 5.750934E-4, 3.2428364E-4, -6.9495087E-4, -4.4422265E-4, 8.148416E-4, 5.6424213E-4, 0.0042822435, 0.006829371, -0.018195834, 0.0153385, -0.05032244, -0.06689671, 0.058113594, -0.19945168, 0.1962843, -0.99594474, 0.1087305, -0.14883712, 0.046853766, 0.030281723, -0.008243972, 0.025288427, -0.0036841978, -0.0011354135, -5.435267E-4, -7.9554884E-4, 4.2367395E-4, 6.755512E-4, -3.0382772E-4, -5.555839E-4, 1.8396162E-4, 4.3560666E-4, -6.411293E-5, -3.15618E-4, -5.5741868E-5, 1.9564688E-4, 1.7559511E-4, -7.5667005E-5, -2.9544203E-4,
			-4.4318913E-5, 4.1531256E-4, 1.6429638E-4, -5.351742E-4, -2.8426817E-4, 6.550046E-4, 4.0425648E-4, -7.748692E-4, -5.2424055E-4, -0.0011561804, -0.0037032985, 0.015904471, 0.0011798729, 0.013242334, 0.06385329, -0.10258274, 0.062515594, -0.3925265, -0.40717736, -0.19943315, 0.05813457, -0.028847339, -0.08841154, 0.023895733, -0.026713021, 0.0043626856, 0.006709017, 5.835047E-4, 8.355405E-4, -4.6361625E-4, -7.1555446E-4, 3.437514E-4, 5.9556396E-4, -2.2391572E-4, -4.755973E-4, 1.04067905E-4, 3.5562163E-4, 1.5788664E-5, -2.356436E-4, -1.3564652E-4, 1.1566009E-4,
			2.5549787E-4, 4.3170367E-6, -3.7536005E-4, -1.2428567E-4, 4.951955E-4, 2.4428198E-4, -6.1505707E-4, -3.6424794E-4, 7.349176E-4, 4.842335E-4, 2.138962E-4, 4.6443703E-4, -0.009756207, -0.0074080974, 0.0101407, -0.040470216, 0.08843046, 0.028868767, 0.08625115, 0.34381738, 0.22234026, 0.20768863, -0.023881078, 0.14121965, -0.04048923, 0.010119548, -0.0012007935, -0.015923562, 0.0015935028, -9.5517875E-4, 5.035826E-4, 7.555403E-4, -3.837111E-4, -6.355833E-4, 2.6386135E-4, 5.155857E-4, -1.4400853E-4, -3.9560723E-4, 2.4165549E-5, 2.7563036E-4, 9.5692296E-5,
			-1.5565142E-4, -2.155457E-4, 3.5672743E-5, 3.354001E-4, 8.430736E-5, -4.5525993E-4, -2.0428417E-4, 5.7509285E-4, 3.242845E-4, -6.9495017E-4, -4.442237E-4, 8.148408E-4, 5.642434E-4, 0.0042822333, 0.0068293777, -0.018195858, 0.015338473, -0.05032234, -0.06689678, 0.058113895, -0.1994516, 0.19628581, -0.99594444, 0.10873073, -0.14883696, 0.04685372, 0.030281795, -0.008244012, 0.025288414, -0.0036841962, -0.0011354191, -5.435255E-4, -7.9554965E-4, 4.236729E-4, 6.755518E-4, -3.0382688E-4, -5.555844E-4, 1.8396095E-4, 4.3560693E-4, -6.411244E-5, -3.1561812E-4,
			-5.5742166E-5, 1.9564679E-4, 1.7559521E-4, -7.5666736E-5, -2.9544198E-4, -4.4319364E-5 };
	assert_complex(expected, 167, output, output_len);
}
END_TEST

START_TEST (test_parital_input_buffer_size) {
	printf("test_parital_input_buffer_size\n");
	size_t input_len = 200; // taps is 57
	setup_filter(0, input_len, 200);
	float complex *output;
	size_t output_len = 0;
	process(input, input_len, &output, &output_len, filter);
	const float expected[] = { 5.32239E-4, 5.2806456E-4, -0.005198962, -0.0038215446, 0.004434482, -0.019613884, 0.04317172, 0.0114030065, 0.048882127, 0.16873941, 0.11678756, 0.09947322, -0.008784744, 0.07265453, -0.021184905, 0.0057728905, -5.2483374E-4, -0.00775056, 5.0187466E-4, -9.116491E-4, 5.035849E-4, 7.555388E-4, -3.8371302E-4, -6.3558214E-4, 2.6386292E-4, 5.155849E-4, -1.4400974E-4, -3.956068E-4, 2.416639E-5, 2.7563027E-4, 9.569182E-5, -1.5565171E-4, -2.155456E-4, 3.5673398E-5 };
	assert_complex(expected, 17, output, output_len);
}
END_TEST

START_TEST (test_parital_input_less_taps) {
}
END_TEST

void teardown() {
	destroy_xlating(filter);
	filter = NULL;
	if (input != NULL) {
		free(input);
		input = NULL;
	}
}

void setup() {
//do nothing
}

Suite* common_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("xlating");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_max_input_buffer_size);
	tcase_add_test(tc_core, test_parital_input_buffer_size);
	tcase_add_test(tc_core, test_parital_input_less_taps);

	tcase_add_checked_fixture(tc_core, setup, teardown);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = common_suite();
	sr = srunner_create(s);

	srunner_set_fork_status(sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
