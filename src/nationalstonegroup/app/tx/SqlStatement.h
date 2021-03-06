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

#ifndef RIPPLE_TX_SQLSTATEMENT_H_INCLUDED
#define RIPPLE_TX_SQLSTATEMENT_H_INCLUDED

#include <ripple/app/paths/RippleCalc.h>
#include <ripple/app/tx/impl/Transactor.h>
#include <ripple/basics/Log.h>
#include <ripple/protocol/TxFlags.h>
#include <nationalstonegroup/app/tx/NSchainTx.h>

namespace ripple {

// See https://ripple.com/wiki/Transaction_Format#Payment_.280.29

class SqlStatement
    : public NSchainTx
{
public:
    SqlStatement(ApplyContext& ctx)
        : NSchainTx(ctx)
    {
    }

    static
    TER
    preflightHandler(const STTx & tx, Application& app);
    static
    TER
    preflight (PreflightContext const& ctx);

    static
    TER
    preclaimHandler(ReadView const& view, const STTx & tx, Application& app);
    static
    TER
    preclaim(PreclaimContext const& ctx);

	static
    TER
    applyHandler(ApplyView& view, const STTx & tx, Application& app);
    TER doApply () override;

	TER preApplyForOperationRule(const STTx & tx);

	std::pair<TER, std::string> dispose(TxStore& txStore, const STTx& tx);
};

} // ripple

#endif
