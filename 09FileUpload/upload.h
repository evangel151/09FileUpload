
-----------------------------1473488174134323214564944701
Content-Disposition: form-data; name="file"; filename="SDImageCacheTests.m"
Content-Type: application/octet-stream

/**
//
//  SDImageCacheTests.m
//  SDWebImage Tests
//
//  Created by Bogdan Poplauschi on 20/06/14.
//
//

#define EXP_SHORTHAND   // required by Expecta


#import <XCTest/XCTest.h>
#import <Expecta.h>

#import "SDImageCache.h"

NSString *kImageTestKey = @"TestImageKey";

@interface SDImageCacheTests : XCTestCase
@property (strong, nonatomic) SDImageCache *sharedImageCache;
@end

@implementation SDImageCacheTests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class
    .
    self.sharedImageCache = [SDImageCache sharedImageCache];
    [self clearAllCaches];
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the
    class.
    [super tearDown];
}

- (void)testSharedImageCache {
    expect(self.sharedImageCache).toNot.beNil();
}

- (void)testSingleton{
    expect(self.sharedImageCache).to.equal([SDImageCache sharedImageCache]);
}

- (void)testClearDiskCache{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache clearDiskOnCompletion:^{
        expect([self.sharedImageCache diskImageExistsWithKey:kImageTestKey]).to.equal(NO);
        expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.equal([self imageForTesting
                                                                                           ]);
    }];
}

- (void)testClearMemoryCache{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache clearMemory];
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
    // Seems not able to access the files correctly (maybe only from test?)
    //expect([self.sharedImageCache diskImageExistsWithKey:kImageTestKey]).to.equal(YES);
    [self.sharedImageCache diskImageExistsWithKey:kImageTestKey completion:^(BOOL isInCache) {
        expect(isInCache).to.equal(YES);
    }];
}

// Testing storeImage:forKey:
- (void)testInsertionOfImage {
    UIImage *image = [self imageForTesting];
    [self.sharedImageCache storeImage:image forKey:kImageTestKey];
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.equal(image);
    expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).to.equal(image);
}

// Testing storeImage:forKey:toDisk:YES
- (void)testInsertionOfImageForcingDiskStorage{
    UIImage *image = [self imageForTesting];
    [self.sharedImageCache storeImage:image forKey:kImageTestKey toDisk:YES];
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.equal(image);
    // Seems not able to access the files correctly (maybe only from test?)
    //expect([self.sharedImageCache diskImageExistsWithKey:kImageTestKey]).to.equal(YES);
    [self.sharedImageCache diskImageExistsWithKey:kImageTestKey completion:^(BOOL isInCache) {
        expect(isInCache).to.equal(YES);
    }];
}

// Testing storeImage:forKey:toDisk:NO
- (void)testInsertionOfImageOnlyInMemory {
    UIImage *image = [self imageForTesting];
    [self.sharedImageCache storeImage:image forKey:@"TestImage" toDisk:NO];
    [self.sharedImageCache diskImageExistsWithKey:@"TestImage" completion:^(BOOL isInCache) {
        expect(isInCache).to.equal(YES);
    }];
    [self.sharedImageCache clearMemory];
    [self.sharedImageCache diskImageExistsWithKey:@"TestImage" completion:^(BOOL isInCache) {
        expect(isInCache).to.equal(NO);
    }];
}

- (void)testRetrievalImageThroughNSOperation{
    //- (NSOperation *)queryDiskCacheForKey:(NSString *)key done:(SDWebImageQueryCompletedBlock)doneBlock
    ;
    UIImage *imageForTesting = [self imageForTesting];
    [self.sharedImageCache storeImage:imageForTesting forKey:kImageTestKey];
    NSOperation *operation = [self.sharedImageCache queryDiskCacheForKey:kImageTestKey done:^(UIImage
                                                                                              *image, SDImageCacheType cacheType) {
        expect(image).to.equal(imageForTesting);
    }];
    expect(operation).toNot.beNil;
}

- (void)testRemoveImageForKey{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey];
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
    expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).to.beNil;
}

- (void)testRemoveImageForKeyWithCompletion{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey withCompletion:^{
        expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).to.beNil;
        expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
    }];
}

- (void)testRemoveImageForKeyNotFromDisk{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey fromDisk:NO];
    expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).toNot.beNil;
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
}

- (void)testRemoveImageForKeyFromDisk{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey fromDisk:NO];
    expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).to.beNil;
    expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
}

- (void)testRemoveImageforKeyNotFromDiskWithCompletion{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey fromDisk:NO withCompletion:^{
        expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).toNot.beNil;
        expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
    }];
}

- (void)testRemoveImageforKeyFromDiskWithCompletion{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    [self.sharedImageCache removeImageForKey:kImageTestKey fromDisk:YES withCompletion:^{
        expect([self.sharedImageCache imageFromDiskCacheForKey:kImageTestKey]).to.beNil;
        expect([self.sharedImageCache imageFromMemoryCacheForKey:kImageTestKey]).to.beNil;
    }];
}

// TODO -- Testing insertion with recalculate
- (void)testInsertionOfImageOnlyInDisk {
}

- (void)testInitialCacheSize{
    expect([self.sharedImageCache getSize]).to.equal(0);
}

- (void)testInitialDiskCount{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    expect([self.sharedImageCache getDiskCount]).to.equal(1);
}

- (void)testDiskCountAfterInsertion{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    expect([self.sharedImageCache getDiskCount]).to.equal(1);
}

- (void)testDefaultCachePathForAnyKey{
    NSString *path = [self.sharedImageCache defaultCachePathForKey:kImageTestKey];
    expect(path).toNot.beNil;
}

- (void)testCachePathForNonExistingKey{
    NSString *path = [self.sharedImageCache cachePathForKey:kImageTestKey inPath:[self.sharedImageCache
                                                                                  defaultCachePathForKey:kImageTestKey]];
    expect(path).to.beNil;
}

- (void)testCachePathForExistingKey{
    [self.sharedImageCache storeImage:[self imageForTesting] forKey:kImageTestKey];
    NSString *path = [self.sharedImageCache cachePathForKey:kImageTestKey inPath:[self.sharedImageCache
                                                                                  defaultCachePathForKey:kImageTestKey]];
    expect(path).notTo.beNil;
}

#pragma mark Helper methods

- (void)clearAllCaches{
    [self.sharedImageCache clearDisk];
    [self.sharedImageCache clearMemory];
}

- (UIImage *)imageForTesting{
    NSBundle *testBundle=[NSBundle bundleForClass:[self class]];
    NSString *testBundlePath=[testBundle pathForResource:@"TestImage" ofType:@"jpg"];
    return [UIImage imageWithContentsOfFile:testBundlePath];
}

@end */  // 所上传的具体的文件
-----------------------------1473488174134323214564944701
Content-Disposition: form-data; name="username"

zhangsan  // 文件上传的具体参数: 用户名
-----------------------------1473488174134323214564944701--
Content-Disposition: form-data; name="passwprd"

evangel   // 文件上传的具体参数2: 密码
-----------------------------1473488174134323214564944701--
// 如果还有其他参数拼接在后面 (单个文件上传的情况)



文件上传 "请求体" 所包含的内容
1. 请求参数
* 文件参数
* 非文件参数
-----------------------------1473488174134323214564944701
Content-Disposition: form-data; name="参数名"; filename="文件名"
Content-Type: 文件类型(MIMEType)

* 非文件参数
-----------------------------1473488174134323214564944701
Content-Disposition: form-data; name="参数名";

2. 结尾标记
-----------------------------1473488174134323214564944701--