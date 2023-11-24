#ifdef __cplusplus
extern "C" {
#endif

#include "cy_usbpd_common.h"
#include "cy_pdstack_common.h"
#include "cy_usbpd_typec.h"
#include "cy_pdstack_dpm.h"
#include "cy_usbpd_vbus_ctrl.h"
#include "cy_usbpd_phy.h"

extern cy_stc_pdstack_context_t gl_PdStackPort0Ctx;
#if PMGDUINO_BOARD
extern cy_stc_pdstack_context_t gl_PdStackPort1Ctx;
#endif


#ifdef __cplusplus
}
#endif

#include <usbpd.h>


void USBPD::updateStatus(void)
{
#if PMGDUINO_BOARD
    cy_stc_pdstack_context_t *ctx = (portIdx == 0) ? &gl_PdStackPort0Ctx : &gl_PdStackPort1Ctx ;
#else
    cy_stc_pdstack_context_t *ctx = &gl_PdStackPort0Ctx;
#endif

    typecAttached = ctx->dpmConfig.attach;
    typecPolarity = ctx->dpmConfig.polarity;
    usbpdConnected = ctx->dpmConfig.connect;

    if (usbpdConnected)
    {
        usbpdPowerRole = ctx->dpmConfig.curPortRole;
    }
}

bool USBPD::setSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent)
{

    return true;
}

bool USBPD::setSinkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t operCurrent)
{

    return true;
}

bool USBPD::updateSrcPdo(void)
{

    return true;
}

bool USBPD::updateSinkPdo(void)
{
    
    return true;
}

USBPD usbpd0 = USBPD(0);
#if PMGDUINO_BOARD
USBPD usbpd1 = USBPD(1);
#endif
