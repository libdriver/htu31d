/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_htu31d.c
 * @brief     driver htu31d source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-02-28
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/02/28  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_htu31d.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "TE HTU31D"        /**< chip name */
#define MANUFACTURER_NAME         "TE"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f               /**< chip max supply voltage */
#define MAX_CURRENT               0.414f             /**< chip max current */
#define TEMPERATURE_MIN           -40.0f             /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f             /**< chip max operating temperature */
#define DRIVER_VERSION            1000               /**< driver version */

/**
 * @brief chip command definition
 */
#define HTU31D_COMMAND_CONVERSION              (0x1 << 6)        /**< conversion command */
#define HTU31D_COMMAND_READ_T_RH               (0x00)            /**< read t && rh command */
#define HTU31D_COMMAND_READ_RH                 (0x1 << 4)        /**< read rh command */
#define HTU31D_COMMAND_RESET                   (0x1E)            /**< reset command */
#define HTU31D_COMMAND_HEATER_ON               (0x1 << 2)        /**< heater on command */
#define HTU31D_COMMAND_HEATER_OFF              (0x1 << 1)        /**< heater off command */
#define HTU31D_COMMAND_READ_SERIAL_NUMBER      (0x0A)            /**< read serial number command */
#define HTU31D_COMMAND_READ_DIAGNOSTIC         (1 << 3)          /**< read diagnostic command */

/**
 * @brief     write bytes
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_htu31d_write(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)        /* iic write */
    {
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_htu31d_read(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)        /* iic read */
    {
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief     check the crc
 * @param[in] *buf pointer to a data buffer
 * @param[in] len buffer length
 * @param[in] crc checked crc
 * @return    status code
 *            - 0 pass
 *            - 1 error
 * @note      none
 */
static uint8_t a_htu31d_crc(uint8_t *buf, uint8_t len, uint32_t crc)
{
    uint32_t polynom = 0x98800000UL;
    uint32_t msb = 0x80000000UL;
    uint32_t mask = 0xFF800000UL;
    uint32_t result = 0;
    
    if (len == 1)                                                             /* if len == 1 */
    {
        result = (uint32_t)buf[0] << 8;                                       /* set to buffer */
    }
    else if (len == 2)                                                        /* if len == 2 */
    {
        result = (uint32_t)buf[0] << 16;                                      /* set part 0 */
        result |= (uint32_t)buf[1] << 8;                                      /* set part 1 */
    }
    else if (len == 3)                                                        /* if len == 3 */
    {
        result = buf[0];                                                      /* set buf 0 */
        result <<= 8;                                                         /* left shift 8 */
        result |= buf[1];                                                     /* set buf 1 */
        result <<= 8;                                                         /* left shift 8 */
        result |= buf[2];                                                     /* set buf 2 */
        result <<= 8;                                                         /* left shift 8 */
    }
    else
    {
        return 1;                                                             /* return error */
    }
    while (msb != 0x80)                                                       /* check the msb */
    {
        if ((result & msb) != 0)                                              /* check the result */
        {
            result = ((result ^ polynom) & mask) | (result & (~mask));        /* get the new result */
        }
        msb >>= 1;                                                            /* right shift 1 */
        mask >>= 1;                                                           /* right shift 1 */
        polynom >>= 1;                                                        /* right shift 1 */
    }
    if ((result & 0xFF) == (crc & 0xFF))                                      /* check the result */
    {
        return 0;                                                             /* success return 0 */
    }
    else
    {
        return 1;                                                             /* return error */
    }
}

/**
 * @brief     set the address pin
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] addr_pin addr pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t htu31d_set_addr_pin(htu31d_handle_t *handle, htu31d_addr_pin_t addr_pin)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    
    handle->iic_addr = addr_pin;        /* set the addr pin */
    
    return 0;                           /* success return 0 */
}

/**
 * @brief      get the address pin
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *addr_pin pointer to an addr pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t htu31d_get_addr_pin(htu31d_handle_t *handle, htu31d_addr_pin_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (htu31d_addr_pin_t)(handle->iic_addr);        /* get the addr pin */
    
    return 0;                                                 /* success return 0 */
}

/**
 * @brief     soft reset
 * @param[in] *handle pointer to an htu31d handle structure
 * @return    status code
 *            - 0 success
 *            - 1 soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_soft_reset(htu31d_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_htu31d_write(handle, HTU31D_COMMAND_RESET, NULL, 0);        /* soft reset */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("htu31d: write failed.\n");                 /* write failed */
        
        return 1;                                                       /* return error */
    }
    handle->delay_ms(15);                                               /* delay 15 ms */
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     enable heater
 * @param[in] *handle pointer to an htu31d handle structure
 * @return    status code
 *            - 0 success
 *            - 1 heater on failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_set_heater_on(htu31d_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_htu31d_write(handle, HTU31D_COMMAND_HEATER_ON, NULL, 0);        /* heater on */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: write failed.\n");                     /* write failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     disable heater
 * @param[in] *handle pointer to an htu31d handle structure
 * @return    status code
 *            - 0 success
 *            - 1 heater off failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_set_heater_off(htu31d_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    res = a_htu31d_write(handle, HTU31D_COMMAND_HEATER_OFF, NULL, 0);       /* heater off */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: write failed.\n");                     /* write failed */
        
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief      get the serial number
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *number pointer to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc checked error
 * @note       none
 */
uint8_t htu31d_get_serial_number(htu31d_handle_t *handle, uint8_t number[3])
{
    uint8_t res;
    uint8_t buf[4];
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = a_htu31d_read(handle, HTU31D_COMMAND_READ_SERIAL_NUMBER, buf, 4);        /* get serial number */
    if (res != 0)                                                                  /* check the result */
    {
        handle->debug_print("htu31d: get serial number failed.\n");                /* get serial number failed */
        
        return 1;                                                                  /* return error */
    }
    if (a_htu31d_crc(buf, 3, buf[3]) != 0)                                         /* check the crc */
    {
        handle->debug_print("htu31d: crc checked error.\n");                       /* crc checked error */
        
        return 4;                                                                  /* return error */
    }
    memcpy(number, buf, sizeof(uint8_t) * 3);                                      /* copy to number */
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief      get the diagnostic
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *diagnostic pointer to a diagnostic buffer
 * @return     status code
 *             - 0 success
 *             - 1 get diagnostic failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc checked error
 * @note       none
 */
uint8_t htu31d_get_diagnostic(htu31d_handle_t *handle, uint8_t *diagnostic)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    res = a_htu31d_read(handle, HTU31D_COMMAND_READ_DIAGNOSTIC, buf, 2);        /* get diagnostic */
    if (res != 0)                                                               /* check the result */
    {
        handle->debug_print("htu31d: get diagnostic failed.\n");                /* get diagnostic failed */
        
        return 1;                                                               /* return error */
    }
    if (a_htu31d_crc(buf, 1, buf[1]) != 0)                                      /* check the crc */
    {
        handle->debug_print("htu31d: crc checked error.\n");                    /* crc checked error */
        
        return 4;                                                               /* return error */
    }
    *diagnostic = buf[1];                                                       /* copy to diagnostic */
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief      read the temperature and humidity data
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *temperature_raw pointer to a raw temperature buffer
 * @param[out] *temperature_s pointer to a converted temperature buffer
 * @param[out] *humidity_raw pointer to a raw humidity buffer
 * @param[out] *humidity_s pointer to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature humidity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 *             - 5 param is error
 * @note       none
 */
uint8_t htu31d_read_temperature_humidity(htu31d_handle_t *handle,
                                         uint16_t *temperature_raw, float *temperature_s,
                                         uint16_t *humidity_raw, float *humidity_s
                                        )
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[6];
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    prev = HTU31D_COMMAND_CONVERSION 
           | (handle->humidity_osr & 0x3) << 3 
           | (handle->temperature_osr & 0x3) << 1;                          /* set the command */
    res = a_htu31d_write(handle, prev, NULL, 0);                            /* conversion */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: write failed.\n");                     /* write failed */
        
        return 1;                                                           /* return error */
    }
    if (handle->humidity_osr == 0)                                          /* osr 0 */
    {
        handle->delay_ms(2);                                                /* delay 2ms */
    }
    else if (handle->humidity_osr == 1)                                     /* osr 1 */
    {
        handle->delay_ms(3);                                                /* delay 3ms */
    }
    else if (handle->humidity_osr == 2)                                     /* osr 2 */
    {
        handle->delay_ms(5);                                                /* delay 5ms */
    }
    else if (handle->humidity_osr == 3)                                     /* osr 3 */
    {
        handle->delay_ms(9);                                                /* delay 9ms */
    }
    else
    {
        handle->debug_print("htu31d: param is error.\n");                   /* param is error */
        
        return 5;                                                           /* return error */
    }
    if (handle->temperature_osr == 0)                                       /* osr 0 */
    {
        handle->delay_ms(2);                                                /* delay 2ms */
    }
    else if (handle->temperature_osr == 1)                                  /* osr 1 */
    {
        handle->delay_ms(4);                                                /* delay 4ms */
    }
    else if (handle->temperature_osr == 2)                                  /* osr 2 */
    {
        handle->delay_ms(7);                                                /* delay 7ms */
    }
    else if (handle->temperature_osr == 3)                                  /* osr 3 */
    {
        handle->delay_ms(13);                                               /* delay 13ms */
    }
    else
    {
        handle->debug_print("htu31d: param is error.\n");                   /* param is error */
        
        return 5;                                                           /* return error */
    }
    
    res = a_htu31d_read(handle, HTU31D_COMMAND_READ_T_RH, buf, 6);          /* read t && rh */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: read t && rh failed.\n");              /* read t && rh failed */
        
        return 1;                                                           /* return error */
    }
    if (a_htu31d_crc(buf, 2, buf[2]) != 0)                                  /* check the crc */
    {
        handle->debug_print("htu31d: crc checked error.\n");                /* crc checked error */
        
        return 4;                                                           /* return error */
    }
    if (a_htu31d_crc(buf + 3, 2, buf[5]) != 0)                              /* check the crc */
    {
        handle->debug_print("htu31d: crc checked error.\n");                /* crc checked error */
        
        return 4;                                                           /* return error */
    }
    *temperature_raw = ((uint16_t)buf[0] << 8) | buf[1];                    /* set temperature raw */
    *humidity_raw = ((uint16_t)buf[3] << 8) | buf[4];                       /* set humidity raw */
    
    *temperature_s = (float)(*temperature_raw) / 65535.0f * 165.0f - 40.0f; /* convert raw temperature */
    *humidity_s = (float)(*humidity_raw) / 65535.0f * 100.0f;               /* convert raw humidity */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief      read the humidity data
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *humidity_raw pointer to a raw humidity buffer
 * @param[out] *humidity_s pointer to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read humidity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 crc is error
 *             - 5 param is error
 * @note       none
 */
uint8_t htu31d_read_humidity(htu31d_handle_t *handle, uint16_t *humidity_raw, float *humidity_s)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    prev = HTU31D_COMMAND_CONVERSION 
           | (handle->humidity_osr & 0x3) << 3 
           | (handle->temperature_osr & 0x3) << 1;                          /* set the command */
    res = a_htu31d_write(handle, prev, NULL, 0);                            /* conversion */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: write failed.\n");                     /* write failed */
        
        return 1;                                                           /* return error */
    }
    if (handle->humidity_osr == 0)                                          /* osr 0 */
    {
        handle->delay_ms(2);                                                /* delay 2ms */
    }
    else if (handle->humidity_osr == 1)                                     /* osr 1 */
    {
        handle->delay_ms(3);                                                /* delay 3ms */
    }
    else if (handle->humidity_osr == 2)                                     /* osr 2 */
    {
        handle->delay_ms(5);                                                /* delay 5ms */
    }
    else if (handle->humidity_osr == 3)                                     /* osr 3 */
    {
        handle->delay_ms(9);                                                /* delay 9ms */
    }
    else
    {
        handle->debug_print("htu31d: param is error.\n");                   /* param is error */
        
        return 5;                                                           /* return error */
    }
    if (handle->temperature_osr == 0)                                       /* osr 0 */
    {
        handle->delay_ms(2);                                                /* delay 2ms */
    }
    else if (handle->temperature_osr == 1)                                  /* osr 1 */
    {
        handle->delay_ms(4);                                                /* delay 4ms */
    }
    else if (handle->temperature_osr == 2)                                  /* osr 2 */
    {
        handle->delay_ms(7);                                                /* delay 7ms */
    }
    else if (handle->temperature_osr == 3)                                  /* osr 3 */
    {
        handle->delay_ms(13);                                               /* delay 13ms */
    }
    else
    {
        handle->debug_print("htu31d: param is error.\n");                   /* param is error */
        
        return 5;                                                           /* return error */
    }
    
    res = a_htu31d_read(handle, HTU31D_COMMAND_READ_RH, buf, 3);            /* read rh */
    if (res != 0)                                                           /* check the result */
    {
        handle->debug_print("htu31d: read rh failed.\n");                   /* read rh failed */
        
        return 1;                                                           /* return error */
    }
    if (a_htu31d_crc(buf, 2, buf[2]) != 0)                                  /* check the crc */
    {
        handle->debug_print("htu31d: crc checked error.\n");                /* crc checked error */
        
        return 4;                                                           /* return error */
    }
    *humidity_raw = ((uint16_t)buf[0] << 8) | buf[1];                       /* set humidity raw */
    *humidity_s = (float)(*humidity_raw) / 65535.0f * 100.0f;               /* convert raw humidity */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an htu31d handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t htu31d_init(htu31d_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->debug_print == NULL)                                    /* check debug_print */
    {
        return 3;                                                       /* return error */
    }
    if (handle->iic_init == NULL)                                       /* check iic_init */
    {
        handle->debug_print("htu31d: iic_init is null.\n");             /* iic_init is null */
       
        return 3;                                                       /* return error */
    }
    if (handle->iic_deinit == NULL)                                     /* check iic_deinit */
    {
        handle->debug_print("htu31d: iic_deinit is null.\n");           /* iic_deinit is null */
       
        return 3;                                                       /* return error */
    }
    if (handle->iic_read == NULL)                                       /* check iic_read */
    {
        handle->debug_print("htu31d: iic_read is null.\n");             /* iic_read is null */
       
        return 3;                                                       /* return error */
    }
    if (handle->iic_write == NULL)                                      /* check iic_write */
    {
        handle->debug_print("htu31d: iic_write is null.\n");            /* iic_write is null */
       
        return 3;                                                       /* return error */
    }
    if (handle->delay_ms == NULL)                                       /* check delay_ms */
    {
        handle->debug_print("htu31d: delay_ms is null.\n");             /* delay_ms is null */
       
        return 3;                                                       /* return error */
    }
    
    if (handle->iic_init() != 0)                                        /* iic init */
    {
        handle->debug_print("htu31d: iic init failed.\n");              /* iic init failed */
       
        return 1;                                                       /* return error */
    }
    res = a_htu31d_write(handle, HTU31D_COMMAND_RESET, NULL, 0);        /* soft reset */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("htu31d: soft reset failed.\n");            /* soft reset failed */
        (void)handle->iic_deinit();                                     /* iic deinit */
        
        return 4;                                                       /* return error */
    }
    handle->delay_ms(15);                                               /* delay 15 ms */
    handle->inited = 1;                                                 /* flag finish initialization */
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an htu31d handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 reset failed
 * @note      none
 */
uint8_t htu31d_deinit(htu31d_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_htu31d_write(handle, HTU31D_COMMAND_RESET, NULL, 0);        /* soft reset */
    if (res != 0)                                                       /* check the result */
    {
        handle->debug_print("htu31d: soft reset failed.\n");            /* soft reset failed */
        
        return 4;                                                       /* return error */
    }
    handle->delay_ms(15);                                               /* delay 15 ms */
    if (handle->iic_deinit() != 0)                                      /* iic deinit */
    {
        handle->debug_print("htu31d: iic deinit failed.\n");            /* iic deinit failed */
       
        return 1;                                                       /* return error */
    }
    handle->inited = 0;                                                 /* flag close */
    
    return 0;                                                           /* success return 0 */
}

/**
 * @brief     set humidity osr
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] osr humidity osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_set_humidity_osr(htu31d_handle_t *handle, htu31d_humidity_osr_t osr)
{
    if (handle == NULL)                /* check handle */
    {
        return 2;                      /* return error */
    }
    if (handle->inited != 1)           /* check handle initialization */
    {
        return 3;                      /* return error */
    }
    
    handle->humidity_osr = osr;        /* set osr */
    
    return 0;                          /* success return 0 */
}

/**
 * @brief      get humidity osr
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *osr pointer to a humidity osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t htu31d_get_humidity_osr(htu31d_handle_t *handle, htu31d_humidity_osr_t *osr)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    *osr = (htu31d_humidity_osr_t)(handle->humidity_osr);        /* get osr */
    
    return 0;                                                    /* success return 0 */
}

/**
 * @brief     set temperature osr
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] osr temperature osr
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_set_temperature_osr(htu31d_handle_t *handle, htu31d_temperature_osr_t osr)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    handle->temperature_osr = osr;        /* set osr */
    
    return 0;                             /* success return 0 */
}

/**
 * @brief      get temperature osr
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *osr pointer to a temperature osr buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t htu31d_get_temperature_osr(htu31d_handle_t *handle, htu31d_temperature_osr_t *osr)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    *osr = (htu31d_temperature_osr_t)(handle->temperature_osr);        /* get osr */
    
    return 0;                                                          /* success return 0 */
}

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t htu31d_set_reg(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }
    
    return a_htu31d_write(handle, reg, buf, len);        /* write command */
}

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t htu31d_get_reg(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                 /* check handle */
    {
        return 2;                                       /* return error */
    }
    if (handle->inited != 1)                            /* check handle initialization */
    {
        return 3;                                       /* return error */
    }
    
    return a_htu31d_read(handle, reg, buf, len);        /* read command */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an htu31d info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t htu31d_info(htu31d_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(htu31d_info_t));                         /* initialize htu31d info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
