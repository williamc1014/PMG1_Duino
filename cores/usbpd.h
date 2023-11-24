

#ifdef __cplusplus
class USBPD
{
    private:
    uint8_t portIdx;

    public:
    
    bool        usbpdConnected;
    uint8_t     usbpdPowerRole;
    bool        typecAttached;
    uint8_t     typecPolarity;
    
    USBPD() {}
    USBPD(uint8_t port) {portIdx = port;}

    void updateStatus(void);

    bool setSrcPdo(uint8_t pdoNum, uint32_t voltage, uint32_t maxCurrent);
    bool setSinkPdo(uint8_t pdoNum, uint32_t voltage, uint32_t operCurrent);
    bool updateSrcPdo(void);
    bool updateSinkPdo(void);
};

extern USBPD usbpd0;
#if PMGDUINO_BOARD
extern USBPD usbpd1;
#endif

#endif