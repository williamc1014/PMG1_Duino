
#ifdef __cplusplus
extern "C" {
#endif

#include "cy_usbpd_common.h"
#include "cy_pdstack_common.h"
#include "cy_usbpd_typec.h"
#include "cy_pdstack_dpm.h"
#include "cy_usbpd_vbus_ctrl.h"
#include "cy_usbpd_phy.h"
#include "cy_pdutils_sw_timer.h"
#include "user_timer_id.h"

extern cy_stc_pdstack_context_t gl_PdStackPort0Ctx;
#if PMGDUINO_BOARD
extern cy_stc_pdstack_context_t gl_PdStackPort1Ctx;
#endif


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class USBPD
{
    private:
    uint8_t                     portIdx;
    cy_stc_pdstack_context_t    *ctx;
    cy_pd_pd_do_t               iSprSnkPdo[7] = {0};
    uint8_t                     iSprSnkPdoCnt = 0;
    uint8_t                     iSprSnkMask = 0x01;
#if PMGDUINO_BOARD
    cy_pdpd_do_t                iSprSrcPdo[7];
    uint8_t                     iSprSrcMask;
    uint8_t                     iSprSrcPdoCnt;
#endif

    public:
    
    bool        usbpdConnected;
    uint8_t     usbpdPowerRole;
    bool        typecAttached;
    uint8_t     typecPolarity;
    
    USBPD() {}
    USBPD(uint8_t port) {
        portIdx = port;
        if (portIdx == 0)
            ctx = &gl_PdStackPort0Ctx;
#if PMGDUINO_BOARD
        else
            ctx = &gl_PdStackPort1Ctx;
#endif
    }
    void begin(void);
    void end(void);

    void updateStatus(void);
    uint8_t getCurrentSinkRdo(void);

    bool setSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent);
    bool setSinkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent, uint32_t maxCurrent);
    bool updateSrcPdo(void);
    bool updateSinkPdo(void);
};

extern USBPD usbpd0;
#if PMGDUINO_BOARD
extern USBPD usbpd1;
#endif

#endif