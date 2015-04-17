//
//  Brick.cpp
//  None的2048
//
//  Created by None on 14-9-23.
//
//

#include "Brick.h"

Brick::Brick()
{
    srand(time(NULL));
    int p = rand() % 100 + 1;
    if (p <= 70)
    {
        value = 2;
    }
    else
    {
        value = 4;
    }
}

Brick::Brick(int _value)
{
    value = _value;
}

bool Brick::init()
{
    if (value == 2)
    {
        sp = CCSprite::create("2.png");
    }
    else if (value == 4)
    {
        sp = CCSprite::create("4.png");
    }
    else if (value == 8)
    {
        sp = CCSprite::create("8.png");
    }
    else if (value == 16)
    {
        sp = CCSprite::create("16.png");
    }
    else if (value == 32)
    {
        sp = CCSprite::create("32.png");
    }
    else if (value == 64)
    {
        sp = CCSprite::create("64.png");
    }
    else if (value == 128)
    {
        sp = CCSprite::create("128.png");
    }
    else if (value == 256)
    {
        sp = CCSprite::create("256.png");
    }
    else if (value == 512)
    {
        sp = CCSprite::create("512.png");
    }
    else if (value == 1024)
    {
        sp = CCSprite::create("1024.png");
    }
    else if (value == 2048)
    {
        sp = CCSprite::create("2048.png");
    }
    sp->setScale(CCDirector::sharedDirector()->getWinSize().height/1136);
    addChild(sp);
    
    return true;
}