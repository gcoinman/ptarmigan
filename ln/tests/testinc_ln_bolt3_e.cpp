////////////////////////////////////////////////////////////////////////
//FAKE関数

//FAKE_VALUE_FUNC(int, external_function, int);

////////////////////////////////////////////////////////////////////////

class ln_derkey: public testing::Test {
protected:
    virtual void SetUp() {
        //RESET_FAKE(external_function)
        utl_dbg_malloc_cnt_reset();
        ptarm_init(PTARM_TESTNET, true);
    }

    virtual void TearDown() {
        ASSERT_EQ(0, utl_dbg_malloc_cnt());
        ptarm_term();
    }

public:
    static void DumpBin(const uint8_t *pData, uint16_t Len)
    {
        for (uint16_t lp = 0; lp < Len; lp++) {
            printf("%02x", pData[lp]);
        }
        printf("\n");
    }
};

////////////////////////////////////////////////////////////////////////
//BOLT3: Appendix E: Key Derivation Test Vectors
// https://github.com/lightningnetwork/lightning-rfc/blob/master/03-transactions.md#appendix-e-key-derivation-test-vectors

static const uint8_t BASE_SECRET[] = {
    //000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
};

static const uint8_t PER_COMMIT_SECRET[] = {
    //1f1e1d1c1b1a191817161514131211100f0e0d0c0b0a09080706050403020100
    0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18,
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10,
    0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
};

static const uint8_t BASE_POINT[] = {
    //036d6caac248af96f6afa7f904f550253a0f3ef3f5aa2fe6838a95b216691468e2
    0x03, 0x6d, 0x6c, 0xaa, 0xc2, 0x48, 0xaf, 0x96,
    0xf6, 0xaf, 0xa7, 0xf9, 0x04, 0xf5, 0x50, 0x25,
    0x3a, 0x0f, 0x3e, 0xf3, 0xf5, 0xaa, 0x2f, 0xe6,
    0x83, 0x8a, 0x95, 0xb2, 0x16, 0x69, 0x14, 0x68,
    0xe2,
};

static const uint8_t PER_COMMITMENT_POINT[] = {
    //025f7117a78150fe2ef97db7cfc83bd57b2e2c0d0dd25eaf467a4a1c2a45ce1486
    0x02, 0x5f, 0x71, 0x17, 0xa7, 0x81, 0x50, 0xfe,
    0x2e, 0xf9, 0x7d, 0xb7, 0xcf, 0xc8, 0x3b, 0xd5,
    0x7b, 0x2e, 0x2c, 0x0d, 0x0d, 0xd2, 0x5e, 0xaf,
    0x46, 0x7a, 0x4a, 0x1c, 0x2a, 0x45, 0xce, 0x14,
    0x86,
};

////////////////////////////////////////////////////////////////////////

TEST_F(ln_derkey, pubkey)
{
    uint8_t localkey[PTARM_SZ_PUBKEY];

    bool ret = ln_derkey_pubkey(localkey, BASE_POINT, PER_COMMITMENT_POINT);
    ASSERT_TRUE(ret);

    const uint8_t LOCALKEY[] = {
        0x02, 0x35, 0xf2, 0xdb, 0xfa, 0xa8, 0x9b, 0x57,
        0xec, 0x7b, 0x05, 0x5a, 0xfe, 0x29, 0x84, 0x9e,
        0xf7, 0xdd, 0xfe, 0xb1, 0xce, 0xfd, 0xb9, 0xeb,
        0xdc, 0x43, 0xf5, 0x49, 0x49, 0x84, 0xdb, 0x29,
        0xe5,
    };
    ASSERT_EQ(0, memcmp(LOCALKEY, localkey, sizeof(LOCALKEY)));
}

TEST_F(ln_derkey, privkey)
{
    uint8_t localprivkey[PTARM_SZ_PRIVKEY];

    bool ret = ln_derkey_privkey(localprivkey,
            BASE_POINT, PER_COMMITMENT_POINT, BASE_SECRET);
    ASSERT_TRUE(ret);

    const uint8_t LOCALPRIVKEY[] = {
        0xcb, 0xce, 0xd9, 0x12, 0xd3, 0xb2, 0x1b, 0xf1,
        0x96, 0xa7, 0x66, 0x65, 0x1e, 0x43, 0x6a, 0xff,
        0x19, 0x23, 0x62, 0x62, 0x1c, 0xe3, 0x17, 0x70,
        0x4e, 0xa2, 0xf7, 0x5d, 0x87, 0xe7, 0xbe, 0x0f,
    };
    ASSERT_EQ(0, memcmp(LOCALPRIVKEY, localprivkey, sizeof(LOCALPRIVKEY)));
}

TEST_F(ln_derkey, revopubkey)
{
    uint8_t localkey[PTARM_SZ_PUBKEY];

    bool ret = ln_derkey_revocationkey(localkey, BASE_POINT, PER_COMMITMENT_POINT);
    ASSERT_TRUE(ret);

    const uint8_t LOCALKEY[] = {
        0x02, 0x91, 0x6e, 0x32, 0x66, 0x36, 0xd1, 0x9c,
        0x33, 0xf1, 0x3e, 0x8c, 0x0c, 0x3a, 0x03, 0xdd,
        0x15, 0x7f, 0x33, 0x2f, 0x3e, 0x99, 0xc3, 0x17,
        0xc1, 0x41, 0xdd, 0x86, 0x5e, 0xb0, 0x1f, 0x8f,
        0xf0,
    };
    ASSERT_EQ(0, memcmp(LOCALKEY, localkey, sizeof(LOCALKEY)));
}

TEST_F(ln_derkey, revoprivkey)
{
    uint8_t privkey[PTARM_SZ_HASH256];

    bool ret = ln_derkey_revocationprivkey(privkey, BASE_POINT, PER_COMMITMENT_POINT, BASE_SECRET, PER_COMMIT_SECRET);
    ASSERT_TRUE(ret);

    const uint8_t LOCALKEY[] = {
        0xd0, 0x9f, 0xff, 0xf6, 0x2d, 0xdb, 0x22, 0x97,
        0xab, 0x00, 0x0c, 0xc8, 0x5b, 0xcb, 0x42, 0x83,
        0xfd, 0xeb, 0x6a, 0xa0, 0x52, 0xaf, 0xfb, 0xc9,
        0xdd, 0xdc, 0xf3, 0x3b, 0x61, 0x07, 0x81, 0x10,
    };
    ASSERT_EQ(0, memcmp(LOCALKEY, privkey, sizeof(LOCALKEY)));
}
