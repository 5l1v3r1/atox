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

#include <nationalstonegroup/app/tx/NSchainTx.h>
#include <nationalstonegroup/protocol/TableDefines.h>
#include <nationalstonegroup/app/sql/TxStore.h>
#include <ripple/app/main/Application.h>
#include <nationalstonegroup/app/storage/TableStorage.h>
#include <ripple/app/ledger/TransactionMaster.h>

namespace ripple {

	NSchainTx::NSchainTx(ApplyContext& ctx)
		: Transactor(ctx)
	{

	}

	TER NSchainTx::preflight(PreflightContext const& ctx)
	{
		const STTx & tx = ctx.tx;
		Application& app = ctx.app;
		auto j = app.journal("preflightNSchain");

		if (tx.isCrossChainUpload() || (tx.isFieldPresent(sfOpType) && tx.getFieldU16(sfOpType) == T_REPORT))
		{
			if (!tx.isFieldPresent(sfOriginalAddress) ||
				!tx.isFieldPresent(sfTxnLgrSeq) ||
				!tx.isFieldPresent(sfCurTxHash) ||
				!tx.isFieldPresent(sfFutureTxHash)
				)
			{
				JLOG(j.trace()) <<
					"params are not invalid";
				return temINVALID;
			}
		}

		return tesSUCCESS;
	}
	TER NSchainTx::preclaim(PreclaimContext const& ctx)
	{
		const STTx & tx = ctx.tx;
		Application& app = ctx.app;
		auto j = app.journal("preflightNSchain");

		if (tx.isCrossChainUpload())
		{
			AccountID sourceID(tx.getAccountID(sfAccount));
			auto key = keylet::table(sourceID);
			auto const tablesle = ctx.view.read(key);

			ripple::uint256 futureHash;
			if (tablesle && tablesle->isFieldPresent(sfFutureTxHash))
			{
				futureHash = tablesle->getFieldH256(sfFutureTxHash);
			}

			if (futureHash.isNonZero() && tx.getFieldH256(sfCurTxHash) != futureHash)
			{
				JLOG(j.trace()) << "currecnt hash is not equal to the expected hash.";
				return temBAD_TRANSFERORDER;
			}
			if (futureHash.isZero() && tx.isFieldPresent(sfOpType) && tx.getFieldU16(sfOpType) == T_REPORT)
			{
				JLOG(j.trace()) << "T_REPORT can't be the first transfer operator.";
				return temBAD_TRANSFERORDER;
			}
		}
		return tesSUCCESS;
	}

	TER NSchainTx::doApply()
	{
		const STTx & tx = ctx_.tx;
		ApplyView & view = ctx_.view();

		if (tx.isCrossChainUpload())
		{
			auto accountId = tx.getAccountID(sfAccount);
			auto id = keylet::table(accountId);
			auto const tablesle1 = view.peek(id);
			tablesle1->setFieldH256(sfFutureTxHash, tx.getFieldH256(sfFutureTxHash));
			view.update(tablesle1);
		}
		return tesSUCCESS;
	}

	std::pair<TER, std::string> NSchainTx::dispose(TxStore& txStore, const STTx& tx)
	{
		auto pair = txStore.Dispose(tx);
		if (pair.first)
			return std::make_pair(tesSUCCESS, pair.second);
		else
			return std::make_pair(tefTABLE_TXDISPOSEERROR, pair.second);
	}

	bool NSchainTx::canDispose(ApplyContext& ctx)
	{
		auto tables = ctx.tx.getFieldArray(sfTables);
		uint160 nameInDB = tables[0].getFieldH160(sfNameInDB);

		auto item = ctx.app.getTableStorage().GetItem(nameInDB);

		//canDispose is false if first_storage is true
		bool canDispose = true;
		if (item != NULL && item->isHaveTx(ctx.tx.getTransactionID()))
			canDispose = false;

		return canDispose;
	}

	std::pair<TxStoreDBConn*, TxStore*> NSchainTx::getTransactionDBEnv(ApplyContext& ctx)
	{
		ApplyView& view = ctx.view();

		ripple::TxStoreDBConn *pConn;
		ripple::TxStore *pStore;
		if (view.flags() & tapFromClient || view.flags() & tapByRelay)
		{
			pConn = &ctx.app.getMasterTransaction().getClientTxStoreDBConn();
			pStore = &ctx.app.getMasterTransaction().getClientTxStore();
		}
		else
		{
			pConn = &ctx.app.getMasterTransaction().getConsensusTxStoreDBConn();
			pStore = &ctx.app.getMasterTransaction().getConsensusTxStore();
		}
		return std::make_pair(pConn, pStore);
	}
}
