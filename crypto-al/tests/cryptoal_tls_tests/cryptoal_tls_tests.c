
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <CUnit/Basic.h>

//#include "nal_common.h"
//#include "nal_rtos_posix.h"
#include "nal_rtos.h"
//#include "nal_libc.h"
#include "nal_mem.h"

#include "com_error.h"
#include "com_entity.h"
#include "com_cryptolib_header.h"
#include "cryptoal_tls.h"

#include "cryptoal_tls_tests.h"


void   entmgt_launcher_setAppRootPath(char * new_path_string);
#define minima(a,b)  ((a<b) ? (a) : (b) )


/* Server thread for test */
#define MEDIUM_TASK_PRIO 112
#define SERVER_TEST_THREAD_STACK_SIZE (16384*2)   /*Limit of machine currently ?*/
#define SERVER_TEST_THREAD_NAME "MirrorServerTestThread"
char server_test_thread_stack[SERVER_TEST_THREAD_STACK_SIZE];
T_nal_task server_test_thread_id;

#define  HTTP_QUESTION  "GET / HTTP/1.0\r\n\r\n"
#define  HTTP_RESPONSE \
            "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n" \
            "<h1>mbed TLS Test Server</h1>\r\n" \
            "<p>Successful connection using: %s</p>\r\n"


/*************** Mirror Thread for tests, MAX Size packet is 4096 in network ****************/
bool  g_client_close_cnx = false;

void server_mirror_test_thread ( void *data )
{
    int returnCode = 0;
    int ret = 0;
    /*mbedtls_*/Ssl_context * pCtx = NULL;
    /*mbedtls_*/Net_context clientCtx;
    char buff[4*1024];
    size_t lenRead = 0u;
    size_t lenWritten = 0u;
    size_t lenWrittenCumul = 0u;
    T_nal_time tick_timetable = {1, 0};
    size_t ii = 0u;
    
    returnCode = cryptoal_create_connect_tls_server_ctx ( "0.0.0.0",  "4455", true, & pCtx );
    printf ("\nAPRES thread: create_connect_tls on server sll Temporaire,  returnCode=%d \n", returnCode);
    //CU_ASSERT_EQUAL(returnCode, 0 );
    
    while (0 == returnCode)
    {
        lenRead = 0u;
        printf ("THREAD: avant  read_tls on server sll\n" );
        returnCode = cryptoal_read_tls_ctx ( pCtx, buff, sizeof(buff), & lenRead );
        printf ("\nTHREAD  %u-ieme MSG recu,: apres  read_tls on server sll,  returnCode=%d, len=%u\n", (unsigned int) ii, returnCode, (unsigned int) lenRead);
        if (false == g_client_close_cnx)
        {
            CU_ASSERT_EQUAL( returnCode, 0 );
        }
        else
        {
            CU_ASSERT_EQUAL( returnCode, 25 );
            goto  exit;
        }

        lenWrittenCumul = 0;
        while ((lenWrittenCumul < lenRead) && (0 == returnCode))
        {
            returnCode = cryptoal_write_tls_ctx ( pCtx, & buff[lenWrittenCumul], /*len*/ (lenRead - lenWrittenCumul), & lenWritten );
            printf ("THREAD: apres  write_tls on server sll,  returnCode=%d, lenWritten=%u\n", returnCode, (unsigned int) lenWritten);
            CU_ASSERT_EQUAL(returnCode, 0 );
            lenWrittenCumul += lenWritten;
        }
        ii ++;
    }

exit:

    printf ("\nThread: Close du Thread Server\n");
    nal_taskExit ();
}

#define             SIZE_PACKET 399
#define MAX_SIZE_PACKET 4096
void cryptoal_test_tls_cnx_exchange (void)
{
    int returnCode = CRYPTOAL_SUCCESS;
    size_t size = 0u;
    int alReturn = 0;
    /*mbedtls_*/Ssl_context * pCtx = NULL;

    char buff[1024];
    size_t lenRead = 0;
    size_t lenWritten = 0;
    T_nal_time tick_timetable = {1, 0};

    int ret = 0;
    FILE * fp = NULL;

    g_client_close_cnx = false;
    
    /* Client connexion w/o server, that shall fail */
    returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4455", & pCtx );    /*Shall fail beause server not ready*/
    printf ("APRES: creat/connect client sll,  returnCode = %d\n", returnCode);
    CU_ASSERT_EQUAL (returnCode, COM_ERR_CONNECT);

    alReturn = nal_taskCreate ( & server_test_thread_id,
                                SERVER_TEST_THREAD_NAME,
                                server_mirror_test_thread,
                                NULL, /* void *data */
                                MEDIUM_TASK_PRIO, /* take care of OS */
                                (void *) server_test_thread_stack,
                                SERVER_TEST_THREAD_STACK_SIZE
                              );
    CU_ASSERT_EQUAL(alReturn, NAL_SUCCESS );

    nal_sleep ( & tick_timetable );

    pCtx = NULL;
    printf ("AVANT: creat/connect client sll \n");

    entmgt_launcher_setAppRootPath("./tests/cryptoal_tls_tests/simple");

    pCtx = NULL;
    returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4456", & pCtx );   /* bad port */
    printf ("APRES: creat/connect client sll with bad Port,  returnCode = %d\n", returnCode);
    CU_ASSERT_EQUAL (returnCode, 22);

    pCtx = NULL;
    returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4455", & pCtx );
    printf ("APRES: creat/connect client sll,  returnCode = %d\n", returnCode);
    CU_ASSERT_EQUAL (returnCode, 0);

    if (0 == returnCode)
    {
        returnCode = cryptoal_write_tls_ctx ( pCtx, buff, SIZE_PACKET, & lenWritten );
        printf ("Apres write buffer, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWritten);
        CU_ASSERT_EQUAL(returnCode, 0 );
        CU_ASSERT_EQUAL(lenWritten, SIZE_PACKET );

        lenRead = 0u;
        returnCode = cryptoal_read_tls_ctx ( pCtx, buff, sizeof(buff)-1, & lenRead );
        printf ("Apres read returnCode=%d, len=%u\n", returnCode, (unsigned int) lenRead);
        CU_ASSERT_EQUAL(returnCode, 0 );
        CU_ASSERT_EQUAL(lenRead, minima(SIZE_PACKET, MAX_SIZE_PACKET) );

        g_client_close_cnx = true;

        returnCode = cryptoal_close_free_tls_ctx( pCtx );
        CU_ASSERT_EQUAL(returnCode, 0 );
        
        nal_sleep ( & tick_timetable );
    }
}


void cryptoal_test_tls_cnx_big_traffic (void)
{
    int returnCode = CRYPTOAL_SUCCESS;
    size_t size = 0u;
    int alReturn = 0;
    /*mbedtls_*/Ssl_context * pCtx = NULL;

    char buff[4 * 1024];
    size_t lenRead = 0;
    size_t lenWritten = 0;
    T_nal_time tick_timetable = {1, 0};
    size_t ii = 0u;

    g_client_close_cnx = false;

    {
        alReturn = nal_taskCreate ( & server_test_thread_id,
                                    SERVER_TEST_THREAD_NAME,
                                    server_mirror_test_thread,
                                    NULL, /* void *data */
                                    MEDIUM_TASK_PRIO, /* take care of OS */
                                    (void *) server_test_thread_stack,
                                    SERVER_TEST_THREAD_STACK_SIZE
                                  );
        CU_ASSERT_EQUAL(alReturn, NAL_SUCCESS );

        nal_sleep ( & tick_timetable );

        pCtx = NULL;
        printf ("AVANT: creat/connect client sll \n");

        entmgt_launcher_setAppRootPath("./tests/cryptoal_tls_tests/simple");

        pCtx = NULL;
        returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4455", & pCtx );
        printf ("APRES: creat/connect client sll,  returnCode = %d\n", returnCode);
        CU_ASSERT_EQUAL (returnCode, 0);

        if (0 == returnCode)
        {
          
//            returnCode = cryptoal_write_tls_ctx ( pCtx, write_buffer, size, & lenWritten );
//            printf ("Apres write buffer, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWritten);
//           CU_ASSERT_EQUAL(returnCode, 0 );
//            CU_ASSERT_EQUAL(lenWritten, 671 );
#undef             SIZE_PACKET
#undef  MAX_SIZE_PACKET
#define             SIZE_PACKET 3399
#define MAX_SIZE_PACKET 4096

            while ( ii < 1000)
            {
                lenWritten = 0u;
                returnCode = cryptoal_write_tls_ctx ( pCtx, buff, SIZE_PACKET, & lenWritten );
                printf ("Apres write buffer, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWritten);
                CU_ASSERT_EQUAL(returnCode, 0 );
                CU_ASSERT_EQUAL(lenWritten, SIZE_PACKET );

                lenRead = 0u;
                returnCode = cryptoal_read_tls_ctx ( pCtx, buff, sizeof(buff), & lenRead );
                printf ("Apres read returnCode=%d, len=%u\n", returnCode, (unsigned int) lenRead);
                CU_ASSERT_EQUAL(returnCode, 0 );
                CU_ASSERT_EQUAL(lenRead, minima(SIZE_PACKET, MAX_SIZE_PACKET) );
                ii++;
            }
            g_client_close_cnx = true;

            returnCode = cryptoal_close_free_tls_ctx ( pCtx );
            CU_ASSERT_EQUAL(returnCode, 0 );

            nal_sleep ( & tick_timetable );
        }

    }
}


/* The big packet to go and come back, with integrity check.
 * A written packet is automaticaly segmented to 16 Kbytes packets, and a received packet is automaticaly segmented to 4 Kbytes packets  */
#undef SIZE_PACKET
#define              SIZE_PACKET 142867
#define  MAX_SIZE_PACKET_WRITE (4*4096)
#define  MAX_SIZE_PACKET_READ 4096
#define  NB_PACKET_WRITE  (SIZE_PACKET/MAX_SIZE_PACKET_WRITE)
#define  LAST_PACKET_SIZE_WRITE  (SIZE_PACKET%MAX_SIZE_PACKET_WRITE)
#define  NB_PACKET_READ  (SIZE_PACKET/MAX_SIZE_PACKET_READ)
#define  LAST_PACKET_SIZE_READ  (SIZE_PACKET%MAX_SIZE_PACKET_READ)
void cryptoal_test_tls_cnx_big_packet (void)
{
    int returnCode = CRYPTOAL_SUCCESS;
    size_t size = 0u;
    int alReturn = 0;
    /*mbedtls_*/Ssl_context * pCtx = NULL;

    char * buff = malloc (SIZE_PACKET + 256);
    size_t lenRead = 0;
    size_t lenReadCumul = 0;
    size_t lenWritten = 0;
    size_t lenWrittenCumul = 0;
    T_nal_time tick_timetable = {1, 0};
    size_t ii = 0u;
    size_t jj = 0u;
    size_t kk = 0u;

    g_client_close_cnx = false;

    alReturn = nal_taskCreate ( & server_test_thread_id,
                                SERVER_TEST_THREAD_NAME,
                                server_mirror_test_thread,
                                NULL, /* void *data */
                                MEDIUM_TASK_PRIO, /* take care of OS */
                                (void *) server_test_thread_stack,
                                SERVER_TEST_THREAD_STACK_SIZE
                              );
    CU_ASSERT_EQUAL(alReturn, NAL_SUCCESS );

    nal_sleep ( & tick_timetable );

    pCtx = NULL;
    printf ("AVANT: creat/connect client sll \n");

    entmgt_launcher_setAppRootPath("./tests/cryptoal_tls_tests/simple");

    pCtx = NULL;
    returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4455", & pCtx );
    printf ("APRES: creat/connect client sll,  returnCode = %d\n", returnCode);
    CU_ASSERT_EQUAL (returnCode, 0);

    if (0 == returnCode)
    {

        while ( ii < 2)
        {
            for (kk=0u; kk<SIZE_PACKET; kk++)    /* initialize write buffer with known values */
            {
              buff[kk] = (char) (kk % 256);
            }

            lenWritten = 0u;
            lenWrittenCumul = 0u;
            jj = 0u;
            while ( (0 == returnCode) && (lenWrittenCumul < SIZE_PACKET) )
            {
                lenWritten = 0u;
                returnCode = cryptoal_write_tls_ctx ( pCtx, & buff[lenWrittenCumul], (SIZE_PACKET-lenWrittenCumul), & lenWritten );
                printf ("Apres write buffer UN segment, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWritten);
                lenWrittenCumul += lenWritten;
            }
            printf ("Apres write buffer multi-segments, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWrittenCumul);
            CU_ASSERT_EQUAL(returnCode, 0 );
            CU_ASSERT_EQUAL(lenWrittenCumul, SIZE_PACKET );

            for (kk=0u; kk<SIZE_PACKET; kk++)   /* Raz the buffer betwenn writes and reads */
            {
                buff[kk] = (char) 0;
            }

            jj =0u;
            lenReadCumul = 0;
            while (jj < NB_PACKET_READ)
            {
                lenRead = 0u;
                returnCode = cryptoal_read_tls_ctx ( pCtx, & buff[lenReadCumul], (SIZE_PACKET-lenReadCumul), & lenRead );
                printf ("Apres read returnCode=%d, len=%u\n", returnCode, (unsigned int) lenRead);
                CU_ASSERT_EQUAL(returnCode, 0 );
                CU_ASSERT_EQUAL(lenRead, MAX_SIZE_PACKET );
                lenReadCumul += lenRead;
                jj ++;
            }
            lenRead = 0u;
            returnCode = cryptoal_read_tls_ctx ( pCtx, & buff[lenReadCumul], (SIZE_PACKET-lenReadCumul), & lenRead );
            lenReadCumul += lenRead;
            printf ("Apres LAST read, returnCode=%d, len=%u, lenReadCumul=%u\n", returnCode, (unsigned int) lenRead, (unsigned int) lenReadCumul);
            CU_ASSERT_EQUAL( returnCode, 0 );
            CU_ASSERT_EQUAL( lenRead, LAST_PACKET_SIZE_READ );
            CU_ASSERT_EQUAL( lenReadCumul, SIZE_PACKET );

            ii++;

            for (kk=0u; kk<  SIZE_PACKET ; kk++)   /* Check integrity of received data */
            {
                if ( buff[kk] != (char) (kk % 256) )
                {                      
                    printf ("*");
                    CU_ASSERT_EQUAL( 0, 1 );    // integrity error
                }
            }
            printf ("Integrity test done, on %d bytes\n", SIZE_PACKET);

        }
        g_client_close_cnx = true;

        returnCode = cryptoal_close_free_tls_ctx ( pCtx );
        CU_ASSERT_EQUAL(returnCode, 0 );

        nal_sleep ( & tick_timetable );
    }

    NAL_FREE (buff);
}

#undef SIZE_PACKET
#define             SIZE_PACKET 399
#define MAX_SIZE_PACKET 4096

#define NB_PATH 7
char client_certificate_path[NB_PATH][64] = 
{            /* Path of certificate */
        "./tests/cryptoal_tls_tests/simple",
        "./tests/cryptoal_tls_tests/simple_other_root",
        "./tests/cryptoal_tls_tests/simple",
        "./tests/cryptoal_tls_tests/simple_expired_box",
        "./tests/cryptoal_tls_tests/simple",
        "./tests/cryptoal_tls_tests/simple_expired_ca",
        "./tests/cryptoal_tls_tests/simple"
    };

int  client_connection_result[NB_PATH] =
{        /* expected result  ("0"  for Ok) */
        0,
        29,
        0,
        29,
        0,
        29,
        0
};
void cryptoal_test_tls_cnx_bad_certificate (void)
{
   int returnCode = CRYPTOAL_SUCCESS;
    size_t size = 0u;
    int alReturn = 0;
    /*mbedtls_*/Ssl_context * pCtx = NULL;

    char buff[1024];
    size_t lenRead = 0;
    size_t lenWritten = 0;
    T_nal_time tick_timetable = {1, 0};
    size_t ii=0u;

    int ret = 0;
    FILE * fp = NULL;

    for (ii=0u; ii<NB_PATH ; ii++)
    {
        g_client_close_cnx = false;
    
        alReturn = nal_taskCreate ( & server_test_thread_id,
                                    SERVER_TEST_THREAD_NAME,
                                    server_mirror_test_thread,
                                    NULL, /* void *data */
                                    MEDIUM_TASK_PRIO, /* take care of OS */
                                    (void *) server_test_thread_stack,
                                    SERVER_TEST_THREAD_STACK_SIZE
                                  );
        CU_ASSERT_EQUAL(alReturn, NAL_SUCCESS );

        nal_sleep ( & tick_timetable );
        pCtx = NULL;
        printf ("AVANT: creat/connect client sll numero %u\n", (unsigned int) ii);
        entmgt_launcher_setAppRootPath(client_certificate_path[ii]);

        pCtx = NULL;
        returnCode = cryptoal_create_connect_tls_client_ctx ( "127.0.0.1",  "4455", & pCtx );
        printf ("APRES: creat/connect client sll,  returnCode = %d\n", returnCode);
        CU_ASSERT_EQUAL (returnCode, client_connection_result[ii]);

        if (0 == returnCode)
        {
            returnCode = cryptoal_write_tls_ctx ( pCtx, buff, SIZE_PACKET, & lenWritten );
            printf ("Apres write buffer, returnCode = %d, len written = %u\n", returnCode, (unsigned int) lenWritten);
            CU_ASSERT_EQUAL(returnCode, 0 );
            CU_ASSERT_EQUAL(lenWritten, SIZE_PACKET );

            lenRead = 0u;
            returnCode = cryptoal_read_tls_ctx ( pCtx, buff, sizeof(buff)-1, & lenRead );
            printf ("Apres read returnCode=%d, len=%u\n", returnCode, (unsigned int) lenRead);
            CU_ASSERT_EQUAL(returnCode, 0 );
            CU_ASSERT_EQUAL(lenRead, minima(SIZE_PACKET, MAX_SIZE_PACKET) );

            g_client_close_cnx = true;

            returnCode = cryptoal_close_free_tls_ctx( pCtx );
            CU_ASSERT_EQUAL(returnCode, 0 );
        }
        nal_sleep ( & tick_timetable );
    }  /* End for */

}



/*************** Bouchons for tests ****************/

/* this path is impacting thread CA (server) and is always good : */
char theRootPathOfResources[] = "./tests/cryptoal_tls_tests/simple_2";

const char * entmgt_launcher_getAppRootPath()
{
    return theRootPathOfResources;
}

void   entmgt_launcher_setAppRootPath(char * new_path_string)
{
    strcpy (theRootPathOfResources, new_path_string);
}
