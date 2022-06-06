
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CUnit/Basic.h>

#include "tools_parserJson.h"
#include "tools_parserJsonTools.h"

#include "tools_parserJsonTools_tests.h"

static const char * Document_String =
        "{\"BtrDefinition\":{\"DEB\":\"1536783148721\",\"FIN\":\"1536778452235\",\"SAS\":true,\"SASR\":true,\"VER\":\"2\",\"MAT\":\"400f14e-998c-11e8-a007-000c29bf58b6\",\"ROL\":\"1\",\"NAT\":\"99\",\"signature\":{\"SignedInfo\":{\"DigestAlgoritm\":\"1\"},\"SignatureValue\":\"bzRnUzd3b1VXb09GT2dpei9DVnUzZWdWRnkrekpRRWFpVVVrbEdJQVF0cVp2Z2krREM2RGJRcTZ1dDRkbXlQSkx0MC9BUnJtei9WeElqeSt1V0M5dmZLOHdsem81SVlpUjJSOFArVjl5WGFGUVlzaUlxdm9WQkYvd3RwMVhCdituZDF5ZC83UXhwdCt5dVBFSXhlcmhiTjFvR3pRN0NqSnF3QVpFOUdLQ0pvUC9XT0lsd2djdURBNWRMUFJlQWtxV0VHdmZoZm1rRkt5R3NnQ3ROeHBmVWNwc1N4QTBPQXdqcitGcml4bW1RRWk0RkpQMmtObUZiaHRDeDNrVXpmdmRBS0dWK3RDK1h3Z3JCSGxqcFZ6VTNPcUxzOGpKS01CMnZnbkpvK0lmdlJVVjZHTUJodVkwdVNhSkNzbE16L1NiZDR3TG53ZitKRHVZbWJwbU9xUk9TMlBTNHEwR1k2aFRtdFZ1N0pZNElUUGhSMXZabVdmMU01aVA3OUE0bkZNcFJnNVgxWWd0SitEWTY3WkNvTlNCZDJhaHptWWYxdEk0KzJtbmJXdGN6V1B0M01sMDZJRitiWVNJTkVyN3Vka3N1VlN3cmU5QlZzZHIyN0d6d0dNbG00QndKdTlqRW9pUzhyZWtTRWtQTnhKUXpyMU40SVgvVVByUkNFaFY1WWpWNHFKQjFuVlpsOWpnZk42TklTMzN5QVFrbTluWVFKTUxUd2s3Q3dlcWM4d2k1M09VYWZGZmpFT1daVEFIMEhYc1ZoVVhTUEpMbUc4REhxVGhtTU9oL29pOThaSE40U3RMVVpRcm5tSHBKSmxiZmhZN1VDTE5xcG9LbkM4d2FrOC9EMjFCT21LbXprV3hzWTRKYWpqRVl6NytUN2pyb0NUWWN2K1RtNmxmTT0=\"},\"empreinte_donnee\":{\"HashInfo\":{\"DigestAlgoritm\":\"1\"},\"HashValue\":\"YTYxMWMyMDBmZDdiMWIwYjE2YmU0M2U2MWUyMGZjMDQxNGU3ZmE1NQ==\"}}}";


static const char * LOCAL_CONFIGURATION_STRING = 
"{ \"localConfiguration\": {\
	\"http://www.example.com/parsing/moduleManagement\": {\
		\"settings\": [\
			{\"hostname\": \"Raspberry\"},\
			{\"ip_addr\": \"192.168.1.1\"}\
			]\
		}\
	}\
}";

void printData(const char * key, const T_tools_json_type dataType, T_tools_union_data dataValue)
{
    printf("\n --> The value of key %s = ", key);
    
    switch (dataType)
    {
    case json_object:
        printf("object: %p with length %ud\n", dataValue.object.values, dataValue.object.length);
        break;
    case json_array:
        break;
    case json_integer:
        printf("int: %ld \n", dataValue.integer);
        break;
    case json_double:
        printf("int: %f", dataValue.dbl);
        break;
    case json_string:
        printf("string: %s\n", dataValue.string.ptr);
        break;
    case json_boolean:
        printf("bool: %d\n", dataValue.boolean);
        break;
    case json_none:
    case json_null:
        printf ("no data available\n");
        break;
    default:
        printf("you should not be here!\n");
        break;
    }
}

void tools_json_grab_data_test()
{
    T_tools_json_char* json = NULL;
    T_tools_json_value* value = NULL;

    json = (T_tools_json_char*) Document_String;
    
    T_tools_json_type dataType = 0;
    T_tools_union_data dataValue = {0};
    
    const char * key01 = "DEB";
    const char * key02 = "FIN";
    const char * key03 = "MAT";
    const char * key04 = "ROL";
    const char * key05 = "signature";
    const char * key06 = "SignatureValue";
    const char * key07 = "HashValue";
    const char * key08 = "unknow";
    const char * key09 = "UTF8-€€€€ééééèèè¨¨¨^^";
    const char * key10 = NULL;

    
    /* Parse the buffer */
    value = tools_json_parse(json, strlen(Document_String));
    CU_ASSERT_NOT_EQUAL(value, NULL);
    if (value == NULL)
    {
        fprintf(stderr, "Unable to parse data\n");
    }

    /* Print the complete JSON tree */
    tools_json_print_tree(value, 0);

    /* Now grab some data from the C structure of the JSON */
    
    /* Key 01 */
    tools_json_grab_data(value, key01, &dataType, &dataValue);
    printData(key01, dataType, dataValue);
    
    /* Key 02 */
    tools_json_grab_data(value, key02, &dataType, &dataValue);
    printData(key02, dataType, dataValue);
    
    /* Key 03 */
    tools_json_grab_data(value, key03, &dataType, &dataValue);
    printData(key03, dataType, dataValue);
    
    /* Key 04 */
    tools_json_grab_data(value, key04, &dataType, &dataValue);
    printData(key04, dataType, dataValue);
    
    /* Key 05 */
    tools_json_grab_data(value, key05, &dataType, &dataValue);
    printData(key05, dataType, dataValue);
    
    /* Key 06 */
    tools_json_grab_data(value, key06, &dataType, &dataValue);
    printData(key06, dataType, dataValue);
    
    /* Key 07 */
    tools_json_grab_data(value, key07, &dataType, &dataValue);
    printData(key07, dataType, dataValue);
    
    /* Key 08 */
    tools_json_grab_data(value, key08, &dataType, &dataValue);
    printData(key08, dataType, dataValue);
    
    /* Key 09 */
    tools_json_grab_data(value, key09, &dataType, &dataValue);
    printData(key09, dataType, dataValue);
    
    /* Key 10 */
    tools_json_grab_data(value, key10, &dataType, &dataValue);
    printData(key10, dataType, dataValue);
    
    /* Invalid parameters */
    tools_json_grab_data(NULL, key01, &dataType, &dataValue);
    
    tools_json_grab_data(value, NULL, &dataType, &dataValue);
    
    tools_json_grab_data(value, key01, NULL, &dataValue);
    
    tools_json_grab_data(value, key01, &dataType, NULL);
    
    tools_json_grab_data(NULL, NULL, NULL, NULL);
    
    /* Clean memory */
    tools_json_value_free(value);
}
