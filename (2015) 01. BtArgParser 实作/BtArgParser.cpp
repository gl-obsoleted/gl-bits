/*!
 * \file BtArgParser.cpp
 * \date 2015/12/22 16:33
 * \author Gu Lu (gulu@kingsoft.com)
 *
 * \brief Implementation of BtArgParser
*/

#include "stdafx.h"
#include "BtArgParser.h"

#include "Core/BtCoreUtil.h"

#include "mcr/core/McCorePublic.h"

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

BtArgParser::BtArgParser(const std::string& args)
{
    split(args, ' ', m_args);
}


