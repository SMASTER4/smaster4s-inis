#pragma once

static void _assert_string(const char* name, const char* a, const char* b);

static void _test_ini_get_char(int count);

static bool _test_ini_get_char_prepare();

static void _test_ini_get_char_clean_up();

static void _run_tests(int count);
