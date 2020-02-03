//
//  Utility.m
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import "Utility.h"

static NSByteCountFormatter *_byteCountFormatter = nil;

@implementation Utility

- (instancetype)init
{
    self = [super init];
    _byteCountFormatter = [[NSByteCountFormatter alloc] init];
    return self;
}

- (const char *)bytesString:(const NSInteger)count {
    return [[_byteCountFormatter stringFromByteCount:count] cStringUsingEncoding:NSUTF8StringEncoding];
}

@end
