
#include <rt6190.h>

#define PD_CFG_COLS (2u)
#define PD_ADDR     (0u)
#define PD_VALUE    (1u)


#define RICKTEK_PD_CFG_COLS    (2u)
#define RICKTEK_PD_ADDR_H      (2u)
#define RICKTEK_PD_ADDR_L      (1u)
#define RICKTEK_PD_VALUE       (1u)

bool RT6190_PD_EN = false; 

bool pd_ctrl_power_rt6190(uint8_t state)
{
    uint8_t wr_buf[PD_CFG_COLS];
    bool return_value = true;

    wr_buf[PD_ADDR] = RT6190_REG_VREF_SC;
    wr_buf[PD_VALUE] = 0x3F;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_SETTING1;
    wr_buf[PD_VALUE] = 0xE4;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_VREF_POCP;
    wr_buf[PD_VALUE] = REG_VREF_POCP_24A;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_SETTING4;
	wr_buf[PD_VALUE] = SETTING4_REG_ADC_AVG_8_TIMES | SETTING4_REG_ADC_EN;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_SETTING2;
    wr_buf[PD_VALUE] = 0x83;
    if(state != 0)
	{
        wr_buf[PD_VALUE] |= SETTING2_REG_EN_PWM;
    }
   
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_FUNCTION;
    wr_buf[PD_VALUE] = MASK_INTERNAL_FUNCTION_FOR_VOLTAGE;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        return_value = false;
	}
	
    return return_value;
}

/* Setting power voltage */
bool set_pd_ctrl_voltage_rt6190(uint16_t volt)
{
    uint8_t wr_buf[PD_CFG_COLS];
    uint16_t mV = volt + 200;

    if (volt == 20000)
    {
        wr_buf[PD_ADDR] = 0x0F;
        wr_buf[PD_VALUE] = 0x30;
        I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_OUTPUT_CURRENT_L;
    wr_buf[PD_VALUE] = OUTPUT_CURRENT_L_12A;
    I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);
    
    wr_buf[PD_ADDR] = RT6190_REG_OUTPUT_CURRENT_H;
    wr_buf[PD_VALUE] = OUTPUT_CURRENT_H_12A;
    I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);
      
    wr_buf[PD_ADDR] = RT6190_REG_OUTPUT_VOLTAGE_L;
    wr_buf[PD_VALUE] = ((mV * 10) / 125u) & 0xFF;
    I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);
    
    wr_buf[PD_ADDR] = RT6190_REG_OUTPUT_VOLTAGE_H;
    wr_buf[PD_VALUE] = ((mV * 10) / 125u) >> 8;
    I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);    
    
    return true;
}

bool set_pd_ocp_rt6190(uint16_t volt)
{
    
    uint8_t wr_buf[PD_CFG_COLS];
    bool returen_value = true;
    
    wr_buf[PD_ADDR] = RT6190_REG_OCP2_SETTING;    

    if (volt != 5000)
        wr_buf[PD_VALUE] = RT6190_5200MA_OCP2_SETTING; 
    else
        wr_buf[PD_VALUE] = 0xC8; 

    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        returen_value = false;
    }    

    return returen_value;        
}

bool pd_ctrl_ocp_rt6190(ocp_status state)
{
    uint8_t wr_buf[PD_CFG_COLS];

    /* Write data in index 1 to address in index 0. */
    bool returen_value = true;
    wr_buf[PD_ADDR] = RT6190_REG_OCP1_DELAY_TIME;
    wr_buf[PD_VALUE] = OCP1_DELAY_TIME_8MS;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        returen_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_OCP2_DELAY_TIME;
    wr_buf[PD_VALUE] = RT6190_56MS_OCP2_TIMING;
    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        returen_value = false;
    }
    
    wr_buf[PD_ADDR] = RT6190_REG_OCP_ENABLE;

    if(state == OCP_Enable)
    {
        wr_buf[PD_VALUE] = RT6190_OCP_ENABLE_OCP2_EN;
    }
    else
    {
        wr_buf[PD_VALUE] = 0;
    }

    if(I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS) == false)
    {
        returen_value = false;
    }

    return returen_value;
}

uint8_t pd_check_pg_rt6190(void)
{
    uint8_t wr_buf[PD_ADDR];
    uint8_t alert_val; 
    
    wr_buf[PD_ADDR] = RT6910_REG_ALERT2;
    while(!I2C_Read(RT6190_SLAVE_ADDRESS, wr_buf));
    
    alert_val = wr_buf[0] & ~((uint8_t)RT6190_REG_ALERT2_PG);

    /* check power good bit */
    if(wr_buf[0] & RT6190_REG_ALERT2_PG)
    {
        wr_buf[PD_ADDR] = RT6910_REG_ALERT2;
        wr_buf[PD_VALUE] = RT6190_REG_ALERT2_PG;
        I2C_Write(RT6190_SLAVE_ADDRESS, wr_buf, PD_CFG_COLS);
    }

    return alert_val;
}

/* [] END OF FILE */
