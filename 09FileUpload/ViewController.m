//
//  ViewController.m
//  09FileUpload
//
//  Created by  a on 16/4/4.
//  Copyright © 2016年 eva. All rights reserved.
//

#import "ViewController.h"

#define FileBoundary @"LJTest"
#define NewLine @"\r\n"
#define Encode(str) [str dataUsingEncoding:NSUTF8StringEncoding]
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
    [self upload];
}

/**
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
 */

- (void)upload {
    
    // 1. 请求路径
    NSURL *url = [NSURL URLWithString:@"http://localhost:8080/MJServer/upload"];
    
    // 2. 创建POST请求
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    request.HTTPMethod = @"POST";
    
    // 设置请求体
    NSMutableData *body = [NSMutableData data];
    
    // 文件参数
    // * -1 请求参数
    [body appendData:Encode(FileBoundary)];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    [body appendData:Encode(@"Content-Disposition: form-data; name=\"file\"; filename=\"3.jpg\"")];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    [body appendData:Encode(@"Content-Type: image/jpg")];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    // * -2 所上传的文件内容
    [body appendData:Encode(NewLine)]; // 参数换行
    UIImage *image = [UIImage imageNamed:@"3"];
    NSData *imageData = UIImageJPEGRepresentation(image, 1.0);
    [body appendData:imageData];
    [body appendData:Encode(NewLine)]; // 参数换行

    // * -3 用户名参数
    [body appendData:Encode(FileBoundary)];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    [body appendData:Encode(@"Content-Disposition: form-data; name=\"username\"")];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    [body appendData:Encode(@"李刚")];
    [body appendData:Encode(NewLine)]; // 参数换行
    
    // * -4 结束标记
    [body appendData:Encode(FileBoundary)];
    [body appendData:Encode(@"--")];
    
    // 设置请求头 (告诉服务器本次上传的是文件数据，本次发送的是一个文件上传请求)
    // * boundary= 后面添加的是标记(可更改) 前面的是固定格式
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", FileBoundary];
    [request setValue:contentType forHTTPHeaderField:@"Content-Type"];
    
    NSString *contentLength = [NSString stringWithFormat:@"%lu", (unsigned long)body.length];
    [request setValue:contentLength  forHTTPHeaderField:@"Content-Length"];

    request.HTTPBody = body;
    
    // 3. 发送请求
    [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse * _Nullable response, NSData * _Nullable data, NSError * _Nullable connectionError) {
        // 解析服务器返回的JSON数据
        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
         NSLog(@"%@——————", dict);
    }];
    
}

- (void)upload:(NSString *)fileName mimeType:(NSString *)mimeType {
    
}



@end
