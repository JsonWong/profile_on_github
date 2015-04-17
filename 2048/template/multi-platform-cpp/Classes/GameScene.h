//
//  GameScene.h
//  None的2048
//
//  Created by None on 14-9-23.
//
//

#ifndef __None_2048__GameScene__
#define __None_2048__GameScene__

#include "cocos2d.h"
#include "Brick.h"
#include "GameOver.h"
USING_NS_CC;

class GameScene : public CCLayer {
    
private:
    CCPoint ptOrigin;
    CCPointArray *ptArray;
    CCArray *brickArray;
    int isEmpty[16];
    CCSize winSize;
    bool moveLocked;
    bool haveMoved;
    int userScore;
    CCLabelTTF *labelScore;
    
public:
    virtual bool init();
    static CCScene * scene();
    CREATE_FUNC(GameScene);
    
    void draw();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    bool isFullOf16Brick();
    Brick * addBrick();
    Brick * addBrick(int value, int index);
    Brick * addBrick(int value, int index, int row, int col);
    Brick * brickAtIndex(int index);
    
    void isOver();
    void up();
    void down();
    void left();
    void right();
};

#endif /* defined(__None_2048__GameScene__) */
