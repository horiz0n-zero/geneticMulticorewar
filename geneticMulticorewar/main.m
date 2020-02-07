//
//  main.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Multicorewar.h"
#import "Utility.h"

static Multicorewar                      *_multicorewar = nil;

struct s_information                     g_information =
{
    .arenas_memory_size = sizeof(struct vm_arena) * VM_TOTAL,
    .opcode_info_memory_size = sizeof(struct vm_opcode_info),
    .process_memory_size = sizeof(struct vm_process),
    .champion_memory_size = sizeof(struct vm_champion),
    .arena_memory_size = sizeof(struct vm_arena),
    .flags = 0,
    .opponents_directory = ".",
    .sources_directory = ".",
    .output_directory = DEFAULT_OUTPUT_DIRECTORY
};
static const struct s_argument           g_arguments[256] =
{
    ['e'] = {"enemies", FLAGS_E, 1, &g_information.opponents_directory},
    ['s'] = {"sources", FLAGS_S, 1, &g_information.sources_directory},
    ['o'] = {"output-directory", FLAGS_O, 1, &g_information.output_directory}
};
static const char                        *g_usages[] =
{
    "usage:\n",
    "-e --enemies <directory>\n",
    "-s --sources <directory>\n",
    "-o --output-directory <directory>\n"
};

int main(int argc, char * argv[]) {
    @autoreleasepool {
        char *error = nil;
        
        arguments_get(argv + 1, g_arguments, &g_information.flags, &error);
        if (error) {
            int index = 0;
            
            while (index < (sizeof(g_usages) / sizeof(g_usages[0]))) {
                ft_dprintf(STDERR_FILENO, "%s", g_usages[index]);
                ++index;
            }
            ft_dprintf(STDERR_FILENO, "%s: %s\n", *argv, error);
            exit(EXIT_FAILURE);
        }
        [Utility setShared:[[Utility alloc] init]];
        ft_printf("number of threads per group : %d\n", THREADS_TOTAL);
        ft_printf("number of groups :            %d\n", GROUPS_TOTAL);
        ft_printf("number of vm per bound :      %d\n\n", VM_TOTAL);
    
        ft_printf("sizeof %-20s   %s\n", "vm_opcode_info", [[Utility shared] bytesString:g_information.opcode_info_memory_size]);
        ft_printf("sizeof %-20s   %s\n", "vm_process", [[Utility shared] bytesString:g_information.process_memory_size]);
        ft_printf("sizeof %-20s   %s\n", "vm_champion", [[Utility shared] bytesString:g_information.champion_memory_size]);
        ft_printf("sizeof %-20s   %s\n\n", "vm_arena", [[Utility shared] bytesString:g_information.arena_memory_size]);
        
        ft_printf("sizeof %-20s   %s\n", "all vm", [[Utility shared] bytesString:g_information.arenas_memory_size]);
        _multicorewar = [[Multicorewar alloc] init];
        ft_printf("enter in loop\n");
        while (1) {
            continue ;
        }
    }
    return 0;
}
