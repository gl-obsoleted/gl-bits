/*!
 * \file TexturePoolGroup.h
 * \date 2014/12/30 14:48
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Definition of TexturePoolGroup 
*/

#pragma once

#include "Core/BtCoreDef.h"

#include "binpack2d.hpp"

/*
    请注意，有两种方式使用一个 TexturePoolCell：
        可以传一个 sprite 也可以传一个图片资源。（对应两个 Assign() 函数）

    如果是前者，纹理池会在将该 sprite 所用到的纹理刷入池中后，自动将该 sprite 指向纹理池内的新地址。
    如果是后者，纹理池会维护一个内部的 sprite （外部自始至终仍可使用图片路径访问对应的资源）

    也就是说，纹理池内部始终使用 sprite 来管理单个的 cell，使用 cocos2d::Sprite::getContentSize() 来计算纹理池内实际的开销。
*/
class TexturePoolCell
{
public:
    TexturePoolCell();

    bool Assign(cocos2d::Sprite* sprite, bool recyclable);
    bool Assign(const std::string& imagePath, bool recyclable);

    void Clear();
    bool IsEmpty() { return !m_sprite;  }
    bool IsRecyclable() { return IsEmpty() || m_recyclable; }
    bool ContainsImage(const std::string& image) { return m_imagePath == image; }
    cocos2d::Sprite* GetSprite() const { return m_sprite; }

private:
    bool m_recyclable;
    cocos2d::Sprite* m_sprite;
    std::string m_imagePath;
};

class TexturePoolGroup
{
public:
    TexturePoolGroup(); 
    ~TexturePoolGroup();

    bool Init(const std::string& name, int textureSize);

    bool AppendCell(const TexturePoolCell& cell);
    bool RemoveCell(cocos2d::Sprite* sprite);
    bool RemoveCell(const std::string& imagePath);
    bool GetCellRect(const std::string& imagePath, cocos2d::Texture2D** outTexture, cocos2d::Rect* outRect);

    bool Flush();
    void Defrag();

    std::string GetName() const { return m_name; }

private:
    std::string m_name;
    int m_textureSize;

    BinPack2D::ContentAccumulator<TexturePoolCell> m_contentList;

    cocos2d::RenderTexture* m_renderTargetTexture;

    void ClearCell(TexturePoolCell& cell);
};

