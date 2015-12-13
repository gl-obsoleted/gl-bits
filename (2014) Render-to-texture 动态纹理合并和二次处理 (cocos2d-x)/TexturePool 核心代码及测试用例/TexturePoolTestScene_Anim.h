/*!
 * \file TexturePoolTestScene_Anim.h
 * \date 2015/01/04 16:51
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolTestScene_Anim 
*/

#pragma once

namespace cocostudio
{
    class Armature;
}

class TexturePool;

class TexturePoolTestScene_Anim : public cocos2d::Layer
{
public: 
    TexturePoolTestScene_Anim() : m_texturePool(nullptr), m_generateTime(0), m_parsingTime(0), m_rootUI(nullptr) {}
    virtual ~TexturePoolTestScene_Anim();

    virtual bool init();  

    CREATE_FUNC(TexturePoolTestScene_Anim);

private:
    void OnMenuItem(cocos2d::Ref* sender);

    TexturePool* m_texturePool;

    std::vector<std::string> m_vArmatrueName;
    std::vector<cocostudio::Armature *>m_vArmature;

    void addArmatures_Single();
    void addArmatures_Uniform();
    void addArmatures_Different(bool pooled = false);
    void flushArmatures(bool pooled = false);
    void clearArmatures();

    long m_generateTime;
    long m_parsingTime;
    std::map<std::string, std::vector<std::string> > m_cachedNames;

    cocos2d::Node* m_rootUI;
};

