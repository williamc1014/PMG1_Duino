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
}

void USBPD::end(void)
{
    // do nothing
}

void USBPD::updateStatus(void)
{
    attached = ctx->dpmConfig.attach;
    polarity = ctx->dpmConfig.polarity;
    contractExisted = ctx->dpmConfig.contractExist;
    powerRole = ctx->dpmConfig.curPortRole;
    attachedDeviceType = (uint8_t)ctx->dpmConfig.attachedDev;
}

uint8_t USBPD::getCurrentSinkRdo(void)
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

bool USBPD::setSrcPdo(uint8_t pdoNum, src_cap_t srcCap)
{
#if PMGDUINO_BOARD    
    if (pdoNum == 0)
    {
        // only the flag and current will be updated
        iSprSrcPdo[0].fixed_src_do_t.drSwap = srcCap.fixed_src_do_t.drSwap;
        iSprSrcPdo[0].fixed_src_do_t.dualRolePower = srcCap.fixed_src_do_t.dualRolePower;
        iSprSrcPdo[0].fixed_src_do_t.extPowered = srcCap.fixed_src_do_t.extPowered;
        iSprSrcPdo[0].fixed_src_do_t.unchunkSup = srcCap.fixed_src_do_t.unchunkSup;
        iSprSrcPdo[0].fixed_src_do_t.usbCommCap = srcCap.fixed_src_do_t.usbCommCap;
        iSprSrcPdo[0].fixed_src_do_t.usbSuspendSup = srcCap.fixed_src_do_t.usbSuspendSup;
        iSprSrcPdo[0].fixed_src_do_t.maxCurrent = srcCap.fixed_src_do_t.maxCurrent;
        iSprSrcPdo[0].fixed_src_do_t.pkCurrent = srcCap.fixed_src_do_t.pkCurrent;

        return true;
    }    


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
#endif

    return true;
}

bool USBPD::setSinkPdo(uint8_t pdoNum, snk_cap_t snkCap)
{
    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum == 0)
    {
        // only update flags and current
        iSprSnkPdo[0].fixed_snk_do_t.drSwap = snkCap.fixed_snk_do_t.drSwap;
        iSprSnkPdo[0].fixed_snk_do_t.dualRolePower = snkCap.fixed_snk_do_t.dualRolePower;
        iSprSnkPdo[0].fixed_snk_do_t.extPowered = snkCap.fixed_snk_do_t.extPowered;
        iSprSnkPdo[0].fixed_snk_do_t.frSwap = 0;
        iSprSnkPdo[0].fixed_snk_do_t.opCurrent = snkCap.fixed_snk_do_t.opCurrent; 
        iSprSnkPdo[0].fixed_snk_do_t.usbCommCap = snkCap.fixed_snk_do_t.usbCommCap;

        return true;
    }

    if (pdoNum > 7)
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


bool USBPD::setFixedSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent, uint8_t peakCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.fixed_src_do_t.voltage = voltage / 50;
    srcCap.fixed_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.fixed_src_do_t.pkCurrent = peakCurrent;
    srcCap.fixed_src_do_t.supplyType = 0;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::setFixedSrcPdo0(uint32_t maxCurrent, uint32_t flags)
{
    src_cap_t srcCap;

    srcCap.val = flags & 0x3F000000 | (maxCurrent / 10);

    return (setSrcPdo(0, srcCap));
}

bool USBPD::setVarSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.var_src_do_t.maxCurrent = maxCurrent / 10;
    srcCap.var_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.var_src_do_t.minVoltage = minVoltage / 50;
    srcCap.var_src_do_t.supplyType = 2;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::setBatSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxPower)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.bat_src_do_t.maxPower = maxPower / 250;
    srcCap.bat_src_do_t.maxVoltage = maxVoltage / 50;
    srcCap.bat_src_do_t.minVoltage = minVoltage / 50;
    srcCap.bat_src_do_t.supplyType = 1;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::setAugmentedSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent)
{
    src_cap_t srcCap;

    srcCap.val = 0;
    srcCap.src_gen_do_t.maxCurPower = maxCurrent / 50;
    srcCap.src_gen_do_t.maxVoltage = maxVoltage / 100;
    srcCap.src_gen_do_t.minVoltage = minVoltage / 100;
    srcCap.src_gen_do_t.supplyType = 3;

    return (setSrcPdo(pdoNum, srcCap));
}

bool USBPD::setFixedSnkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.fixed_snk_do_t.voltage = voltage / 50;
    snkCap.fixed_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.fixed_snk_do_t.supplyType = 0;

    return (setSinkPdo(pdoNum, snkCap));
}

bool USBPD::setFixedSnkPdo0(uint32_t opCurrent, uint32_t flags)
{
    snk_cap_t snkCap;

    snkCap.val = flags & 0x3E000000 | (opCurrent / 10);

    return (setSinkPdo(0, snkCap));
}

bool USBPD::setVarSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opCurrent)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.var_snk_do_t.opCurrent = opCurrent / 10;
    snkCap.var_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.var_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.var_snk_do_t.supplyType = 2;

    return (setSinkPdo(pdoNum, snkCap));
}

bool USBPD::setBatSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opPower)
{
    snk_cap_t snkCap;

    snkCap.val = 0;
    snkCap.bat_snk_do_t.opPower = opPower / 250;
    snkCap.bat_snk_do_t.maxVoltage = maxVoltage / 50;
    snkCap.bat_snk_do_t.minVoltage = minVoltage / 50;
    snkCap.bat_snk_do_t.supplyType = 1;

    return (setSinkPdo(pdoNum, snkCap));
}


bool USBPD::updateSrcPdo(void)
{

#if PMGDUINO_BOARD
    cy_en_pdstack_status_t status;

    cy_pd_pd_do_t srcPdo[7];

    for (uint8_t i=0; i<iSprSrcPdoCnt; i++)
        srcPdo[i].val = iSprSrcPdo[i].val;


    status = Cy_PdStack_Dpm_UpdateSrcCap(ctx, iSprSrcPdoCnt, srcPdo);
    if (status == CY_PDSTACK_STAT_SUCCESS)
    {
        status = Cy_PdStack_Dpm_UpdateSrcCapMask(ctx, (iSprSrcMask | 0x80));
        if (status == CY_PDSTACK_STAT_SUCCESS)
        {
    		if ((ctx->dpmConfig.connect) && (ctx->dpmConfig.curPortRole == CY_PD_PRT_ROLE_SOURCE))
    		{
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

bool USBPD::updateSinkPdo(void)
{
    cy_en_pdstack_status_t status;
    cy_pd_pd_do_t snkPdo[7];

    for (uint8_t i=0; i<iSprSnkPdoCnt; i++)
        snkPdo[i].val = iSprSnkPdo[i].val;

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

bool USBPD::getPartnerPortSrcCap(src_cap_t *srcCapPdo)
{
    return true;
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
