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

#include <BeastConfig.h>
#include <ripple/basics/StringUtilities.h>
#include <ripple/protocol/JsonFields.h>
#include <nationalstonegroup/rpc/impl/TxCommonPrepare.h>
#include <nationalstonegroup/rpc/TableUtils.h>
//#include <ripple/protocol/TableDefines.h>
//#include <ripple/protocol/digest.h>
//#include <ripple/basics/Slice.h>

namespace ripple {
	TxCommonPrepare::TxCommonPrepare(Application& app, const std::string& secret,const std::string& publickey, Json::Value& tx_json, getCheckHashFunc func, bool ws):
		TxPrepareBase(app,secret,publickey,tx_json,func,ws)
	{
	}
	TxCommonPrepare::~TxCommonPrepare()
	{

	}
}
