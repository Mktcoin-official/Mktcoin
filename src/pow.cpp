// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "main.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

#include <math.h>


unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime)
{
    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < Params().TargetTimespan()/4)
        nActualTimespan = Params().TargetTimespan()/4;
    if (nActualTimespan > Params().TargetTimespan()*4)
        nActualTimespan = Params().TargetTimespan()*4;

    // Retarget
    const uint256 bnPowLimit = Params().ProofOfWorkLimit();
    uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= Params().TargetTimespan();

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{

    int nHeight = pindexLast->nHeight + 1;
    
    if (nHeight >= Params().BlockExtended()) {
        unsigned int result = LWMACalculateNextWorkRequired(pindexLast);
        return result;
    }


    uint256 powLimit = Params().ProofOfWorkLimit();

    unsigned int nProofOfWorkLimit = powLimit.GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // 50 % premine
    if (pindexLast->nHeight <= 210000) {
        const uint256 premine1 = ~uint256(0) >> 1;
        return premine1.GetCompact();
    }

    int64_t DifficultyAdjustmentInterval = Params().TargetTimespan() / Params().TargetSpacing();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % DifficultyAdjustmentInterval != 0)
    {
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (DifficultyAdjustmentInterval-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime());
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (Params().SkipProofOfWorkCheck())
        return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow) {
        return error("CheckProofOfWork() : bad range");
    }

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}

unsigned int LWMACalculateNextWorkRequired(const CBlockIndex* pindexLast)
{
    if (Params().PowNoRetargeting()) {
        return pindexLast->nBits;
    }

    int N = 30;
    int64_t T = 150;
    const int64_t height = pindexLast->nHeight + 1;
    if (height > Params().LAST_POW_BLOCK()) {
        T = 120;
    }


    if (pindexLast->nHeight <= 5) { return 1; }
    else if (pindexLast->nHeight < N + 1) { N = pindexLast->nHeight - 1; }

    // k = (N+1)/2 * 0.9989^(500/N) * T
    const double adjust = pow(0.9989, 500 / N);
    const double k = (N+1)/2 * adjust * T;

    uint256 sum_target;
    int t = 0, j = 0;

    // loop through N most recent blocks.
    for (int64_t i = height - N; i < height; i++) {
        const CBlockIndex* block = pindexLast->GetAncestor(i);
        const CBlockIndex* block_Prev = block->GetAncestor(i - 1);
        int64_t solvetime = block->GetBlockTime() - block_Prev->GetBlockTime();

        if (solvetime > 7 * T) {
            solvetime = 7 * T;
        }
        if (solvetime < -(7 * T)) {
            solvetime = -(7 * T);
        }

        j++;
        t += solvetime * j;

        uint256 target;
        target.SetCompact(block->nBits);
        sum_target += target / (k * N * N);
    }

    if (t < 1) {
        t = 1;
    }

    uint256 next_target = t * sum_target;
    if (next_target > Params().ProofOfWorkLimit()) {
        next_target = Params().ProofOfWorkLimit();
    }

    return next_target.GetCompact();
}
