#include <usbpd.h>

void (*usbpd_ebent_handler[APP_TOTAL_EVENTS])(void) = {NULL};

void srcCapChangeRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, SRC_CAP_CHANGE_RETRY_TIMER), 
                                 SRC_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                 srcCapChangeRetryCbk);
    }
}

void snkCapChangeRetryCbk(cy_timer_id_t id, void *ptrContext)
{
    cy_stc_pdstack_context_t *ptrPdStackContext = (cy_stc_pdstack_context_t*) ptrContext;

    if(Cy_PdStack_Dpm_SendPdCommand(ptrPdStackContext, CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    {
    	Cy_PdUtils_SwTimer_Start(ptrPdStackContext->ptrTimerContext, 
                                 ptrPdStackContext, 
                                 GET_USER_TIMER_ID(ptrPdStackContext, SNK_CAP_CHANGE_RETRY_TIMER), 
                                 SNK_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                 snkCapChangeRetryCbk);
    }
}

void updatePeerSrcPdo(uint8_t port, const cy_stc_pdstack_pd_packet_t* srcCap)
{

    uint8_t numPdo;
    uint8_t i;
    
    if ((numPdo = srcCap->len) != 0)
    {
        for (i=0; i<numPdo; i++)
        {
            if (port == 0)
            {
                usbpd0.iPartnerSrcPdo[i].val = srcCap->dat[i].val;
            }
#if PMGDUINO_BOARD
            else
            {
                usbpd1.iPartnerSrcPdo[i].val = srcCap->dat[i].val;
            }
#endif
        }
    }
    
    if (port == 0)
    {
        usbpd0.iPartnerSrcPdoCnt = numPdo;
    }
#if PMGDUINO_BOARD
    else 
    {
        usbpd1.iPartnerSrcPdoCnt = numPdo;
    }
#endif
}

bool enterVBUSSafe5V(uint8_t port)
{
    uint8_t i;
    uint8_t cnt;

    if (port == 0)
    {
        usbpd0.updateStatus();

        if (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK)  
            cnt = usbpd0.getSnkPdoNum();
#if PMGDUINO_BOARD
        else
            cnt = usbpd0.getSrcPdoNum();
#endif

        if ( usbpd0.contractExisted && 
             (
                (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK && usbpd0.getCurrentSnkPdo() != 0) 
#if PMGDUINO_BOARD
             || (usbpd0.powerRole == CY_PD_PRT_ROLE_SOURCE && usbpd0.getCurrentSrcRdo() != 0)
#endif
             )
           )
        {
            if (usbpd0.powerRole == CY_PD_PRT_ROLE_SINK)
            {
                for (i=1; i<cnt; i++)
                    usbpd0.removeSnkPdo(i);
                usbpd0.updateSnkPdo();
                return true;
            }
#if PMGDUINO_BOARD
            else
            {
                for (i=1; i<cnt; i++)
                    usbpd0.removeSrcPdo(i);
                usbpd0.updateSrcPdo();
                return true;
            }
#endif
        }
    }
#if PMGDUINO_BOARD
    else
    {
        usbpd1.updateStatus();

        if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
            cnt = usbpd1.getSnkPdoNum();
        else
            cnt = usbpd1.getSrcPdoNum();

        if ( usbpd1.contractExisted && 
             (
             (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK && usbpd1.getCurrentSnkPdo() != 0) ||
             (usbpd1.powerRole == CY_PD_PRT_ROLE_SOURCE && usbpd1.getCurrentSrcRdo() != 0)
             )
           )
        {
            if (usbpd1.powerRole == CY_PD_PRT_ROLE_SINK)
            {
                for (i=1; i<cnt; i++)
                    usbpd1.removeSnkPdo(i);
                usbpd1.updateSnkPdo();
                return true;
            }
            else
            {
                for (i=1; i<cnt; i++)
                    usbpd1.removeSrcPdo(i);
                usbpd1.updateSrcPdo();
                return true;
            }
        }
    }
#endif
    return false;
}

static void usbPdCmdCbk(cy_stc_pdstack_context_t *ptrPdStackContext,
					  cy_en_pdstack_resp_status_t resp,
					  const cy_stc_pdstack_pd_packet_t* pkt_ptr)
{
	uint8_t port = ((cy_stc_pdstack_context_t *)ptrPdStackContext)->port;
}

void USBPD::begin(void)
{
    memcpy((void *)iSprSnkPdo, (const void *)ctx->dpmStat.snkPdo, sizeof(cy_pd_pd_do_t)*7);
    iSprSnkMask = ctx->dpmStat.snkPdoMask;
    iSprSnkPdoCnt = ctx->dpmStat.snkPdoCount;
#if PMGDUINO_BOARD
    memcpy((void *)iSprSrcPdo, (const void *)ctx->dpmStat.srcPdo, sizeof(cy_pd_pd_do_t)*7);
    iSprSrcMask = ctx->dpmStat.srcPdoMask;
    iSprSrcPdoCnt = ctx->dpmStat.srcPdoCount;
#endif
    portInitated[portIdx] = true;
}

void USBPD::end(void)
{
    // do nothing
}

void USBPD::updateStatus(void)
{
    attached = ctx->dpmConfig.attach;
    polarity = ctx->dpmConfig.polarity;
    contractExisted = ctx->dpmConfig.connect;
    powerRole = ctx->dpmConfig.curPortRole;
    attachedDeviceType = (uint8_t)ctx->dpmConfig.attachedDev;
}

uint8_t USBPD::getCurrentSrcRdo(void)
{
    uint8_t rdoNum = 0xFF;

    if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SOURCE))
    {
        for (uint8_t i=0; i<7; i++)
        {
            if (ctx->dpmStat.srcSelPdo.fixed_src.voltage == ctx->dpmStat.srcPdo[i].fixed_src.voltage)
            {
                rdoNum = i;
                break;
            }
        }
    }

    return rdoNum;
}

uint8_t USBPD::getCurrentSnkPdo(void)
{
    uint8_t rdoNum = 0xFF;

    if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SINK))
    {
        for (uint8_t i=0; i<7; i++)
        {
            if (ctx->dpmStat.snkSelPdo.fixed_snk.voltage == ctx->dpmStat.snkPdo[i].fixed_snk.voltage)
            {
                rdoNum = i;
                break;
            }
        }
    }

    return rdoNum;
}

#if PMGDUINO_BOARD  
bool USBPD::setSrcPdo(uint8_t pdoNum, src_cap_t srcCap)
{  
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum > 7)
        return false;
    
    memcpy((void *)&iSprSrcPdo[pdoNum], (const void *)&srcCap, sizeof(uint32_t));
    
    iSprSrcMask |= (1 << pdoNum);

    for (i=0; i<7; i++)
    {
        if (iSprSrcMask & (1 << i))
            pdoCnt ++;
    }

    iSprSrcPdoCnt = pdoCnt;

    return true;
}
#endif

bool USBPD::setSnkPdo(uint8_t pdoNum, snk_cap_t snkCap)
{
    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum == 0 || pdoNum > 7)
        return false;
    
    memcpy((void *)&iSprSnkPdo[pdoNum], (const void*)&snkCap, sizeof(uint32_t));
    
    iSprSnkMask |= (1 << pdoNum);

    for (i=0; i<7; i++)
    {
        if (iSprSnkMask & (1 << i))
            pdoCnt ++;
    }

    iSprSnkPdoCnt = pdoCnt;

    if (iSprSnkPdoCnt > 1)
        iSprSnkPdo[0].fixed_snk_do_t.highCap = 1;

    return true;
}

#if PMGDUINO_BOARD
bool USBPD::addFixedSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent, uint8_t peakCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.fixed_src_do_t.voltage = voltage / 50;
    srcCap.fixed_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.fixed_src_do_t.pkCurrent = peakCurrent;
    srcCap.fixed_src_do_t.supplyType = 0;

    return (setSrcPdo(pdoNum, srcCap));
}

void USBPD::setSrcUSBCommFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.usbCommCap = enable ? 1 : 0;
}

void USBPD::setSrcDualDataRoleFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.drSwap = enable ? 1 : 0;
}

void USBPD::setSrcDualPowerModeFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.dualRolePower = enable ? 1 : 0;
}

void USBPD::setSrcUnconstrainedPowerFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.extPowered = enable ? 1 : 0;
    iSprSrcMask &= 0x7F;
    iSprSrcMask |= enable ? 0x80 : 0x00;
}

#if CY_PD_EPR_ENABLE
void USBPD::setSrcEPRFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.eprModeCapable = enable ? 1 : 0;
}
#endif

void USBPD::setUnchunkedExtMsgFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.unchunkSup = enable ? 1 : 0;
}

void USBPD::setSrcUSBSuspendCapFlag(bool enable)
{
    iSprSrcPdo[0].fixed_src_do_t.usbSuspendSup = enable ? 1 : 0;
}

void USBPD::setSrcPeakCurrent(uint8_t peakCurrent)
{
    if (peakCurrent > 3)
        return;
    iSprSrcPdo[0].fixed_src_do_t.pkCurrent = peakCurrent;
}

bool USBPD::addVarSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.var_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.var_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.var_src_do_t.minVoltage = minVoltage / 50;
    srcCap.var_src_do_t.supplyType = 2;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::addBatSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxPower)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.bat_src_do_t.maxPower = maxPower / 250;
    srcCap.bat_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.bat_src_do_t.minVoltage = minVoltage / 50;
    srcCap.bat_src_do_t.supplyType = 1;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::addAugmentedSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.src_gen_do_t.maxCurPower = maxCurrent / 50;
    srcCap.src_gen_do_t.maxVoltage = maxVoltage / 100;
    srcCap.src_gen_do_t.minVoltage = minVoltage / 100;
    srcCap.src_gen_do_t.supplyType = 3;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::removeSrcPdo(uint8_t pdoNum)
{
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    iSprSrcMask &= ~(1 << pdoNum);

    if (iSprSrcPdoCnt > 1)
        iSprSrcPdoCnt --;

    iSprSrcPdo[pdoNum].val = 0;

    return true;
}

uint8_t USBPD::getSrcPdoNum(void)
{
    return iSprSrcPdoCnt;
}
#endif

void USBPD::setSnkDualDataRoleFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.drSwap = enable ? 1 : 0;
}

void USBPD::setSnkUnconstrainedPowerFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.extPowered = enable ? 1 : 0;
}

void USBPD::setSnkUSBCommFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.usbCommCap = enable ? 1 : 0;
}

void USBPD::setSnkDualPowerModeFlag(bool enable)
{
    iSprSnkPdo[0].fixed_snk_do_t.dualRolePower = enable ? 1 : 0;
}

bool USBPD::addFixedSnkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.fixed_snk_do_t.voltage = voltage / 50;
    snkCap.fixed_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.fixed_snk_do_t.supplyType = 0;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::addVarSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.var_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.var_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.var_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.var_snk_do_t.supplyType = 2;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::addBatSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opPower)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.bat_snk_do_t.opPower = opPower / 250;
    snkCap.bat_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.bat_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.bat_snk_do_t.supplyType = 1;

    return (setSnkPdo(pdoNum, snkCap));
}

bool USBPD::removeSnkPdo(uint8_t pdoNum)
{
    if (pdoNum == 0 || pdoNum > 7)
        return false;

    iSprSnkMask &= ~(1 << pdoNum);

    if (iSprSnkPdoCnt > 1)
        iSprSnkPdoCnt --;

    if (iSprSnkPdoCnt == 1)
         iSprSnkPdo[0].fixed_snk_do_t.highCap = 0;

    iSprSnkPdo[pdoNum].val = 0;

    return true;
}

uint8_t USBPD::getSnkPdoNum(void)
{
    return iSprSnkPdoCnt;
}

bool USBPD::updateSrcPdo(void)
{

#if PMGDUINO_BOARD
    cy_en_pdstack_status_t status;
    cy_pd_pd_do_t srcPdo[7];

    if (portInitated[portIdx] == false)
        return false;

    for (uint8_t i=0; i<iSprSrcPdoCnt; i++)
        srcPdo[i].val = iSprSrcPdo[i].val;

#if CY_PD_EPR_ENABLE
    if (iSprSrcPdo[0].fixed_src_do_t.eprModeCapable)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_EPR_MODE_CAP;
#endif
    if (iSprSrcPdo[0].fixed_src_do_t.unchunkSup)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_UNCHUNKED_SUPP;

    if (iSprSrcPdo[0].fixed_src_do_t.drSwap)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_DUAL_ROLE_DATA;

    if (iSprSrcPdo[0].fixed_src_do_t.usbCommCap)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_USB_COMM;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_USB_COMM;

    if (iSprSrcPdo[0].fixed_src_do_t.extPowered)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_0_UNCONSTRAINED_POW;

    if (iSprSrcPdo[0].fixed_src_do_t.usbSuspendSup)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_1_USB_SUSP_SUPP;

    if (iSprSrcPdo[0].fixed_src_do_t.dualRolePower)
        ctx->dpmStat.srcPdoFlags[0] |= CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER;
    else
        ctx->dpmStat.srcPdoFlags[0] &= ~CY_PD_SRCCAP_FLAGS_1_DUAL_ROLE_POWER;

    ctx->dpmStat.srcPdoFlags[1] = (iSprSrcPdo[0].fixed_src_do_t.pkCurrent & 0x03);

    // optional, called if the PDO content changes
    status = Cy_PdStack_Dpm_UpdateSrcCap(ctx, iSprSrcPdoCnt, srcPdo);
    if (status == CY_PDSTACK_STAT_SUCCESS)
    {
        // optional, called if the PDO mask changes
        status = Cy_PdStack_Dpm_UpdateSrcCapMask(ctx, iSprSrcMask);
        if (status == CY_PDSTACK_STAT_SUCCESS)
        {
    		if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SOURCE))
    		{
                // optional, set a timer for the callback to handle the command sending failure condition
    			if(Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SRC_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    			{
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										GET_USER_TIMER_ID(ctx, SRC_CAP_CHANGE_RETRY_TIMER), 
                                            SRC_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                            srcCapChangeRetryCbk);
    			}
    		}
        }

    }
#endif

    return true;
}

bool USBPD::updateSnkPdo(void)
{
    cy_en_pdstack_status_t status;
    cy_pd_pd_do_t snkPdo[7];

    if (portInitated[portIdx] == false)
        return false;
        
    for (uint8_t i=0; i<iSprSnkPdoCnt; i++)
        snkPdo[i].val = iSprSnkPdo[i].val;

    if (iSprSnkPdo[0].fixed_snk_do_t.drSwap)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_DUAL_ROLE_DATA;
    
    if (iSprSnkPdo[0].fixed_snk_do_t.usbCommCap)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_USB_COMM;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_USB_COMM;

    if (iSprSnkPdo[0].fixed_snk_do_t.extPowered)
        ctx->dpmStat.snkPdoFlags[0] |= CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW;
    else
        ctx->dpmStat.snkPdoFlags[0] &= ~CY_PD_SNKCAP_FLAGS_0_UNCONSTRAINED_POW;

    if (iSprSnkPdo[0].fixed_snk_do_t.highCap)
        ctx->dpmStat.snkPdoFlags[1] |= CY_PD_SNKCAP_FLAGS_1_HIGH_CAP;
    else
        ctx->dpmStat.snkPdoFlags[1] &= ~CY_PD_SNKCAP_FLAGS_1_HIGH_CAP;

    if (iSprSnkPdo[0].fixed_snk_do_t.dualRolePower)
        ctx->dpmStat.snkPdoFlags[1] |= CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER;
    else
        ctx->dpmStat.snkPdoFlags[1] &= ~CY_PD_SNKCAP_FLAGS_1_DUAL_ROLE_POWER;

    status = Cy_PdStack_Dpm_UpdateSnkCap(ctx, iSprSnkPdoCnt, snkPdo);
    if (status == CY_PDSTACK_STAT_SUCCESS)
    {
        Cy_PdStack_Dpm_UpdateSnkCapMask(ctx, iSprSnkMask);
        if (status == CY_PDSTACK_STAT_SUCCESS)
        {
    		if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SINK))
    		{
    			if(Cy_PdStack_Dpm_SendPdCommand(ctx, CY_PDSTACK_DPM_CMD_SNK_CAP_CHNG, NULL, false, NULL) != CY_PDSTACK_STAT_SUCCESS)
    			{
    				Cy_PdUtils_SwTimer_Start(ctx->ptrTimerContext, ctx,
    										GET_USER_TIMER_ID(ctx, SNK_CAP_CHANGE_RETRY_TIMER), 
                                            SNK_CAP_CHANGE_RETRY_TIMER_PERIOD, 
                                            snkCapChangeRetryCbk);
    			}
    		}
        }
    }

    return (status == CY_PDSTACK_STAT_SUCCESS) ? true : false;
}

void USBPD::registerEvent(uint8_t event, void (*userFunc)(void))
{
    usbpd_ebent_handler[event] = userFunc;
}

void USBPD::unRegisterEvent(uint8_t event)
{
    usbpd_ebent_handler[event] = NULL;
}

USBPD usbpd0 = USBPD(0);
#if PMGDUINO_BOARD
USBPD usbpd1 = USBPD(1);
#endif
