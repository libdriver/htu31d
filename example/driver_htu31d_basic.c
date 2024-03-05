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
 * @file      driver_htu31d_basic.c
 * @brief     driver htu31d basic source file
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

#include "driver_htu31d_basic.h"

static htu31d_handle_t gs_handle;        /**< htu31d handle */

/**
 * @brief     basic example init
 * @param[in] addr_pin is the addr pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t htu31d_basic_init(htu31d_addr_pin_t addr_pin)
{
    uint8_t res;
    
    /* link functions */
    DRIVER_HTU31D_LINK_INIT(&gs_handle, htu31d_handle_t);
    DRIVER_HTU31D_LINK_IIC_INIT(&gs_handle, htu31d_interface_iic_init);
    DRIVER_HTU31D_LINK_IIC_DEINIT(&gs_handle, htu31d_interface_iic_deinit);
    DRIVER_HTU31D_LINK_IIC_WRITE(&gs_handle, htu31d_interface_iic_write);
    DRIVER_HTU31D_LINK_IIC_READ(&gs_handle, htu31d_interface_iic_read);
    DRIVER_HTU31D_LINK_DELAY_MS(&gs_handle, htu31d_interface_delay_ms);
    DRIVER_HTU31D_LINK_DEBUG_PRINT(&gs_handle, htu31d_interface_debug_print);
    
    /* set addr pin */
    res = htu31d_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set addr pin failed.\n");
       
        return 1;
    }
    
    /* htu31d init */
    res = htu31d_init(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: init failed.\n");
        
        return 1;
    }
    
    /* set default humidity osr */
    res = htu31d_set_humidity_osr(&gs_handle, HTU21D_BASIC_DEFAULT_HUMIDITY_OSR);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set default temperature osr */
    res = htu31d_set_temperature_osr(&gs_handle, HTU21D_BASIC_DEFAULT_TEMPERATURE_OSR);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set heater off */
    res = htu31d_set_heater_off(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set heater off failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read
 * @param[out] *temperature points to a converted temperature buffer
 * @param[out] *humidity points to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t htu31d_basic_read(float *temperature, float *humidity)
{
    uint16_t temperature_raw;
    uint16_t humidity_raw;
   
    /* read data */
    if (htu31d_read_temperature_humidity(&gs_handle, (uint16_t *)&temperature_raw, temperature,
                                        (uint16_t *)&humidity_raw, humidity) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t htu31d_basic_deinit(void)
{
    /* close htu31d */
    if (htu31d_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example get the serial number
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t htu31d_basic_get_serial_number(uint8_t number[3])
{
    /* get the serial number */
    if (htu31d_get_serial_number(&gs_handle, number) != 0)
    {
        return 1;
    }
    
    return 0;
}
