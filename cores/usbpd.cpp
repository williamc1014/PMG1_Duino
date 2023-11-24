#include <usbpd.h>

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

bool USBPD::setSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent)
{
    if (pdoNum == 0)
        return false;

#if PMGDUINO_BOARD
    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum == 0 || pdoNum > 7)
        return false;
    
    iSprSrcPdo[pdoNum].fixed_src.maxCurrent = maxCurrent / 10;
    iSprSrcPdo[pdoNum].fixed_src.voltage = voltage / 50;
    iSprSrcPdo[pdoNum].fixed_src.supplyType = 0;

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

bool USBPD::setSinkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent, uint32_t maxCurrent)
{
    uint8_t i;
    uint8_t pdoCnt = 0;

    if (pdoNum == 0 || pdoNum > 7)
        return false;
    
    iSprSnkPdo[pdoNum].fixed_snk.opCurrent = opCurrent / 10;
    iSprSnkPdo[pdoNum].fixed_snk.voltage = voltage / 50;
    iSprSnkPdo[pdoNum].fixed_snk.supplyType = 0;

    iSprSnkMask |= (1 << pdoNum);

    for (i=0; i<7; i++)
    {
        if (iSprSnkMask & (1 << i))
            pdoCnt ++;
    }

    iSprSnkPdoCnt = pdoCnt;

    return true;
}

bool USBPD::updateSrcPdo(void)
{

#if PMGDUINO_BOARD
    cy_en_pdstack_status_t status;
    status = Cy_PdStack_Dpm_UpdateSrcCap(ctx, iSprSrcPdoCnt, iSprSrcPdo);
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

    status = Cy_PdStack_Dpm_UpdateSnkCap(ctx, iSprSnkPdoCnt, iSprSnkPdo);

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

USBPD usbpd0 = USBPD(0);
#if PMGDUINO_BOARD
USBPD usbpd1 = USBPD(1);
#endif
