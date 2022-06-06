/*
 * File:   myAuthRbac_test_authent.c
 * 
 * This is Cunit tests
 * Created on Nov. 2019
 */

/* This source file DO NOT create Rbac tables: */
#define EXTERN_AUTH extern


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
#include "tools_parserJson.h"
#include "tools_fileManip.h"

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

//1532394403 1552394403
extern T_com_entity_status AUTH_RBAC_Status;


#define NB_CONF_SETTINGS_GOOD 2u
T_com_entity_setting  myConfSettingsGood[NB_CONF_SETTINGS_GOOD] =
{
    {"AUTH_RBAC_SESSION_DURATION_SEC", 15},
    {"AUTH_RBAC_LOCKACCOUNT_DURATION_SEC", 25}
};
void test1()
{

    T_com_status result = COM_SUCCESS;
    int alReturn = 0;
    T_nal_time timeStruct;
    bool success = false;
    bool granted = false;
    char Id[10][64]; 

    AUTH_RBAC_Status = COM_ENTITY_IS_STOPPED;
    result = auth_rbac_init();
    result = auth_rbac_configure(2u, myConfSettingsGood);
    result = auth_rbac_start();

    CU_ASSERT_EQUAL(AUTH_RBAC_Status, COM_ENTITY_IS_STARTED);

    Id[0][0] = 0;
    Id[1][0] = 0;
    Id[2][0] = 0;
    Id[3][0] = 0;
    Id[4][0] = 0;
    Id[5][0] = 0;
    result = auth_create_session ( Id[0] );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);

    result = auth_create_session ( Id[1] );
    printf ("%s\n", Id[1]);
    result = auth_create_session ( Id[2] );
    printf ("%s\n", Id[2]);
    result = auth_create_session ( Id[3] );
    printf ("%s\n", Id[3]);
    result = auth_create_session ( Id[4] );
    printf ("%s\n", Id[4]);
    result = auth_create_session ( Id[5] );
    printf ("%s\n", Id[5]);

    result = auth_delete_session ( Id[0] );
    result = auth_delete_session ( Id[1] );
    result = auth_delete_session ( Id[2] );
    result = auth_delete_session ( Id[4] );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    result = auth_delete_session ( Id[5] );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    result = auth_delete_session ( Id[5] );
    CU_ASSERT_EQUAL(result, COM_ERR_UNKNOWN_ELEMENT);

//    Id[0] = 0;
//    result = auth_create_session ( Id );
//    CU_ASSERT_EQUAL(result, COM_SUCCESS);

    result = auth_authenticate_session ( Id[3], "installer_bidon", "6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96", & success );
    CU_ASSERT_EQUAL(result, COM_ERR_UNKNOWN_ELEMENT);
//    CU_ASSERT_EQUAL(success, false);
    result = auth_authenticate_session ( Id[3], "installer", "6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa97", & success );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(success, false);
    result = auth_authenticate_session ( Id[3], "installer", "6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96", & success );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(success, true);

    result = auth_authenticate_session ( Id[4], "installer", "6a51c8231013a106bed56fd2d6084f07851de165bccafec5672dbd24d623fa96", & success );
    CU_ASSERT_EQUAL(result, COM_ERR_UNKNOWN_ELEMENT);

    result = auth_check_authorization_in_session ( Id[3],   "peter-la-rbac", & granted );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(granted, false);

    result = auth_check_authorization_in_session ( Id[4],   "change-rbac", & granted );
    CU_ASSERT_EQUAL(result, COM_ERR_UNKNOWN_ELEMENT);

    result = auth_check_authorization_in_session ( Id[3],   "change-rbac", & granted );
    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    CU_ASSERT_EQUAL(granted, true);

    
    
    //    result = auth_loadJsonBuffer_in_rbac_ram (void * p_json_buffer);
//    CU_ASSERT_EQUAL(result, COM_SUCCESS);
    
    
    
    
}


int main()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("myAuthRbac_test_authent", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if (    (NULL == CU_add_test(pSuite, "test1", test1))
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


/***********************************************************    Stubs    ***********************************************************/

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


