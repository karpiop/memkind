/*
 * Copyright (C) 2017 Intel Corporation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice(s),
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice(s),
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <stdio.h>
#include <fcntl.h>

class ProcStat
{
public:
    //Note: this function is not thread-safe.
    bool get_stat(const char* field_name, char* value)
    {
        int file = open("/proc/self/status", O_RDONLY);
        if(file == -1) {
            return false;
        }

        read(file, buff, sizeof(buff));
        char* pos = strstr(buff, field_name);
        if(pos) {
            sscanf(pos, "%64[a-zA-Z_0-9()]: %s", current_entry_name, value);
        }

        close(file);
        return pos;
    }

    size_t get_virtual_memory_size_bytes()
    {
        get_stat("VmSize", str_value);
        return strtol(str_value, NULL, 10) * 1024;
    }

    size_t get_physical_memory_size_bytes()
    {
        get_stat("VmRSS", str_value);
        return strtol(str_value, NULL, 10) * 1024;
    }
private:
    /* We are avoiding to allocate local buffers,
     * since it can produce noise in memory footprint tests.
     */
    char buff[1024];
    char current_entry_name[1024];
    char str_value[1024];
};

