//
//  Multicorewar.h
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>
#import "Utility.h"
#import "Bounds.h"
#include "./libmutlicorewar/metal.h"

NS_ASSUME_NONNULL_BEGIN

@interface Multicorewar : NSObject {
    id <MTLDevice>                 _device;
    id <MTLCommandQueue>           _commandQueue;
    id <MTLLibrary>                _library;
    id <MTLComputePipelineState>   _pipelineStateStart;
    id <MTLComputePipelineState>   _pipelineStateRun;
}

@property (strong, nonatomic) Bounds *frontBounds;
@property (strong, nonatomic) Bounds *backBounds;

- (id <MTLDevice>)device;

@end

NS_ASSUME_NONNULL_END
