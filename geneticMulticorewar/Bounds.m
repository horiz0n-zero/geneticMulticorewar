//
//  Bounds.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Bounds.h"
#import "Multicorewar.h"

@interface Bounds () {
    id <MTLBuffer>       _vm_buffer;
}

@end
@implementation Bounds

- (instancetype)init:(Multicorewar *const)multicorewar {
    self = [super init];
    
    self->_vm_buffer = [[multicorewar device] newBufferWithLength:sizeof(struct vm_arena) * VM_TOTAL
                                                          options:MTLResourceStorageModeShared];
    
    return self;
}

@end
