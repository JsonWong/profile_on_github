//
//  GameScene.cpp
//  None的2048
//
//  Created by None on 14-9-23.
//
//

#include "GameScene.h"


bool GameScene::init()
{
    if (!CCLayer::init()) return false;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    userScore = 0;
    moveLocked = true;
    haveMoved = false;
    
    srand(time(NULL));
    
    winSize = CCDirector::sharedDirector()->getWinSize();

    setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    
    ptArray = CCPointArray::create(16);
    ptArray->retain();
    for (int i = 0; i <= 3; ++i)
    {
        ptArray->addControlPoint(ccp((95 + i * 150) * winSize.height/1136, 545 * winSize.width/640));
    }
    for (int i = 0; i <= 3; ++i)
    {
        ptArray->addControlPoint(ccp((95 + i * 150) * winSize.height/1136, 395 * winSize.width/640));
    }
    for (int i = 0; i <= 3; ++i)
    {
        ptArray->addControlPoint(ccp((95 + i * 150) * winSize.height/1136, 245 * winSize.width/640));
    }
    for (int i = 0; i <= 3; ++i)
    {
        ptArray->addControlPoint(ccp((95 + i * 150) * winSize.height/1136, 95 * winSize.width/640));
    }
    
    for (int i = 0; i <= 15; ++i)
    {
        isEmpty[i] = 1;
    }
    
    brickArray = CCArray::create();
    brickArray->retain();
    
    Brick *brick1 = addBrick();
    if (brick1) addChild(brick1);
    Brick *brick2 = addBrick();
    if (brick2) addChild(brick2);
    
    CCLabelTTF *lable = CCLabelTTF::create("Score :", "黑体", 40);
    lable->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width/4,
                               CCDirector::sharedDirector()->getWinSize().height * 3/4));
    lable->setScale(2.0f);
    addChild(lable);
    
    labelScore = CCLabelTTF::create("0", "黑体", 40);
    labelScore->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width/2,
                               CCDirector::sharedDirector()->getWinSize().height * 3/4 - 100));
    labelScore->setScale(2.0f);
    addChild(labelScore);
    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return true;
}

CCScene * GameScene::scene()
{
    CCScene *scene = CCScene::create();
    
    GameScene *layer = GameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::isFullOf16Brick()
{
    for (int i = 0; i <= 15; ++i)
    {
        if (isEmpty[i] == 1)
        {
            return false;
        }
    }
    
    return true;
}

Brick * GameScene::addBrick()
{
    if (!isFullOf16Brick())
    {
        Brick *brick = Brick::create();
        brick->index = rand() % 16;
        while (isEmpty[brick->index] == 0)
        {
            brick->index = rand() % 16;
        }
        brickArray->addObject(brick);
        isEmpty[brick->index] = 0;
        brick->setPosition(ptArray->getControlPointAtIndex(brick->index));
        brick->row = brick->index/4;
        brick->col = brick->index % 4;
        return brick;
    }
    else
    {
        return NULL;
    }
}

Brick * GameScene::addBrick(int value, int index)
{
    if (!isFullOf16Brick())
    {
        Brick *brick = new Brick(value);
        if (brick && brick->init())
        {
            brick->autorelease();
            
            if (isEmpty[index] == 0)
            {
                return NULL;
            }
            brick->index = index;
            brickArray->addObject(brick);
            isEmpty[brick->index] = 0;
            brick->setPosition(ptArray->getControlPointAtIndex(brick->index));
            brick->row = brick->index/4;
            brick->col = brick->index % 4;
            
            return brick;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

Brick * GameScene::addBrick(int value, int index, int row, int col)
{
    Brick *brick = new Brick(value);
    if (brick && brick->init())
    {
        brick->index = index;
        brick->row = row;
        brick->col = col;
        brickArray->addObject(brick);
        isEmpty[brick->index] = 0;
        brick->setPosition(ptArray->getControlPointAtIndex(brick->index));
        return brick;
    }
    return NULL;
}

Brick * GameScene::brickAtIndex(int index)
{
    for (int i = 0; i < brickArray->count(); ++i)
    {
        if (((Brick *)brickArray->objectAtIndex(i))->index == index)
        {
            return (Brick *)brickArray->objectAtIndex(i);
        }
    }
    return NULL;
}

void GameScene::draw()
{
    ccDrawColor4B(255, 255, 255, 255);
    glLineWidth(2);
    for (int i = 0; i <= 4; ++i)
    {
        ccDrawLine(ccp(20 * winSize.height/1136, (20 + i * 150) * winSize.width/640), ccp(620 * winSize.height/1136, (20 + i * 150)* winSize.width/640));
        ccDrawLine(ccp((20 + i * 150) * winSize.height/1136, 20 * winSize.width/640), ccp((20 + i * 150) * winSize.height/1136, 620 * winSize.width/640));
    }
    CCLayer::draw();
}

void GameScene::isOver()
{
    for (int i = 0; i < brickArray->count(); ++i)
    {
        if (((Brick *)brickArray->objectAtIndex(i))->value == 2048)
        {
            CCDirector::sharedDirector()->replaceScene(GameWin::scene());
        }
    }
    
    if (brickArray->count() == 16 && haveMoved == false)
    {
        CCDirector::sharedDirector()->replaceScene(GameOver::scene());
    }
}

bool GameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    moveLocked = false;
    haveMoved = false;
    ptOrigin = pTouch->getLocation();
    
    //CCLog("began %d", moveLocked);
    return true;
}

void GameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (haveMoved == true)
    {
        Brick *brick = addBrick();
        if (brick) addChild(brick);
        
        char tempStr[10];
        sprintf(tempStr, "%d", userScore);
        labelScore->setString(tempStr);
    }
    
    isOver();
    //CCLog("ended %d", moveLocked);
}

void GameScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (moveLocked == false)
    {
        moveLocked = true;
        //向右滑动
        if (pTouch->getLocation().x - ptOrigin.x > 10)
        {
            unsigned rowCount[4] = {0, 0, 0, 0};
            for (int i = 0; i <= 15; ++i)
            {
                if (isEmpty[i] == 0)
                {
                    rowCount[i/4]++;
                }
            }
            
            for (int i = 0; i <= 3; ++i)
            {
                switch (rowCount[i])
                {
                    case 1:
                        if (isEmpty[i * 4 + 0] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 0)->value, i * 4 + 3, i, (i * 4 + 3) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 0), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 0));
                            isEmpty[i * 4 + 0] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[i * 4 + 1] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 1)->value, i * 4 + 3, i, (i * 4 + 3) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 1), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[i * 4 + 2] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 2)->value, i * 4 + 3, i, (i * 4 + 3) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 2), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            
                            haveMoved = true;
                        }
                        break;
                    case 2:
                        if (isEmpty[i * 4 + 0] == 0)
                        {
                            if (isEmpty[i * 4 + 1] == 0)
                            {
                                if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(i * 4 + 0)->value;
                                    int value1 = brickAtIndex(i * 4 + 1)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value1, i * 4 + 3, i, (i * 4 + 3) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                            else //isEmpty[i * 4 + 1] != 0
                            {
                                if (isEmpty[i * 4 + 2] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        
                                        Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value2, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[i * 4 + 3] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 3)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                        }
                        else if (isEmpty[i * 4 + 3] == 0)
                        {
                            if (isEmpty[i * 4 + 2] == 0)
                            {
                                if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 2), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    removeChild(brickAtIndex(i * 4 + 3), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else //isEmpty[i * 4 + 2] != 0
                            {
                                if (isEmpty[i * 4 + 1] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 1)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew = addBrick(value1, i * 4 + 2, i, (i * 4 + 3) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                            {
                                int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(i * 4 + 1), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                removeChild(brickAtIndex(i * 4 + 2), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                
                                Brick *brickNew = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                if (brickNew) addChild(brickNew);
                                
                                haveMoved = true;
                            }
                            else
                            {
                                int value1 = brickAtIndex(i * 4 + 1)->value;
                                int value2 = brickAtIndex(i * 4 + 2)->value;
                                
                                removeChild(brickAtIndex(i * 4 + 2), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                removeChild(brickAtIndex(i * 4 + 1), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, i * 4 + 2, i, (i * 4 + 2) % 4);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value2, i * 4 + 3, i, (i * 4 + 3) % 4);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                        }
                        break;
                    case 3:
                        if (isEmpty[i * 4 + 0] == 0)
                        {
                            if (isEmpty[i * 4 + 1] == 0)
                            {
                                if (isEmpty[i * 4 + 2] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value2, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        Brick *brickNew3 = addBrick(value2, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew3) addChild(brickNew3);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[i * 4 + 3] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 1)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                            else //isEmpty[i * 4 + 1] != 0
                            {
                                if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                    int value0 = brickAtIndex(i * 4 + 0)->value;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 3), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 2), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                                else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 2)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 2), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(i * 4 + 0)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0), true);
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[i * 4 + 3] == 0)
                        {
                            if (isEmpty[i * 4 + 2] == 0)
                            {
                                if (isEmpty[i * 4 + 1] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 3), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        
                                        Brick *brickNew = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[i * 4 + 0] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 3), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value0, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 2), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(i * 4 + 0)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0), true);
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                        }
                        break;
                    case 4:
                        if (brickAtIndex(i * 4 + 2)->value == brickAtIndex(i * 4 + 3)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                            int value0 = brickAtIndex(i * 4 + 0)->value;
                            int value1 = brickAtIndex(i * 4 + 1)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 0), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 0));
                            isEmpty[i * 4 + 0] = 1;
                            removeChild(brickAtIndex(i * 4 + 3), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 3));
                            isEmpty[i * 4 + 3] = 1;
                            removeChild(brickAtIndex(i * 4 + 2), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            removeChild(brickAtIndex(i * 4 + 1), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            
                            Brick *brickNew1 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value1, i * 4 + 2, i, (i * 4 + 2) % 4);
                            if (brickNew2) addChild(brickNew2);
                            Brick *brickNew3 = addBrick(valueNew, i * 4 + 3, i, (i * 4 + 3) % 4);
                            if (brickNew3) addChild(brickNew3);
                            
                            if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                            {
                                int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(i * 4 + 2), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                removeChild(brickAtIndex(i * 4 + 1), true);
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                
                                Brick *brickNew = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                                if (brickNew) addChild(brickNew);
                            }
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(i * 4 + 2)->value == brickAtIndex(i * 4 + 1)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 +1)->value * 2;
                            int value0 = brickAtIndex(i * 4 + 0)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 0), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 0));
                            isEmpty[i * 4 + 0] = 1;
                            removeChild(brickAtIndex(i * 4 + 1), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            removeChild(brickAtIndex(i * 4 + 2), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value0, i * 4 + 1, i, (i * 4 + 1) % 4);
                            if (brickNew2) addChild(brickNew2);
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 0), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 0));
                            isEmpty[i * 4 + 0] = 1;
                            removeChild(brickAtIndex(i * 4 + 1), true);
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            
                            Brick *brickNew = addBrick(valueNew, i * 4 + 1, i, (i % 4 + 1) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            haveMoved = true;
                        }
                        break;
                    default:
                        break;
                }
            }
            return;
        }
        //向左滑动
        else if (ptOrigin.x - pTouch->getLocation().x > 10)
        {
            unsigned rowCount[4] = {0, 0, 0, 0};
            for (int i = 0; i <= 15; ++i)
            {
                if (isEmpty[i] == 0)
                {
                    rowCount[i/4]++;
                }
            }
            
            for (int i = 0; i <= 3; ++i)
            {
                switch (rowCount[i])
                {
                    case 1:
                        if (isEmpty[i * 4 + 3] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 3)->value, i * 4 + 0, i, (i * 4 + 0) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 3));
                            brickArray->removeObject(brickAtIndex(i * 4 + 3));
                            isEmpty[i * 4 + 3] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[i * 4 + 2] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 2)->value, i * 4 + 0, i, (i * 4 + 0) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 2));
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[i * 4 + 1] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(i * 4 + 1)->value, i * 4 + 0, i, (i * 4 + 0) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            removeChild(brickAtIndex(i * 4 + 1));
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            
                            haveMoved = true;
                        }
                        break;
                    case 2:
                        if (isEmpty[i * 4 + 3] == 0)
                        {
                            if (isEmpty[i * 4 + 2] == 0)
                            {
                                if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 2));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value2 = brickAtIndex(i * 4 + 2)->value;
                                    int value3 = brickAtIndex(i * 4 + 3)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 2));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value2, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[i * 4 + 1] == 0)
                            {
                                if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 1)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value1 = brickAtIndex(i * 4 + 1)->value;
                                    int value3 = brickAtIndex(i * 4 + 3)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value1, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[i * 4 + 0] == 0)
                            {
                                if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 3)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 3)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    removeChild(brickAtIndex(i * 4 + 0));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value3 = brickAtIndex(i * 4 + 3)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    
                                    Brick *brickNew = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[i * 4 + 0] == 0)
                        {
                            if (isEmpty[i * 4 + 1] == 0)
                            {
                                if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4, i, (i * 4) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[i * 4 + 2] == 0)
                            {
                                if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 2)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 2));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value2 = brickAtIndex(i * 4 + 2)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 2));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                    isEmpty[i * 4 + 2] = 1;
                                    
                                    Brick *brickNew = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else
                        {
                            if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                            {
                                int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(i * 4 + 1));
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                removeChild(brickAtIndex(i * 4 + 2));
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                
                                Brick *brickNew = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                if (brickNew) addChild(brickNew);
                                
                                haveMoved = true;
                            }
                            else
                            {
                                int value1 = brickAtIndex(i * 4 + 1)->value;
                                int value2 = brickAtIndex(i * 4 + 2)->value;
                                
                                removeChild(brickAtIndex(i * 4 + 1));
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                removeChild(brickAtIndex(i * 4 + 2));
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, i * 4 + 0, i, (i * 4 + 0) % 4);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                        }
                        break;
                    case 3:
                        if (isEmpty[i * 4 + 3] == 0)
                        {
                            if (isEmpty[i * 4 + 2] == 0)
                            {
                                if (isEmpty[i * 4 + 1] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                        int value3 = brickAtIndex(i * 4 + 3)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value1, i * 4 + 0, i, (i * 4 + 0) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value1 = brickAtIndex(i * 4 + 1)->value;
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        int value3 = brickAtIndex(i * 4 + 3)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value1, i * 4 + 0, i, (i * 4 + 0) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        Brick *brickNew3 = addBrick(value3, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew3) addChild(brickNew3);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[i * 4 + 0] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        int value3 = brickAtIndex(i * 4 + 3)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 0));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew2 = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        int value3 = brickAtIndex(i * 4 + 3)->value;
                                        
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 3));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                        isEmpty[i * 4 + 3] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value3, i * 4 + 2, i, (i * 4 + 2) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                            else //isEmpty[i * 4 + 2] != 0
                            {
                                if (brickAtIndex(i * 4 + 3)->value == brickAtIndex(i * 4 + 1)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 1));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    
                                    Brick *brickNew2 = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                                else if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                {
                                    int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                    int value3 = brickAtIndex(i * 4 + 3)->value;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(i * 4 + 0));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                    isEmpty[i * 4 + 0] = 1;
                                    removeChild(brickAtIndex(i * 4 + 1));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                    isEmpty[i * 4 + 1] = 1;
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, i * 4 + 1, i, (i * 4 + 1) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value3 = brickAtIndex(i * 4 + 3)->value;
                                    
                                    removeChild(brickAtIndex(i * 4 + 3));
                                    brickArray->removeObject(brickAtIndex(i * 4 + 3));
                                    isEmpty[i * 4 + 3] = 1;
                                    
                                    Brick *brickNew2 = addBrick(value3, i * 4 + 2, i, (i * 4 + 2) % 4);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[i * 4 + 0] == 0)
                        {
                            if (isEmpty[i * 4 + 1] == 0)
                            {
                                if (isEmpty[i * 4 + 2] == 0)
                                {
                                    if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                                        int value2 = brickAtIndex(i * 4 + 2)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        removeChild(brickAtIndex(i * 4 + 0));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 0));
                                        isEmpty[i * 4 + 0] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                                    {
                                        int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(i * 4 + 1));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                        isEmpty[i * 4 + 1] = 1;
                                        removeChild(brickAtIndex(i * 4 + 2));
                                        brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                        isEmpty[i * 4 + 2] = 1;
                                        
                                        Brick *brickNew = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                                        if (brickNew) addChild(brickNew);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                        }
                        break;
                    case 4:
                        if (brickAtIndex(i * 4 + 0)->value == brickAtIndex(i * 4 + 1)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 + 0)->value * 2;
                            int value2 = brickAtIndex(i * 4 + 2)->value;
                            int value3 = brickAtIndex(i * 4 + 3)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 0));
                            brickArray->removeObject(brickAtIndex(i * 4 + 0));
                            isEmpty[i * 4 + 0] = 1;
                            removeChild(brickAtIndex(i * 4 + 2));
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            removeChild(brickAtIndex(i * 4 + 1));
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            removeChild(brickAtIndex(i * 4 + 3));
                            brickArray->removeObject(brickAtIndex(i * 4 + 3));
                            isEmpty[i * 4 + 3] = 1;
                            
                            
                            Brick *brickNew1 = addBrick(valueNew, i * 4 + 0, i, (i * 4 + 0) % 4);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value2, i * 4 + 1, i, (i * 4 + 1) % 4);
                            if (brickNew2) addChild(brickNew2);
                            Brick *brickNew3 = addBrick(value3, i * 4 + 2, i, (i * 4 + 2) % 4);
                            if (brickNew3) addChild(brickNew3);
                            
                            if (brickAtIndex(i * 4 + 1)->value == brickAtIndex(i * 4 + 2)->value)
                            {
                                int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(i * 4 + 2));
                                brickArray->removeObject(brickAtIndex(i * 4 + 2));
                                isEmpty[i * 4 + 2] = 1;
                                removeChild(brickAtIndex(i * 4 + 1));
                                brickArray->removeObject(brickAtIndex(i * 4 + 1));
                                isEmpty[i * 4 + 1] = 1;
                                
                                Brick *brickNew = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                                if (brickNew) addChild(brickNew);
                            }
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(i * 4 + 2)->value == brickAtIndex(i * 4 + 1)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 + 1)->value * 2;
                            int value3 = brickAtIndex(i * 4 + 3)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 2));
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            removeChild(brickAtIndex(i * 4 + 1));
                            brickArray->removeObject(brickAtIndex(i * 4 + 1));
                            isEmpty[i * 4 + 1] = 1;
                            removeChild(brickAtIndex(i * 4 + 3));
                            brickArray->removeObject(brickAtIndex(i * 4 + 3));
                            isEmpty[i * 4 + 3] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, i * 4 + 1, i, (i * 4 + 1) % 4);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value3, i * 4 + 2, i, (i * 4 + 2) % 4);
                            if (brickNew2) addChild(brickNew2);
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(i * 4 + 2)->value == brickAtIndex(i * 4 + 3)->value)
                        {
                            int valueNew = brickAtIndex(i * 4 + 2)->value * 2;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(i * 4 + 2));
                            brickArray->removeObject(brickAtIndex(i * 4 + 2));
                            isEmpty[i * 4 + 2] = 1;
                            removeChild(brickAtIndex(i * 4 + 3));
                            brickArray->removeObject(brickAtIndex(i * 4 + 3));
                            isEmpty[i * 4 + 3] = 1;
                            
                            Brick *brickNew = addBrick(valueNew, i * 4 + 2, i, (i * 4 + 2) % 4);
                            if (brickNew) addChild(brickNew);
                            
                            haveMoved = true;
                        }
                        break;
                    default:
                        break;
                }
            }
            return;
        }
        //向上滑动
        else if (pTouch->getLocation().y - ptOrigin.y > 10)
        {
            unsigned colCount[4];
            for (int i = 0; i <= 4; ++i)
            {
                colCount[i] = 0;
            }
            for (int i = 0; i <= 15; ++i)
            {
                if (isEmpty[i] == 0)
                {
                    colCount[i % 4]++;
                }
            }
            
            for (int i = 0; i <= 3; ++i)
            {
                switch (colCount[i])
                {
                    case 1:
                        if (isEmpty[1 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(1 * 4 + i)->value, 0 * 4 + i, 0, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[2 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(2 * 4 + i)->value, 0 * 4 + i, 0, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[3 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(3 * 4 + i)->value, 0 * 4 + i, 0, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(3 * 4 + i));
                            brickArray->removeObject(brickAtIndex(3 * 4 + i));
                            isEmpty[3 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        break;
                    case 2:
                        if (isEmpty[0 * 4 + i] == 0)
                        {
                            if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[2 * 4 + i] == 0)
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value2 = brickAtIndex(2 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(value2, 1 * 4 + i, 1, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[3 * 4 + i] == 0)
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value3 = brickAtIndex(3 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(value3, 1 * 4 + i, 1, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[3 * 4 + i] == 0)
                        {
                            if (isEmpty[2 * 4 + i] == 0)
                            {
                                if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(2 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value2 = brickAtIndex(2 * 4 + i)->value;
                                    int value3 = brickAtIndex(3 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value2, 0 * 4 + i, 0, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, 1 * 4 + i, 1, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value1 = brickAtIndex(1 * 4 + i)->value;
                                    int value3 = brickAtIndex(3 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value1, 0 * 4 + i, 0, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, 1 * 4 + i, 1, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else
                        {
                            if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew = addBrick(valueNew, 0 * 4 + i, 0, i);
                                if (brickNew) addChild(brickNew);
                                
                                haveMoved = true;
                            }
                            else
                            {
                                int value1 = brickAtIndex(1 * 4 + i)->value;
                                int value2 = brickAtIndex(2 * 4 + i)->value;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, 0 * 4 + i, 0, i);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value2, 1 * 4 + i, 1, i);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                        }
                        break;
                    case 3:
                        if (isEmpty[0 * 4 + i] == 0)
                        {
                            if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (isEmpty[2 * 4 + i] == 0)
                                {
                                    if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                        int value2 = brickAtIndex(2 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 0 * 4 + i, 0, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value2, 1 * 4 + i, 1, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[3 * 4 + i] == 0)
                                {
                                    if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                        int value3 = brickAtIndex(3 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(3 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                        isEmpty[3 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 0 * 4 + i, 0, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value3, 1 * 4 + i, 1, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(3 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(3 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                        isEmpty[3 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value3 = brickAtIndex(3 * 4 + i)->value * 2;
                                        
                                        removeChild(brickAtIndex(3 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                        isEmpty[3 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value3, 2 * 4 + i, 2, i);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                            else //isEmpty[1 * 4 + i] != 0
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                    int value3 = brickAtIndex(3 * 4 + i)->value;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, 0 * 4 + i, 0, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, 1 * 4 + i, 1, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                                else if (brickAtIndex(3 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value2 = brickAtIndex(2 * 4 + i)->value;
                                    int value3 = brickAtIndex(3 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value2, 1 * 4 + i, 1, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value3, 2 * 4 + i, 2, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[3 * 4 + i] == 0)
                        {
                            if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                int value3 = brickAtIndex(3 * 4 + i)->value;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(3 * 4 + i));
                                brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                isEmpty[3 * 4 + i] = 1;
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(valueNew, 0 * 4 + i, 0, i);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value3, 1 * 4 + i, 1, i);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                            else if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(2 * 4 + i)->value * 2;
                                int value1 = brickAtIndex(1 * 4 + i)->value;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(3 * 4 + i));
                                brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                isEmpty[3 * 4 + i] = 1;
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, 0 * 4 + i, 0, i);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(valueNew, 1 * 4 + i, 1, i);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                            else
                            {
                                int value1 = brickAtIndex(1 * 4 + i)->value;
                                int value2 = brickAtIndex(2 * 4 + i)->value;
                                int value3 = brickAtIndex(3 * 4 + i)->value;
                                
                                removeChild(brickAtIndex(3 * 4 + i));
                                brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                isEmpty[3 * 4 + i] = 1;
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, 0 * 4 + i, 0, i);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value2, 1 * 4 + i, 1, i);
                                if (brickNew2) addChild(brickNew2);
                                Brick *brickNew3 = addBrick(value3, 2 * 4 + i, 2, i);
                                if (brickNew3) addChild(brickNew3);
                                
                                haveMoved = true;
                            }
                        }
                        break;
                    case 4:
                        if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                            int value2 = brickAtIndex(2 * 4 + i)->value;
                            int value3 = brickAtIndex(3 * 4 + i)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(3 * 4 + i));
                            brickArray->removeObject(brickAtIndex(3 * 4 + i));
                            isEmpty[3 * 4 + i] = 1;
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            removeChild(brickAtIndex(0 * 4 + i));
                            brickArray->removeObject(brickAtIndex(0 * 4 + i));
                            isEmpty[0 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 0 * 4 + i, 0, i);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value2, 1 * 4 + i, 1, i);
                            if (brickNew2) addChild(brickNew2);
                            Brick *brickNew3 = addBrick(value3, 2 * 4 + i, 2, i);
                            if (brickNew3) addChild(brickNew3);
                            
                            if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                                if (brickNew1) addChild(brickNew1);
                            }
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                            int value3 = brickAtIndex(3 * 4 + i)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(3 * 4 + i));
                            brickArray->removeObject(brickAtIndex(3 * 4 + i));
                            isEmpty[3 * 4 + i] = 1;
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value3, 2 * 4 + i, 2, i);
                            if (brickNew2) addChild(brickNew2);
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(2 * 4 + i)->value * 2;
                            
                            removeChild(brickAtIndex(3 * 4 + i));
                            brickArray->removeObject(brickAtIndex(3 * 4 + i));
                            isEmpty[3 * 4 + i] = 1;
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                            if (brickNew1) addChild(brickNew1);
                            
                            haveMoved = true;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        //向下滑动
        else if (ptOrigin.y - pTouch->getLocation().y > 10)
        {
            unsigned colCount[4];
            for (int i = 0; i <= 4; ++i)
            {
                colCount[i] = 0;
            }
            for (int i = 0; i <= 15; ++i)
            {
                if (isEmpty[i] == 0)
                {
                    colCount[i % 4]++;
                }
            }
            
            for (int i = 0; i <= 3; ++i)
            {
                switch (colCount[i])
                {
                    case 1:
                        if (isEmpty[1 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(1 * 4 + i)->value, 3 * 4 + i, 3, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[2 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(2 * 4 + i)->value, 3 * 4 + i, 3, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        if (isEmpty[0 * 4 + i] == 0)
                        {
                            Brick *brickNew = addBrick(brickAtIndex(0 * 4 + i)->value, 3 * 4 + i, 3, i);
                            addChild(brickNew);
                            
                            removeChild(brickAtIndex(0 * 4 + i));
                            brickArray->removeObject(brickAtIndex(0 * 4 + i));
                            isEmpty[0 * 4 + i] = 1;
                            
                            haveMoved = true;
                        }
                        break;
                    case 2:
                        if (isEmpty[3 * 4 + i] == 0)
                        {
                            if (isEmpty[2 * 4 + i] == 0)
                            {
                                if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value1 = brickAtIndex(1 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(value1, 2 * 4 + i, 2, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[0 * 4 + i] == 0)
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(0 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(value0, 2 * 4 + i, 2, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else if (isEmpty[0 * 4 + i] == 0)
                        {
                            if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    
                                    Brick *brickNew = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew) addChild(brickNew);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(0 * 4 + i)->value;
                                    int value1 = brickAtIndex(1 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(1 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                    isEmpty[1 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value0, 2 * 4 + i, 2, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value1, 3 * 4 + i, 3, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                            else if (isEmpty[2 * 4 + i] == 0)
                            {
                                if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(0 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(0 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(0 * 4 + i)->value;
                                    int value2 = brickAtIndex(2 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value0, 2 * 4 + i, 2, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value2, 3 * 4 + i, 3, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        else
                        {
                            if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                if (brickNew1) addChild(brickNew1);
                                
                                haveMoved = true;
                            }
                            else
                            {
                                int value1 = brickAtIndex(1 * 4 + i)->value;
                                int value2 = brickAtIndex(2 * 4 + i)->value;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(value1, 2 * 4 + i, 2, i);
                                if (brickNew1) addChild(brickNew1);
                                Brick *brickNew2 = addBrick(value2, 3 * 4 + i, 3, i);
                                if (brickNew2) addChild(brickNew2);
                                
                                haveMoved = true;
                            }
                        }
                        break;
                    case 3:
                        if (isEmpty[3 * 4 + i] == 0)
                        {
                            if (isEmpty[2 * 4 + i] == 0)
                            {
                                if (isEmpty[1 * 4 + i] == 0)
                                {
                                    if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                        int value1 = brickAtIndex(1 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(3 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                        isEmpty[3 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                        }
                        if (isEmpty[0 * 4 + i] == 0)
                        {
                            if (isEmpty[1 * 4 + i] == 0)
                            {
                                if (isEmpty[2 * 4 + i] == 0)
                                {
                                    if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(2 * 4 + i)->value * 2;
                                        int value0 = brickAtIndex(0 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value0, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                        int value2 = brickAtIndex(2 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value2, 3 * 4 + i, 3, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(valueNew, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(0 * 4 + i)->value;
                                        int value1 = brickAtIndex(1 * 4 + i)->value;
                                        int value2 = brickAtIndex(2 * 4 + i)->value;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(2 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                        isEmpty[2 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, 1 * 4 + i, 1, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        Brick *brickNew3 = addBrick(value2, 3 * 4 + i, 3, i);
                                        if (brickNew3) addChild(brickNew3);
                                        
                                        haveMoved = true;
                                    }
                                }
                                else if (isEmpty[3 * 4 + i] == 0)
                                {
                                    if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                        int value0 = brickAtIndex(0 * 4 + i)->value;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        removeChild(brickAtIndex(3 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                        isEmpty[3 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value0, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                    else if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                                    {
                                        int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                        userScore += valueNew;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                                        if (brickNew1) addChild(brickNew1);
                                        
                                        haveMoved = true;
                                    }
                                    else
                                    {
                                        int value0 = brickAtIndex(0 * 4 + i)->value;
                                        int value1 = brickAtIndex(1 * 4 + i)->value;
                                        
                                        removeChild(brickAtIndex(0 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                        isEmpty[0 * 4 + i] = 1;
                                        removeChild(brickAtIndex(1 * 4 + i));
                                        brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                        isEmpty[1 * 4 + i] = 1;
                                        
                                        Brick *brickNew1 = addBrick(value0, 1 * 4 + i, 1, i);
                                        if (brickNew1) addChild(brickNew1);
                                        Brick *brickNew2 = addBrick(value1, 2 * 4 + i, 2, i);
                                        if (brickNew2) addChild(brickNew2);
                                        
                                        haveMoved = true;
                                    }
                                }
                            }
                            else //isEmpty[1 * 4 + i] != 0
                            {
                                if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                                    int value0 = brickAtIndex(0 * 4 + i)->value;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    removeChild(brickAtIndex(3 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(3 * 4 + i));
                                    isEmpty[3 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                                    if (brickNew1) addChild(brickNew1);
                                    Brick *brickNew2 = addBrick(value0, 2 * 4 + i, 2, i);
                                    if (brickNew2) addChild(brickNew2);
                                    
                                    haveMoved = true;
                                }
                                else if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(0 * 4 + i)->value)
                                {
                                    int valueNew = brickAtIndex(2 * 4 + i)->value * 2;
                                    userScore += valueNew;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    removeChild(brickAtIndex(2 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                    isEmpty[2 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                                else
                                {
                                    int value0 = brickAtIndex(0 * 4 + i)->value;
                                    
                                    removeChild(brickAtIndex(0 * 4 + i));
                                    brickArray->removeObject(brickAtIndex(0 * 4 + i));
                                    isEmpty[0 * 4 + i] = 1;
                                    
                                    Brick *brickNew1 = addBrick(value0, 1 * 4 + i, 1, i);
                                    if (brickNew1) addChild(brickNew1);
                                    
                                    haveMoved = true;
                                }
                            }
                        }
                        break;
                    case 4:
                        if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(3 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(3 * 4 + i)->value * 2;
                            int value0 = brickAtIndex(0 * 4 + i)->value;
                            int value1 = brickAtIndex(1 * 4 + i)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(3 * 4 + i));
                            brickArray->removeObject(brickAtIndex(3 * 4 + i));
                            isEmpty[3 * 4 + i] = 1;
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            removeChild(brickAtIndex(0 * 4 + i));
                            brickArray->removeObject(brickAtIndex(0 * 4 + i));
                            isEmpty[0 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 3 * 4 + i, 3, i);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value0, 1 * 4 + i, 1, i);
                            if (brickNew2) addChild(brickNew2);
                            Brick *brickNew3 = addBrick(value1, 2 * 4 + i, 2, i);
                            if (brickNew3) addChild(brickNew3);
                            
                            if (brickAtIndex(1 * 4 + i)->value == brickAtIndex(2 * 4 + i)->value)
                            {
                                int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                                userScore += valueNew;
                                
                                removeChild(brickAtIndex(1 * 4 + i));
                                brickArray->removeObject(brickAtIndex(1 * 4 + i));
                                isEmpty[1 * 4 + i] = 1;
                                removeChild(brickAtIndex(2 * 4 + i));
                                brickArray->removeObject(brickAtIndex(2 * 4 + i));
                                isEmpty[2 * 4 + i] = 1;
                                
                                Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                                if (brickNew1) addChild(brickNew1);
                            }
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(2 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                            int value0 = brickAtIndex(0 * 4 + i)->value;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            removeChild(brickAtIndex(2 * 4 + i));
                            brickArray->removeObject(brickAtIndex(2 * 4 + i));
                            isEmpty[2 * 4 + i] = 1;
                            removeChild(brickAtIndex(0 * 4 + i));
                            brickArray->removeObject(brickAtIndex(0 * 4 + i));
                            isEmpty[0 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 2 * 4 + i, 2, i);
                            if (brickNew1) addChild(brickNew1);
                            Brick *brickNew2 = addBrick(value0, 1 * 4 + i, 1, i);
                            if (brickNew2) addChild(brickNew2);
                            
                            haveMoved = true;
                        }
                        else if (brickAtIndex(0 * 4 + i)->value == brickAtIndex(1 * 4 + i)->value)
                        {
                            int valueNew = brickAtIndex(1 * 4 + i)->value * 2;
                            userScore += valueNew;
                            
                            removeChild(brickAtIndex(0 * 4 + i));
                            brickArray->removeObject(brickAtIndex(0 * 4 + i));
                            isEmpty[0 * 4 + i] = 1;
                            removeChild(brickAtIndex(1 * 4 + i));
                            brickArray->removeObject(brickAtIndex(1 * 4 + i));
                            isEmpty[1 * 4 + i] = 1;
                            
                            Brick *brickNew1 = addBrick(valueNew, 1 * 4 + i, 1, i);
                            if (brickNew1) addChild(brickNew1);
                            
                            haveMoved = true;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}