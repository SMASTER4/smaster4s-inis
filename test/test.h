#pragma once

#ifndef TEST_TEXT
#define TEST_TEXT "[notImportantSection]key=valueFromTheWrongSection\n[section]\nwrongKey=valueFromTheWrongKey# Comment\n# Comment\nkey=value\nevenMoreWrongKey=valueFromTheWrongKey"
#endif

static void _assert_string(const char* name, const char* a, const char* b);

static void _test_ini_get_str_from_str(int count);

static void _test_ini_get_str(int count);

static bool _test_ini_get_str_prepare();

static void _test_ini_get_str_clean_up();

static void _run_tests(int count);
