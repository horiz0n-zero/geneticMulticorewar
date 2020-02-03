//
//  Multicorewar.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Multicorewar.h"

@implementation Multicorewar

- (instancetype)init
{
    self = [super init];
    
    NSError *error = nil;
    
    self->_device = MTLCreateSystemDefaultDevice();
    self->_commandQueue = [self->_device newCommandQueue];
    
    self->_library = [self->_device newDefaultLibrary];
    self->_pipelineStateStart = [self->_device newComputePipelineStateWithFunction:[self->_library newFunctionWithName:@"vm_start"]
                                                                             error:&error];
    self->_pipelineStateRun = [self->_device newComputePipelineStateWithFunction:[self->_library newFunctionWithName:@"vm_run"]
                                                                           error:&error];
    return self;
}
- (id<MTLDevice>)device {
    return self->_device;
}

@end
