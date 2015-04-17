//
//  PubbleViewController.m
//  Pubble
//
//  Created by None on 14-8-11.
//  Copyright (c) 2014年 None. All rights reserved.
//

#import "PubbleViewController.h"

@interface PubbleViewController ()

@end

@implementation PubbleViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    [self newGame];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches)
    {
        CGPoint touchPoint = [touch locationInView:self.view];
        
        if (touch1 == nil && touchPoint.y < 240)
        {
            touch1 = touch;
            self.viewPaddle1.center = CGPointMake(touchPoint.x, self.viewPaddle1.center.y);
        }
        else if (touch2 == nil && touchPoint.y >= 240)
        {
            touch2 = touch;
            self.viewPaddle2.center = CGPointMake(touchPoint.x, self.viewPaddle2.center.y);
        }
    }
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches)
    {
        CGPoint touchPoint = [touch locationInView:self.view];
        
        if (touch == touch1)
        {
            self.viewPaddle1.center = CGPointMake(touchPoint.x,self.viewPaddle1.center.y);
        }
        else if (touch == touch2)
        {
            self.viewPaddle2.center = CGPointMake(touchPoint.x,self.viewPaddle2.center.y);
        }
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches)
    {
        if (touch == touch1)
        {
            touch1 = nil;
        }
        else if (touch == touch2)
        {
            touch2 = nil;
        }
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

-(void)reset
{
    if ((arc4random() % 2) == 0)
    {
        dx = -1;
    }
    else
    {
        dx = 1;
    }
    
    if (dy != 0)
    {
        dy = -dy;
    }
    else if ((arc4random() % 2) == 0)
    {
        dy = -1;
    }
    else
    {
        dy = 1;
    }
    
    self.viewPuck.center = CGPointMake(15 + arc4random() % 290, 240);
    
    speed = 4;
}

-(void)start
{
    if (timer == nil)
    {
        timer = [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:self selector:@selector(animate) userInfo:NULL repeats:YES];
    }
    
    self.viewPuck.hidden = NO;
}

-(void)stop
{
    if (timer != nil)
    {
        [timer invalidate];
        timer = nil;
    }
    
    self.viewPuck.hidden = YES;
}

-(void)animate
{
    self.viewPuck.center = CGPointMake(self.viewPuck.center.x + dx*speed, self.viewPuck.center.y + dy*speed);
    
    [self checkPuckCollision:CGRectMake(-10, 0, 20, 480) DirX:fabsf(dx) DirY:0];
    [self checkPuckCollision:CGRectMake(310, 0, 20, 480) DirX:-fabs(dx) DirY:0];
    [self checkPuckCollision:self.viewPaddle1.frame DirX:(self.viewPuck.center.x - self.viewPaddle1.center.x)/32.0 DirY:1];
    [self checkPuckCollision:self.viewPaddle2.frame DirX:(self.viewPuck.center.x - self.viewPaddle2.center.x)/32.0 DirY:-1];
    
    [self checkGoal];
}

-(BOOL)checkPuckCollision:(CGRect)rect
                     DirX:(float)x
                     DirY:(float)y
{
    if (CGRectIntersectsRect(self.viewPuck.frame, rect))
    {
        if (x != 0)
        {
            dx = x;
        }
        if (y != 0)
        {
            dy = y;
        }
        return TRUE;
    }
    return FALSE;
}

-(BOOL)checkGoal
{
    if (self.viewPuck.center.y < 0 || self.viewPuck.center.y >= 480)
    {
        int s1 = [self.score1.text intValue];
        int s2 = [self.score2.text intValue];
        
        if (self.viewPuck.center.y <0)
        {
            ++s2;
        }
        else
        {
            ++s1;
        }
        
        self.score1.text = [NSString stringWithFormat:@"%u",s1];
        self.score2.text = [NSString stringWithFormat:@"%u",s2];
        
        if ([self gameOver] == 1)
        {
            [self displayMessage:@"Player1 has won!"];
        }
        else if ([self gameOver] == 2)
        {
            [self displayMessage:@"Player2 has won!"];
        }
        else
        {
            [self reset];
        }
        
        return TRUE;
    }
    return FALSE;
}

-(void)displayMessage:(NSString *)msg
{
    if (alert != nil)
    {
        return;
    }
    
    [self stop];
    
    alert = [[UIAlertView alloc]initWithTitle:@"Game" message:msg delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil, nil];
    
    [alert show];
}

-(void)newGame
{
    [self reset];
    
    self.score1.text = @"0";
    self.score2.text = @"0";
    
    [self displayMessage:@"Ready to play?"];
}

-(void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    alert = nil;
    
    if ([self gameOver])
    {
        [self newGame];
        return;
    }
    
    [self reset];
    
    [self start];
}

-(int)gameOver
{
    if ([self.score1.text intValue] >= 3)
    {
        return 1;
    }
    if ([self.score2.text intValue] >= 3)
    {
        return 2;
    }
    return 0;
}

@end
