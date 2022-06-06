
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "nal_common.h"
#include "nal_libc.h"
#include "nal_mem.h"

#include "nal_libc_tests.h"

/* Test the function nal_printf() by printing a simple string */
void nal_test_print_simple_string(void)
{
    int result = NAL_SUCCESS;
    char * str = "Mr. Anderson. It seems that you've been living two \
lives. In one life, you're Thomas A. Anderson, program writer for a respectable \
software company, you have a social security number, you pay your taxes, and you \
help your landlady carry out her garbage. The other life is lived in computers, \
where you go by the hacker alias Neo and are guilty of virtually every computer \
crime we have a law for.";
    
    result = nal_printf(str);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/* Test the function nal_printf() by printing a more complexe string (using format) */
void nal_test_print_a_formated_string(void)
{
    int result = NAL_SUCCESS;
    char * str = "Christophe Agius and Philippe Chereau";
    
    result = nal_printf("Et il n\'y aura pas %d, siiimpleeementtt %d.(%s).", 3, 2, str);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/* Test the function nal_printf() by printing a utf8 string */
void nal_test_print_utf8_string(void)
{
    int result = NAL_SUCCESS;
    char * str = "おはようございます。";
    
    result = nal_printf(str);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/** 
 * Test the function nal_printf() by printing a big string 
 * (string = 1024 character) 
 * The function nal_printf() truncate the string at 256
 */
void nal_test_print_big_string(void)
{
    int result = NAL_SUCCESS;
    char * str = "1nfFn3mYbeckoQQsi4KPGuLkSTY4lfTj8W0S2LDPzZ1kgPZq9Ei2VWhj2hopTL\
HCrAqk2sIsFxDJ3OMVDOv7QkJN07M09hvIQJyfxTYsEJbRPdv7brYTT3hGjcn1pkooQKEGbcwua2mcFW\
fhCz7VquaYuCz4g7WZcYqJagqQDNlLRwcqLkME2c7Uv9xdQoxTlHTFfJYYLH5gUIqtnQhhwRlHssoJOm\
ODL4HM5ugIYY1SKj8MYNgPcjOPgTNmIWjW4tHFrOlqA6yA6xe3ghIvh8bdFe7uTJMlr31TOS9d1bzqyF\
D7PFpACRmCj6uK7slgu0Gtf1gBpYM4OaYX0HiI6GqvNwcKmZkUfcYevw15sSQBDkPmaWqkxqCTTAGYBM\
y1cYbIYwW4MnfW6zBFUSEqXAQsUSg5FWBi4MOMWFKbwdbMGKG4C2kdWS3TazXNbkiPy9CKZ4w27vovbU\
ZTtxJ4MrE73j09bKmx3l9EFjQpiIOfu9C1K5SjVzKYYSLTlwTUWgLhjgO3NwaMNtyCrmvcmobh8b2PCd\
eNlQF7rQCUdCTu0UjuPgRYGRNIdUNSeBg9CdMZ1lpIiziYiPUUj3sne4kgY32QGDn3q0uIbSV4hSqpin\
j1A3aiHFZSRwZjNPne4PvXKNukCGTi4nUWkKe90wJdiNuN5taZfXeLHNHSBxd1fY4ZAdpxyKFuLNz7DOS\
jajAsyBaVvUxpnSAcZyApoxh79Q5teLBIzWqOFUGBQwanCk81mpC3Qh3XbMlmZlykzxJNoiG1Y7vlHGj\
vuJHWeVTUMTyZDz3rBAwqgFq4O4c6hetfwqRliADYsMEdougU1Miw1RwPxTYi31uEhrXjEhGLtDzIwu4\
RboXtHBMeCppOJNCsGNxGMQGPpC288C88Mo2bgE94B6BolOO5KgcBGVhP6kDdmTDkNpCvCujVnjC46Bk\
VhrB8IogRSAGJ9kcg0YgZ84XxAmUrV4T6Ns4mAs3zRrPcFBch9Q32W4uZmF5jSJkqaXuweUl8KA3DmVh";
    
    result = nal_printf(str);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
}

/* Testing the function nal_snprintf() by writing a simple string */
void nal_test_write_simple_string(void)
{
    int result = NAL_SUCCESS;
    char testedString[NAL_CONS_BUFF_SIZE * 2] = {0};
    size_t writtenSize = 0u;

    char * str = "You take the blue pill, \
the story ends, you wake up in your bed and believe whatever you want to \
believe. You take the red pill, you stay in Wonderland, and I show you how deep \
the rabbit hole goes... Remember, all I'm offering is the truth, nothing more!";

    result = nal_snprintf_s(testedString,
                           NAL_CONS_BUFF_SIZE * 2,
                           NAL_CONS_BUFF_SIZE * 2,
                           &writtenSize,
                           str);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS);
    
    result = nal_printf(testedString);
    CU_ASSERT_EQUAL(result, NAL_SUCCESS); 
}
