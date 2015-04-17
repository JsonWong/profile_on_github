//
//  myScene.cpp
//  Snake
//
//  Created by None on 14-9-3.
//
//

#include "myScene.h"

bool myScene::init()
{
    if ( !CCLayer::init() )return false;
    
    CCMenuItemFont * item1 = CCMenuItemFont::create("Start", this, menu_selector(myScene::menuCallBack));
    item1->setTag(1);
    item1->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 + 50));
    
    CCMenuItemFont * item2 = CCMenuItemFont::create("Exit", this, menu_selector(myScene::menuCallBack));
    item2->setTag(2);
    item2->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 -50));
    
    CCMenu * menu = CCMenu::create(item1, item2, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    return true;
}

CCScene* myScene::scene()
{
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    myScene *layer = myScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void myScene::menuCallBack(CCObject * pSender)
{
    switch ( ((CCNode *)pSender)->getTag() ) {
        case 1:
            CCDirector::sharedDirector()->replaceScene(GameLayer::scene());
            break;
        case 2:
            CCDirector::sharedDirector()->end();
            exit(0);
            break;
        default:
            break;
    }
}

bool GameLayer::init()
{
    if ( !CCLayer::init() )return false;
    
    CCMenuItemFont * item1 = CCMenuItemFont::create("MainMenu", this, menu_selector(GameLayer::menuBackToMain));
    item1->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width * 4/5, CCDirector::sharedDirector()->getWinSize().height/2));
    
    CCMenu * menu = CCMenu::create(item1, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    //cHead = CCTextureCache::sharedTextureCache()->addImage("Icon-57.png");
    
    this->setTouchEnabled(true);
    //this->setAccelerometerEnabled(true);
    //this->setKeyboardEnabled(true);
    
    sHead = new SnakeNode();
    //sHead->autorelease();
    sHead->row = arc4random() % 10;
    sHead->col = arc4random() % 10;
    sHead->dir = arc4random() % 4 + 1;
    
    sFood = new SnakeNode();
    //sFood->autorelease();
    sFood->row = arc4random() % 10;
    sFood->col = arc4random() % 10;
    
    this->allBody = CCArray::create();
    this->allBody->retain();
    
    this->schedule(schedule_selector(GameLayer::logic01), 0.5);
    this->schedule(schedule_selector(GameLayer::update), 0.5);
    
    return true;
}

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void GameLayer::menuBackToMain(CCObject * pSender)
{
    CCDirector::sharedDirector()->replaceScene(myScene::scene());
}

void GameLayer::draw()
{
//    ccDrawColor4B(255, 0, 0, 255);
//    glLineWidth(2);
//    for (int i = 0; i < 11; ++i)
//    {
//        ccDrawLine(CCPoint(0, i * 32), CCPoint(320, i * 32));
//        ccDrawLine(CCPoint(i * 32, 0), CCPoint(i * 32, 320));
//    }
//    
//    ccColor3B color1 = ccc3(255, 0, 0);
//    CCPoint p1(sHead->col * 32 + 2, sHead->row * 32 + 2);
//    CCPoint p2(sHead->col * 32 + 32, sHead->row * 32 + 32);
//    ccDrawSolidRect(p1, p2, ccc4FFromccc3B(color1));
//    
//    ccColor3B color2 = ccc3(0, 255, 0);
//    CCPoint p3(sFood->col * 32 + 2, sFood->row * 32 + 2);
//    CCPoint p4(sFood->col * 32 + 32, sFood->row * 32 + 32);
//    ccDrawSolidRect(p3, p4, ccc4FFromccc3B(color2));
//    
//    for (int i = 0; i < allBody->count(); ++i)
//    {
//        SnakeNode * node = (SnakeNode *)allBody->objectAtIndex(i);
//        CCPoint p5(node->col * 32 + 2, node->row * 32 + 2);
//        CCPoint p6(node->col * 32 + 32, node->row * 32 + 32);
//        ccDrawSolidRect(p5, p6, ccc4FFromccc3B(color1));
//    }

    ccDrawColor4B(255, 0, 0, 255);
    glLineWidth(2);
    for (int i = 0; i < 11; ++i)
    {
        ccDrawLine(CCPoint(0, i * 64), CCPoint(640, i * 64));
        ccDrawLine(CCPoint(i * 64, 0), CCPoint(i * 64, 640));
    }
    
    ccColor3B color1 = ccc3(255, 0, 0);
    CCPoint p1(sHead->col * 64 + 2, sHead->row * 64 + 2);
    CCPoint p2(sHead->col * 64 + 64, sHead->row * 64 + 64);
    ccDrawSolidRect(p1, p2, ccc4FFromccc3B(color1));
    
    ccColor3B color2 = ccc3(0, 255, 0);
    CCPoint p3(sFood->col * 64 + 2, sFood->row * 64 + 2);
    CCPoint p4(sFood->col * 64 + 64, sFood->row * 64 + 64);
    ccDrawSolidRect(p3, p4, ccc4FFromccc3B(color2));
    
    for (int i = 0; i < allBody->count(); ++i)
    {
        SnakeNode * node = (SnakeNode *)allBody->objectAtIndex(i);
        CCPoint p5(node->col * 64 + 2, node->row * 64 + 2);
        CCPoint p6(node->col * 64 + 64, node->row * 64 + 64);
        ccDrawSolidRect(p5, p6, ccc4FFromccc3B(color1));
    }
    
//    CCRect r(340, 0, 57, 57);
//    cHead->drawInRect(r);
    
    CCLayer::draw();
}

void GameLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

bool GameLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLog("%.4f,%.4f",pTouch->getLocation().x,pTouch->getLocation().y);
    CCPoint p = pTouch->getLocation();
    int now_row;
    now_row = ((int)p.y)/32;
    int now_col;
    now_col = ((int)p.x)/32;
    
    if (abs(now_row - sHead->row) > abs(now_col - sHead->col))
    {
        if (now_row > sHead->row)
        {
            sHead->dir = UP;
        }
        else
        {
            sHead->dir = DOWN;
        }
    }
    else
    {
        if (now_col > sHead->col)
        {
            sHead->dir = RIGHT;
        }
        else
        {
            sHead->dir = LEFT;
        }
    }
    
    return true;
}

void GameLayer::logic01(float t)
{
    for (int i = allBody->count() - 1; i >= 0; --i)
    {
        SnakeNode * sn = (SnakeNode *)allBody->objectAtIndex(i);
        if (i > 0)
        {
            SnakeNode * snpre = (SnakeNode *)allBody->objectAtIndex(i-1);
            sn->dir = snpre->dir;
            sn->row = snpre->row;
            sn->col = snpre->col;
        }
        else if (i == 0)
        {
            sn->dir = sHead->dir;
            sn->row = sHead->row;
            sn->col = sHead->col;
        }
    }
    
    switch (sHead->dir) {
        case 1:
            sHead->row++;
            if (sHead->row >= 10)CCDirector::sharedDirector()->replaceScene(myScene::scene());
            break;
        case 2:
            sHead->row--;
            if (sHead->row < 0)CCDirector::sharedDirector()->replaceScene(myScene::scene());
            break;
        case 3:
            sHead->col--;
            if (sHead->col < 0)CCDirector::sharedDirector()->replaceScene(myScene::scene());
            break;
        case 4:
            sHead->col++;
            if (sHead->col >= 10)CCDirector::sharedDirector()->replaceScene(myScene::scene());
            break;
            
        default:
            break;
    }
    
    if (sHead->row == sFood->row && sHead->col == sFood->col)
    {
        sFood->row = arc4random() % 10;
        sFood->col = arc4random() % 10;
        
        SnakeNode * sn = new SnakeNode;
        sn->autorelease();
        SnakeNode * lastNode = NULL;
        
        if (allBody->count() > 0)
        {
            lastNode = (SnakeNode *)allBody->lastObject();
        }
        else
        {
            lastNode = sHead;
        }
        
        switch (lastNode->dir) {
            case 1:
                sn->row = lastNode->row - 1;
                sn->col = lastNode->col;
                break;
            case 2:
                sn->row = lastNode->row + 1;
                sn->col = lastNode->col;
                break;
            case 3:
                sn->row = lastNode->row;
                sn->col = lastNode->col + 1;
                break;
            case 4:
                sn->row = lastNode->row;
                sn->col = lastNode->col - 1;
                break;
                
            default:
                break;
        }
        
        this->allBody->addObject(sn);
    }
}
