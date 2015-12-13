/*!
 * \file TexturePool.h
 * \date 2014/12/29 17:02
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePool 
*/

#pragma once

#include "Core/BtCoreDef.h"

const int   TexPool_MaxGroupCount = 2;

BtConstStr  TexPool_DefaulGroupName     = "__default__";
const int   TexPool_DefaulTextureSize = 2048;

BtConstStr  TexPool_Group_UI            = "UI";
BtConstStr  TexPool_Group_Character     = "characters";
BtConstStr  TexPool_Group_Effect        = "effects";


struct TexPoolGroupInitParam
{
public:
    const char* name;
    int size;
};

const TexPoolGroupInitParam GDefaultTexturePoolInitParams[TexPool_MaxGroupCount] = 
{
    { TexPool_DefaulGroupName,  TexPool_DefaulTextureSize },
    { TexPool_Group_UI,         TexPool_DefaulTextureSize },
    //{ TexPool_Group_Effect,     TexPool_DefaulTextureSize },
};

class TexturePoolGroup;

class TexturePool 
{
public: 
    TexturePool();
    ~TexturePool();

    /*
        初始化纹理池（可以通过下面的参数来控制内存的占用情况）
            initParams - 为池内每一个 group 分别制定初始化参数
            initParamCnt - initParams 数组内所含有效元素的个数 
    */
    bool Init(const TexPoolGroupInitParam* initParams, int initParamCnt);
    
    // 销毁纹理池，释放所有占用的内存
    void Destroy();
    // 当前的纹理池是否可用
    bool IsValid();


    /*
        把精灵放入池中
            groupName - 指定这个参数，将会被放入对应的贴图组；未指定或名称不存在则放入默认组
            manualReleaseRequired - 如果为 true 则必须手动释放，如果为 false 则会在纹理池已满或手动整理时被释放

        实现细节：
            该 sprite 指向的原贴图将被复制入纹理池，操作成功后 sprite 将指向池内的对应位置
     */
    bool PushSprite(cocos2d::Sprite* sprite, const std::string& groupName = "", bool manualReleaseRequired = false);
    /* 
        从纹理池中移除该精灵
    */
    void ReleaseSprite(cocos2d::Sprite* sprite);


    /* 
        直接以 Image 为单位操作，参数含义大致同 PushSprite / ReleaseSprite，不再赘述
    */
    bool PushImage(const std::string& imagePath, const std::string& groupName = "", bool manualReleaseRequired = false);
    void ReleaseImage(const std::string& imagePath);
    /*
        获取已经在池中的 Image 信息
            - outTexture 该图档所在的贴图
            - outRect 该图档所在的位置
    */
    bool GetImageRect(const std::string& imagePath, cocos2d::Texture2D** outTexture, cocos2d::Rect* outRect);


    /* 
        刷新纹理池中指定的 group，如果未指定则全局刷新。
    */
    void Flush(const std::string& groupName = "");
    /*
        整理纹理池的使用情况

        细节：
            整理时会释放那些可回收的精灵（PushSprite 时 manualReleaseRequired 使用了默认参数 false）
            曾考虑过增加一个 bool flushAfterDefrag，后来没有加，因为在这种情况下（刚刚 defrag）是否 flush 不会有任何可见的作用
     */
    void Defrag();


private:
    TexturePoolGroup* FindGroup(const std::string& groupName);

    TexturePoolGroup* m_groups[TexPool_MaxGroupCount];
};

