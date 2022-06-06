/* vim: set et ts=3 sw=3 sts=3 ft=c:
 *
 * Copyright (C) 2012, 2013, 2014 James McLaughlin et al.  All rights reserved.
 * https://github.com/udp/json-parser
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * The names of functions has been change to respect the name coding of the module
 */

#include <stdio.h>
#include <string.h>

#include "tools_parserJson.h"
#include "tools_parserJsonTools.h"

/*------------------------ Private functions (for printing) ------------------*/

/**
 * This function is used in case of JSON data printing
 * @detail This function print a number a space according the depth
 * @param depth Contains the level of depth un the JSON tree
 */
static void tools_json_print_depth_shift(int depth)
{
    int j;
    for (j = 0; j < depth; j++)
    {
        printf(" ");
    }
}

/**
 * This function is used in case of JSON struct printing
 * @detail This function manage the object type ( = key in the JSON)
 * @param value Constains the JSON data
 * @param depth Contains the level of depth un the JSON tree
 */
static void tools_json_process_object(T_tools_json_value* value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++)
    {
        tools_json_print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        tools_json_print_tree(value->u.object.values[x].value, depth + 1);
    }
}

/**
 * This function is used in case of JSON struct printing
 * @detail This function manage the array type
 * @param value Constains the JSON data
 * @param depth Contains the level of depth un the JSON tree
 */
static void tools_json_process_array(T_tools_json_value* value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++)
    {
        tools_json_print_tree(value->u.array.values[x], depth);
    }
}

/*------------------------ Private functions (for searching) -----------------*/

static void tools_json_grab_data_ex(
                                  const T_tools_json_value* value,
                                  const char * key,
                                  T_tools_json_type * dataType,
                                  T_tools_union_data * dataValue)
{
    int length = 0;
    int x = 0;

    length = value->u.object.length;

    for (x = 0; x < length; x++)
    {
        if (strcmp(value->u.object.values[x].name, key) == 0)
        {
            switch (value->u.object.values[x].value->type)
            {
            case json_array:
                *dataType = value->u.object.values[x].value->type;
                dataValue->array.values = value->u.object.values[x].value->u.array.values;
                dataValue->array.length = value->u.object.values[x].value->u.array.length;
                break;
            case json_integer:
                *dataType = value->u.object.values[x].value->type;
                dataValue->integer = value->u.object.values[x].value->u.integer;
                break;
            case json_double:
                *dataType = value->u.object.values[x].value->type;
                dataValue->dbl = value->u.object.values[x].value->u.dbl;
                break;
            case json_string:
                *dataType = value->u.object.values[x].value->type;
                dataValue->string.ptr = value->u.object.values[x].value->u.string.ptr;
                dataValue->string.length = value->u.object.values[x].value->u.string.length;
                break;
            case json_boolean:
                *dataType = value->u.object.values[x].value->type;
                dataValue->boolean = value->u.object.values[x].value->u.boolean;
                break;
            case json_none:
                break;
            case json_object:
                *dataType = value->u.object.values[x].value->type;
                dataValue->object.values = value->u.object.values[x].value->u.object.values;
                dataValue->object.length = value->u.object.values[x].value->u.object.length;
                break;
            default:
                printf("you should not be here!\n");
                break;
            }
            return;
        }

        /* Check inside an another level */
        if ((value->u.object.values[x].value->type == json_object) && (value->u.object.values[x].value->u.object.length > 0))
            tools_json_grab_data_ex(value->u.object.values[x].value, key, dataType, dataValue);
    }
}

/*------------------------ Public functions (for printing) -------------------*/

/**
 * \brief This function can be use to print the JSON tree
 * @param value JSON string parsed (C structure)
 * @param depth Depth of tree to print 
 */
void tools_json_print_tree(T_tools_json_value* value, int depth)
{
    if (value == NULL)
    {
        return;
    }
    if (value->type != json_object)
    {
        tools_json_print_depth_shift(depth);
    }
    switch (value->type)
    {
    case json_none:
        printf("none\n");
        break;
    case json_object:
        tools_json_process_object(value, depth + 1);
        break;
    case json_array:
        tools_json_process_array(value, depth + 1);
        break;
    case json_integer:
        printf("int: %10" PRId64 "\n", value->u.integer);
        break;
    case json_double:
        printf("double: %f\n", value->u.dbl);
        break;
    case json_string:
        printf("string: %s\n", value->u.string.ptr);
        break;
    case json_boolean:
        printf("bool: %d\n", value->u.boolean);
        break;
    default:
        printf("you should not be here!\n");
        break;
    }
}

/*------------------------ Public functions (for searching) ------------------*/

/**
 * Get data in the parsing tree according the parameter key
 * @warning Nothing is allocate, it's just using pointer of the parsing tree
 * @warning The caller must deallocate the parsing tree with the function tools_json_value_free()
 * @param value Parsing tree
 * @param key Key to find
 * @param dataType Type of the value found (json_null is returned in case of error or if the key was not found)
 * @param dataValue Value found (Union is fill according the type)
 */
void tools_json_grab_data(const T_tools_json_value* value,
                        const char * key,
                        T_tools_json_type * dataType,
                        T_tools_union_data * dataValue)
{
    /* Sanity check */
    if (value == NULL || key == NULL || dataType == NULL || dataValue == NULL)
    {
        if (dataType != NULL)
            *dataType = json_null;
        if (dataValue != NULL)
            memset(dataValue, 0, sizeof (T_tools_union_data));
        printf("Check param error!\n");
        return;
    }

    /* Initialize out param. to default value */
    *dataType = json_null;
    memset(dataValue, 0, sizeof (T_tools_union_data));

    tools_json_grab_data_ex(value, key, dataType, dataValue);

    return;
}

