
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

#include "tools_type.h"
#include "tools_trace.h"
#include "tools_conversion.h"

/**
 * @brief Convert a string to the corresponding unsigned int
 * @details This Function follows the CERT-INT06 rules concerning string conversions.
 * @param[in] str A string .
 * @param[out] convertedUint The pointer to the uint that should contain the converted value
 * @return True (1) if success, false if function error
 */
T_tools_boolean tools_text2unsigned(const char* str, uint32_t* convertedUint)
{
    char* end_ptr = NULL;
    uint32_t result = 0u;
    T_tools_boolean returnValue = TOOLS_TRUE;

    /* defensive code check */
    if ((str == NULL) || (convertedUint == NULL))
    {
        returnValue = TOOLS_FALSE;
        goto exit;
    }

    result = (uint32_t) strtoul(str, &end_ptr, 10);
    if (result == ULONG_MAX)
    {
        TRACE_GEN(ERROR, "String conversion error", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if (end_ptr == str)
    {
        TRACE_GEN(ERROR, "String conversion error", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if (result > UINT_MAX)
    {
        TRACE_GEN(ERROR, "Too large value", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if ('\0' != *end_ptr)
    {
        TRACE_GEN(ERROR, "Extra characters on input", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else
    {
        *convertedUint = result;
        returnValue = TOOLS_TRUE;
    }
exit:
    return returnValue;

}

/**
 * @brief Convert a string to the corresponding int
 * @details This Function follows the CERT-INT06 rules concerning string conversions.
 * @param[in] str A string .
 * @param[out] convertedInt The pointer to the int that should contain the converted value
 * @return True (1) if success, false if function error
 */
T_tools_boolean tools_text2integer(const char* str, int32_t* convertedInt)
{
    char* end_ptr = NULL;
    int32_t result = 0;
    T_tools_boolean returnValue = TOOLS_TRUE;

    if ((str == NULL) || (convertedInt == NULL))
    {
        returnValue = TOOLS_FALSE;
        goto exit;
    }

    result = (int32_t) strtol(str, &end_ptr, 10);
    if (result == LONG_MAX)
    {
        TRACE_GEN(ERROR, "String conversion error", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if (end_ptr == str)
    {
        TRACE_GEN(ERROR, "String conversion error", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if (result > INT_MAX)
    {
        TRACE_GEN(ERROR, "Too large value", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else if ('\0' != *end_ptr)
    {
        TRACE_GEN(ERROR, "Extra characters on input", __FUNCTION__, __LINE__);
        returnValue = TOOLS_FALSE;
    }
    else
    {
        *convertedInt = result;
        returnValue = TOOLS_TRUE;
    }
exit:
    return returnValue;

}


/**
 * @brief Convert an 8 bits number in its ASCII representation
 * @details i.e. 0x4F will give "4F"
 * @param[in] input 8 bits value to convert
 * @param[out] dest table of 2 char to store the result
 * @return void
 */
//void tools_Byte2Ascii (char* dest, char input)
//{
//    int i;
//
//    dest[ 1 ] = (char) (((uint8_t) input) & ((uint8_t) 0x0F));
//    dest[ 0 ] = (char) ((((uint8_t) input) >> 4) & ((uint8_t) 0x0F));
//
//    for (i = 0; i < 2; i++)
//    {
//        if ( ((uint8_t)dest[ i ]) > ((uint8_t) 9) )
//		{
//            /* A to F */
//            dest[ i ] = (char) (((uint8_t)dest[ i ]) - ((uint8_t) 0x0A) + ((uint8_t)'a'));
//		}
//        else
//		{
//            dest[ i ] += '0';
//		}
//    }
//}




/**
 * @brief Convert an unsigned 32 bit integer into a 4 unsigned char array
 * @param[in]  input unsingned 32 bit to convert
 * @param[out] dest 4 8 bit array
 * @return True (1) if success, false if function error
 */
//AL_BOOL tools_Uint32_2_4Uint8 (uint32_t input, uint8_t dest[4])
//{
//    AL_BOOL returnValue = AL_TRUE;
//
//    dest[0] = input >> 24;
//	dest[1] = input >> 16;
//	dest[2] = input >> 8;
//	dest[3] = input;
//
//    return returnValue;
//}
//
///**
//* @brief Convert a binary data to a hexadecimal string
//* @param[in] binaryData Binary data to convert
//* @param[in] binaryDataSize Binary data size
//* @param[out] hexStringData Hexadecimal data (converted data)
//* @param[in] hexStringDataSize Size available of the hexStringData variable
//* @return True (1) if success, false if function error
//*/
//AL_BOOL tools_Byte_2_HexString (uint8_t * binaryData, uint32_t binaryDataSize, char * hexStringData, uint32_t hexStringDataSize)
//{
//    AL_BOOL		returnValue		= AL_TRUE;
//	uint32_t	i				= 0u;
//
//	/* defensive code check */
//	if ((binaryData == NULL) || (hexStringData == NULL) || (binaryDataSize == 0u) || (hexStringDataSize == 0u))
//	{
//		returnValue = AL_FALSE;
//		goto exit;
//	}
//	if (hexStringDataSize <= (binaryDataSize * 2u))
//	{
//		returnValue = AL_FALSE;
//		goto exit;
//	}
//
//    /* Convert binary to hex string */
//    for (i = 0u; i < binaryDataSize; i++)
//    {
//        tools_Byte2Ascii ( ( char * )& hexStringData[ i * 2 ], (char) (binaryData[ i ]) );
//    }
//
//    /* Add 0x00 at end to form a string */
//    hexStringData[binaryDataSize * 2u] = '\0';
//
//exit:
//	return returnValue;
//}

