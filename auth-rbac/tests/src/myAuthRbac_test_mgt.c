/*
 * File:   myAuthRbac_test_mgt.c
 *
 * This is Cunit tests
 * Created on apr. 2020
 */


/*------------------------------ Definitions --------------------------------------*/
/* This source file create Rbac tables (is extern in other source files): */
#define EXTERN_AUTH

/*------------------------------ headers included -------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <pthread.h>

#include "nal_common.h"
#include "nal_rtos_posix.h"
#include "nal_rtos.h"
#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"
#include "tools_trace.h"
#include "tools_fileManip.h"
#include "tools_parserJson.h"

#include "com_auth_rbac_api.h"

#include "auth_global_rbac.h"



int init_suite(void)
{
  char string_cmd_rbac[500];
    system (" mkdir -p  /tmp/running/rbac;  rm -f /tmp/running/rbac/rbac.json");
    sprintf (string_cmd_rbac, "%s", "/bin/echo -e   {\\\"rbac\\\":{\\\"version\\\":1,\\\"users\\\":[{\\\"user\\\":\\\"installer\\\",\\\"hashedPassword\\\":\\\"6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96\\\",\\\"role-idx\\\":[0],\\\"inactif\\\":false}],\\\"roles\\\":[{\\\"role\\\":\\\"installation\\\",\\\"author-idx\\\":[0]}],\\\"authorizations\\\":[\\\"change-rbac\\\"]}} >  /tmp/running/rbac/rbac.json");
    
    system (string_cmd_rbac );
    return 0;
}

int clean_suite(void)
{
    return 0;
}

extern T_com_entity_status AUTH_RBAC_Status;

void test_init()
{
    T_com_status result = COM_SUCCESS;

    AUTH_RBAC_Status = COM_ENTITY_IS_STOPPED;
    result = auth_rbac_init();
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_INITIALIZED);

}

#define NB_CONF_SETTINGS_GOOD 2u
T_com_entity_setting  myConfSettingsGood[NB_CONF_SETTINGS_GOOD] =
{
    {"AUTH_RBAC_SESSION_DURATION_SEC", 15},
    {"AUTH_RBAC_LOCKACCOUNT_DURATION_SEC", 25}
};

#define NB_CONF_SETTINGS_FUZZ 47u
T_com_entity_setting  myConfSettingsFuzz[NB_CONF_SETTINGS_FUZZ] =
{
    {"dsfs -*/dffds", "DSFD0─:\\,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:\\,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:&éà@SF"},
    {"ababababfs&é ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {" ", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", ""},
    {"ababababfs&é ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é\" ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf"},
    {"mn}^@]@|`{[#~mnmn", "MNMN"},
    {"dsfs -*/dffds", "DSFD0─:,&éà@SF"},
    {"ababababfs&é ,:!ùdffds", "ABABd/.§?%M£°=)àç~{sf                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "},
    {"mn}^@]@                                                                                                                                                                   |`{[#~mnmn", "MNMN"}
};
void test_configure()
{
    T_com_status result = COM_SUCCESS;

    printf ("Configure avec Settings malformés");
    AUTH_RBAC_Status = COM_ENTITY_IS_INITIALIZED;
    result = auth_rbac_configure(47, myConfSettingsFuzz);
    CU_ASSERT_EQUAL(result, COM_ERR_NOT_ALLOWED);
    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_INITIALIZED);


    printf ("Configure avec Settings inexistant, comme en vrai");
    AUTH_RBAC_Status = COM_ENTITY_IS_INITIALIZED;
    result = auth_rbac_configure(2, myConfSettingsGood);
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_CONFIGURED);

    CU_ASSERT_EQUAL (25, get_auth_rbac_lockaccount_duration() );
    CU_ASSERT_EQUAL (15, get_auth_rbac_session_duration() );
}

void test_start()
{
    T_com_status result = COM_SUCCESS;
    bool granted = false;

    AUTH_RBAC_Status = COM_ENTITY_IS_CONFIGURED;
    result = auth_rbac_start();
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_STARTED);

    /*check that Rbac is built correctly in start function, with rbac test file*/
    result = auth_user_check_for_authent_authoriz ("installer", "6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96", "change-rbac", & granted);
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(granted, true);


}

void test_stop()
{
    T_com_status result = COM_SUCCESS;

    AUTH_RBAC_Status = COM_ENTITY_IS_STARTED;
    result = auth_rbac_stop();
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_STOPPED);

}


int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("myAuthRbacMgtTest", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    
    /* Add the tests to the suite */
    if (    (NULL == CU_add_test(pSuite, "test1", test_init)) ||
            (NULL == CU_add_test(pSuite, "test2", test_configure)) ||
            (NULL == CU_add_test(pSuite, "test3", test_start)) ||
            (NULL == CU_add_test(pSuite, "test4", test_stop))
        )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}


/***********************************************************    Test Stubs    ***********************************************************/

/*  Stub for test
 *
 */

char theRootPathOfResources[] = "/tmp/";
const char * entmgt_launcher_getAppRootPath ()
{
    return theRootPathOfResources;
}

T_com_status entmgt_readConfigurationFile ( const char * configurationFilePath, char ** buffer, size_t * bufferSize )
{
    uint8_t * byte = NULL;
    size_t byteSize = 0u;

    tools_readFile(configurationFilePath, &byte, &byteSize);
    *buffer = (char*) NAL_MALLOC(byteSize * sizeof (char));
    memset(*buffer, 0, byteSize * sizeof (char));
    memcpy(*buffer, byte, byteSize * sizeof (char));

    *bufferSize = byteSize;

    return COM_SUCCESS;
}

T_com_status entmgt_parseConfigurationBuffer ( const char * buffer, size_t bufferSize,   T_tools_json_value ** rawConfigurationData )
{
    *rawConfigurationData = tools_json_parse(buffer, bufferSize);  
    return COM_SUCCESS;
}

