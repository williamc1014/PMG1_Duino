
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

extern cy_stc_pdstack_context_t *get_pdstack_context(uint8_t portIdx);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

typedef union
{
    uint32_t val;
    /** @brief Structure representing a Fixed Supply PDO - Source. */
    struct FIXED_SRC_DO
    {
        uint32_t maxCurrent               : 10;     /**< Maximum current in 100mA units. */
        uint32_t voltage                  : 10;     /**< Voltage in 50mV units. */
        uint32_t pkCurrent                : 2;      /**< Peak current. */
#if CY_PD_EPR_ENABLE
        uint32_t rsrvd                    : 1;      /**< Reserved field. */
        uint32_t eprModeCapable           : 1;      /**< EPR mode capable. */
#else
        uint32_t rsrvd                    : 2;      /**< Reserved field. */
#endif /* CY_PD_EPR_ENABLE */
        uint32_t unchunkSup               : 1;      /**< Unchunked extended messages supported. */
        uint32_t drSwap                   : 1;      /**< Data Role Swap supported. */
        uint32_t usbCommCap               : 1;      /**< USB communication capability. */
        uint32_t extPowered               : 1;      /**< Externally powered. */
        uint32_t usbSuspendSup            : 1;      /**< USB suspend supported. */
        uint32_t dualRolePower            : 1;      /**< Dual role power support. */
        uint32_t supplyType               : 2;      /**< Supply type - should be 'b00. */
    } fixed_src_do_t;                                    /**< DO interpreted as a Fixed Supply PDO - Source. */

    /** @brief Structure representing a Variable Supply PDO - Source. */
    struct VAR_SRC_DO
    {
        uint32_t maxCurrent                : 10;   /**< Maximum current in 10mA units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type - should be 'b10. */
    } var_src_do_t;                                      /**< DO interpreted as a Variable Supply PDO - Source. */

    /** @brief Structure representing a Battery Supply PDO - Source. */
    struct BAT_SRC_DO
    {
        uint32_t maxPower                  : 10;   /**< Maximum power in 250mW units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type - should be 'b01. */
    } bat_src_do_t;                                      /**< DO interpreted as a Battery Supply PDO - Source. */

    /** @brief Structure representing a generic source PDO. */
    struct SRC_GEN_DO
    {
        uint32_t maxCurPower               : 10;   /**< Maximum current in 10 mA or power in 250 mW units. */
        uint32_t minVoltage                : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                : 2;    /**< Supply type. */
    } src_gen_do_t;                                      /**< DO interpreted as a generic PDO - Source. */
} src_cap_t;

typedef union 
{
    uint32_t val;
    struct FIXED_SNK_DO
    {
        uint32_t opCurrent                  : 10;   /**< Operational current in 10mA units. */
        uint32_t voltage                    : 10;   /**< Voltage in 50mV units. */
        uint32_t rsrvd                      : 3;    /**< Reserved field. */
        uint32_t frSwap                     : 2;    /**< FR swap support. */
        uint32_t drSwap                     : 1;    /**< Data Role Swap supported. */
        uint32_t usbCommCap                 : 1;    /**< USB communication capability. */
        uint32_t extPowered                 : 1;    /**< Externally powered. */
        uint32_t highCap                    : 1;    /**< Higher capability possible. */
        uint32_t dualRolePower              : 1;    /**< Dual role power support. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b00. */
    } fixed_snk_do_t;                                    /**< DO interpreted as a Fixed Supply PDO - Sink. */

    /** @brief Structure representing a Variable Supply PDO - Sink. */
    struct VAR_SNK_DO
    {
        uint32_t opCurrent                  : 10;   /**< Operational current in 10mA units. */
        uint32_t minVoltage                 : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                 : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b10. */
    } var_snk_do_t;                                      /**< DO interpreted as a Variable Supply PDO - Sink. */

    /** @brief Structure representing a Battery Supply PDO - Sink. */
    struct BAT_SNK_DO
    {
        uint32_t opPower                    : 10;   /**< Maximum power in 250mW units. */
        uint32_t minVoltage                 : 10;   /**< Minimum voltage in 50mV units. */
        uint32_t maxVoltage                 : 10;   /**< Maximum voltage in 50mV units. */
        uint32_t supplyType                 : 2;    /**< Supply type - should be 'b01. */
    } bat_snk_do_t;                                      /**< DO interpreted as a Battery Supply PDO - Sink. */
} snk_cap_t;


class USBPD
{
private:
    uint8_t                     portIdx;
    cy_stc_pdstack_context_t    *ctx;
    snk_cap_t                   iSprSnkPdo[7] = {0};
    uint8_t                     iSprSnkPdoCnt = 0x01;
    uint8_t                     iSprSnkMask = 0x01;

#if PMGDUINO_BOARD
    src_cap_t                   iSprSrcPdo[7] = {0};
    uint8_t                     iSprSrcPdoCnt = 0x01;
    uint8_t                     iSprSrcMask = 0x01;
#endif

    bool setSrcPdo(uint8_t pdoNum, src_cap_t srcCap);
    bool setSinkPdo(uint8_t pdoNum, snk_cap_t snkCap);
    
public:    
    bool        attached;
    uint8_t     polarity;
    uint8_t     attachedDeviceType;
    bool        contractExisted;
    uint8_t     powerRole;
    
    
    USBPD() {}
    USBPD(uint8_t port) {
        portIdx = port;
        ctx = get_pdstack_context(port);
    }

    void begin(void);
    void end(void);

    void updateStatus(void);
    uint8_t getCurrentSinkRdo(void);

    bool setFixedSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent, uint8_t peakCurrent);
    bool setFixedSrcPdo0(uint32_t maxCurrent, uint32_t flags);
    bool setVarSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent);
    bool setBatSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxPower);
    bool setAugmentedSrcPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t maxCurrent);

    bool setFixedSnkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t opCurrent);
    bool setFixedSnkPdo0(uint32_t opCurrent, uint32_t flags);
    bool setVarSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opCurrent);
    bool setBatSnkPdo(uint8_t pdoNum, uint32_t minVoltage, uint32_t maxVoltage, uint32_t opPower);


    bool updateSrcPdo(void);
    bool updateSinkPdo(void);

    bool getPartnerPortSrcCap(src_cap_t *srcCapPdo);

    void registerEvent(uint8_t event, void (*userFunc)(void));
    void unRegisterEvent(uint8_t event);
};

extern USBPD usbpd0;
#if PMGDUINO_BOARD
extern USBPD usbpd1;
#endif

extern void (*usbpd_ebent_handler[APP_TOTAL_EVENTS])(void);

#endif