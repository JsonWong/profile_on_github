//
//  myScene.h
//  Snake
//
//  Created by None on 14-9-3.
//
//

#ifndef __Snake__myScene__
#define __Snake__myScene__

#include "cocos2d.h"

USING_NS_CC;

#define UP 1;
#define DOWN 2;
#define LEFT 3;
#define RIGHT 4;

/*typedef enum
{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT
}DIR_DEF;*/

class SnakeNode :public CCObject {
    
public:
    int row;
    int col;
    int dir;
};

class myScene : public CCLayer {
    
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(myScene);
    
    void menuCallBack(CCObject * pSender);
};

class GameLayer : public CCLayer {
    
protected:
    SnakeNode * sHead;
    SnakeNode * sFood;
    CCArray * allBody;
    CCTexture2D * cHead;
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameLayer);
    
    void menuBackToMain(CCObject * pSender);
    void logic01(float t);
    void draw();
    virtual void registerWithTouchDispatcher(void);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__Snake__myScene__) */
