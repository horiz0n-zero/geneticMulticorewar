//
//  Utility.h
//  geneticMulticorewar
//
//  Created by Antoine Feuerstein on 01/02/2020.
//  Copyright © 2020 Antoine Feuerstein. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <stdarg.h>

NS_ASSUME_NONNULL_BEGIN

@interface Utility : NSObject

- (const char *)bytesString:(const NSInteger)count;

@property(nonatomic, strong, class) Utility *shared;

- (void)fatalError:(NSError *_Nullable const)error msg:(NSString *const)msg;
- (void)fatalErrno:(NSString *const)msg;

- (void)messageCPU:(const char *const)format, ...;
- (void)messageGPU:(const char *const)format, ...;

- (void)printArenaMemory:(void *const)memory;

@end

NS_ASSUME_NONNULL_END
