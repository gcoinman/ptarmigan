////////////////////////////////////////////////////////////////////////
//FAKE関数

//FAKE_VALUE_FUNC(int, external_function, int);

////////////////////////////////////////////////////////////////////////

class sw: public testing::Test {
protected:
    virtual void SetUp() {
        //RESET_FAKE(external_function)
        utl_dbg_malloc_cnt_reset();
        btc_init(BTC_TESTNET, false);
    }

    virtual void TearDown() {
        ASSERT_EQ(0, utl_dbg_malloc_cnt());
        btc_term();
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

TEST_F(sw, scriptcode_p2wpkh)
{
    utl_buf_t code;

    //wif: cV7N1pozd3SNCkXYJAWeTUvoGpq9gcbvjWuWn8S2SCWy8W3zKmhk
    //pri: e0a29cfd87bf70e5f4e8c9bbf347d0185cd2057c7af1f19f2c13d264a7678189
    const uint8_t PUB[] = {
        0x02, 0x81, 0x00, 0xca, 0x14, 0xc4, 0x4e, 0x2f,
        0xe3, 0x63, 0xf9, 0x6c, 0xff, 0x64, 0x98, 0x5d,
        0xd6, 0xda, 0x11, 0xe0, 0x52, 0x0e, 0xb4, 0x79,
        0x59, 0x1b, 0xd1, 0x41, 0xde, 0x27, 0x65, 0x66,
        0x24,
    };
    const uint8_t CODE[] = {
        0x19, 0x76, 0xa9, 0x14, 0x12, 0xfa, 0x3e, 0x2d,
        0xde, 0x96, 0xa2, 0x16, 0x1b, 0xad, 0x30, 0xa3,
        0xeb, 0x0b, 0x92, 0x92, 0x08, 0xe6, 0x4e, 0x59,
        0x88, 0xac,
    };
    btc_sw_scriptcode_p2wpkh(&code, PUB);
    ASSERT_EQ(0, memcmp(CODE, code.buf, sizeof(CODE)));
    ASSERT_EQ(sizeof(CODE), code.len);
    utl_buf_free(&code);
}


TEST_F(sw, scriptcode_p2wsh)
{
    utl_buf_t code;

    //wif: cV7N1pozd3SNCkXYJAWeTUvoGpq9gcbvjWuWn8S2SCWy8W3zKmhk
    //pri: e0a29cfd87bf70e5f4e8c9bbf347d0185cd2057c7af1f19f2c13d264a7678189
    const uint8_t WIT[] = {
        0x52, 0x21, 0x03, 0xd7, 0x98, 0x23, 0x4d, 0xf0,
        0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08, 0xeb, 0x5a,
        0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38, 0xa0, 0xe6,
        0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56, 0xf1, 0x32,
        0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c, 0x5f, 0x25,
        0x61, 0x40, 0x24, 0x56, 0xcc, 0x46, 0x8f, 0xac,
        0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32, 0x2b, 0x74,
        0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b, 0x08, 0x42,
        0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21, 0x03, 0x06,
        0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6, 0x80, 0x69,
        0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18, 0x33, 0xf6,
        0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b, 0x19, 0xe6,
        0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e, 0x74, 0x53,
        0xae,
    };
    const utl_buf_t wit = { (uint8_t *)WIT, sizeof(WIT) };
    const uint8_t CODE[] = {
        0x69,
        0x52, 0x21, 0x03, 0xd7, 0x98, 0x23, 0x4d, 0xf0,
        0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08, 0xeb, 0x5a,
        0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38, 0xa0, 0xe6,
        0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56, 0xf1, 0x32,
        0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c, 0x5f, 0x25,
        0x61, 0x40, 0x24, 0x56, 0xcc, 0x46, 0x8f, 0xac,
        0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32, 0x2b, 0x74,
        0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b, 0x08, 0x42,
        0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21, 0x03, 0x06,
        0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6, 0x80, 0x69,
        0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18, 0x33, 0xf6,
        0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b, 0x19, 0xe6,
        0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e, 0x74, 0x53,
        0xae,
    };
    btc_sw_scriptcode_p2wsh(&code, &wit);
    ASSERT_EQ(0, memcmp(CODE, code.buf, sizeof(CODE)));
    ASSERT_EQ(sizeof(CODE), code.len);
    utl_buf_free(&code);
}


TEST_F(sw, read_tx_p2wpkh)
{
    //5fb7d9c00b99fe93c1228e428985fbc8eaed1d27eb864711ad6b74a32f4eb8f1
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x04,
        0xdd, 0x9b, 0x67, 0x17, 0x75, 0x69, 0x55, 0x45,
        0xfe, 0x65, 0xe5, 0x6b, 0x50, 0x2c, 0x58, 0xaa,
        0x22, 0x93, 0x4c, 0x81, 0xe8, 0x84, 0x69, 0x8d,
        0x4d, 0x94, 0xcb, 0x34, 0xa0, 0xe2, 0x1f, 0x01,
        0x00, 0x00, 0x00, 0x17, 0x16, 0x00, 0x14, 0x12,
        0xfa, 0x3e, 0x2d, 0xde, 0x96, 0xa2, 0x16, 0x1b,
        0xad, 0x30, 0xa3, 0xeb, 0x0b, 0x92, 0x92, 0x08,
        0xe6, 0x4e, 0x59, 0xff, 0xff, 0xff, 0xff, 0x01,
        0xc0, 0x27, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x19, 0x76, 0xa9, 0x14, 0x7c, 0x7f, 0x7e, 0xcc,
        0xbc, 0x3f, 0x6d, 0xbb, 0xfa, 0x74, 0x04, 0x26,
        0x16, 0x96, 0xea, 0x57, 0xa2, 0xf9, 0x8f, 0xa4,
        0x88, 0xac, 0x02, 0x48, 0x30, 0x45, 0x02, 0x21,
        0x00, 0xfa, 0x1b, 0x5c, 0xd8, 0x34, 0x08, 0xb1,
        0x30, 0xae, 0x41, 0x10, 0x66, 0x16, 0xf5, 0x24,
        0x35, 0xdb, 0x3a, 0xa2, 0x8b, 0x8a, 0xf3, 0x9d,
        0x23, 0x71, 0x24, 0x37, 0xc2, 0x81, 0x34, 0x51,
        0xf8, 0x02, 0x20, 0x4b, 0xe4, 0x97, 0xa2, 0x97,
        0x48, 0x6e, 0xd8, 0xde, 0x66, 0x60, 0x7d, 0x0f,
        0x54, 0x66, 0x4b, 0xbe, 0x71, 0x95, 0xb6, 0xf4,
        0x6b, 0x14, 0x73, 0x26, 0xef, 0x9c, 0x55, 0xaa,
        0x09, 0x05, 0x6e, 0x01, 0x21, 0x02, 0x81, 0x00,
        0xca, 0x14, 0xc4, 0x4e, 0x2f, 0xe3, 0x63, 0xf9,
        0x6c, 0xff, 0x64, 0x98, 0x5d, 0xd6, 0xda, 0x11,
        0xe0, 0x52, 0x0e, 0xb4, 0x79, 0x59, 0x1b, 0xd1,
        0x41, 0xde, 0x27, 0x65, 0x66, 0x24, 0x00, 0x00,
        0x00, 0x00,
    };
    const uint8_t TXID[] = {
        0x04, 0xdd, 0x9b, 0x67, 0x17, 0x75, 0x69, 0x55,
        0x45, 0xfe, 0x65, 0xe5, 0x6b, 0x50, 0x2c, 0x58,
        0xaa, 0x22, 0x93, 0x4c, 0x81, 0xe8, 0x84, 0x69,
        0x8d, 0x4d, 0x94, 0xcb, 0x34, 0xa0, 0xe2, 0x1f,
    };
    const uint8_t SCRIPTSIG[] = {
        0x16, 0x00, 0x14, 0x12, 0xfa, 0x3e, 0x2d, 0xde,
        0x96, 0xa2, 0x16, 0x1b, 0xad, 0x30, 0xa3, 0xeb,
        0x0b, 0x92, 0x92, 0x08, 0xe6, 0x4e, 0x59,
    };
    const uint8_t WIT0[] = {
        0x30, 0x45, 0x02, 0x21, 0x00, 0xfa, 0x1b, 0x5c,
        0xd8, 0x34, 0x08, 0xb1, 0x30, 0xae, 0x41, 0x10,
        0x66, 0x16, 0xf5, 0x24, 0x35, 0xdb, 0x3a, 0xa2,
        0x8b, 0x8a, 0xf3, 0x9d, 0x23, 0x71, 0x24, 0x37,
        0xc2, 0x81, 0x34, 0x51, 0xf8, 0x02, 0x20, 0x4b,
        0xe4, 0x97, 0xa2, 0x97, 0x48, 0x6e, 0xd8, 0xde,
        0x66, 0x60, 0x7d, 0x0f, 0x54, 0x66, 0x4b, 0xbe,
        0x71, 0x95, 0xb6, 0xf4, 0x6b, 0x14, 0x73, 0x26,
        0xef, 0x9c, 0x55, 0xaa, 0x09, 0x05, 0x6e, 0x01,
    };
    const uint8_t WIT1[] = {
        0x02, 0x81, 0x00, 0xca, 0x14, 0xc4, 0x4e, 0x2f,
        0xe3, 0x63, 0xf9, 0x6c, 0xff, 0x64, 0x98, 0x5d,
        0xd6, 0xda, 0x11, 0xe0, 0x52, 0x0e, 0xb4, 0x79,
        0x59, 0x1b, 0xd1, 0x41, 0xde, 0x27, 0x65, 0x66,
        0x24,
    };
    const uint8_t SCRIPTPK[] = {
        0x76, 0xa9, 0x14, 0x7c, 0x7f, 0x7e, 0xcc, 0xbc,
        0x3f, 0x6d, 0xbb, 0xfa, 0x74, 0x04, 0x26, 0x16,
        0x96, 0xea, 0x57, 0xa2, 0xf9, 0x8f, 0xa4, 0x88,
        0xac,
    };

    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    ASSERT_EQ(1, tx.version);
    ASSERT_EQ(1, tx.vin_cnt);
    ASSERT_TRUE(NULL != tx.vin);
    ASSERT_EQ(1, tx.vout_cnt);
    ASSERT_TRUE(NULL != tx.vout);
    ASSERT_EQ(0, tx.locktime);

    const btc_vin_t *vin = &tx.vin[0];
    ASSERT_EQ(0, memcmp(TXID, vin->txid, BTC_SZ_TXID));
    ASSERT_EQ(1, vin->index);
    ASSERT_EQ(0, memcmp(SCRIPTSIG, vin->script.buf, sizeof(SCRIPTSIG)));
    ASSERT_EQ(sizeof(SCRIPTSIG), vin->script.len);
    ASSERT_EQ(2, vin->wit_item_cnt);
    ASSERT_EQ(0, memcmp(WIT0, vin->witness[0].buf, sizeof(WIT0)));
    ASSERT_EQ(sizeof(WIT0), vin->witness[0].len);
    ASSERT_EQ(0, memcmp(WIT1, vin->witness[1].buf, sizeof(WIT1)));
    ASSERT_EQ(sizeof(WIT1), vin->witness[1].len);
    ASSERT_EQ(0xffffffff, vin->sequence);

    const btc_vout_t *vout = &tx.vout[0];
    ASSERT_EQ(BTC_BTC2SATOSHI(0.006), vout->value);
    ASSERT_EQ(0, memcmp(SCRIPTPK, vout->script.buf, sizeof(SCRIPTPK)));
    ASSERT_EQ(sizeof(SCRIPTPK), vout->script.len);

    utl_buf_t txbuf = UTL_BUF_INIT;
    btc_tx_write(&tx, &txbuf);
    ASSERT_EQ(0, memcmp(TX, txbuf.buf, sizeof(TX)));
    ASSERT_EQ(sizeof(TX), txbuf.len);
    btc_print_rawtx(txbuf.buf, txbuf.len);
    utl_buf_free(&txbuf);

    btc_print_tx(&tx);
    btc_tx_free(&tx);
}


TEST_F(sw, read_tx_p2wsh)
{
    //3c5f0cc27c54ffca22458a5ba840bccb99fcbde7823414d5c6b2c572932eff71
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x4e,
        0x1d, 0xcd, 0x4a, 0x4e, 0x10, 0xdb, 0x68, 0x00,
        0x3d, 0xe7, 0x84, 0x2b, 0x76, 0x9b, 0xa6, 0x2a,
        0x67, 0x7a, 0xfe, 0xec, 0xf8, 0x06, 0x0b, 0xd6,
        0x32, 0xca, 0xe6, 0xa4, 0x85, 0x78, 0x61, 0x01,
        0x00, 0x00, 0x00, 0x23, 0x22, 0x00, 0x20, 0x3a,
        0x33, 0x0a, 0xc7, 0xa5, 0xeb, 0xec, 0x6a, 0xcf,
        0x16, 0x09, 0x50, 0xfb, 0x88, 0x1f, 0x38, 0x8c,
        0xcf, 0xfe, 0x8d, 0x74, 0x88, 0x93, 0xd2, 0x78,
        0x75, 0x44, 0xb6, 0x5f, 0x3f, 0x71, 0x7e, 0xff,
        0xff, 0xff, 0xff, 0x01, 0x00, 0x35, 0x0c, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x19, 0x76, 0xa9, 0x14,
        0x09, 0xfc, 0x8b, 0xaa, 0xf5, 0x51, 0x22, 0x6f,
        0x79, 0x39, 0x2a, 0xca, 0x6f, 0x8f, 0x9c, 0x3a,
        0x7d, 0xb4, 0x78, 0x7c, 0x88, 0xac, 0x04, 0x00,
        0x47, 0x30, 0x44, 0x02, 0x20, 0x1f, 0x03, 0x0c,
        0xa8, 0x56, 0x31, 0xc8, 0x30, 0xfd, 0xa0, 0x6c,
        0xa6, 0x5e, 0xc6, 0xc0, 0x08, 0x11, 0xa5, 0xcb,
        0x23, 0x93, 0xf2, 0xe9, 0x59, 0xae, 0x5d, 0x9a,
        0xf9, 0xf2, 0x37, 0xc7, 0x0b, 0x02, 0x20, 0x28,
        0xd2, 0x15, 0x72, 0x1f, 0x5e, 0xa9, 0xce, 0x41,
        0xcd, 0xbf, 0x77, 0xf7, 0xd8, 0x7e, 0xef, 0xba,
        0xf3, 0x6a, 0x86, 0x5b, 0x9b, 0xae, 0xb9, 0x7b,
        0x6f, 0x61, 0xfb, 0xed, 0x8f, 0x4d, 0xa1, 0x01,
        0x48, 0x30, 0x45, 0x02, 0x21, 0x00, 0xe3, 0x80,
        0xd9, 0x0d, 0x21, 0x6e, 0xac, 0xde, 0x41, 0x6d,
        0x27, 0xbf, 0xed, 0xf0, 0xbb, 0x94, 0x0b, 0xeb,
        0x06, 0x94, 0xe7, 0xe0, 0x2b, 0x4e, 0x15, 0x7b,
        0x2b, 0x57, 0x00, 0x24, 0xe0, 0xe1, 0x02, 0x20,
        0x7f, 0xa2, 0x31, 0x27, 0x56, 0x8a, 0x51, 0xbc,
        0xf7, 0x45, 0x44, 0x5d, 0x0b, 0x2c, 0xd5, 0x99,
        0x60, 0x31, 0xb5, 0xfa, 0x9a, 0x4b, 0xb1, 0xc0,
        0x9a, 0x98, 0x24, 0x5e, 0x42, 0x5e, 0x56, 0x50,
        0x01, 0x69, 0x52, 0x21, 0x03, 0xd7, 0x98, 0x23,
        0x4d, 0xf0, 0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08,
        0xeb, 0x5a, 0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38,
        0xa0, 0xe6, 0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56,
        0xf1, 0x32, 0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c,
        0x5f, 0x25, 0x61, 0x40, 0x24, 0x56, 0xcc, 0x46,
        0x8f, 0xac, 0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32,
        0x2b, 0x74, 0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b,
        0x08, 0x42, 0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21,
        0x03, 0x06, 0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6,
        0x80, 0x69, 0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18,
        0x33, 0xf6, 0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b,
        0x19, 0xe6, 0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e,
        0x74, 0x53, 0xae, 0x00, 0x00, 0x00, 0x00,
    };
    const uint8_t TXID[] = {
        0x4e, 0x1d, 0xcd, 0x4a, 0x4e, 0x10, 0xdb, 0x68,
        0x00, 0x3d, 0xe7, 0x84, 0x2b, 0x76, 0x9b, 0xa6,
        0x2a, 0x67, 0x7a, 0xfe, 0xec, 0xf8, 0x06, 0x0b,
        0xd6, 0x32, 0xca, 0xe6, 0xa4, 0x85, 0x78, 0x61,
    };
    const uint8_t SCRIPTSIG[] = {
        0x22, 0x00, 0x20, 0x3a, 0x33, 0x0a, 0xc7, 0xa5,
        0xeb, 0xec, 0x6a, 0xcf, 0x16, 0x09, 0x50, 0xfb,
        0x88, 0x1f, 0x38, 0x8c, 0xcf, 0xfe, 0x8d, 0x74,
        0x88, 0x93, 0xd2, 0x78, 0x75, 0x44, 0xb6, 0x5f,
        0x3f, 0x71, 0x7e,
    };
    const uint8_t SCRIPTPK[] = {
        0x76, 0xa9, 0x14, 0x09, 0xfc, 0x8b, 0xaa, 0xf5,
        0x51, 0x22, 0x6f, 0x79, 0x39, 0x2a, 0xca, 0x6f,
        0x8f, 0x9c, 0x3a, 0x7d, 0xb4, 0x78, 0x7c, 0x88,
        0xac,
    };
    const uint8_t WIT1[] = {
        0x30, 0x44, 0x02, 0x20, 0x1f, 0x03, 0x0c, 0xa8,
        0x56, 0x31, 0xc8, 0x30, 0xfd, 0xa0, 0x6c, 0xa6,
        0x5e, 0xc6, 0xc0, 0x08, 0x11, 0xa5, 0xcb, 0x23,
        0x93, 0xf2, 0xe9, 0x59, 0xae, 0x5d, 0x9a, 0xf9,
        0xf2, 0x37, 0xc7, 0x0b, 0x02, 0x20, 0x28, 0xd2,
        0x15, 0x72, 0x1f, 0x5e, 0xa9, 0xce, 0x41, 0xcd,
        0xbf, 0x77, 0xf7, 0xd8, 0x7e, 0xef, 0xba, 0xf3,
        0x6a, 0x86, 0x5b, 0x9b, 0xae, 0xb9, 0x7b, 0x6f,
        0x61, 0xfb, 0xed, 0x8f, 0x4d, 0xa1, 0x01,
    };
    const uint8_t WIT2[] = {
        0x30, 0x45, 0x02, 0x21, 0x00, 0xe3, 0x80, 0xd9,
        0x0d, 0x21, 0x6e, 0xac, 0xde, 0x41, 0x6d, 0x27,
        0xbf, 0xed, 0xf0, 0xbb, 0x94, 0x0b, 0xeb, 0x06,
        0x94, 0xe7, 0xe0, 0x2b, 0x4e, 0x15, 0x7b, 0x2b,
        0x57, 0x00, 0x24, 0xe0, 0xe1, 0x02, 0x20, 0x7f,
        0xa2, 0x31, 0x27, 0x56, 0x8a, 0x51, 0xbc, 0xf7,
        0x45, 0x44, 0x5d, 0x0b, 0x2c, 0xd5, 0x99, 0x60,
        0x31, 0xb5, 0xfa, 0x9a, 0x4b, 0xb1, 0xc0, 0x9a,
        0x98, 0x24, 0x5e, 0x42, 0x5e, 0x56, 0x50, 0x01,
    };
    const uint8_t WIT3[] = {
        0x52, 0x21, 0x03, 0xd7, 0x98, 0x23, 0x4d, 0xf0,
        0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08, 0xeb, 0x5a,
        0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38, 0xa0, 0xe6,
        0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56, 0xf1, 0x32,
        0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c, 0x5f, 0x25,
        0x61, 0x40, 0x24, 0x56, 0xcc, 0x46, 0x8f, 0xac,
        0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32, 0x2b, 0x74,
        0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b, 0x08, 0x42,
        0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21, 0x03, 0x06,
        0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6, 0x80, 0x69,
        0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18, 0x33, 0xf6,
        0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b, 0x19, 0xe6,
        0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e, 0x74, 0x53,
        0xae,
    };
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    ASSERT_EQ(1, tx.version);
    ASSERT_EQ(1, tx.vin_cnt);
    ASSERT_TRUE(NULL != tx.vin);
    ASSERT_EQ(1, tx.vout_cnt);
    ASSERT_TRUE(NULL != tx.vout);
    ASSERT_EQ(0, tx.locktime);

    const btc_vin_t *vin = &tx.vin[0];
    ASSERT_EQ(0, memcmp(TXID, vin->txid, BTC_SZ_TXID));
    ASSERT_EQ(1, vin->index);
    ASSERT_EQ(0, memcmp(SCRIPTSIG, vin->script.buf, sizeof(SCRIPTSIG)));
    ASSERT_EQ(sizeof(SCRIPTSIG), vin->script.len);
    ASSERT_EQ(4, vin->wit_item_cnt);
    ASSERT_EQ(0, vin->witness[0].len);
    ASSERT_EQ(0, memcmp(WIT1, vin->witness[1].buf, sizeof(WIT1)));
    ASSERT_EQ(sizeof(WIT1), vin->witness[1].len);
    ASSERT_EQ(0, memcmp(WIT2, vin->witness[2].buf, sizeof(WIT2)));
    ASSERT_EQ(sizeof(WIT2), vin->witness[2].len);
    ASSERT_EQ(0, memcmp(WIT3, vin->witness[3].buf, sizeof(WIT3)));
    ASSERT_EQ(sizeof(WIT3), vin->witness[3].len);
    ASSERT_EQ(0xffffffff, vin->sequence);

    const btc_vout_t *vout = &tx.vout[0];
    ASSERT_EQ(BTC_BTC2SATOSHI(0.008), vout->value);
    ASSERT_EQ(0, memcmp(SCRIPTPK, vout->script.buf, sizeof(SCRIPTPK)));
    ASSERT_EQ(sizeof(SCRIPTPK), vout->script.len);

    utl_buf_t txbuf = UTL_BUF_INIT;
    btc_tx_write(&tx, &txbuf);
    ASSERT_EQ(0, memcmp(TX, txbuf.buf, sizeof(TX)));
    ASSERT_EQ(sizeof(TX), txbuf.len);
    utl_buf_free(&txbuf);
    btc_tx_free(&tx);
}

TEST_F(sw, sighash_p2wpkh)
{
    uint8_t txhash[BTC_SZ_HASH256];

    //5fb7d9c00b99fe93c1228e428985fbc8eaed1d27eb864711ad6b74a32f4eb8f1
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x04,
        0xdd, 0x9b, 0x67, 0x17, 0x75, 0x69, 0x55, 0x45,
        0xfe, 0x65, 0xe5, 0x6b, 0x50, 0x2c, 0x58, 0xaa,
        0x22, 0x93, 0x4c, 0x81, 0xe8, 0x84, 0x69, 0x8d,
        0x4d, 0x94, 0xcb, 0x34, 0xa0, 0xe2, 0x1f, 0x01,
        0x00, 0x00, 0x00, 0x17, 0x16, 0x00, 0x14, 0x12,
        0xfa, 0x3e, 0x2d, 0xde, 0x96, 0xa2, 0x16, 0x1b,
        0xad, 0x30, 0xa3, 0xeb, 0x0b, 0x92, 0x92, 0x08,
        0xe6, 0x4e, 0x59, 0xff, 0xff, 0xff, 0xff, 0x01,
        0xc0, 0x27, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x19, 0x76, 0xa9, 0x14, 0x7c, 0x7f, 0x7e, 0xcc,
        0xbc, 0x3f, 0x6d, 0xbb, 0xfa, 0x74, 0x04, 0x26,
        0x16, 0x96, 0xea, 0x57, 0xa2, 0xf9, 0x8f, 0xa4,
        0x88, 0xac, 0x02, 0x48, 0x30, 0x45, 0x02, 0x21,
        0x00, 0xfa, 0x1b, 0x5c, 0xd8, 0x34, 0x08, 0xb1,
        0x30, 0xae, 0x41, 0x10, 0x66, 0x16, 0xf5, 0x24,
        0x35, 0xdb, 0x3a, 0xa2, 0x8b, 0x8a, 0xf3, 0x9d,
        0x23, 0x71, 0x24, 0x37, 0xc2, 0x81, 0x34, 0x51,
        0xf8, 0x02, 0x20, 0x4b, 0xe4, 0x97, 0xa2, 0x97,
        0x48, 0x6e, 0xd8, 0xde, 0x66, 0x60, 0x7d, 0x0f,
        0x54, 0x66, 0x4b, 0xbe, 0x71, 0x95, 0xb6, 0xf4,
        0x6b, 0x14, 0x73, 0x26, 0xef, 0x9c, 0x55, 0xaa,
        0x09, 0x05, 0x6e, 0x01, 0x21, 0x02, 0x81, 0x00,
        0xca, 0x14, 0xc4, 0x4e, 0x2f, 0xe3, 0x63, 0xf9,
        0x6c, 0xff, 0x64, 0x98, 0x5d, 0xd6, 0xda, 0x11,
        0xe0, 0x52, 0x0e, 0xb4, 0x79, 0x59, 0x1b, 0xd1,
        0x41, 0xde, 0x27, 0x65, 0x66, 0x24, 0x00, 0x00,
        0x00, 0x00,
    };
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    utl_buf_t script_code = UTL_BUF_INIT;
    bool ret = btc_sw_scriptcode_p2wpkh_vin(&script_code, &tx.vin[0]);
    ASSERT_TRUE(ret);
    ret = btc_sw_sighash(txhash, &tx, 0, BTC_BTC2SATOSHI(0.007), &script_code);
    ASSERT_TRUE(ret);
    //printf("txhash=\n");
    //sw::DumpBin(txhash, sizeof(txhash));

    const uint8_t TXHASH[] = {
        0x29, 0xda, 0xa4, 0x66, 0xce, 0xe9, 0x86, 0xa8,
        0x17, 0x5a, 0x09, 0x2a, 0xe5, 0xaf, 0xf2, 0xd5,
        0x2f, 0x02, 0xac, 0x07, 0x74, 0x43, 0x5a, 0xfe,
        0xda, 0x70, 0x2a, 0xba, 0xd8, 0xf7, 0x23, 0x04,
    };
    ASSERT_EQ(0, memcmp(TXHASH, txhash, sizeof(TXHASH)));

    utl_buf_free(&script_code);
    btc_tx_free(&tx);
}


TEST_F(sw, sighash_p2wsh)
{
    uint8_t txhash[BTC_SZ_HASH256];

    //3c5f0cc27c54ffca22458a5ba840bccb99fcbde7823414d5c6b2c572932eff71
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x4e,
        0x1d, 0xcd, 0x4a, 0x4e, 0x10, 0xdb, 0x68, 0x00,
        0x3d, 0xe7, 0x84, 0x2b, 0x76, 0x9b, 0xa6, 0x2a,
        0x67, 0x7a, 0xfe, 0xec, 0xf8, 0x06, 0x0b, 0xd6,
        0x32, 0xca, 0xe6, 0xa4, 0x85, 0x78, 0x61, 0x01,
        0x00, 0x00, 0x00, 0x23, 0x22, 0x00, 0x20, 0x3a,
        0x33, 0x0a, 0xc7, 0xa5, 0xeb, 0xec, 0x6a, 0xcf,
        0x16, 0x09, 0x50, 0xfb, 0x88, 0x1f, 0x38, 0x8c,
        0xcf, 0xfe, 0x8d, 0x74, 0x88, 0x93, 0xd2, 0x78,
        0x75, 0x44, 0xb6, 0x5f, 0x3f, 0x71, 0x7e, 0xff,
        0xff, 0xff, 0xff, 0x01, 0x00, 0x35, 0x0c, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x19, 0x76, 0xa9, 0x14,
        0x09, 0xfc, 0x8b, 0xaa, 0xf5, 0x51, 0x22, 0x6f,
        0x79, 0x39, 0x2a, 0xca, 0x6f, 0x8f, 0x9c, 0x3a,
        0x7d, 0xb4, 0x78, 0x7c, 0x88, 0xac, 0x04, 0x00,
        0x47, 0x30, 0x44, 0x02, 0x20, 0x1f, 0x03, 0x0c,
        0xa8, 0x56, 0x31, 0xc8, 0x30, 0xfd, 0xa0, 0x6c,
        0xa6, 0x5e, 0xc6, 0xc0, 0x08, 0x11, 0xa5, 0xcb,
        0x23, 0x93, 0xf2, 0xe9, 0x59, 0xae, 0x5d, 0x9a,
        0xf9, 0xf2, 0x37, 0xc7, 0x0b, 0x02, 0x20, 0x28,
        0xd2, 0x15, 0x72, 0x1f, 0x5e, 0xa9, 0xce, 0x41,
        0xcd, 0xbf, 0x77, 0xf7, 0xd8, 0x7e, 0xef, 0xba,
        0xf3, 0x6a, 0x86, 0x5b, 0x9b, 0xae, 0xb9, 0x7b,
        0x6f, 0x61, 0xfb, 0xed, 0x8f, 0x4d, 0xa1, 0x01,
        0x48, 0x30, 0x45, 0x02, 0x21, 0x00, 0xe3, 0x80,
        0xd9, 0x0d, 0x21, 0x6e, 0xac, 0xde, 0x41, 0x6d,
        0x27, 0xbf, 0xed, 0xf0, 0xbb, 0x94, 0x0b, 0xeb,
        0x06, 0x94, 0xe7, 0xe0, 0x2b, 0x4e, 0x15, 0x7b,
        0x2b, 0x57, 0x00, 0x24, 0xe0, 0xe1, 0x02, 0x20,
        0x7f, 0xa2, 0x31, 0x27, 0x56, 0x8a, 0x51, 0xbc,
        0xf7, 0x45, 0x44, 0x5d, 0x0b, 0x2c, 0xd5, 0x99,
        0x60, 0x31, 0xb5, 0xfa, 0x9a, 0x4b, 0xb1, 0xc0,
        0x9a, 0x98, 0x24, 0x5e, 0x42, 0x5e, 0x56, 0x50,
        0x01, 0x69, 0x52, 0x21, 0x03, 0xd7, 0x98, 0x23,
        0x4d, 0xf0, 0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08,
        0xeb, 0x5a, 0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38,
        0xa0, 0xe6, 0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56,
        0xf1, 0x32, 0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c,
        0x5f, 0x25, 0x61, 0x40, 0x24, 0x56, 0xcc, 0x46,
        0x8f, 0xac, 0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32,
        0x2b, 0x74, 0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b,
        0x08, 0x42, 0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21,
        0x03, 0x06, 0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6,
        0x80, 0x69, 0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18,
        0x33, 0xf6, 0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b,
        0x19, 0xe6, 0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e,
        0x74, 0x53, 0xae, 0x00, 0x00, 0x00, 0x00,
    };
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    utl_buf_t script_code = UTL_BUF_INIT;
    bool ret = btc_sw_scriptcode_p2wsh_vin(&script_code, &tx.vin[0]);
    ASSERT_TRUE(ret);
    //printf("script_code=\n");
    //sw::DumpBin(script_code.buf, script_code.len);
    ret = btc_sw_sighash(txhash, &tx, 0, BTC_MBTC2SATOSHI(9), &script_code);
    ASSERT_TRUE(ret);
    //printf("txhash=\n");
    //sw::DumpBin(txhash, sizeof(txhash));

    const uint8_t TXHASH[] = {
        0x6b, 0x40, 0xb8, 0x0f, 0xe8, 0x52, 0xe1, 0xe5,
        0xcd, 0x3a, 0x91, 0xc7, 0x7f, 0x6a, 0x24, 0x1a,
        0x4d, 0xb9, 0x51, 0xca, 0xef, 0xf7, 0xa0, 0x4f,
        0x65, 0x50, 0xa6, 0x16, 0x94, 0x9e, 0x07, 0x91,
    };
    ASSERT_EQ(0, memcmp(TXHASH, txhash, sizeof(TXHASH)));

    utl_buf_free(&script_code);
    btc_tx_free(&tx);
}


TEST_F(sw, set_vin_p2wpkh)
{
    //空トランザクション
    //$ bitcoin-cli createrawtransaction '[{"txid":"1fe2a034cb944d8d6984e8814c9322aa582c506be565fe4555697517679bdd04","vout":1}]' '{"mrsEpNSjy54rLxwPCU8sp7skjVPDLsMgKh":0.006}'
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x01, 0x04, 0xdd, 0x9b,
        0x67, 0x17, 0x75, 0x69, 0x55, 0x45, 0xfe, 0x65,
        0xe5, 0x6b, 0x50, 0x2c, 0x58, 0xaa, 0x22, 0x93,
        0x4c, 0x81, 0xe8, 0x84, 0x69, 0x8d, 0x4d, 0x94,
        0xcb, 0x34, 0xa0, 0xe2, 0x1f, 0x01, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0xc0,
        0x27, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19,
        0x76, 0xa9, 0x14, 0x7c, 0x7f, 0x7e, 0xcc, 0xbc,
        0x3f, 0x6d, 0xbb, 0xfa, 0x74, 0x04, 0x26, 0x16,
        0x96, 0xea, 0x57, 0xa2, 0xf9, 0x8f, 0xa4, 0x88,
        0xac, 0x00, 0x00, 0x00, 0x00,
    };
    const uint8_t SIG[] = {
        0x30, 0x45, 0x02, 0x21, 0x00, 0xfa, 0x1b, 0x5c,
        0xd8, 0x34, 0x08, 0xb1, 0x30, 0xae, 0x41, 0x10,
        0x66, 0x16, 0xf5, 0x24, 0x35, 0xdb, 0x3a, 0xa2,
        0x8b, 0x8a, 0xf3, 0x9d, 0x23, 0x71, 0x24, 0x37,
        0xc2, 0x81, 0x34, 0x51, 0xf8, 0x02, 0x20, 0x4b,
        0xe4, 0x97, 0xa2, 0x97, 0x48, 0x6e, 0xd8, 0xde,
        0x66, 0x60, 0x7d, 0x0f, 0x54, 0x66, 0x4b, 0xbe,
        0x71, 0x95, 0xb6, 0xf4, 0x6b, 0x14, 0x73, 0x26,
        0xef, 0x9c, 0x55, 0xaa, 0x09, 0x05, 0x6e, 0x01,
    };
    const utl_buf_t sig = { (uint8_t *)SIG, sizeof(SIG) };
    const uint8_t PUB[] = {
        0x02, 0x81, 0x00, 0xca, 0x14, 0xc4, 0x4e, 0x2f,
        0xe3, 0x63, 0xf9, 0x6c, 0xff, 0x64, 0x98, 0x5d,
        0xd6, 0xda, 0x11, 0xe0, 0x52, 0x0e, 0xb4, 0x79,
        0x59, 0x1b, 0xd1, 0x41, 0xde, 0x27, 0x65, 0x66,
        0x24,
    };
    const uint8_t SCRIPT_SIG[] = {
        0x16, 0x00, 0x14, 0x12, 0xfa, 0x3e, 0x2d, 0xde,
        0x96, 0xa2, 0x16, 0x1b, 0xad, 0x30, 0xa3, 0xeb,
        0x0b, 0x92, 0x92, 0x08, 0xe6, 0x4e, 0x59,
    };
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));
    bool ret = btc_sw_set_vin_p2wpkh(&tx, 0, &sig, PUB);
    ASSERT_TRUE(ret);

    ASSERT_EQ(1, tx.vin_cnt);
    const btc_vin_t *vin = &tx.vin[0];
    ASSERT_EQ(0, memcmp(SCRIPT_SIG, vin->script.buf, sizeof(SCRIPT_SIG)));
    ASSERT_EQ(sizeof(SCRIPT_SIG), vin->script.len);
    ASSERT_EQ(2, vin->wit_item_cnt);
    ASSERT_EQ(0, memcmp(SIG, vin->witness[0].buf, sizeof(SIG)));
    ASSERT_EQ(sizeof(SIG), vin->witness[0].len);
    ASSERT_EQ(0, memcmp(PUB, vin->witness[1].buf, sizeof(PUB)));
    ASSERT_EQ(sizeof(PUB), vin->witness[1].len);

    btc_tx_free(&tx);
}


TEST_F(sw, set_vin_p2wsh)
{
    //空トランザクション
    //$ bitcoin-cli createrawtransaction '[{"txid":"617885a4e6ca32d60b06f8ecfe7a672aa69b762b84e73d0068db104e4acd1d4e","vout":1}]' '{"mgRkuxLK1FdyE2E4Hv55QA6obixAGAzVcr":0.008}'
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x01, 0x4e, 0x1d, 0xcd,
        0x4a, 0x4e, 0x10, 0xdb, 0x68, 0x00, 0x3d, 0xe7,
        0x84, 0x2b, 0x76, 0x9b, 0xa6, 0x2a, 0x67, 0x7a,
        0xfe, 0xec, 0xf8, 0x06, 0x0b, 0xd6, 0x32, 0xca,
        0xe6, 0xa4, 0x85, 0x78, 0x61, 0x01, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00,
        0x35, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19,
        0x76, 0xa9, 0x14, 0x09, 0xfc, 0x8b, 0xaa, 0xf5,
        0x51, 0x22, 0x6f, 0x79, 0x39, 0x2a, 0xca, 0x6f,
        0x8f, 0x9c, 0x3a, 0x7d, 0xb4, 0x78, 0x7c, 0x88,
        0xac, 0x00, 0x00, 0x00, 0x00,
    };
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    const uint8_t SIG1[] = {
        0x30, 0x44, 0x02, 0x20, 0x1f, 0x03, 0x0c, 0xa8,
        0x56, 0x31, 0xc8, 0x30, 0xfd, 0xa0, 0x6c, 0xa6,
        0x5e, 0xc6, 0xc0, 0x08, 0x11, 0xa5, 0xcb, 0x23,
        0x93, 0xf2, 0xe9, 0x59, 0xae, 0x5d, 0x9a, 0xf9,
        0xf2, 0x37, 0xc7, 0x0b, 0x02, 0x20, 0x28, 0xd2,
        0x15, 0x72, 0x1f, 0x5e, 0xa9, 0xce, 0x41, 0xcd,
        0xbf, 0x77, 0xf7, 0xd8, 0x7e, 0xef, 0xba, 0xf3,
        0x6a, 0x86, 0x5b, 0x9b, 0xae, 0xb9, 0x7b, 0x6f,
        0x61, 0xfb, 0xed, 0x8f, 0x4d, 0xa1, 0x01,
    };
    const uint8_t SIG2[] = {
        0x30, 0x45, 0x02, 0x21, 0x00, 0xe3, 0x80, 0xd9,
        0x0d, 0x21, 0x6e, 0xac, 0xde, 0x41, 0x6d, 0x27,
        0xbf, 0xed, 0xf0, 0xbb, 0x94, 0x0b, 0xeb, 0x06,
        0x94, 0xe7, 0xe0, 0x2b, 0x4e, 0x15, 0x7b, 0x2b,
        0x57, 0x00, 0x24, 0xe0, 0xe1, 0x02, 0x20, 0x7f,
        0xa2, 0x31, 0x27, 0x56, 0x8a, 0x51, 0xbc, 0xf7,
        0x45, 0x44, 0x5d, 0x0b, 0x2c, 0xd5, 0x99, 0x60,
        0x31, 0xb5, 0xfa, 0x9a, 0x4b, 0xb1, 0xc0, 0x9a,
        0x98, 0x24, 0x5e, 0x42, 0x5e, 0x56, 0x50, 0x01,
    };
    const uint8_t WIT[] = {
        0x52, 0x21, 0x03, 0xd7, 0x98, 0x23, 0x4d, 0xf0,
        0x07, 0xfe, 0x4d, 0x6f, 0x9c, 0x08, 0xeb, 0x5a,
        0x81, 0xc7, 0xca, 0xe1, 0x06, 0x38, 0xa0, 0xe6,
        0xc8, 0x40, 0xad, 0x80, 0xfd, 0x56, 0xf1, 0x32,
        0xa2, 0x4c, 0xaf, 0x21, 0x02, 0x1c, 0x5f, 0x25,
        0x61, 0x40, 0x24, 0x56, 0xcc, 0x46, 0x8f, 0xac,
        0x15, 0xe2, 0x15, 0x2d, 0xf0, 0x32, 0x2b, 0x74,
        0xef, 0xe9, 0x33, 0xce, 0x21, 0x2b, 0x08, 0x42,
        0xb4, 0x76, 0x77, 0x5d, 0x22, 0x21, 0x03, 0x06,
        0x84, 0xb3, 0x3c, 0xde, 0x5a, 0xd6, 0x80, 0x69,
        0x76, 0x22, 0x1a, 0x8e, 0xac, 0x18, 0x33, 0xf6,
        0x43, 0x23, 0x95, 0x03, 0xbf, 0x4b, 0x19, 0xe6,
        0x18, 0x2e, 0x82, 0x04, 0x95, 0x3e, 0x74, 0x53,
        0xae,
    };
    const uint8_t SCRIPT_SIG[] = {
        0x22, 0x00, 0x20, 0x3a, 0x33, 0x0a, 0xc7, 0xa5,
        0xeb, 0xec, 0x6a, 0xcf, 0x16, 0x09, 0x50, 0xfb,
        0x88, 0x1f, 0x38, 0x8c, 0xcf, 0xfe, 0x8d, 0x74,
        0x88, 0x93, 0xd2, 0x78, 0x75, 0x44, 0xb6, 0x5f,
        0x3f, 0x71, 0x7e,
    };
    const utl_buf_t wit0 = { NULL, 0 };
    const utl_buf_t wit1 = { (uint8_t *)SIG1, sizeof(SIG1) };
    const utl_buf_t wit2 = { (uint8_t *)SIG2, sizeof(SIG2) };
    const utl_buf_t wit3 = { (uint8_t *)WIT,  sizeof(WIT)  };
    const utl_buf_t *wits[] = { &wit0, &wit1, &wit2, &wit3 };
    bool ret = btc_sw_set_vin_p2wsh(&tx, 0, (const utl_buf_t **)wits, 4);
    ASSERT_TRUE(ret);
    ASSERT_EQ(1, tx.vin_cnt);
    const btc_vin_t *vin = &tx.vin[0];
    ASSERT_EQ(0, memcmp(SCRIPT_SIG, vin->script.buf, sizeof(SCRIPT_SIG)));
    ASSERT_EQ(sizeof(SCRIPT_SIG), vin->script.len);
    ASSERT_EQ(4, vin->wit_item_cnt);
    ASSERT_EQ(0, vin->witness[0].len);
    ASSERT_EQ(0, memcmp(SIG1, vin->witness[1].buf, sizeof(SIG1)));
    ASSERT_EQ(sizeof(SIG1), vin->witness[1].len);
    ASSERT_EQ(0, memcmp(SIG2, vin->witness[2].buf, sizeof(SIG2)));
    ASSERT_EQ(sizeof(SIG2), vin->witness[2].len);
    ASSERT_EQ(0, memcmp(WIT, vin->witness[3].buf, sizeof(WIT)));
    ASSERT_EQ(sizeof(WIT), vin->witness[3].len);

    btc_tx_free(&tx);
}


TEST_F(sw, sign_p2wpkh)
{
    //空トランザクション
    //$ bitcoin-cli createrawtransaction '[{"txid":"1fe2a034cb944d8d6984e8814c9322aa582c506be565fe4555697517679bdd04","vout":1}]' '{"mrsEpNSjy54rLxwPCU8sp7skjVPDLsMgKh":0.006}'
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x01, 0x04, 0xdd, 0x9b,
        0x67, 0x17, 0x75, 0x69, 0x55, 0x45, 0xfe, 0x65,
        0xe5, 0x6b, 0x50, 0x2c, 0x58, 0xaa, 0x22, 0x93,
        0x4c, 0x81, 0xe8, 0x84, 0x69, 0x8d, 0x4d, 0x94,
        0xcb, 0x34, 0xa0, 0xe2, 0x1f, 0x01, 0x00, 0x00,
        0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0xc0,
        0x27, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19,
        0x76, 0xa9, 0x14, 0x7c, 0x7f, 0x7e, 0xcc, 0xbc,
        0x3f, 0x6d, 0xbb, 0xfa, 0x74, 0x04, 0x26, 0x16,
        0x96, 0xea, 0x57, 0xa2, 0xf9, 0x8f, 0xa4, 0x88,
        0xac, 0x00, 0x00, 0x00, 0x00,
    };
    const uint8_t PRIV[] = {
        0xe0, 0xa2, 0x9c, 0xfd, 0x87, 0xbf, 0x70, 0xe5,
        0xf4, 0xe8, 0xc9, 0xbb, 0xf3, 0x47, 0xd0, 0x18,
        0x5c, 0xd2, 0x05, 0x7c, 0x7a, 0xf1, 0xf1, 0x9f,
        0x2c, 0x13, 0xd2, 0x64, 0xa7, 0x67, 0x81, 0x89,
    };
//    const uint8_t SIG[] = {
//        0x30, 0x45, 0x02, 0x21, 0x00, 0xfa, 0x1b, 0x5c,
//        0xd8, 0x34, 0x08, 0xb1, 0x30, 0xae, 0x41, 0x10,
//        0x66, 0x16, 0xf5, 0x24, 0x35, 0xdb, 0x3a, 0xa2,
//        0x8b, 0x8a, 0xf3, 0x9d, 0x23, 0x71, 0x24, 0x37,
//        0xc2, 0x81, 0x34, 0x51, 0xf8, 0x02, 0x20, 0x4b,
//        0xe4, 0x97, 0xa2, 0x97, 0x48, 0x6e, 0xd8, 0xde,
//        0x66, 0x60, 0x7d, 0x0f, 0x54, 0x66, 0x4b, 0xbe,
//        0x71, 0x95, 0xb6, 0xf4, 0x6b, 0x14, 0x73, 0x26,
//        0xef, 0x9c, 0x55, 0xaa, 0x09, 0x05, 0x6e, 0x01,
//    };
    const uint8_t PUB[] = {
        0x02, 0x81, 0x00, 0xca, 0x14, 0xc4, 0x4e, 0x2f,
        0xe3, 0x63, 0xf9, 0x6c, 0xff, 0x64, 0x98, 0x5d,
        0xd6, 0xda, 0x11, 0xe0, 0x52, 0x0e, 0xb4, 0x79,
        0x59, 0x1b, 0xd1, 0x41, 0xde, 0x27, 0x65, 0x66,
        0x24,
    };
    const uint8_t SCRIPT_SIG[] = {
        0x16, 0x00, 0x14, 0x12, 0xfa, 0x3e, 0x2d, 0xde,
        0x96, 0xa2, 0x16, 0x1b, 0xad, 0x30, 0xa3, 0xeb,
        0x0b, 0x92, 0x92, 0x08, 0xe6, 0x4e, 0x59,
    };
    btc_keys_t keys;
    btc_tx_t tx;
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    memcpy(keys.priv, PRIV, sizeof(PRIV));
    memcpy(keys.pub, PUB, sizeof(PUB));
    bool ret = btc_util_sign_p2wpkh(&tx, 0, BTC_MBTC2SATOSHI(7), &keys);
    ASSERT_TRUE(ret);

    ASSERT_EQ(1, tx.vin_cnt);
    const btc_vin_t *vin = &tx.vin[0];
    ASSERT_EQ(0, memcmp(SCRIPT_SIG, vin->script.buf, sizeof(SCRIPT_SIG)));
    ASSERT_EQ(sizeof(SCRIPT_SIG), vin->script.len);
    ASSERT_EQ(2, vin->wit_item_cnt);
//    ASSERT_EQ(0, memcmp(SIG, vin->witness[0].buf, sizeof(SIG)));
//    ASSERT_EQ(sizeof(SIG), vin->witness[0].len);
    ASSERT_EQ(0, memcmp(PUB, vin->witness[1].buf, sizeof(PUB)));
    ASSERT_EQ(sizeof(PUB), vin->witness[1].len);

    //verify
    ret = btc_sw_verify_p2wpkh_addr(&tx, 0, BTC_MBTC2SATOSHI(7), "2N1DdPiRC4nHb4y3tnhKFq48QQ44ArwMz6X");
    ASSERT_TRUE(ret);

    //他でチェックする
    printf("pubkey= ");
    sw::DumpBin(PUB, sizeof(PUB));
    printf("txhash= ");
    uint8_t txhash[BTC_SZ_HASH256];
    utl_buf_t script_code = UTL_BUF_INIT;
    ret = btc_sw_scriptcode_p2wpkh_vin(&script_code, &tx.vin[0]);
    ASSERT_TRUE(ret);
    ret = btc_sw_sighash(txhash, &tx, 0, BTC_BTC2SATOSHI(0.007), &script_code);
    ASSERT_TRUE(ret);
    sw::DumpBin(txhash, sizeof(txhash));
    utl_buf_free(&script_code);
    printf("sigData= ");
    sw::DumpBin(vin->witness[0].buf, vin->witness[0].len);

    btc_tx_free(&tx);
}


TEST_F(sw, verify_p2wpkh)
{
    btc_tx_t tx;

    //BIP143のP2SH-P2WPKH
    //  https://github.com/bitcoin/bips/blob/master/bip-0143.mediawiki#P2SHP2WPKH
    const uint8_t TX[] = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0xdb,
        0x6b, 0x1b, 0x20, 0xaa, 0x0f, 0xd7, 0xb2, 0x38,
        0x80, 0xbe, 0x2e, 0xcb, 0xd4, 0xa9, 0x81, 0x30,
        0x97, 0x4c, 0xf4, 0x74, 0x8f, 0xb6, 0x60, 0x92,
        0xac, 0x4d, 0x3c, 0xeb, 0x1a, 0x54, 0x77, 0x01,
        0x00, 0x00, 0x00, 0x17, 0x16, 0x00, 0x14, 0x79,
        0x09, 0x19, 0x72, 0x18, 0x6c, 0x44, 0x9e, 0xb1,
        0xde, 0xd2, 0x2b, 0x78, 0xe4, 0x0d, 0x00, 0x9b,
        0xdf, 0x00, 0x89, 0xfe, 0xff, 0xff, 0xff, 0x02,
        0xb8, 0xb4, 0xeb, 0x0b, 0x00, 0x00, 0x00, 0x00,
        0x19, 0x76, 0xa9, 0x14, 0xa4, 0x57, 0xb6, 0x84,
        0xd7, 0xf0, 0xd5, 0x39, 0xa4, 0x6a, 0x45, 0xbb,
        0xc0, 0x43, 0xf3, 0x5b, 0x59, 0xd0, 0xd9, 0x63,
        0x88, 0xac, 0x00, 0x08, 0xaf, 0x2f, 0x00, 0x00,
        0x00, 0x00, 0x19, 0x76, 0xa9, 0x14, 0xfd, 0x27,
        0x0b, 0x1e, 0xe6, 0xab, 0xca, 0xea, 0x97, 0xfe,
        0xa7, 0xad, 0x04, 0x02, 0xe8, 0xbd, 0x8a, 0xd6,
        0xd7, 0x7c, 0x88, 0xac, 0x02, 0x47, 0x30, 0x44,
        0x02, 0x20, 0x47, 0xac, 0x8e, 0x87, 0x83, 0x52,
        0xd3, 0xeb, 0xbd, 0xe1, 0xc9, 0x4c, 0xe3, 0xa1,
        0x0d, 0x05, 0x7c, 0x24, 0x17, 0x57, 0x47, 0x11,
        0x6f, 0x82, 0x88, 0xe5, 0xd7, 0x94, 0xd1, 0x2d,
        0x48, 0x2f, 0x02, 0x20, 0x21, 0x7f, 0x36, 0xa4,
        0x85, 0xca, 0xe9, 0x03, 0xc7, 0x13, 0x33, 0x1d,
        0x87, 0x7c, 0x1f, 0x64, 0x67, 0x7e, 0x36, 0x22,
        0xad, 0x40, 0x10, 0x72, 0x68, 0x70, 0x54, 0x06,
        0x56, 0xfe, 0x9d, 0xcb, 0x01, 0x21, 0x03, 0xad,
        0x1d, 0x8e, 0x89, 0x21, 0x2f, 0x0b, 0x92, 0xc7,
        0x4d, 0x23, 0xbb, 0x71, 0x0c, 0x00, 0x66, 0x2a,
        0xd1, 0x47, 0x01, 0x98, 0xac, 0x48, 0xc4, 0x3f,
        0x7d, 0x6f, 0x93, 0xa2, 0xa2, 0x68, 0x73, 0x92,
        0x04, 0x00, 0x00,
    };
    btc_tx_init(&tx);
    ASSERT_TRUE(btc_tx_read(&tx, TX, sizeof(TX)));

    //verify
    const uint8_t SCRIPTPK[] = {
        0xa9, 0x14, 0x47, 0x33, 0xf3, 0x7c, 0xf4, 0xdb,
        0x86, 0xfb, 0xc2, 0xef, 0xed, 0x25, 0x00, 0xb4,
        0xf4, 0xe4, 0x9f, 0x31, 0x20, 0x23, 0x87,
    };
    bool ret = btc_sw_verify_p2wpkh(&tx, 0, (uint64_t)1000000000, SCRIPTPK + 2);
    ASSERT_TRUE(ret);

    //簡易NGチェック
    ret = btc_sw_verify_p2wpkh(&tx, 0, (uint64_t)1000000000, SCRIPTPK + 1);
    ASSERT_FALSE(ret);
    ret = btc_sw_verify_p2wpkh(&tx, 0, (uint64_t)1000000001, SCRIPTPK + 2);
    ASSERT_FALSE(ret);

    btc_tx_free(&tx);
}
