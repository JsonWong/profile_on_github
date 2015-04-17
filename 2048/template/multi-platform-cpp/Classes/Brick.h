//
//  Brick.h
//  None的2048
//
//  Created by None on 14-9-23.
//
//

#ifndef __None_2048__Brick__
#define __None_2048__Brick__

#include "cocos2d.h"
USING_NS_CC;

class Brick : public CCNode {
    
private:
    CCSprite *sp;
    
public:
    int value;
    int index;
    int row, col;
    
public:
    Brick();
    Brick(int _value);
    virtual bool init();
    CREATE_FUNC(Brick);
};

#endif /* defined(__None_2048__Brick__) */
