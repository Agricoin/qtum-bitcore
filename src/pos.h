// Copyright (c) 2012-2013 The PPCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef QUANTUM_POS_H
#define QUANTUM_POS_H

#include "chain.h"
#include "primitives/transaction.h"
#include "consensus/validation.h"
#include "txdb.h"
#include "validation.h"
#include "arith_uint256.h"
#include "hash.h"
#include "timedata.h"
#include "chainparams.h"
#include "script/sign.h"
#include "consensus/consensus.h"

// To decrease granularity of timestamp
// Supposed to be 2^n-1
static const uint32_t STAKE_TIMESTAMP_MASK = 15;

struct CStakeCache{
    CStakeCache(CBlockHeader blockFrom_, CDiskTxPos txindex_, CAmount amount_) : blockFrom(blockFrom_), txindex(txindex_), amount(amount_){
    }
    CBlockHeader blockFrom;
    CDiskTxPos txindex;
    CAmount amount;
};

void CacheKernel(std::map<COutPoint, CStakeCache>& cache, const COutPoint& prevout);

// Compute the hash modifier for proof-of-stake
uint256 ComputeStakeModifier(const CBlockIndex* pindexPrev, const uint256& kernel);

// Check whether stake kernel meets hash target
// Sets hashProofOfStake on success return
bool CheckStakeKernelHash(CBlockIndex* pindexPrev, unsigned int nBits, const CBlockHeader& blockFrom, unsigned int nTxPrevOffset, CAmount prevoutAmount, const COutPoint& prevout, unsigned int nTimeTx, uint256& hashProofOfStake, uint256& targetProofOfStake, bool fPrintProofOfStake=false);

// Check kernel hash target and coinstake signature
// Sets hashProofOfStake on success return
bool CheckProofOfStake(CBlockIndex* pindexPrev, CValidationState& state, const CTransaction& tx, unsigned int nBits, uint32_t nTimeBlock, uint256& hashProofOfStake, uint256& targetProofOfStake);

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(uint32_t nTimeBlock);

// Wrapper around CheckStakeKernelHash()
// Also checks existence of kernel input and min age
// Convenient for searching a kernel
bool CheckKernel(CBlockIndex* pindexPrev, unsigned int nBits, uint32_t nTimeBlock, const COutPoint& prevout, uint32_t* pBlockTime=NULL);
bool CheckKernel(CBlockIndex* pindexPrev, unsigned int nBits, uint32_t nTimeBlock, const COutPoint& prevout, uint32_t* pBlockTime, const std::map<COutPoint, CStakeCache>& cache);

#endif // QUANTUM_POS_H
