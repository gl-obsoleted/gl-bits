/*!
 * \file TexturePoolTestScene_Anim.cpp
 * \date 2015/01/04 16:52
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of TexturePoolTestScene_Anim
*/

#include "stdafx.h"
#include "TexturePoolTestScene_Anim.h"

#include "Core/BtCoreDef.h"
#include "Core/BtGuiUtil.h"

#include "Scenes/BtSceneUtil.h"

#include "TexturePool/TexturePool.h"

#include <cocostudio/CocoStudio.h>

#include <chrono>

#include "AppMacros.h"

static BtConstStr MI_Close      = "bt_close";

static BtConstStr BtRes_TestImagePath = "test_texture_pool/map/";

TexturePoolTestScene_Anim::~TexturePoolTestScene_Anim()
{
    BtDeletePointer(m_texturePool);
}

cocos2d::LabelTTF* timingLabel = nullptr;

bool TexturePoolTestScene_Anim::init()
{
    if ( !Layer::init() )
        return false;

    auto root = BtCreateDefaultUIElements(std::bind(&BtStdHandler_BackToMainMenu, std::placeholders::_1), BTSCN_TexturePool_Anim);
    if (root)
        addChild(root, 1);

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    timingLabel = cocos2d::LabelTTF::create("timing", "Arial", TITLE_FONT_SIZE / 2);
    timingLabel->setPosition(origin.x + visibleSize.width/2, origin.y + 100);
    root->addChild(timingLabel, 1);

    BtTextMenuBuilder mb;
    mb.AddItem("single");
    mb.AddItem("uniform");
    mb.AddItem("different");
    mb.AddItem("different_pooled");
    mb.AddItem("ui_sim");
    mb.AddItem("ui_sim_pooled");
    mb.SetItemAlign(BtTextMenuBuilder::Left);
    mb.SetHandler(std::bind(&TexturePoolTestScene_Anim::OnMenuItem, this, std::placeholders::_1));
    cocos2d::Menu* menuBuild = mb.Build();
    if (menuBuild)
    {
        cocos2d::Vec2 menuPos;
        menuPos.x = origin.x + 50;
        menuPos.y = origin.y + visibleSize.height - 100;
        menuBuild->setPosition(menuPos);

        addChild(menuBuild, 1);
    }

    m_rootUI = cocos2d::Node::create();
    if (!m_rootUI)
        return false;

    addChild(m_rootUI, 1);

    m_texturePool = new TexturePool;
    m_texturePool->Init(GDefaultTexturePoolInitParams, TexPool_MaxGroupCount);
    if (!m_texturePool)
        return false;

    return true;
}

BtConstStr UIResources[] = {
    "test_texture_pool/1419872819_box1.png",
    "test_texture_pool/1419872828_box3.png",
    "test_texture_pool/1419872831_box2.png",
    "test_texture_pool/1419872836_balloons.png",
}; 


void TexturePoolTestScene_Anim::OnMenuItem(cocos2d::Ref* sender)
{
    auto text = dynamic_cast<cocos2d::MenuItemLabel*>(sender);
    if (text)
    {
        auto label = dynamic_cast<cocos2d::LabelProtocol*>(text->getLabel());
        if (label)
        {
            clearArmatures();

            if (label->getString() == "single")
            {
                addArmatures_Single();
            }
            else if (label->getString() == "uniform")
            {
                addArmatures_Uniform();
            }
            else if (label->getString() == "different")
            {
                addArmatures_Different(false);
            }
            else if (label->getString() == "different_pooled")
            {
                addArmatures_Different(true);
            }
            else if (label->getString() == "ui_sim")
            {
                m_rootUI->removeAllChildren();
                for (int i = 0; i < BT_ARRAY_SIZE(UIResources); ++i)
                {
                    BtConstStr res = UIResources[i];
                    auto sprite = cocos2d::Sprite::create(res);
                    sprite->setPosition(cocos2d::Vec2(50 + 30 * (i % 5), 50 + 80 * (i / 5)));
                    m_rootUI->addChild(sprite, -1);
                }
            }
            else if (label->getString() == "ui_sim_pooled")
            {
                m_rootUI->removeAllChildren();

                for (int i = 0; i < BT_ARRAY_SIZE(UIResources); ++i)
                {
                    BtConstStr res = UIResources[i];
                    auto sprite = cocos2d::Sprite::create(res);
                    sprite->setPosition(cocos2d::Vec2(50 + 30 * (i % 5), 50 + 80 * (i / 5)));
                    m_rootUI->addChild(sprite, -1);
                    m_texturePool->PushSprite(sprite, TexPool_Group_UI);
                }
                m_texturePool->Flush(TexPool_Group_UI);
            }
        }
    }
}

void TexturePoolTestScene_Anim::addArmatures_Single()
{
    m_vArmatrueName.push_back("SpearmenN");
    flushArmatures();
}

void TexturePoolTestScene_Anim::addArmatures_Uniform()
{
    for (int i = 0; i < 40; ++i)
    {
        m_vArmatrueName.push_back("DefenderN");
    }

    flushArmatures();
}

void TexturePoolTestScene_Anim::addArmatures_Different(bool pooled)
{
    m_vArmatrueName.push_back("DefenderN");
    m_vArmatrueName.push_back("SpearmenN");
    m_vArmatrueName.push_back("ArcherN");
    m_vArmatrueName.push_back("ShieldMaster");
    m_vArmatrueName.push_back("Defender");
    m_vArmatrueName.push_back("Archer");
    m_vArmatrueName.push_back("Spearmen");
    m_vArmatrueName.push_back("Witch");
    m_vArmatrueName.push_back("Catapult");
    m_vArmatrueName.push_back("Artillery");
    m_vArmatrueName.push_back("Cavalry");
    m_vArmatrueName.push_back("ElephantSoldier");
    m_vArmatrueName.push_back("Hoplite");
    m_vArmatrueName.push_back("Priest");
    m_vArmatrueName.push_back("ArmorCavalry");
    m_vArmatrueName.push_back("Assasin");
    m_vArmatrueName.push_back("Berserker");
    m_vArmatrueName.push_back("IceWitch");
    m_vArmatrueName.push_back("Pastor");
    m_vArmatrueName.push_back("WitchDoctor");
    m_vArmatrueName.push_back("CaoZhi");
    m_vArmatrueName.push_back("DiaoChan");
    m_vArmatrueName.push_back("DongZhuo");
    m_vArmatrueName.push_back("DongZhuoEvil");
    m_vArmatrueName.push_back("FaZheng");
    m_vArmatrueName.push_back("GongSunZan");
    //m_vArmatrueName.push_back("GuYong");
    //m_vArmatrueName.push_back("GuanYu");
    //m_vArmatrueName.push_back("HuaXiong");
    //m_vArmatrueName.push_back("HuangYueYing");
    //m_vArmatrueName.push_back("JiangWei");
    //m_vArmatrueName.push_back("LingTong");
    //m_vArmatrueName.push_back("LiuBei");
    //m_vArmatrueName.push_back("LuSu");
    //m_vArmatrueName.push_back("LuXun");
    //m_vArmatrueName.push_back("LvBu");
    //m_vArmatrueName.push_back("LvBuEvil");
    //m_vArmatrueName.push_back("MaDai");
    //m_vArmatrueName.push_back("PanFeng");
    //m_vArmatrueName.push_back("PoJun");
    //m_vArmatrueName.push_back("SunCe");
    //m_vArmatrueName.push_back("SunJian");
    //m_vArmatrueName.push_back("SunLuBan");
    //m_vArmatrueName.push_back("XiaoQiao");
    //m_vArmatrueName.push_back("XinXianYing");
    //m_vArmatrueName.push_back("XuHuang");
    //m_vArmatrueName.push_back("XuShu");
    //m_vArmatrueName.push_back("XuHuang");
    //m_vArmatrueName.push_back("YanLiangWenChou");
    //m_vArmatrueName.push_back("YuJin");
    //m_vArmatrueName.push_back("YuanShao");
    //m_vArmatrueName.push_back("YuanShu");
    //m_vArmatrueName.push_back("ZhangFei");
    //m_vArmatrueName.push_back("ZhangJiao");
    //m_vArmatrueName.push_back("ZhangJiaoEvil");
    //m_vArmatrueName.push_back("PangDe");
    //m_vArmatrueName.push_back("ZhaoYun");
    //m_vArmatrueName.push_back("ZhouCang");
    //m_vArmatrueName.push_back("ZuoCi");
    //m_vArmatrueName.push_back("HellDog");
    //m_vArmatrueName.push_back("MaYuanYi");
    //m_vArmatrueName.push_back("PengTuo");
    //m_vArmatrueName.push_back("Ooze");
    //m_vArmatrueName.push_back("TenAttendants");
    //m_vArmatrueName.push_back("Ghoul");
    //m_vArmatrueName.push_back("Butcher");
    //m_vArmatrueName.push_back("ZhangLiangEvil");
    //m_vArmatrueName.push_back("ZhangLiangBao");
    //m_vArmatrueName.push_back("ZhangManCheng");
    //m_vArmatrueName.push_back("ThunderDragon");
    //m_vArmatrueName.push_back("TaoTie");

    flushArmatures(pooled);
}

void TexturePoolTestScene_Anim::flushArmatures(bool pooled)
{
    m_cachedNames.clear();

    for (auto strArmatrueName : m_vArmatrueName)
    {
        std::string sPng = BtRes_TestImagePath + strArmatrueName + ".png";
        std::string sPlist = BtRes_TestImagePath + strArmatrueName + ".plist";
        std::string sXml = BtRes_TestImagePath + strArmatrueName + ".xml";

        cocostudio::DataReaderHelper::setPositionReadScale(0.25f);
        cocostudio::ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo( sPng.c_str(), sPlist.c_str(), sXml.c_str() );

        if (pooled)
        {
            m_texturePool->PushImage(sPng);

            std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(sPlist);
            cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(fullPath);
            cocos2d::ValueMap& framesDict = dict["frames"].asValueMap();
            for (auto iter = framesDict.begin(); iter != framesDict.end(); ++iter)
            {
                m_cachedNames[strArmatrueName].push_back(iter->first);
            }
        }

        cocostudio::Armature *pArmature = cocostudio::Armature::create();
        pArmature->init(strArmatrueName.c_str());
        pArmature->setScale(2.0f);
        pArmature->getAnimation()->setAnimationScale(0.2f);
        pArmature->getAnimation()->playByIndex(1, -1, -1/*, 1, 10000*/);
        pArmature->setPosition((pooled ? 200 : 100) + rand() % 100, 100 + rand() % 100);
        this->addChild(pArmature);

        m_vArmature.push_back(pArmature);
    }

    if (pooled)
    {
        {
            std::chrono::high_resolution_clock::time_point _startTime = std::chrono::high_resolution_clock::now();
            
            m_texturePool->Flush();

            std::chrono::high_resolution_clock::time_point _stopTime = std::chrono::high_resolution_clock::now();
            m_generateTime = static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(_stopTime - _startTime).count());
        }

        {
            std::chrono::high_resolution_clock::time_point _startTime = std::chrono::high_resolution_clock::now();

            for (auto strArmatrueName : m_vArmatrueName)
            {
                std::string sPng = BtRes_TestImagePath + strArmatrueName + ".png";

                cocos2d::Texture2D* texture = nullptr;
                cocos2d::Rect rect;
                if (m_texturePool->GetImageRect(sPng, &texture, &rect))
                {
                    auto& names = m_cachedNames[strArmatrueName];
                    for (auto spriteFrameName : names)
                    {
                        cocos2d::SpriteFrame* sf = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName); 
                        if (sf && texture)
                        {
                            cocos2d::Rect newRect = sf->getRect();
                            newRect.origin += rect.origin;
                            if (sf->getTexture() != texture)
                            {
                                sf->setTexture(texture);
                            }
                            sf->setRect(newRect);
                        }
                    }
                }
            }

            std::chrono::high_resolution_clock::time_point _stopTime = std::chrono::high_resolution_clock::now();
            m_parsingTime = static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(_stopTime - _startTime).count());
        }

        if (timingLabel)
        {
            char test[256];
            sprintf(test, "generate: %d microseconds, parsing: %d microseconds", m_generateTime, m_parsingTime);
            timingLabel->setString(test);
        }

        cocos2d::Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }
}

void TexturePoolTestScene_Anim::clearArmatures()
{
    m_texturePool->Defrag();

    m_vArmatrueName.clear();

    while( !m_vArmature.empty() ) 
    {
        cocostudio::Armature *pCCArmature = m_vArmature[m_vArmature.size() - 1];
        m_vArmature.pop_back();
        pCCArmature->removeFromParentAndCleanup(true);
    }
}

