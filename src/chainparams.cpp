// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2019 The MktCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("dcbcae705b0af93fe0aaad1cc588ba243d02ed543d09bf5b167127c05cfcd5ce"))
    (256350, uint256("b46ff123bb89bfade87a01585bf9896d79cb5936c1088f9f691a0878ad5facc2"))
    (465065, uint256("2092d14ed1bfa3bf155fd9307da936ef078e479eca3bac8989985bb094bf541b"))
    (474999, uint256("cac0faa291dca03140654f2d6853d41cd1672e4617fcb4adb7d3af3281687b58"))
    (475000, uint256("6d3dd189a8d19dbe976b8c80ca981b5a1ce7415c0b34ebdb7b5eb73a548b0e0f"))
    (475500, uint256("afd83553c224bb97f8c70eb239955bc1380cd3ec82e55d8affc6e5db19150ab3"))
    (476000, uint256("5f0f21172e8b3e93a4a0ac6002cd00e05ab880510dd6d0db6b3d2fe055aac088"))
    (476500, uint256("65220a57564cb38d37ec09a5104ff7f22837fb56801669c14f16c9f8d55587a7"))
    (500000, uint256("e2e8894ddd4c01ce2fcf38137518926e51ecfd0f6231857a8bfa82fc46bc672a"))
    (525000, uint256("6beda66a0493acc8811ce1bb25127d7bb68bb9e7782f4e68260180a715ede19c"))
    (550000, uint256("77f25735821e27c6cfbfbb3f34a393f50ca80185a4cda904862dcc957c97ad4d"))
    (575000, uint256("beda9d34ef0501960b93d3737608bb7524d653fc0144c3f985468dc9bfa71c12"))
    (591600, uint256("1d2852877a88c64dab7e05e817b0e1bed8c3d3b45fad4ccb9f01a3ab0a25ddd4"))
;

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1571034118, // * UNIX timestamp of last checkpoint block
    933271,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    100        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1549789990,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1549789991,
    0,
    100};


class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        vAlertPubKey = ParseHex("04a05e58cd36976ffa3290fdc9fa1c72bc589faaae302973d80ef1ac3bd2c42bdbaf2f9c9c886e74615ec2772fd3363a0a24977af14fb5710a9de98cd50ad3e963");
        nDefaultPort = 9275;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // MktCoin starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        //nLastPOWBlock = 470000; // hardfork 2
        nLastPOWBlock = 475000; // hardfork 2


        nTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        nTargetSpacing = 10 * 60;
        nMaturity = 100;
        nMaxMoneyOut =  2000000000 * COIN;
        nMasternodeCountDrift = 20;
        nMinerThreads = 0;
        nModifierUpdateBlock = 0;

        nEnforceBlockUpgradeMajority = nLastPOWBlock+1;
        nRejectBlockOutdatedMajority = nLastPOWBlock+2;
        nToCheckBlockUpgradeMajority = nLastPOWBlock+3;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         */
        const char* pszTimestamp = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 5000 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1503190481;
        genesis.nBits = 0x1e0fffff;
        genesis.nNonce = 488002;

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("dcbcae705b0af93fe0aaad1cc588ba243d02ed543d09bf5b167127c05cfcd5ce"));
        assert(genesis.hashMerkleRoot == uint256("a1d38ec583043b2d863efb257862f9046a65e3e374719961d553464a89d0903c"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 110);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 115);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 238);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0xC6)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0xC6)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("mkt-coin.com", "seeds2.mkt-coin.com"));

        fPowNoRetargeting = false;
        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;
        nStartMasternodePayments = 1; // from start

        nPoolMaxTransactions = 3;
        strSporkKey = "049be6280c597d3b4267d0865843001c33c95ea7cbb0fca9e08e76a76ec3b2a8c71d45d2a96a5497c2a7d82c073b21946104d6c18306855a271e3fc6376ca8398d";
        strObfuscationPoolDummyAddress = "mXbpD4S7g2VzAnNe6xkqc2hXYhPqTA1D7t";

        // hardfork 1
        nBlockExtended = 256350;
        nExtendedCoins = 1000000000;
        premine1Address = "mP3S3ryiuifRKoRJSpNKHNddkrGDEztQPH";
        premine2Address = "mVBm1bARBCxPXdzkNBUhxGmTeAfsDCm5D2";
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        vAlertPubKey = ParseHex("04357a749bd50ff75f36a0d46a6ea2fba0df3f33fefbf3e23b3dd5b2835fe0817e8d347a387dbe86dee006ee68b5a3c5bc637227415c238d0495556dd955526385");
        nDefaultPort = 19275;
        nEnforceBlockUpgradeMajority = nLastPOWBlock+1;
        nRejectBlockOutdatedMajority = nLastPOWBlock+2;
        nToCheckBlockUpgradeMajority = nLastPOWBlock+3;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60;
        nTargetSpacing = 1 * 60;
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 1000000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1522342412;
        genesis.nNonce = 406042;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x247b208ebeb1ecd60df2c2884fa39c4e13ac17da56efdf0348e036363697ee75"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 110); // Testnet mktcoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 115);  // Testnet mktcoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 238);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet mktcoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0xC6)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Testnet mktcoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0xC6)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        vFixedSeeds.clear();
        vSeeds.clear();

        fPowNoRetargeting = false;
        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "0401e2800fe3ad20a2687f5f0b358dc462619e9b6c9a80d4c1b50720983c7a02eea11adbbee43ae50a4eac32d92c38f1cf62d0ae9fc4c4f29e205bbea30cd11088";
        strObfuscationPoolDummyAddress = "mXbpD4S7g2VzAnNe6xkqc2hXYhPqTA1D7t";
        nStartMasternodePayments = 1;

        // hardfork
        nBlockExtended = 55;
        nExtendedCoins = 1000000000;
        premine1Address = "mP3S3ryiuifRKoRJSpNKHNddkrGDEztQPH";
        premine2Address = "mVBm1bARBCxPXdzkNBUhxGmTeAfsDCm5D2";

    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        nDefaultPort = 19277;
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nEnforceBlockUpgradeMajority = nLastPOWBlock+1;
        nRejectBlockOutdatedMajority = nLastPOWBlock+2;
        nToCheckBlockUpgradeMajority = nLastPOWBlock+3;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // mktcoin: 1 minutes
        nTargetSpacing = 1 * 60;  // mktcoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 19279;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
