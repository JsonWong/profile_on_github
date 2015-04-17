//
//  GameOver.h
//  None的2048
//
//  Created by None on 14-9-24.
//
//

#ifndef __None_2048__GameOver__
#define __None_2048__GameOver__

#include "cocos2d.h"
USING_NS_CC;

class GameOver : public CCLayer {
    
public:
    virtual bool init();
    static CCScene * scene();
    CREATE_FUNC(GameOver);
};

class GameWin : public CCLayer {
    
public:
    virtual bool init();
    static CCScene * scene();
    CREATE_FUNC(GameWin);
    
    void menuCallBack(CCObject * sender);
};

#endif /* defined(__None_2048__GameOver__) */
