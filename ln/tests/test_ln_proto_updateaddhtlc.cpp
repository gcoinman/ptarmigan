#include "gtest/gtest.h"
#include <string.h>
#include "tests/fff.h"
DEFINE_FFF_GLOBALS;


extern "C" {
#undef LOG_TAG
#include "../../utl/utl_thread.c"
#undef LOG_TAG
#include "../../utl/utl_log.c"
#include "../../utl/utl_dbg.c"
#include "../../utl/utl_buf.c"
#include "../../utl/utl_push.c"
#include "../../utl/utl_time.c"
#include "../../utl/utl_int.c"
#include "../../utl/utl_str.c"
#undef LOG_TAG
#include "../../btc/btc.c"
#include "../../btc/btc_buf.c"
#include "../../btc/btc_extkey.c"
#include "../../btc/btc_keys.c"
#include "../../btc/btc_sw.c"
#include "../../btc/btc_sig.c"
#include "../../btc/btc_script.c"
#include "../../btc/btc_tx.c"
#include "../../btc/btc_tx_buf.c"
#include "../../btc/btc_crypto.c"
#include "../../btc/segwit_addr.c"
#include "../../btc/btc_segwit_addr.c"
#include "../../btc/btc_test_util.c"

#undef LOG_TAG
#include "ln_derkey.c"
#include "ln_derkey_ex.c"
#include "ln_msg_anno.c"
#include "ln_msg_close.c"
//#include "ln_msg_establish.c"
#include "ln_msg_normalope.c"
#include "ln_msg_setupctl.c"
#include "ln_setupctl.c"
#include "ln_node.c"
#include "ln_onion.c"
#include "ln_script.c"
#include "ln_noise.c"
#include "ln_signer.c"
#include "ln_invoice.c"
#include "ln_print.c"
#include "ln_normalope.c"

#include "ln.c"
}

////////////////////////////////////////////////////////////////////////
//FAKE関数

FAKE_VOID_FUNC(ln_db_preimg_cur_close, void *);
FAKE_VALUE_FUNC(bool, ln_db_annocnlupd_load, utl_buf_t *, uint32_t *, uint64_t, uint8_t);
FAKE_VALUE_FUNC(bool, ln_db_preimg_del, const uint8_t *);
FAKE_VALUE_FUNC(bool, ln_db_preimg_cur_open, void **);
FAKE_VALUE_FUNC(bool, ln_db_preimg_cur_get, void *, bool *, ln_db_preimg_t *);
FAKE_VALUE_FUNC(bool, ln_db_channel_search, ln_db_func_cmp_t, void *);
FAKE_VALUE_FUNC(bool, ln_db_channel_search_readonly, ln_db_func_cmp_t, void *);
FAKE_VALUE_FUNC(bool, ln_db_phash_save, const uint8_t*, const uint8_t*, ln_htlctype_t, uint32_t);
FAKE_VALUE_FUNC(bool, ln_db_preimg_search, ln_db_func_preimg_t, void*);
FAKE_VALUE_FUNC(bool, ln_db_preimg_set_expiry, void *, uint32_t);

FAKE_VALUE_FUNC(bool, ln_msg_open_channel_write, utl_buf_t *, const ln_msg_open_channel_t *);
FAKE_VALUE_FUNC(bool, ln_msg_open_channel_read, ln_msg_open_channel_t*, const uint8_t*, uint16_t);
FAKE_VALUE_FUNC(bool, ln_msg_accept_channel_write, utl_buf_t *, const ln_msg_accept_channel_t *);
FAKE_VALUE_FUNC(bool, ln_msg_accept_channel_read, ln_msg_accept_channel_t *, const uint8_t *, uint16_t );
FAKE_VALUE_FUNC(bool, ln_msg_funding_created_write, utl_buf_t *, const ln_msg_funding_created_t *);
FAKE_VALUE_FUNC(bool, ln_msg_funding_created_read, ln_msg_funding_created_t *, const uint8_t *, uint16_t );
FAKE_VALUE_FUNC(bool, ln_msg_funding_signed_write, utl_buf_t *, const ln_msg_funding_signed_t *);
FAKE_VALUE_FUNC(bool, ln_msg_funding_signed_read, ln_msg_funding_signed_t *, const uint8_t *, uint16_t );
FAKE_VALUE_FUNC(bool, ln_comtx_create_to_remote, const ln_channel_t *, ln_commit_tx_t *, ln_close_force_t *, uint8_t **, uint64_t);


////////////////////////////////////////////////////////////////////////

class ln: public testing::Test {
protected:
    virtual void SetUp() {
        //utl_log_init_stderr();
        RESET_FAKE(ln_db_preimg_cur_close)
        RESET_FAKE(ln_db_annocnlupd_load)
        RESET_FAKE(ln_db_preimg_del)
        RESET_FAKE(ln_db_preimg_cur_open)
        RESET_FAKE(ln_db_preimg_cur_get)
        RESET_FAKE(ln_db_channel_search)
        RESET_FAKE(ln_db_channel_search_readonly)
        RESET_FAKE(ln_db_phash_save)
        RESET_FAKE(ln_db_preimg_search)
        RESET_FAKE(ln_db_preimg_set_expiry)
        RESET_FAKE(ln_msg_open_channel_read)
        RESET_FAKE(ln_msg_accept_channel_write)
        RESET_FAKE(ln_msg_accept_channel_read)
        RESET_FAKE(ln_msg_funding_created_write)
        RESET_FAKE(ln_msg_funding_created_read)
        RESET_FAKE(ln_msg_funding_signed_write)
        RESET_FAKE(ln_msg_funding_signed_read)
        RESET_FAKE(ln_comtx_create_to_remote)

        ln_comtx_create_to_remote_fake.return_val = true;
        utl_dbg_malloc_cnt_reset();
        btc_init(BTC_TESTNET, true);
    }

    virtual void TearDown() {
        ln_node_term();
        btc_term();
        ASSERT_EQ(0, utl_dbg_malloc_cnt());
    }

public:
    static void DumpBin(const uint8_t *pData, uint16_t Len)
    {
        for (uint16_t lp = 0; lp < Len; lp++) {
            printf("%02x", pData[lp]);
        }
        printf("\n");
    }
    static bool DumpCheck(const void *pData, uint32_t Len, uint8_t Fill)
    {
        bool ret = true;
        const uint8_t *p = (const uint8_t *)pData;
        for (uint32_t lp = 0; lp < Len; lp++) {
            if (p[lp] != Fill) {
                ret = false;
                break;
            }
        }
        return ret;
    }
    static void LnCallbackType(ln_channel_t *pChannel, ln_cb_t type, void *p_param) {
        (void)pChannel; (void)p_param;
        const char *p_str;
        switch (type) {
        case LN_CB_ERROR: p_str = "LN_CB_ERROR"; break;
        case LN_CB_INIT_RECV: p_str = "LN_CB_INIT_RECV"; break;
        case LN_CB_REESTABLISH_RECV: p_str = "LN_CB_REESTABLISH_RECV"; break;
        case LN_CB_SIGN_FUNDINGTX_REQ: p_str = "LN_CB_SIGN_FUNDINGTX_REQ"; break;
        case LN_CB_FUNDINGTX_WAIT: p_str = "LN_CB_FUNDINGTX_WAIT"; break;
        case LN_CB_FUNDINGLOCKED_RECV: p_str = "LN_CB_FUNDINGLOCKED_RECV"; break;
        case LN_CB_UPDATE_ANNODB: p_str = "LN_CB_UPDATE_ANNODB"; break;
        case LN_CB_ADD_HTLC_RECV_PREV: p_str = "LN_CB_ADD_HTLC_RECV_PREV"; break;
        case LN_CB_ADD_HTLC_RECV: p_str = "LN_CB_ADD_HTLC_RECV"; break;
        case LN_CB_FWD_ADDHTLC_START: p_str = "LN_CB_FWD_ADDHTLC_START"; break;
        case LN_CB_FULFILL_HTLC_RECV: p_str = "LN_CB_FULFILL_HTLC_RECV"; break;
        case LN_CB_FAIL_HTLC_RECV: p_str = "LN_CB_FAIL_HTLC_RECV"; break;
        case LN_CB_REV_AND_ACK_EXCG: p_str = "LN_CB_REV_AND_ACK_EXCG"; break;
        case LN_CB_PAYMENT_RETRY: p_str = "LN_CB_PAYMENT_RETRY"; break;
        case LN_CB_UPDATE_FEE_RECV: p_str = "LN_CB_UPDATE_FEE_RECV"; break;
        case LN_CB_SHUTDOWN_RECV: p_str = "LN_CB_SHUTDOWN_RECV"; break;
        case LN_CB_CLOSED_FEE: p_str = "LN_CB_CLOSED_FEE"; break;
        case LN_CB_CLOSED: p_str = "LN_CB_CLOSED"; break;
        case LN_CB_SEND_REQ: p_str = "LN_CB_SEND_REQ"; break;
        case LN_CB_SEND_QUEUE: p_str = "LN_CB_SEND_QUEUE"; break;
        case LN_CB_GET_LATEST_FEERATE: p_str = "LN_CB_GET_LATEST_FEERATE"; break;
        case LN_CB_GETBLOCKCOUNT: p_str = "LN_CB_GETBLOCKCOUNT"; break;
        default:
            p_str = "unknown";
        }
        printf("*** callback: %s(%d)\n", p_str, type);
    }
    static void LnInit(ln_channel_t *pChannel)
    {
        ln_anno_prm_t annoprm;

        memset(pChannel, 0xcc, sizeof(ln_channel_t));
        pChannel->noise.p_handshake = NULL;
        annoprm.cltv_expiry_delta = 10;
        annoprm.htlc_minimum_msat = 1000;
        annoprm.fee_base_msat = 20;
        annoprm.fee_prop_millionths = 200;
        ln_init(pChannel, &annoprm, (ln_callback_t)0x123456);
        pChannel->commit_tx_local.dust_limit_sat = BTC_DUST_LIMIT;
        pChannel->commit_tx_local.htlc_minimum_msat = 0;
        pChannel->commit_tx_local.max_accepted_htlcs = 10;
        pChannel->commit_tx_remote.dust_limit_sat = BTC_DUST_LIMIT;
        pChannel->commit_tx_remote.htlc_minimum_msat = 0;
        pChannel->commit_tx_remote.max_accepted_htlcs = 10;
        pChannel->our_msat = 1000000;
        pChannel->their_msat = 1000000;
        btc_tx_init(&pChannel->tx_funding);
        utl_buf_init(&pChannel->redeem_fund);
        pChannel->p_callback = LnCallbackType;
    }
};


namespace LN_UPDATE_ADD_HTLC_A {
    // -[update_add_htlc]-------------------------------
    // channel-id: 40fdde217bb2d6bc4c9e20c5e53193d071ebef7c13810419826af8862af122ad
    // id: 0
    // amount_msat: 100000
    // cltv_expiry: 447
    // payment_sha256: af06689876eb2016cd66e8e654f2e7fcf8b652d2ba300fb7263b5d9e859b4f98
    // onion_route: 000293dfc14c38d769f33422f2c6b453e38a6d9b24a1eb24940ad0a74387...
    const char WIF[] = "cVmfTVJVbkNZTf6EtPnvdcuG4EUSy1TmbmmcWFW3dA25H4J2sZ4P";
    const uint8_t PEER_NODE_ID[] = {
        0x03, 0xbd, 0x23, 0x06, 0x28, 0x76, 0xc8, 0x85, 0xea, 0xfd, 0x9e, 0xca, 0x32, 0x32, 0xc4, 0x5f,
        0xac, 0x5a, 0xfd, 0x65, 0x79, 0x4f, 0x97, 0x5a, 0xff, 0x6a, 0x59, 0xe3, 0x83, 0x5b, 0xeb, 0xc4,
        0x3f,
    };
    const uint64_t AMOUNT_MSAT = 100000;
    uint32_t CLTV_EXPIRY = 447;
    const uint8_t PREIMAGE[] = {
        0xea, 0x58, 0xf7, 0x87, 0x44, 0x1c, 0x5c, 0x2c, 0x0e, 0x91, 0xf3, 0x44, 0x60, 0xe9, 0x9d, 0x68,
        0x32, 0x72, 0x41, 0xbc, 0x9b, 0x60, 0x55, 0x4e, 0x0e, 0xdf, 0x33, 0xb1, 0x0f, 0x5f, 0x22, 0x14,
    };
    const uint8_t UPDATE_ADD_HTLC[] = {
        0x00, 0x80, 0x40, 0xfd, 0xde, 0x21, 0x7b, 0xb2, 0xd6, 0xbc, 0x4c, 0x9e, 0x20, 0xc5, 0xe5, 0x31,
        0x93, 0xd0, 0x71, 0xeb, 0xef, 0x7c, 0x13, 0x81, 0x04, 0x19, 0x82, 0x6a, 0xf8, 0x86, 0x2a, 0xf1,
        0x22, 0xad, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x86, 0xa0, 0xaf, 0x06, 0x68, 0x98, 0x76, 0xeb, 0x20, 0x16, 0xcd, 0x66, 0xe8, 0xe6, 0x54, 0xf2,
        0xe7, 0xfc, 0xf8, 0xb6, 0x52, 0xd2, 0xba, 0x30, 0x0f, 0xb7, 0x26, 0x3b, 0x5d, 0x9e, 0x85, 0x9b,
        0x4f, 0x98, 0x00, 0x00, 0x01, 0xbf, 0x00, 0x02, 0x93, 0xdf, 0xc1, 0x4c, 0x38, 0xd7, 0x69, 0xf3,
        0x34, 0x22, 0xf2, 0xc6, 0xb4, 0x53, 0xe3, 0x8a, 0x6d, 0x9b, 0x24, 0xa1, 0xeb, 0x24, 0x94, 0x0a,
        0xd0, 0xa7, 0x43, 0x87, 0xf5, 0x83, 0xd4, 0x2a, 0x28, 0xad, 0x25, 0x06, 0xbd, 0xa3, 0xe9, 0x4a,
        0x13, 0xf5, 0x62, 0xde, 0xbc, 0xb1, 0xf5, 0x82, 0x44, 0x5f, 0xf3, 0x6e, 0x39, 0x94, 0x72, 0x97,
        0x80, 0x9e, 0x9d, 0x71, 0x2e, 0xb7, 0x14, 0xc5, 0x50, 0x49, 0x6a, 0x3a, 0x47, 0x6a, 0x66, 0xb5,
        0xc7, 0x1e, 0x76, 0x02, 0x30, 0xd6, 0xaf, 0x64, 0x9b, 0x3e, 0x6f, 0x3d, 0xb5, 0xb1, 0xe6, 0x0f,
        0x48, 0x12, 0xce, 0x85, 0x0f, 0x54, 0x7e, 0xfd, 0x4a, 0xd1, 0x45, 0x75, 0xc7, 0x17, 0xbc, 0x2f,
        0x90, 0xbb, 0x39, 0xa3, 0xe3, 0x9a, 0x08, 0x82, 0x98, 0xe4, 0xf2, 0xab, 0x6a, 0x1c, 0xcf, 0x14,
        0xa6, 0xa9, 0x2c, 0xd5, 0x7d, 0x72, 0x3a, 0x56, 0x7f, 0x89, 0x90, 0x9b, 0xea, 0xf5, 0xcc, 0xf8,
        0x30, 0xb5, 0x6f, 0x6c, 0x71, 0xc0, 0x60, 0xba, 0x10, 0xa6, 0x2b, 0x60, 0x03, 0xc5, 0x68, 0xaf,
        0x59, 0x41, 0xdc, 0xc3, 0x66, 0x2b, 0xb9, 0x06, 0x3b, 0x57, 0x16, 0x82, 0x9c, 0x42, 0x7d, 0xd1,
        0x87, 0x18, 0x12, 0x3e, 0x27, 0x00, 0xb1, 0xf0, 0x2d, 0x6d, 0x32, 0x2c, 0x9d, 0x71, 0xe8, 0x8b,
        0x46, 0xe3, 0xf5, 0xf1, 0x46, 0x05, 0x7d, 0x4f, 0x43, 0x1a, 0x99, 0xe1, 0xdf, 0x07, 0xde, 0x14,
        0x8c, 0x53, 0x79, 0xc6, 0x3d, 0xb7, 0x84, 0x1b, 0xd5, 0xe8, 0x96, 0x72, 0x35, 0x66, 0x30, 0x24,
        0x53, 0x85, 0x03, 0x10, 0x4d, 0x8f, 0x22, 0x07, 0xa7, 0x13, 0x43, 0x28, 0xfb, 0xb3, 0x72, 0x51,
        0x1c, 0xd6, 0x46, 0x9d, 0x56, 0x01, 0x8c, 0x12, 0x1d, 0x44, 0x27, 0x8c, 0x7d, 0xb2, 0xd1, 0x6e,
        0x1a, 0x1c, 0xc5, 0xb8, 0x84, 0x37, 0xa5, 0xbd, 0x0d, 0xcd, 0xd4, 0xf6, 0x0f, 0x07, 0xe6, 0xda,
        0x61, 0xab, 0x98, 0xc6, 0xee, 0x06, 0xea, 0x1f, 0x92, 0x93, 0x47, 0x1b, 0xec, 0xff, 0xea, 0x63,
        0x97, 0x4d, 0x9c, 0x8c, 0xc4, 0x01, 0x0d, 0x27, 0x16, 0xc4, 0x35, 0x11, 0x35, 0xd9, 0xee, 0xbd,
        0x8c, 0x40, 0xd1, 0x28, 0x59, 0xb9, 0x92, 0x52, 0x89, 0xd9, 0xe8, 0x13, 0x07, 0x83, 0x32, 0xed,
        0xe0, 0xf2, 0x17, 0x2b, 0xf4, 0xa3, 0xc8, 0x36, 0x26, 0x1d, 0x16, 0x4c, 0x12, 0x79, 0x9c, 0xf6,
        0x23, 0x31, 0xcf, 0xab, 0x6d, 0x07, 0x84, 0x8a, 0x39, 0x50, 0x17, 0x67, 0x4e, 0x67, 0xd9, 0x5b,
        0xd2, 0xf3, 0x09, 0x38, 0xa0, 0x64, 0xc4, 0x1f, 0x3b, 0x0e, 0x2c, 0x09, 0xa5, 0xa1, 0xa6, 0xf5,
        0x27, 0x2a, 0xea, 0x42, 0x57, 0x06, 0xc1, 0xcd, 0xb9, 0xad, 0x2a, 0x3c, 0x33, 0xb5, 0xf0, 0xff,
        0xa1, 0xa8, 0x0f, 0x35, 0x5a, 0x19, 0x28, 0x9b, 0xc8, 0x97, 0x33, 0x13, 0x51, 0x1b, 0x6c, 0x0c,
        0x2f, 0xac, 0x1c, 0xeb, 0xf6, 0xcc, 0xca, 0x33, 0x4a, 0x27, 0x85, 0x3d, 0xae, 0x00, 0xb9, 0x2b,
        0x48, 0xd1, 0x03, 0xe4, 0xb7, 0x84, 0xde, 0xf3, 0x46, 0xe5, 0xe2, 0x33, 0xa5, 0x7e, 0x45, 0xab,
        0xa9, 0x73, 0xfd, 0x60, 0x44, 0x74, 0x0b, 0x7d, 0xea, 0xfc, 0x8c, 0x11, 0x43, 0x59, 0x3a, 0x35,
        0x1e, 0x6d, 0xeb, 0x75, 0x85, 0x6b, 0x2c, 0xc0, 0x7b, 0x68, 0x2e, 0x5f, 0xab, 0x8a, 0xda, 0x15,
        0x15, 0xc8, 0x18, 0x7f, 0xc4, 0x03, 0x25, 0x70, 0x75, 0x6c, 0x20, 0x43, 0xeb, 0xa5, 0xfc, 0x35,
        0x0b, 0x40, 0xc5, 0x4c, 0x42, 0x19, 0xf3, 0x10, 0xe2, 0x32, 0x1c, 0x76, 0x2a, 0x9b, 0x14, 0x1e,
        0x78, 0xb1, 0xb0, 0x31, 0xbd, 0x61, 0xb3, 0x7d, 0xe0, 0x9c, 0xc3, 0xf6, 0x75, 0x68, 0x13, 0xef,
        0xde, 0x89, 0xce, 0xe6, 0xe0, 0xaf, 0xd5, 0x72, 0x84, 0x70, 0x16, 0xfb, 0x3c, 0x30, 0x93, 0x82,
        0x7d, 0x88, 0x1e, 0xe7, 0x85, 0xbb, 0x34, 0x4a, 0xcb, 0x62, 0x91, 0xc4, 0x93, 0x45, 0x2c, 0x37,
        0xe2, 0xd0, 0xcd, 0x43, 0x57, 0x0e, 0x9a, 0x2e, 0x32, 0x2c, 0x70, 0xfe, 0xb7, 0x0c, 0xda, 0xd1,
        0x7d, 0x28, 0xb0, 0xed, 0x3b, 0x2d, 0xcb, 0x39, 0x16, 0x59, 0x8c, 0x74, 0xae, 0xf5, 0x0e, 0x1b,
        0x03, 0x36, 0x9c, 0x5b, 0x08, 0x0b, 0x0c, 0x5a, 0x58, 0x46, 0x61, 0xc5, 0xc5, 0x44, 0xb8, 0xb6,
        0x00, 0xa8, 0x0c, 0x0d, 0xb8, 0xd0, 0x73, 0x07, 0xbd, 0xed, 0xcc, 0x00, 0xd9, 0x5c, 0x3b, 0x7c,
        0x8f, 0xd0, 0xaa, 0xc2, 0x63, 0x75, 0x54, 0xcb, 0xf3, 0x78, 0x4e, 0xf0, 0x0f, 0xbc, 0x66, 0x01,
        0x98, 0x28, 0x6e, 0xa0, 0x08, 0x70, 0x99, 0x01, 0xbe, 0x6e, 0x34, 0x38, 0x76, 0xd0, 0x41, 0xbd,
        0x16, 0x66, 0x92, 0x19, 0x98, 0x10, 0x12, 0x79, 0x41, 0xf0, 0x4f, 0x6b, 0xb8, 0x95, 0xec, 0x7e,
        0xb5, 0xcf, 0x73, 0xcf, 0x83, 0xa5, 0x4a, 0x27, 0x55, 0x0d, 0x1d, 0xac, 0xf3, 0xb1, 0xa1, 0x00,
        0x39, 0xc1, 0xf7, 0x44, 0x19, 0x6f, 0x2c, 0x42, 0x2c, 0x61, 0x23, 0x59, 0x71, 0x30, 0xdb, 0xff,
        0xb3, 0x6e, 0xcb, 0x84, 0x0e, 0xb6, 0x60, 0x75, 0xfb, 0xf4, 0x10, 0x95, 0x8e, 0xcf, 0x0a, 0xf9,
        0xe1, 0x23, 0x81, 0x39, 0xde, 0x9b, 0x1c, 0x4c, 0x97, 0xe5, 0x06, 0x2e, 0xa6, 0x5b, 0x04, 0xba,
        0x73, 0xfe, 0xc1, 0xe5, 0x28, 0xd2, 0x9e, 0x8d, 0x15, 0x4b, 0x2d, 0xcd, 0xda, 0xe9, 0x51, 0xe2,
        0xda, 0x0b, 0xf6, 0x87, 0xd3, 0x64, 0xa5, 0xb5, 0x61, 0x5a, 0x7b, 0x1d, 0x06, 0x4a, 0x1e, 0x1a,
        0x6e, 0x50, 0x4b, 0xaa, 0xe6, 0x7e, 0x31, 0x28, 0x52, 0x19, 0x44, 0x52, 0xc8, 0xb5, 0x20, 0x2a,
        0x48, 0xf7, 0x71, 0x91, 0x9d, 0x11, 0xe7, 0xad, 0x8c, 0xbb, 0xc7, 0x61, 0x96, 0x03, 0x4c, 0x83,
        0xe8, 0x06, 0x00, 0x15, 0xf1, 0x8a, 0x06, 0x00, 0x7f, 0xd1, 0x50, 0xb7, 0xb4, 0xd4, 0x11, 0x0c,
        0x1f, 0xca, 0x63, 0x9f, 0xd4, 0xf5, 0x32, 0x57, 0x5d, 0x1c, 0xac, 0xdc, 0x46, 0x19, 0x1f, 0x52,
        0xa6, 0x08, 0x9f, 0xf1, 0x57, 0xbc, 0x1e, 0xd7, 0x35, 0x7f, 0x9d, 0xf7, 0x0c, 0xbb, 0x02, 0x4f,
        0x19, 0x9b, 0xe8, 0xee, 0x0f, 0xee, 0x18, 0x7f, 0x48, 0x17, 0x95, 0x98, 0xa2, 0x95, 0x6b, 0xb4,
        0x50, 0x65, 0x0a, 0xe7, 0xe6, 0x6b, 0x67, 0x0c, 0x29, 0x02, 0x53, 0x13, 0xe4, 0x69, 0xac, 0x80,
        0x7f, 0xff, 0x08, 0xe2, 0xbc, 0xbd, 0x98, 0xfe, 0x70, 0x9c, 0x8c, 0xfb, 0x2b, 0x0c, 0x80, 0x95,
        0x59, 0x7d, 0x57, 0x10, 0xe9, 0xe7, 0x72, 0x09, 0xcc, 0x18, 0xae, 0x12, 0x36, 0x52, 0x23, 0x29,
        0x97, 0xe1, 0x00, 0x95, 0xf8, 0x46, 0x90, 0x79, 0x69, 0xe9, 0x8e, 0x36, 0x8d, 0x29, 0xc9, 0x3f,
        0x0b, 0x69, 0x47, 0x8c, 0x39, 0xda, 0x79, 0x83, 0xc6, 0x52, 0xb1, 0x4a, 0x29, 0xb6, 0xaf, 0x17,
        0xd5, 0xd1, 0x0a, 0x81, 0x65, 0x02, 0x8b, 0x45, 0x68, 0x00, 0x90, 0xa8, 0x11, 0xc6, 0x2f, 0xa0,
        0xae, 0x9a, 0x25, 0x8f, 0x5b, 0x57, 0xb7, 0xe0, 0xd4, 0x18, 0xd3, 0x3d, 0xcd, 0x9f, 0xd3, 0x3e,
        0x1f, 0xcd, 0x26, 0x7b, 0xa2, 0x5e, 0x7d, 0x86, 0xac, 0xca, 0xbf, 0xbf, 0x7f, 0xe8, 0x2c, 0xa6,
        0x3c, 0x39, 0xa9, 0xe1, 0x2e, 0x06, 0x97, 0x92, 0x3a, 0x2a, 0xb4, 0x2b, 0x71, 0xca, 0x02, 0xe4,
        0x7a, 0xeb, 0xd7, 0xa7, 0x26, 0x14, 0xdc, 0x5c, 0x03, 0x39, 0x2c, 0xfe, 0x30, 0xb2, 0x89, 0x53,
        0xf7, 0x01, 0x55, 0xaf, 0x1c, 0xcb, 0xb7, 0x49, 0x1d, 0x28, 0xc0, 0x7e, 0x0d, 0x76, 0xdc, 0xae,
        0xa5, 0x20, 0x02, 0x19, 0x46, 0x5e, 0xdb, 0xb4, 0x96, 0xfd, 0x0b, 0xc9, 0x9a, 0x65, 0xa5, 0xbd,
        0xf5, 0xb6, 0x05, 0x5b, 0x45, 0xe5, 0x36, 0xe4, 0xa4, 0x99, 0xc4, 0x03, 0x5e, 0xf6, 0xdf, 0x3b,
        0x78, 0xb3, 0x0b, 0x35, 0x22, 0x80, 0xd5, 0xf1, 0x3d, 0x0d, 0x9e, 0xfa, 0xc4, 0x66, 0xe8, 0xb7,
        0xba, 0x66, 0xef, 0x06, 0x10, 0x8e, 0x8d, 0x77, 0xc1, 0xf2, 0xd5, 0x23, 0xc7, 0x8d, 0x5f, 0xc3,
        0x24, 0x87, 0xdb, 0x51, 0x8f, 0xa8, 0xfa, 0xc9, 0x87, 0x90, 0x9f, 0x26, 0x28, 0x15, 0x78, 0x29,
        0x61, 0x1c, 0x8d, 0x04, 0xe2, 0xcd, 0xdd, 0x71, 0xac, 0x3b, 0x6f, 0xe9, 0x5e, 0x4f, 0x3f, 0x2d,
        0xaf, 0xce, 0x45, 0x73, 0x8f, 0x33, 0x81, 0x68, 0x08, 0x2f, 0xa8, 0x67, 0xe7, 0x35, 0xda, 0x9c,
        0xd9, 0x04, 0xec, 0xde, 0xbb, 0x38, 0x15, 0xdb, 0xa1, 0x33, 0xd4, 0x8f, 0x5e, 0xf7, 0x45, 0x74,
        0x16, 0x54, 0x04, 0x53, 0x05, 0x19, 0xb5, 0x3f, 0x6f, 0x0a, 0x41, 0xd5, 0xb4, 0x45, 0x6c, 0x78,
        0x7c, 0x2a, 0x9b, 0x6b, 0x3f, 0x36, 0xf0, 0x85, 0x3b, 0x29, 0x00, 0x4b, 0x12, 0x72, 0x48, 0xa3,
        0xcd, 0x75, 0x25, 0x57, 0xaa, 0xd8, 0x70, 0x41, 0xa7, 0xc1, 0x9c, 0xdb, 0xbe, 0xc5, 0xb6, 0xfe,
        0x3f, 0x78, 0xc9, 0xeb, 0xb5, 0xf7, 0x06, 0x15, 0x5e, 0xfa, 0xc8, 0xb1, 0x2a, 0x15, 0xf5, 0x15,
        0xaf, 0x07, 0xbc, 0xf8, 0xea, 0xc0, 0x15, 0x0b, 0x77, 0x3f, 0xb3, 0x4f, 0x15, 0xf8, 0x0d, 0x2c,
        0x57, 0x63, 0xe9, 0x85, 0x07, 0x44, 0x40, 0xf7, 0xd3, 0xf1, 0x25, 0xed, 0xa2, 0x5e, 0x5c, 0xb7,
        0x31, 0x93, 0xaa, 0xcf, 0xa0, 0x7e, 0xbe, 0xf8, 0xca, 0xb0, 0xf5, 0x0c, 0x46, 0x9a, 0x3d, 0x88,
        0xa2, 0xc5, 0xdf, 0x52, 0x74, 0x30, 0xb0, 0xa8, 0xb2, 0xc4, 0x38, 0x19, 0xa0, 0x79, 0xdb, 0x5a,
        0xd3, 0x7a, 0xee, 0x66, 0x5c, 0x7e, 0x12, 0x21, 0x50, 0xe7, 0xa4, 0x6b, 0x81, 0x01, 0x0c, 0xd5,
        0x2d, 0x87, 0x0c, 0xa8, 0xe7, 0xad, 0x2a, 0x58, 0x3d, 0x77, 0x3a, 0x7e, 0x12, 0x0f, 0x29, 0xcc,
        0x18, 0x82, 0x88, 0xe6, 0x1f, 0xdd, 0x61, 0x36, 0x09, 0x79, 0x8c, 0xbd, 0x45, 0xd2, 0xe1, 0x68,
        0x50, 0x14, 0x96, 0x6b, 0xe7, 0x69, 0x20, 0x5f, 0xfb, 0x66, 0x5b, 0xcd, 0xa2, 0x4b, 0x1b, 0xfd,
        0x41, 0x37, 0x94, 0x6a, 0xdb, 0xa3, 0x62, 0x9f, 0xcd, 0x02, 0x10, 0xee, 0x1f, 0x3a, 0x37, 0x24,
        0xbf, 0x09, 0xb3, 0x11, 0x0a, 0x08, 0xb6, 0xe7, 0x0d, 0x8f, 0x58, 0x70,
    };
    const uint8_t CHANNEL_ID[] = {
        0x40, 0xfd, 0xde, 0x21, 0x7b, 0xb2, 0xd6, 0xbc, 0x4c, 0x9e, 0x20, 0xc5, 0xe5, 0x31, 0x93, 0xd0,
        0x71, 0xeb, 0xef, 0x7c, 0x13, 0x81, 0x04, 0x19, 0x82, 0x6a, 0xf8, 0x86, 0x2a, 0xf1, 0x22, 0xad,
    };
}

////////////////////////////////////////////////////////////////////////

TEST_F(ln, set_add_htlc1)
{
    ln_channel_t channel;
    LnInit(&channel);

    bool ret;

    uint64_t htlcid;
    utl_buf_t buf_reason = UTL_BUF_INIT;
    uint8_t onion[LN_SZ_ONION_ROUTE];
    uint64_t amount_msat = 123;
    uint32_t cltv_expiry = 98765;
    uint8_t payhash[BTC_SZ_HASH256];
    uint64_t prev_schid = 0x1234567;
    uint16_t prev_idx = 3;
    utl_buf_t shared_secret = UTL_BUF_INIT;

    memset(onion, 0xcc, LN_SZ_ONION_ROUTE);
    channel.noise.p_handshake = NULL;
    memset(payhash, 0xdd, BTC_SZ_HASH256);
    ln_signer_create_channel_keys(&channel);

    /*** TEST ***/
    ret = ln_add_htlc_set(&channel, &htlcid, &buf_reason, onion,
                amount_msat, cltv_expiry, payhash,
                prev_schid, prev_idx, &shared_secret);

    /*** CHECK ***/
    ASSERT_TRUE(ret);
    ASSERT_EQ(0, buf_reason.len);
    ASSERT_EQ(amount_msat, channel.cnl_add_htlc[0].amount_msat);
    ASSERT_EQ(cltv_expiry, channel.cnl_add_htlc[0].cltv_expiry);
    ASSERT_EQ(prev_schid, channel.cnl_add_htlc[0].prev_short_channel_id);
    ASSERT_EQ(prev_idx, channel.cnl_add_htlc[0].prev_idx);
    //
    ASSERT_TRUE(LN_HTLC_WILL_ADDHTLC(&channel.cnl_add_htlc[0]));
    ASSERT_FALSE(LN_HTLC_WILL_DELHTLC(&channel.cnl_add_htlc[0]));
    //
    ln_htlcflag_t *p_flag = &channel.cnl_add_htlc[0].stat.flag;
    ASSERT_EQ(LN_ADDHTLC_OFFER, p_flag->addhtlc);
    ASSERT_EQ(0, p_flag->delhtlc);
    ASSERT_EQ(0, p_flag->updsend);
    ASSERT_EQ(0, p_flag->comsend);
    ASSERT_EQ(0, p_flag->revrecv);
    ASSERT_EQ(0, p_flag->comrecv);
    ASSERT_EQ(0, p_flag->revsend);

    ASSERT_EQ(1000000, channel.our_msat);
    ASSERT_EQ(1000000, channel.their_msat);
    ASSERT_EQ(1, channel.htlc_id_num);
    ASSERT_EQ(0, channel.commit_tx_local.htlc_num);
    ASSERT_EQ(0, channel.commit_tx_remote.htlc_num);

    ln_term(&channel);
}


TEST_F(ln, create_add_htlc1)
{
    ln_channel_t channel;
    LnInit(&channel);

    bool ret;

    uint64_t htlcid;
    utl_buf_t buf_reason = UTL_BUF_INIT;
    uint8_t onion[LN_SZ_ONION_ROUTE];
    uint64_t amount_msat = 123;
    uint32_t cltv_expiry = 98765;
    uint8_t payhash[BTC_SZ_HASH256];
    uint64_t prev_schid = 0x1234567;
    uint16_t prev_idx = 3;
    utl_buf_t shared_secret = UTL_BUF_INIT;

    memset(onion, 0xcc, LN_SZ_ONION_ROUTE);
    memset(payhash, 0xdd, BTC_SZ_HASH256);
    ln_signer_create_channel_keys(&channel);

    ret = ln_add_htlc_set(&channel, &htlcid, &buf_reason, onion,
                amount_msat, cltv_expiry, payhash,
                prev_schid, prev_idx, &shared_secret);
    ASSERT_TRUE(ret);
    ASSERT_EQ(0, buf_reason.len);

    /*** TEST ***/
    utl_buf_t add = UTL_BUF_INIT;
    add_htlc_create(&channel, &add, 0);

    /*** CHECK ***/
    ASSERT_EQ(amount_msat, channel.cnl_add_htlc[0].amount_msat);
    ASSERT_EQ(cltv_expiry, channel.cnl_add_htlc[0].cltv_expiry);
    ASSERT_EQ(prev_schid, channel.cnl_add_htlc[0].prev_short_channel_id);
    ASSERT_EQ(prev_idx, channel.cnl_add_htlc[0].prev_idx);
    //
    ASSERT_FALSE(LN_HTLC_WILL_ADDHTLC(&channel.cnl_add_htlc[0]));
    ASSERT_FALSE(LN_HTLC_WILL_DELHTLC(&channel.cnl_add_htlc[0]));
    //
    ln_htlcflag_t *p_flag = &channel.cnl_add_htlc[0].stat.flag;
    ASSERT_EQ(LN_ADDHTLC_OFFER, p_flag->addhtlc);
    ASSERT_EQ(0, p_flag->delhtlc);
    ASSERT_EQ(1, p_flag->updsend);
    ASSERT_EQ(0, p_flag->comsend);
    ASSERT_EQ(0, p_flag->revrecv);
    ASSERT_EQ(0, p_flag->comrecv);
    ASSERT_EQ(0, p_flag->revsend);

    ASSERT_EQ(1000000, channel.our_msat);
    ASSERT_EQ(1000000, channel.their_msat);
    ASSERT_EQ(1, channel.htlc_id_num);
    ASSERT_EQ(0, channel.commit_tx_local.htlc_num);
    ASSERT_EQ(0, channel.commit_tx_remote.htlc_num);

    utl_buf_free(&add);
    ln_term(&channel);
}


//fulfill
TEST_F(ln, update_add_htlc_recv1)
{
    class dummy {
    public:
        static bool ln_db_preimg_cur_open(void **ppCur) {
            return true;
        }
        static bool ln_db_preimg_cur_get(void *pCur, bool *pDetect, ln_db_preimg_t *pPreImg) {
            *pDetect = true;
            pPreImg->amount_msat = LN_UPDATE_ADD_HTLC_A::AMOUNT_MSAT;
            memcpy(pPreImg->preimage, LN_UPDATE_ADD_HTLC_A::PREIMAGE, LN_SZ_PREIMAGE);
            pPreImg->creation_time = 1538375408;
            pPreImg->expiry = LN_UPDATE_ADD_HTLC_A::CLTV_EXPIRY;
            return true;
        }
        static void callback(ln_channel_t *pChannel, ln_cb_t type, void *p_param) {
            (void)pChannel;
            if (type == LN_CB_GETBLOCKCOUNT) {
                int32_t *p_height = (int32_t *)p_param;
                *p_height = 438;
            }
        }
    };
    ln_db_preimg_cur_open_fake.custom_fake = dummy::ln_db_preimg_cur_open;
    ln_db_preimg_cur_get_fake.custom_fake = dummy::ln_db_preimg_cur_get;


    ln_channel_t channel;
    LnInit(&channel);

    btc_chain_t chain;
    btc_keys_wif2keys(&mNode.keys, &chain, LN_UPDATE_ADD_HTLC_A::WIF);
    channel.p_callback = dummy::callback;
    memcpy(channel.peer_node_id, LN_UPDATE_ADD_HTLC_A::PEER_NODE_ID, BTC_SZ_PUBKEY);
    memcpy(channel.channel_id, LN_UPDATE_ADD_HTLC_A::CHANNEL_ID, sizeof(LN_UPDATE_ADD_HTLC_A::CHANNEL_ID));
    //utl_buf_alloccopy(&channel.cnl_add_htlc[0].buf_shared_secret,

    bool ret;

    /*** TEST ***/
    ret = ln_update_add_htlc_recv(&channel, LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC, sizeof(LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC));

    /*** CHECK ***/
    ASSERT_TRUE(ret);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::AMOUNT_MSAT, channel.cnl_add_htlc[0].amount_msat);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::CLTV_EXPIRY, channel.cnl_add_htlc[0].cltv_expiry);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_short_channel_id);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_idx);
    //
    ASSERT_FALSE(LN_HTLC_WILL_ADDHTLC(&channel.cnl_add_htlc[0]));
    ASSERT_FALSE(LN_HTLC_WILL_DELHTLC(&channel.cnl_add_htlc[0]));
    //
    ln_htlcflag_t *p_flag = &channel.cnl_add_htlc[0].stat.flag;
    ASSERT_EQ(LN_ADDHTLC_RECV, p_flag->addhtlc);
    ASSERT_EQ(0, p_flag->delhtlc);
    ASSERT_EQ(LN_DELHTLC_FULFILL, p_flag->fin_delhtlc);
    ASSERT_EQ(0, p_flag->comsend);
    ASSERT_EQ(0, p_flag->revrecv);
    ASSERT_EQ(0, p_flag->comrecv);
    ASSERT_EQ(0, p_flag->revsend);

    ASSERT_EQ(1000000, channel.our_msat);
    ASSERT_EQ(1000000, channel.their_msat);
    ASSERT_EQ(0, channel.htlc_id_num);
    ASSERT_EQ(0, channel.commit_tx_local.htlc_num);
    ASSERT_EQ(0, channel.commit_tx_remote.htlc_num);

    ln_term(&channel);
}


//fail
TEST_F(ln, update_add_htlc_recv2)
{
    class dummy {
    public:
        static bool ln_db_preimg_cur_open(void **ppCur) {
            return true;
        }
        static bool ln_db_preimg_cur_get(void *pCur, bool *pDetect, ln_db_preimg_t *pPreImg) {
            *pDetect = true;
            pPreImg->amount_msat = LN_UPDATE_ADD_HTLC_A::AMOUNT_MSAT;
            memcpy(pPreImg->preimage, LN_UPDATE_ADD_HTLC_A::PREIMAGE, LN_SZ_PREIMAGE);
            pPreImg->creation_time = 1538375408;
            pPreImg->expiry = LN_UPDATE_ADD_HTLC_A::CLTV_EXPIRY;
            return true;
        }
        static void callback(ln_channel_t *pChannel, ln_cb_t type, void *p_param) {
            (void)pChannel;
            if (type == LN_CB_GETBLOCKCOUNT) {
                int32_t *p_height = (int32_t *)p_param;
                //cltv_expiry too soon(final)
                *p_height = 440;
            }
        }
    };
    ln_db_preimg_cur_open_fake.custom_fake = dummy::ln_db_preimg_cur_open;
    ln_db_preimg_cur_get_fake.custom_fake = dummy::ln_db_preimg_cur_get;


    ln_channel_t channel;
    LnInit(&channel);

    btc_chain_t chain;
    btc_keys_wif2keys(&mNode.keys, &chain, LN_UPDATE_ADD_HTLC_A::WIF);
    channel.p_callback = dummy::callback;
    memcpy(channel.peer_node_id, LN_UPDATE_ADD_HTLC_A::PEER_NODE_ID, BTC_SZ_PUBKEY);
    memcpy(channel.channel_id, LN_UPDATE_ADD_HTLC_A::CHANNEL_ID, sizeof(LN_UPDATE_ADD_HTLC_A::CHANNEL_ID));
    //utl_buf_alloccopy(&channel.cnl_add_htlc[0].buf_shared_secret,

    bool ret;

    /*** TEST ***/
    ret = ln_update_add_htlc_recv(&channel, LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC, sizeof(LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC));

    /*** CHECK ***/
    ASSERT_TRUE(ret);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::AMOUNT_MSAT, channel.cnl_add_htlc[0].amount_msat);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::CLTV_EXPIRY, channel.cnl_add_htlc[0].cltv_expiry);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_short_channel_id);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_idx);
    //
    ASSERT_FALSE(LN_HTLC_WILL_ADDHTLC(&channel.cnl_add_htlc[0]));
    ASSERT_FALSE(LN_HTLC_WILL_DELHTLC(&channel.cnl_add_htlc[0]));
    //
    ln_htlcflag_t *p_flag = &channel.cnl_add_htlc[0].stat.flag;
    ASSERT_EQ(LN_ADDHTLC_RECV, p_flag->addhtlc);
    ASSERT_EQ(0, p_flag->delhtlc);
    ASSERT_EQ(LN_DELHTLC_FAIL, p_flag->fin_delhtlc);
    ASSERT_EQ(0, p_flag->updsend);
    ASSERT_EQ(0, p_flag->comsend);
    ASSERT_EQ(0, p_flag->revrecv);
    ASSERT_EQ(0, p_flag->comrecv);
    ASSERT_EQ(0, p_flag->revsend);

    ASSERT_EQ(1000000, channel.our_msat);
    ASSERT_EQ(1000000, channel.their_msat);
    ASSERT_EQ(0, channel.htlc_id_num);
    ASSERT_EQ(0, channel.commit_tx_local.htlc_num);
    ASSERT_EQ(0, channel.commit_tx_remote.htlc_num);

    ln_term(&channel);
}


//malformed
TEST_F(ln, update_add_htlc_recv3)
{
    class dummy {
    public:
        static bool ln_db_preimg_cur_open(void **ppCur) {
            return false;
        }
    };
    ln_db_preimg_cur_open_fake.custom_fake = dummy::ln_db_preimg_cur_open;


    ln_channel_t channel;
    LnInit(&channel);

    memcpy(channel.channel_id, LN_UPDATE_ADD_HTLC_A::CHANNEL_ID, sizeof(LN_UPDATE_ADD_HTLC_A::CHANNEL_ID));

    bool ret;

    /*** TEST ***/
    ret = ln_update_add_htlc_recv(&channel, LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC, sizeof(LN_UPDATE_ADD_HTLC_A::UPDATE_ADD_HTLC));

    /*** CHECK ***/
    ASSERT_TRUE(ret);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::AMOUNT_MSAT, channel.cnl_add_htlc[0].amount_msat);
    ASSERT_EQ(LN_UPDATE_ADD_HTLC_A::CLTV_EXPIRY, channel.cnl_add_htlc[0].cltv_expiry);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_short_channel_id);
    // ASSERT_EQ(0, channel.cnl_add_htlc[0].prev_idx);
    //
    ASSERT_FALSE(LN_HTLC_WILL_ADDHTLC(&channel.cnl_add_htlc[0]));
    ASSERT_FALSE(LN_HTLC_WILL_DELHTLC(&channel.cnl_add_htlc[0]));
    //
    ln_htlcflag_t *p_flag = &channel.cnl_add_htlc[0].stat.flag;
    ASSERT_EQ(LN_ADDHTLC_RECV, p_flag->addhtlc);
    ASSERT_EQ(0, p_flag->delhtlc);
    ASSERT_EQ(LN_DELHTLC_MALFORMED, p_flag->fin_delhtlc);
    ASSERT_EQ(0, p_flag->updsend);
    ASSERT_EQ(0, p_flag->comsend);
    ASSERT_EQ(0, p_flag->revrecv);
    ASSERT_EQ(0, p_flag->comrecv);
    ASSERT_EQ(0, p_flag->revsend);

    ASSERT_EQ(1000000, channel.our_msat);
    ASSERT_EQ(1000000, channel.their_msat);
    ASSERT_EQ(0, channel.htlc_id_num);
    ASSERT_EQ(0, channel.commit_tx_local.htlc_num);
    ASSERT_EQ(0, channel.commit_tx_remote.htlc_num);

    ln_term(&channel);
}
