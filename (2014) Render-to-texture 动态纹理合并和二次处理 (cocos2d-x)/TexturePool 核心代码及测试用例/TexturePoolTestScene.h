/*!
 * \file TexturePoolTestScene.h
 * \date 2014/12/29 17:39
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolTestScene 
*/

#pragma once

class TexturePool;

class TexturePoolTestScene : public cocos2d::Layer
{
public: 
    TexturePoolTestScene() : m_texturePool(nullptr) {}
    virtual ~TexturePoolTestScene();

    virtual bool init();  

    CREATE_FUNC(TexturePoolTestScene);

private:
    void OnMenuItem(Ref* sender);

    cocos2d::RenderTexture *_target;

    TexturePool* m_texturePool;
};

