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
#include "./libcorewar/includes/libcorewar.h"
#include "./libmutlicorewar/metal.h"

NS_ASSUME_NONNULL_BEGIN

struct                          s_information {
    NSInteger                   arenas_memory_size;
    NSInteger                   opcode_info_memory_size;
    NSInteger                   process_memory_size;
    NSInteger                   champion_memory_size;
    NSInteger                   arena_memory_size;
    
    int                         flags;
    # define FLAGS_E 1 << 0
    # define FLAGS_S 1 << 1
    # define FLAGS_O 1 << 2
    char                        *opponents_directory;
    char                        *sources_directory;
    char                        *output_directory;
    # define DEFAULT_OUTPUT_DIRECTORY "generated"
    # define CREATE_FILE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)
    # define CREATE_DIR (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
};
extern struct s_information     g_information;

@interface Multicorewar : NSObject {
    id <MTLDevice>                 _device;
    id <MTLCommandQueue>           _commandQueue;
    id <MTLLibrary>                _library;
    id <MTLComputePipelineState>   _pipelineStateRun;
    id <MTLComputePipelineState>   _pipelineStateStart;
    id <MTLBuffer>                 _buffer;
    
    Bounds                         *_bounds;
}



- (id <MTLDevice>)device;

@end

NS_ASSUME_NONNULL_END
