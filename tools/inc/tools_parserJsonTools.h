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

#ifndef TOOLS_PARSERJSONTOOLS_H
#define TOOLS_PARSERJSONTOOLS_H

#include "tools_parserJson.h"


#ifdef __cplusplus
extern "C" {
#endif

/*------------------------ Types ---------------------------------------------*/

typedef union
   {
      int boolean;
      int64_t integer;
      double dbl;

      struct
      {
         unsigned int length;
         char * ptr; /* null terminated */
      } string;

      struct
      {
         unsigned int length;
         T_tools_json_object_entry * values;
      } object;

      struct
      {
         unsigned int length;
         struct tools_tools_value_struct ** values;
      } array;

   } T_tools_union_data;

/*------------------------ Functions -----------------------------------------*/

    /**
     * \brief This function can be use to print the JSON tree
     * @param value JSON string parsed (C structure)
     * @param depth Depth of tree to print 
     */
    void tools_json_print_tree(T_tools_json_value* value, int depth);

    void tools_json_grab_data(
            const T_tools_json_value* value,
            const char * key,
            T_tools_json_type * dataType,
            T_tools_union_data * dataValue);

#ifdef __cplusplus
}
#endif

#endif /* TOOLS_PARSERJSONTOOLS_H */

