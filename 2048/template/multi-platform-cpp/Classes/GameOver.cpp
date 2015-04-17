//
//  GameOver.cpp
//  None的2048
//
//  Created by None on 14-9-24.
//
//

#include "GameOver.h"
#include "GameScene.h"

bool GameOver::init()
{
    if (!CCLayer::init()) return false;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CCLabelTTF *labelOver = CCLabelTTF::create("GameOver!", "黑体", 100);
    labelOver->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
    addChild(labelOver);
    
    CCMenuItemFont *item = CCMenuItemFont::create("Play Again...", this, menu_selector(GameWin::menuCallBack));
    item->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 80));
    
    CCMenu *menu = CCMenu::create(item, NULL);
    menu->setPosition(ccp(0, 0));
    addChild(menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return true;
}

CCScene * GameOver::scene()
{
    CCScene *scene = CCScene::create();
    
    GameOver *layer = GameOver::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameWin::init()
{
    if (!CCLayer::init()) return false;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    CCLabelTTF *labelOver = CCLabelTTF::create("GameWin!", "黑体", 100);
    labelOver->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2));
    addChild(labelOver);
    
    CCMenuItemFont *item = CCMenuItemFont::create("Play Again...", this, menu_selector(GameWin::menuCallBack));
    item->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height/2 - 80));
    
    CCMenu *menu = CCMenu::create(item, NULL);
    menu->setPosition(ccp(0, 0));
    addChild(menu);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return true;
}

void GameWin::menuCallBack(cocos2d::CCObject *sender)
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

CCScene * GameWin::scene()
{
    CCScene *scene = CCScene::create();
    
    GameWin *layer = GameWin::create();
    
    scene->addChild(layer);
    
    return scene;
}