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

#include "tools_parserJson.h"

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

const struct tools_tools_value_struct json_value_none;

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*------------------------ Defines -------------------------------------------*/

typedef unsigned int T_tools_json_uchar;

#define tools_json_whitespace \
   case '\n': ++ state.cur_line;  state.cur_col = 0; \
   case ' ': case '\t': case '\r'

#define tools_json_string_add(b)  \
   do { if (!state.first_pass) string [string_length] = b;  ++ string_length; } while (0);

#define tools_json_line_and_col \
   state.cur_line, state.cur_col

/*------------------------ Types ---------------------------------------------*/

typedef struct
{
    unsigned long used_memory;

    unsigned int uint_max;
    unsigned long ulong_max;

    T_tools_json_settings settings;
    int first_pass;

    const T_tools_json_char * ptr;
    unsigned int cur_line, cur_col;

} T_tools_json_state;

static const long
flag_next = 1 << 0,
        flag_reproc = 1 << 1,
        flag_need_comma = 1 << 2,
        flag_seek_value = 1 << 3,
        flag_escaped = 1 << 4,
        flag_string = 1 << 5,
        flag_need_colon = 1 << 6,
        flag_done = 1 << 7,
        flag_num_negative = 1 << 8,
        flag_num_zero = 1 << 9,
        flag_num_e = 1 << 10,
        flag_num_e_got_sign = 1 << 11,
        flag_num_e_negative = 1 << 12,
        flag_line_comment = 1 << 13,
        flag_block_comment = 1 << 14;

/*------------------------ Private functions (for parsing) -------------------*/

static unsigned char tools_json_hex_value(T_tools_json_char c)
{
    if (isdigit(c))
        return c - '0';

    switch (c)
    {
    case 'a': case 'A': return 0x0A;
    case 'b': case 'B': return 0x0B;
    case 'c': case 'C': return 0x0C;
    case 'd': case 'D': return 0x0D;
    case 'e': case 'E': return 0x0E;
    case 'f': case 'F': return 0x0F;
    default: return 0xFF;
    }
}

static void * tools_json_default_alloc(size_t size, int zero, void * user_data)
{
    return zero ? calloc(1, size) : malloc(size);
}

static void tools_json_default_free(void * ptr, void * user_data)
{
    if (ptr != NULL)
    {
        free(ptr);
        /*ptr = NULL; static analysis warning, no effect*/
    }
}

static void * tools_json_alloc(T_tools_json_state * state, unsigned long size, int zero)
{
    if ((state->ulong_max - state->used_memory) < size)
        return 0;

    if (state->settings.max_memory
            && (state->used_memory += size) > state->settings.max_memory)
    {
        return 0;
    }

    return state->settings.mem_alloc(size, zero, state->settings.user_data);
}

static int tools_json_new_value(T_tools_json_state * state,
                              T_tools_json_value ** top, T_tools_json_value ** root, T_tools_json_value ** alloc,
                              T_tools_json_type type)
{
    T_tools_json_value * value;
    int values_size;

    if (!state->first_pass)
    {
        value = *top = *alloc;
        *alloc = (*alloc)->_reserved.next_alloc;

        if (!*root)
            *root = value;

        switch (value->type)
        {
        case json_array:

            if (value->u.array.length == 0)
                break;

            if (!(value->u.array.values = (T_tools_json_value **) tools_json_alloc
                    (state, value->u.array.length * sizeof (T_tools_json_value *), 0)))
            {
                return 0;
            }

            value->u.array.length = 0;
            break;

        case json_object:

            if (value->u.object.length == 0)
                break;

            values_size = sizeof (*value->u.object.values) * value->u.object.length;

            if (!(value->u.object.values = (T_tools_json_object_entry *) tools_json_alloc
                    (state, values_size + ((unsigned long) value->u.object.values), 0)))
            {
                return 0;
            }

            value->_reserved.object_mem = (*(char **) &value->u.object.values) + values_size;

            value->u.object.length = 0;
            break;

        case json_string:

            if (!(value->u.string.ptr = (T_tools_json_char *) tools_json_alloc
                    (state, (value->u.string.length + 1) * sizeof (T_tools_json_char), 0)))
            {
                return 0;
            }

            value->u.string.length = 0;
            break;

        default:
            break;
        };

        return 1;
    }

    if (!(value = (T_tools_json_value *) tools_json_alloc
            (state, sizeof (T_tools_json_value) + state->settings.value_extra, 1)))
    {
        return 0;
    }

    if (!*root)
        *root = value;

    value->type = type;
    value->parent = *top;

#ifdef JSON_TRACK_SOURCE
    value->line = state->cur_line;
    value->col = state->cur_col;
#endif

    if (*alloc)
        (*alloc)->_reserved.next_alloc = value;

    *alloc = *top = value;

    return 1;
}

/*------------------------ Public functions (for parsing) --------------------*/

/**
 * This function take a JSON buffer in entry and return a structure with the JSON data
 * @remark This function is the extended version (with more parameter)
 * @param settings
 * @param json
 * @param length
 * @param error_buf
 * @return 
 */
T_tools_json_value * tools_json_parse_ex(T_tools_json_settings * settings,
                                     const T_tools_json_char * json,
                                     size_t length,
                                     char * error_buf)
{
    T_tools_json_char error [json_error_max];
    const T_tools_json_char * end;
    T_tools_json_value * top, * root, * alloc = 0;
    T_tools_json_state state = {0};
    long flags;
    long num_digits = 0, num_e = 0;
    json_int_t num_fraction = 0;

    /* Skip UTF-8 BOM
     */
    if (length >= 3 && ((unsigned char) json [0]) == 0xEF
            && ((unsigned char) json [1]) == 0xBB
            && ((unsigned char) json [2]) == 0xBF)
    {
        json += 3;
        length -= 3;
    }

    error[0] = '\0';
    end = (json + length);

    memcpy(&state.settings, settings, sizeof (T_tools_json_settings));

    if (!state.settings.mem_alloc)
        state.settings.mem_alloc = tools_json_default_alloc;

    if (!state.settings.mem_free)
        state.settings.mem_free = tools_json_default_free;

    memset(&state.uint_max, 0xFF, sizeof (state.uint_max));
    memset(&state.ulong_max, 0xFF, sizeof (state.ulong_max));

    state.uint_max -= 8; /* limit of how much can be added before next check */
    state.ulong_max -= 8;

    for (state.first_pass = 1; state.first_pass >= 0; --state.first_pass)
    {
        T_tools_json_uchar uchar;
        unsigned char uc_b1, uc_b2, uc_b3, uc_b4;
        T_tools_json_char * string = 0;
        unsigned int string_length = 0;

        top = root = 0;
        flags = flag_seek_value;

        state.cur_line = 1;

        for (state.ptr = json;; ++state.ptr)
        {
            T_tools_json_char b = (state.ptr == end ? 0 : *state.ptr);

            if (flags & flag_string)
            {
                if (!b)
                {
                    sprintf(error, "Unexpected EOF in string (at %u:%u)", tools_json_line_and_col);
                    goto e_failed;
                }

                if (string_length > state.uint_max)
                    goto e_overflow;

                if (flags & flag_escaped)
                {
                    flags &= ~flag_escaped;

                    switch (b)
                    {
                    case 'b': tools_json_string_add('\b');
                        break;
                    case 'f': tools_json_string_add('\f');
                        break;
                    case 'n': tools_json_string_add('\n');
                        break;
                    case 'r': tools_json_string_add('\r');
                        break;
                    case 't': tools_json_string_add('\t');
                        break;
                    case 'u':

                        if (end - state.ptr <= 4 ||
                                (uc_b1 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                (uc_b2 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                (uc_b3 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                (uc_b4 = tools_json_hex_value(*++state.ptr)) == 0xFF)
                        {
                            sprintf(error, "Invalid character value `%c` (at %u:%u)", b, tools_json_line_and_col);
                            goto e_failed;
                        }

                        uc_b1 = (uc_b1 << 4) | uc_b2;
                        uc_b2 = (uc_b3 << 4) | uc_b4;
                        uchar = (uc_b1 << 8) | uc_b2;

                        if ((uchar & 0xF800) == 0xD800)
                        {
                            T_tools_json_uchar uchar2;

                            if (end - state.ptr <= 6 || (*++state.ptr) != '\\' || (*++state.ptr) != 'u' ||
                                    (uc_b1 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                    (uc_b2 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                    (uc_b3 = tools_json_hex_value(*++state.ptr)) == 0xFF ||
                                    (uc_b4 = tools_json_hex_value(*++state.ptr)) == 0xFF)
                            {
                                sprintf(error, "Invalid character value `%c` (at %u:%u)", b, tools_json_line_and_col);
                                goto e_failed;
                            }

                            uc_b1 = (uc_b1 << 4) | uc_b2;
                            uc_b2 = (uc_b3 << 4) | uc_b4;
                            uchar2 = (uc_b1 << 8) | uc_b2;

                            uchar = 0x010000 | ((uchar & 0x3FF) << 10) | (uchar2 & 0x3FF);
                        }

                        if (sizeof (T_tools_json_char) >= sizeof (T_tools_json_uchar) || (uchar <= 0x7F))
                        {
                            tools_json_string_add((T_tools_json_char) uchar);
                            break;
                        }

                        if (uchar <= 0x7FF)
                        {
                            if (state.first_pass)
                                string_length += 2;
                            else
                            {
                                string [string_length++] = 0xC0 | (uchar >> 6);
                                string [string_length++] = 0x80 | (uchar & 0x3F);
                            }

                            break;
                        }

                        if (uchar <= 0xFFFF)
                        {
                            if (state.first_pass)
                                string_length += 3;
                            else
                            {
                                string [string_length++] = 0xE0 | (uchar >> 12);
                                string [string_length++] = 0x80 | ((uchar >> 6) & 0x3F);
                                string [string_length++] = 0x80 | (uchar & 0x3F);
                            }

                            break;
                        }

                        if (state.first_pass)
                            string_length += 4;
                        else
                        {
                            string [string_length++] = 0xF0 | (uchar >> 18);
                            string [string_length++] = 0x80 | ((uchar >> 12) & 0x3F);
                            string [string_length++] = 0x80 | ((uchar >> 6) & 0x3F);
                            string [string_length++] = 0x80 | (uchar & 0x3F);
                        }

                        break;

                    default:
                        tools_json_string_add(b);
                    };

                    continue;
                }

                if (b == '\\')
                {
                    flags |= flag_escaped;
                    continue;
                }

                if (b == '"')
                {
                    if (!state.first_pass)
                        string [string_length] = 0;

                    flags &= ~flag_string;
                    string = 0;

                    switch (top->type)
                    {
                    case json_string:

                        top->u.string.length = string_length;
                        flags |= flag_next;

                        break;

                    case json_object:

                        if (state.first_pass)
                            (*(T_tools_json_char **) & top->u.object.values) += string_length + 1;
                        else
                        {
                            top->u.object.values [top->u.object.length].name
                                    = (T_tools_json_char *) top->_reserved.object_mem;

                            top->u.object.values [top->u.object.length].name_length
                                    = string_length;

                            (*(T_tools_json_char **) &top->_reserved.object_mem) += string_length + 1;
                        }

                        flags |= flag_seek_value | flag_need_colon;
                        continue;

                    default:
                        break;
                    };
                }
                else
                {
                    tools_json_string_add(b);
                    continue;
                }
            }

            if (state.settings.settings & tools_json_enable_comments)
            {
                if (flags & (flag_line_comment | flag_block_comment))
                {
                    if (flags & flag_line_comment)
                    {
                        if (b == '\r' || b == '\n' || !b)
                        {
                            flags &= ~flag_line_comment;
                            --state.ptr; /* so null can be reproc'd */
                        }

                        continue;
                    }

                    if (flags & flag_block_comment)
                    {
                        if (!b)
                        {
                            sprintf(error, "%u:%u: Unexpected EOF in block comment", tools_json_line_and_col);
                            goto e_failed;
                        }

                        if (b == '*' && state.ptr < (end - 1) && state.ptr [1] == '/')
                        {
                            flags &= ~flag_block_comment;
                            ++state.ptr; /* skip closing sequence */
                        }

                        continue;
                    }
                }
                else if (b == '/')
                {
                    if (!(flags & (flag_seek_value | flag_done)) && top->type != json_object)
                    {
                        sprintf(error, "%u:%u: Comment not allowed here", tools_json_line_and_col);
                        goto e_failed;
                    }

                    if (++state.ptr == end)
                    {
                        sprintf(error, "%u:%u: EOF unexpected", tools_json_line_and_col);
                        goto e_failed;
                    }

                    switch (b = *state.ptr)
                    {
                    case '/':
                        flags |= flag_line_comment;
                        continue;

                    case '*':
                        flags |= flag_block_comment;
                        continue;

                    default:
                        sprintf(error, "%u:%u: Unexpected `%c` in comment opening sequence", tools_json_line_and_col, b);
                        goto e_failed;
                    };
                }
            }

            if (flags & flag_done)
            {
                if (!b)
                    break;

                switch (b)
                {
tools_json_whitespace:
                    continue;

                default:

                    sprintf(error, "%u:%u: Trailing garbage: `%c`",
                            state.cur_line, state.cur_col, b);

                    goto e_failed;
                };
            }

            if (flags & flag_seek_value)
            {
                switch (b)
                {
tools_json_whitespace:
                    continue;

                case ']':

                    if (top && top->type == json_array)
                        flags = (flags & ~(flag_need_comma | flag_seek_value)) | flag_next;
                    else
                    {
                        sprintf(error, "%u:%u: Unexpected ]", tools_json_line_and_col);
                        goto e_failed;
                    }

                    break;

                default:

                    if (flags & flag_need_comma)
                    {
                        if (b == ',')
                        {
                            flags &= ~flag_need_comma;
                            continue;
                        }
                        else
                        {
                            sprintf(error, "%u:%u: Expected , before %c",
                                    state.cur_line, state.cur_col, b);

                            goto e_failed;
                        }
                    }

                    if (flags & flag_need_colon)
                    {
                        if (b == ':')
                        {
                            flags &= ~flag_need_colon;
                            continue;
                        }
                        else
                        {
                            sprintf(error, "%u:%u: Expected : before %c",
                                    state.cur_line, state.cur_col, b);

                            goto e_failed;
                        }
                    }

                    flags &= ~flag_seek_value;

                    switch (b)
                    {
                    case '{':

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_object))
                            goto e_alloc_failure;

                        continue;

                    case '[':

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_array))
                            goto e_alloc_failure;

                        flags |= flag_seek_value;
                        continue;

                    case '"':

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_string))
                            goto e_alloc_failure;

                        flags |= flag_string;

                        string = top->u.string.ptr;
                        string_length = 0;

                        continue;

                    case 't':

                        if ((end - state.ptr) < 3 || *(++state.ptr) != 'r' ||
                                *(++state.ptr) != 'u' || *(++state.ptr) != 'e')
                        {
                            goto e_unknown_value;
                        }

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_boolean))
                            goto e_alloc_failure;

                        top->u.boolean = 1;

                        flags |= flag_next;
                        break;

                    case 'f':

                        if ((end - state.ptr) < 4 || *(++state.ptr) != 'a' ||
                                *(++state.ptr) != 'l' || *(++state.ptr) != 's' ||
                                *(++state.ptr) != 'e')
                        {
                            goto e_unknown_value;
                        }

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_boolean))
                            goto e_alloc_failure;

                        flags |= flag_next;
                        break;

                    case 'n':

                        if ((end - state.ptr) < 3 || *(++state.ptr) != 'u' ||
                                *(++state.ptr) != 'l' || *(++state.ptr) != 'l')
                        {
                            goto e_unknown_value;
                        }

                        if (!tools_json_new_value(&state, &top, &root, &alloc, json_null))
                            goto e_alloc_failure;

                        flags |= flag_next;
                        break;

                    default:

                        if (isdigit(b) || b == '-')
                        {
                            if (!tools_json_new_value(&state, &top, &root, &alloc, json_integer))
                                goto e_alloc_failure;

                            if (!state.first_pass)
                            {
                                while (isdigit(b) || b == '+' || b == '-'
                                        || b == 'e' || b == 'E' || b == '.')
                                {
                                    if ((++state.ptr) == end)
                                    {
                                        b = 0;
                                        break;
                                    }

                                    b = *state.ptr;
                                }

                                flags |= flag_next | flag_reproc;
                                break;
                            }

                            flags &= ~(flag_num_negative | flag_num_e |
                                    flag_num_e_got_sign | flag_num_e_negative |
                                    flag_num_zero);

                            num_digits = 0;
                            num_fraction = 0;
                            num_e = 0;

                            if (b != '-')
                            {
                                flags |= flag_reproc;
                                break;
                            }

                            flags |= flag_num_negative;
                            continue;
                        }
                        else
                        {
                            sprintf(error, "%u:%u: Unexpected %c when seeking value", tools_json_line_and_col, b);
                            goto e_failed;
                        }
                    };
                };
            }
            else
            {
                switch (top->type)
                {
                case json_object:

                    switch (b)
                    {
tools_json_whitespace:
                        continue;

                    case '"':

                        if (flags & flag_need_comma)
                        {
                            sprintf(error, "%u:%u: Expected , before \"", tools_json_line_and_col);
                            goto e_failed;
                        }

                        flags |= flag_string;

                        string = (T_tools_json_char *) top->_reserved.object_mem;
                        string_length = 0;

                        break;

                    case '}':

                        flags = (flags & ~flag_need_comma) | flag_next;
                        break;

                    case ',':

                        if (flags & flag_need_comma)
                        {
                            flags &= ~flag_need_comma;
                            break;
                        }

                    default:
                        sprintf(error, "%u:%u: Unexpected `%c` in object", tools_json_line_and_col, b);
                        goto e_failed;
                    };

                    break;

                case json_integer:
                case json_double:

                    if (isdigit(b))
                    {
                        ++num_digits;

                        if (top->type == json_integer || flags & flag_num_e)
                        {
                            if (!(flags & flag_num_e))
                            {
                                if (flags & flag_num_zero)
                                {
                                    sprintf(error, "%u:%u: Unexpected `0` before `%c`", tools_json_line_and_col, b);
                                    goto e_failed;
                                }

                                if (num_digits == 1 && b == '0')
                                    flags |= flag_num_zero;
                            }
                            else
                            {
                                flags |= flag_num_e_got_sign;
                                num_e = (num_e * 10) + (b - '0');
                                continue;
                            }

                            top->u.integer = (top->u.integer * 10) + (b - '0');
                            continue;
                        }

                        num_fraction = (num_fraction * 10) + (b - '0');
                        continue;
                    }

                    if (b == '+' || b == '-')
                    {
                        if ((flags & flag_num_e) && !(flags & flag_num_e_got_sign))
                        {
                            flags |= flag_num_e_got_sign;

                            if (b == '-')
                                flags |= flag_num_e_negative;

                            continue;
                        }
                    }
                    else if (b == '.' && top->type == json_integer)
                    {
                        if (!num_digits)
                        {
                            sprintf(error, "%u:%u: Expected digit before `.`", tools_json_line_and_col);
                            goto e_failed;
                        }

                        top->type = json_double;
                        top->u.dbl = (double) top->u.integer;

                        num_digits = 0;
                        continue;
                    }

                    if (!(flags & flag_num_e))
                    {
                        if (top->type == json_double)
                        {
                            if (!num_digits)
                            {
                                sprintf(error, "%u:%u: Expected digit after `.`", tools_json_line_and_col);
                                goto e_failed;
                            }

                            top->u.dbl += ((double) num_fraction) / (pow(10.0, (double) num_digits));
                        }

                        if (b == 'e' || b == 'E')
                        {
                            flags |= flag_num_e;

                            if (top->type == json_integer)
                            {
                                top->type = json_double;
                                top->u.dbl = (double) top->u.integer;
                            }

                            num_digits = 0;
                            flags &= ~flag_num_zero;

                            continue;
                        }
                    }
                    else
                    {
                        if (!num_digits)
                        {
                            sprintf(error, "%u:%u: Expected digit after `e`", tools_json_line_and_col);
                            goto e_failed;
                        }

                        top->u.dbl *= pow(10.0, (double)
                                          (flags & flag_num_e_negative ? - num_e : num_e));
                    }

                    if (flags & flag_num_negative)
                    {
                        if (top->type == json_integer)
                            top->u.integer = -top->u.integer;
                        else
                            top->u.dbl = -top->u.dbl;
                    }

                    flags |= flag_next | flag_reproc;
                    break;

                default:
                    break;
                };
            }

            if (flags & flag_reproc)
            {
                flags &= ~flag_reproc;
                --state.ptr;
            }

            if (flags & flag_next)
            {
                flags = (flags & ~flag_next) | flag_need_comma;

                if (!top->parent)
                {
                    /* root value done */

                    flags |= flag_done;
                    continue;
                }

                if (top->parent->type == json_array)
                    flags |= flag_seek_value;

                if (!state.first_pass)
                {
                    T_tools_json_value * parent = top->parent;

                    switch (parent->type)
                    {
                    case json_object:

                        parent->u.object.values
                                [parent->u.object.length].value = top;

                        break;

                    case json_array:

                        parent->u.array.values
                                [parent->u.array.length] = top;

                        break;

                    default:
                        break;
                    };
                }

                if ((++top->parent->u.array.length) > state.uint_max)
                    goto e_overflow;

                top = top->parent;

                continue;
            }
        }

        alloc = root;
    }

    return root;

e_unknown_value:

    sprintf(error, "%u:%u: Unknown value", tools_json_line_and_col);
    goto e_failed;

e_alloc_failure:

    strcpy(error, "Memory allocation failure");
    goto e_failed;

e_overflow:

    sprintf(error, "%u:%u: Too long (caught overflow)", tools_json_line_and_col);
    goto e_failed;

e_failed:

    if (error_buf)
    {
        if (*error)
            strcpy(error_buf, error);
        else
            strcpy(error_buf, "Unknown error");
    }

    if (state.first_pass)
        alloc = root;

    while (alloc)
    {
        top = alloc->_reserved.next_alloc;
        state.settings.mem_free(alloc, state.settings.user_data);
        alloc = top;
    }

    if (!state.first_pass)
        tools_json_value_free_ex(&state.settings, root);

    return 0;
}

/**
 * This function take a JSON buffer in entry and return a structure with the JSON data
 * \brief Parse a JSON string to a C structure
 * @param json JSON string
 * @param length Size of the JSON string
 * @return A C structure with each element of the JSON
 */
T_tools_json_value * tools_json_parse(const T_tools_json_char * json, size_t length)
{
    T_tools_json_settings settings = {0};
    return tools_json_parse_ex(&settings, json, length, 0);
}

/**
 * \brief Free memory allocate for value
 * @param settings
 * @param value
 */
void tools_json_value_free_ex(T_tools_json_settings * settings, T_tools_json_value * value)
{
    T_tools_json_value * cur_value;

    if (!value)
        return;

    value->parent = 0;

    while (value)
    {
        switch (value->type)
        {
        case json_array:

            if (!value->u.array.length)
            {
                settings->mem_free(value->u.array.values, settings->user_data);
                break;
            }

            value = value->u.array.values [--value->u.array.length];
            continue;

        case json_object:

            if (!value->u.object.length)
            {
                settings->mem_free(value->u.object.values, settings->user_data);
                break;
            }

            value = value->u.object.values [--value->u.object.length].value;
            continue;

        case json_string:

            settings->mem_free(value->u.string.ptr, settings->user_data);
            break;

        default:
            break;
        };

        cur_value = value;
        value = value->parent;
        settings->mem_free(cur_value, settings->user_data);
    }
}

/**
 * \brief Free memory allocate for value
 * @remark This function must be called after a successful parsing
 * @param value A C structure of the JSON
 */
void tools_json_value_free(T_tools_json_value * value)
{
    T_tools_json_settings settings = {0};

    if (value != NULL)
    {
        settings.mem_free = tools_json_default_free;
        tools_json_value_free_ex(&settings, value);
    }
}

