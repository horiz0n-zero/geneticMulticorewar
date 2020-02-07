//
//  Utility.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Utility.h"
#include "./libcorewar/includes/libcorewar.h"

static NSByteCountFormatter *_byteCountFormatter = nil;

@implementation Utility

static Utility *_shared;
+ (Utility *)shared {
    return _shared;
}
+ (void)setShared:(Utility *)shared {
    _shared = shared;
}

- (instancetype)init
{
    self = [super init];
    _byteCountFormatter = [[NSByteCountFormatter alloc] init];
    return self;
}

- (void)fatalError:(NSError *const)error msg:(NSString *const)msg {
    if (error) {
        NSLog(@"%@\n%@\n%@\n%@: %@", error.localizedDescription, error.localizedFailureReason,
                                error.localizedRecoverySuggestion, error.localizedRecoveryOptions, msg);
    }
    else {
        NSLog(@"%@", msg);
    }
    exit(EXIT_FAILURE);
}
- (void)fatalErrno:(NSString *const)msg {
    NSLog(@"%@: %s", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

- (const char *)bytesString:(const NSInteger)count {
    return [[_byteCountFormatter stringFromByteCount:count] cStringUsingEncoding:NSUTF8StringEncoding];
}

- (void)messageCPU:(const char *const)format, ... {
    va_list args;

    va_start(args, format);
    ft_printf("[ + ] CPU\n");
    ft_vprintf(format, &args);
    va_end(args);
}
- (void)messageGPU:(const char *const)format, ... {
    va_list args;

    va_start(args, format);
    ft_printf("[ * ] GPU\n");
    ft_vprintf(format, &args);
    va_end(args);
}

- (void)printArenaMemory:(void *const)memory {
    int                index;
    const char         *ptr;

    ptr = memory;
    index = 0;
    while (index < LINES)
    {
        ft_dprintf(STDOUT_FILENO, "%#04hh[* ]x\n", BYTES_LINE, ptr);
        ptr += BYTES_LINE;
        ++index;
    }
    write(STDOUT_FILENO, "\n", 1);
}

@end
