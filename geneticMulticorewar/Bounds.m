//
//  Bounds.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Multicorewar.h"

@interface Bounds () {
    __weak Multicorewar *_multicorewar;
}

@end
@implementation Bounds

- (instancetype)init:(Multicorewar *const)multicorewar {
    self = [super init];
    self->_multicorewar = multicorewar;
    
    void *const ptr = malloc(g_information.arenas_memory_size);
    
    if (!ptr)
        [[Utility shared] fatalErrno:@"bounds allocation"];
    [self setBuffer:[[multicorewar device] newBufferWithBytesNoCopy:ptr length:g_information.arenas_memory_size
                                                            options:MTLResourceStorageModeShared deallocator:^(void *ptr, NSUInteger size){
        free(ptr);
    }]];
    bzero([[self buffer] contents], g_information.arenas_memory_size);
    [[Utility shared] messageCPU:"ok: new bounds with size: %lu\nok: bzero\n", g_information.arenas_memory_size];
    return self;
}


@end
