#include <check.h>
#include <stdio.h>

START_TEST(s21_cat_test_) {
    FILE *cat_test_file = fopen("cat/cat_test_file.txt", "r");
    FILE *cat_origin_file = fopen("cat/cat_origin_file.txt", "r");
    if (cat_test_file) {
        if (cat_origin_file) {
            char ct = '\0', co = '\0';
            while (co != EOF) {
                ct = getc(cat_test_file);
                co = getc(cat_origin_file);
                ck_assert_int_eq(ct, co);
            }
            fclose(cat_origin_file);
        }
        fclose(cat_test_file);
    }
}
END_TEST

START_TEST(s21_grep_test_) {
    FILE *grep_test_file = fopen("grep/grep_test_file.txt", "r");
    FILE *grep_origin_file = fopen("grep/grep_origin_file.txt", "r");
    if (grep_test_file) {
        if (grep_origin_file) {
            char ct = '\0', co = '\0';
            while (co != EOF) {
                ct =getc(grep_test_file);
                co = getc(grep_origin_file);
                ck_assert_int_eq(ct, co);
            }
            fclose(grep_origin_file);
        }
        fclose(grep_test_file);
    }
}
END_TEST


int main() {
    Suite *suite = suite_create("bash");
    TCase *s21_cat_case = tcase_create("s21_cat_test_case");
    TCase *s21_grep_case = tcase_create("s21_grep_test_case");
    SRunner *sr = srunner_create(suite);
    int nf;

    suite_add_tcase(suite, s21_cat_case);
    tcase_add_test(s21_cat_case, s21_cat_test_);

    suite_add_tcase(suite, s21_grep_case);
    tcase_add_test(s21_grep_case, s21_grep_test_);

    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}