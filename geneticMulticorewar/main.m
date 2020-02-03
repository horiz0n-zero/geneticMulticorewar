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

static Utility                           *_utility = nil;
static Multicorewar                      *_multicorewar = nil;

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        _utility = [[Utility alloc] init];
        ft_printf("number of threads per group : %d\n", THREADS_TOTAL);
        ft_printf("number of groups :            %d\n", GROUPS_TOTAL);
        ft_printf("number of vm per bound :      %d\n\n", VM_TOTAL);
    
        ft_printf("sizeof %-20s   %s\n", "vm_opcode_info", [_utility bytesString:sizeof(struct vm_opcode_info)]);
        ft_printf("sizeof %-20s   %s\n", "vm_process", [_utility bytesString:sizeof(struct vm_process)]);
        ft_printf("sizeof %-20s   %s\n", "vm_champion", [_utility bytesString:sizeof(struct vm_champion)]);
        ft_printf("sizeof %-20s   %s\n\n", "vm_arena", [_utility bytesString:sizeof(struct vm_arena)]);
        
        ft_printf("sizeof %-20s   %s\n", "all vm", [_utility bytesString:sizeof(struct vm_arena) * VM_TOTAL]);
        _multicorewar = [[Multicorewar alloc] init];
        
    }
    return 0;
}
