//------------------------------------------------------------------------------
/*
 This file is part of nschaind: https://github.com/nschain/nschaind
 Copyright (c) 2016-2018 NationalStoneGroup Technology Co., Ltd.
 
	nschaind is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	nschaind is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
 */
//==============================================================================

#ifndef RIPPLE_BASICS_CHARACTERUTILITIES_H_INCLUDED
#define RIPPLE_BASICS_CHARACTERUTILITIES_H_INCLUDED

#include <string>
namespace ripple {

    bool  TransGBK_UTF8(const std::string &sSrc, std::string &sDest, bool bFromGBK2UTF8);    
}
#endif
