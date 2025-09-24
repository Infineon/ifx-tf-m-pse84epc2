/*
 * Copyright (c) 2023-2024 Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "extra_s_tests.h"

static void tfm_pse84_s_test_1001(struct test_result_t *ret)
{
    TEST_LOG("PSE84 dummy secure test case started!\r\n");
    ret->val = TEST_PASSED;
}

static struct test_t pse84_s_tests[] = {
    {&tfm_pse84_s_test_1001, "TFM_S_PSE84_TEST_1001",
     "PSE84 secure dummy test"},
};

void register_testsuite_extra_s_interface(struct test_suite_t *p_test_suite)
{
    uint32_t list_size;

    list_size = (sizeof(pse84_s_tests) /
                 sizeof(pse84_s_tests[0]));

    set_testsuite("PSE84 secure test suite (TFM_S_PSE84_TEST_1XXX)",
                  pse84_s_tests, list_size, p_test_suite);
}
