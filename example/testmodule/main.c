// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
//
//  Copyright (C) 2019-2023 checkra1n team
//  This file is part of pongoOS.
//
#include <pongo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "rt.h"

void (*existing_preboot_hook)();

void m_preboot_hook()
{
    puts("Called pre-boot hook");
    /* Do patches here */
    if (existing_preboot_hook != NULL)
    	existing_preboot_hook();
    return;
}

void hello() {
    puts("Hello world! And now continuing to XNU.");
    queue_rx_string("bootx\n"); // Adding boot XNU command to buffer
}

void fb_test() {
    memset(gFramebuffer, 5, gHeight * gRowPixels * sizeof(uint32_t));
}

void rt_test() {
    main();
}

void module_entry() {
    existing_preboot_hook = preboot_hook;
    preboot_hook = m_preboot_hook;
    command_register("test", "A test command to execute test_module.", hello);
    command_register("fbtest", "A framebuffer test.", fb_test);
    command_register("rttest", "A test of a basic raytracer.", rt_test);
}

char* module_name = "test_module";

struct pongo_exports exported_symbols[] = {
    {.name = 0, .value = 0}
};
