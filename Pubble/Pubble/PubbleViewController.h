//
//  PubbleViewController.h
//  Pubble
//
//  Created by None on 14-8-11.
//  Copyright (c) 2014年 None. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PubbleViewController : UIViewController<UIAlertViewDelegate>
{
    UITouch *touch1,*touch2;
    float dx,dy,speed;
    NSTimer *timer;
    UIAlertView *alert;
}
@property (weak, nonatomic) IBOutlet UIView *viewPuck;
@property (weak, nonatomic) IBOutlet UIView *viewPaddle1;
@property (weak, nonatomic) IBOutlet UIView *viewPaddle2;
@property (weak, nonatomic) IBOutlet UILabel *score1;
@property (weak, nonatomic) IBOutlet UILabel *score2;

-(void)reset;
-(void)start;
-(void)stop;
-(void)animate;
-(BOOL)checkPuckCollision:(CGRect)rect
                     DirX:(float)x
                     DirY:(float)y;
-(BOOL)checkGoal;
-(void)displayMessage:(NSString *)msg;
-(void)newGame;
-(int)gameOver;

@end
