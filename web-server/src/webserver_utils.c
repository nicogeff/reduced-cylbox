#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <regex.h>

#include "nal_common.h"

#include "webserver_utils.h"

static int webserver_rest_ncasecmp(const char *str1, const char *str2, size_t str1Size)
{
    int diff = 0;

    if (str1Size > 0)
    {
        do
        {
            diff = tolower(*(const unsigned char *) (str1++)) - tolower(*(const unsigned char *) (str2++));
        }
        while (diff == 0 && str1[-1] != '\0' && --str1Size > 0);
    }

    return diff;
}

static int webserver_rest_url_decode(const char *src, int srcSize, char *dst, int dstSize, int is_form_url_encoded)
{
    int i, j, a, b;

#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

    for (i = j = 0; i < srcSize && j < dstSize - 1; i++, j++)
    {
        if (src[i] == '%')
        {
            if (i < srcSize - 2 && isxdigit(*(const unsigned char *) (src + i + 1)) &&
                    isxdigit(*(const unsigned char *) (src + i + 2)))
            {
                a = tolower(*(const unsigned char *) (src + i + 1));
                b = tolower(*(const unsigned char *) (src + i + 2));
                dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
                i += 2;
            }
            else
                return -1;
        }
        else if (is_form_url_encoded && src[i] == '+')
            dst[j] = ' ';
        else
            dst[j] = src[i];
    }

    dst[j] = '\0'; /* Null-terminate the destination */

    return i >= srcSize ? j : -1;
}

/**
 * @brief This function parse an HTTP buffer (body) and return the value 
 * according the key
 * @param[in] buffer Buffer to analyze
 * @param[in] bufferSize Size of buffer
 * @param[in] key Key to find in buffer
 * @param[out] value Value of the key sought
 * @param[in] valueSize Available size of value
 * @return
 * \n 0 Success
 * \n -1 Error on parameters
 * \n -2 Key doesn't exist
 * \n -3 Decoding failed
 */
int webserver_utils_parse_http_variable(const char *buffer,
                                        const size_t bufferSize,
                                        const char *key,
                                        char *value,
                                        size_t valueSize)
{
    const char *p = NULL, *e = NULL, *s = NULL;
    size_t keySize = 0u;
    int returnCode = 0;

    /* Sanity check */
    if (NULL == buffer || NULL == key || NULL == value || 0 == valueSize)
    {
        returnCode = -1;
    }
    else
    {
        keySize = strlen(key);
        e = buffer + bufferSize;
        returnCode = -2;
        value[0] = '\0';

        for (p = buffer; p + keySize < e; p++)
        {
            if ((p == buffer || p[-1] == '&') && p[keySize] == '=' &&
                    !webserver_rest_ncasecmp(key, p, keySize))
            {
                p += keySize + 1;
                s = (const char *) memchr(p, '&', (size_t) (e - p));
                if (s == NULL)
                    s = e;

                returnCode = webserver_rest_url_decode(p, (size_t) (s - p), value, valueSize, 1);
                if (returnCode == -1)
                    returnCode = -3;
                break;
            }
        }
    }

    return returnCode;
}

NAL_BOOL webserver_utils_isAnSsdpDiscoveryRequest(const char *buffer,
                                                 const size_t bufferSize)
{
    NAL_BOOL returnCode = NAL_FALSE;
    int ret = 0;
    regex_t regex = {0};

    /* Prepare regular expression */
    ret = regcomp(&regex, "(^M-SEARCH)", REG_EXTENDED|REG_NOSUB|REG_NEWLINE);
    if (ret)
        goto exit;

    /* Execute regular expression */
    ret = regexec(&regex, buffer, 0, NULL, 0);
    if (!ret)
        returnCode = NAL_TRUE;

    regfree(&regex);

exit:
    return returnCode;
}