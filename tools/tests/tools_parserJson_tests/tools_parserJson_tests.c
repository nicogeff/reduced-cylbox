#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>

#include "tools_type.h"
#include "tools_parserJson.h"
#include "tools_trace.h"

/**
 * Global variables
 */

static const char * Document_String =
        "{\"BtrDefinition\":{\"DEB\":\"1536783148721\",\"FIN\":\"1536778452235\",\"SAS\":true,\"SASR\":true,\"VER\":\"2\",\"MAT\":\"400f14e-998c-11e8-a007-000c29bf58b6\",\"ROL\":\"1\",\"NAT\":\"99\",\"signature\":{\"SignedInfo\":{\"DigestAlgoritm\":\"1\"},\"SignatureValue\":\"bzRnUzd3b1VXb09GT2dpei9DVnUzZWdWRnkrekpRRWFpVVVrbEdJQVF0cVp2Z2krREM2RGJRcTZ1dDRkbXlQSkx0MC9BUnJtei9WeElqeSt1V0M5dmZLOHdsem81SVlpUjJSOFArVjl5WGFGUVlzaUlxdm9WQkYvd3RwMVhCdituZDF5ZC83UXhwdCt5dVBFSXhlcmhiTjFvR3pRN0NqSnF3QVpFOUdLQ0pvUC9XT0lsd2djdURBNWRMUFJlQWtxV0VHdmZoZm1rRkt5R3NnQ3ROeHBmVWNwc1N4QTBPQXdqcitGcml4bW1RRWk0RkpQMmtObUZiaHRDeDNrVXpmdmRBS0dWK3RDK1h3Z3JCSGxqcFZ6VTNPcUxzOGpKS01CMnZnbkpvK0lmdlJVVjZHTUJodVkwdVNhSkNzbE16L1NiZDR3TG53ZitKRHVZbWJwbU9xUk9TMlBTNHEwR1k2aFRtdFZ1N0pZNElUUGhSMXZabVdmMU01aVA3OUE0bkZNcFJnNVgxWWd0SitEWTY3WkNvTlNCZDJhaHptWWYxdEk0KzJtbmJXdGN6V1B0M01sMDZJRitiWVNJTkVyN3Vka3N1VlN3cmU5QlZzZHIyN0d6d0dNbG00QndKdTlqRW9pUzhyZWtTRWtQTnhKUXpyMU40SVgvVVByUkNFaFY1WWpWNHFKQjFuVlpsOWpnZk42TklTMzN5QVFrbTluWVFKTUxUd2s3Q3dlcWM4d2k1M09VYWZGZmpFT1daVEFIMEhYc1ZoVVhTUEpMbUc4REhxVGhtTU9oL29pOThaSE40U3RMVVpRcm5tSHBKSmxiZmhZN1VDTE5xcG9LbkM4d2FrOC9EMjFCT21LbXprV3hzWTRKYWpqRVl6NytUN2pyb0NUWWN2K1RtNmxmTT0=\"},\"empreinte_donnee\":{\"HashInfo\":{\"DigestAlgoritm\":\"1\"},\"HashValue\":\"YTYxMWMyMDBmZDdiMWIwYjE2YmU0M2U2MWUyMGZjMDQxNGU3ZmE1NQ==\"}}}";

void tools_parserJson_parse_test()
{
    T_tools_json_char* json = NULL;
    T_tools_json_value* value = NULL;
    
    json = (T_tools_json_char*) Document_String;

    value = tools_json_parse(json, strlen(Document_String));
    CU_ASSERT_NOT_EQUAL (value, NULL);
    if (value == NULL)
    {
        fprintf(stderr, "Unable to parse data\n");
    }

    tools_json_print_tree(value, 0);

    tools_json_value_free(value);
    
    tools_json_print_tree(value, 0);
}
