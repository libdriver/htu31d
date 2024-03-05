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
 * @file      driver_htu31d_read_test.c
 * @brief     driver htu31d read test source file
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

#include "driver_htu31d_read_test.h"

static htu31d_handle_t gs_handle;        /**< htu31d handle */

/**
 * @brief     read test
 * @param[in] addr_pin is the addr pin
 * @param[in] times is test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t htu31d_read_test(htu31d_addr_pin_t addr_pin, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    htu31d_info_t info;
    
    /* link functions */
    DRIVER_HTU31D_LINK_INIT(&gs_handle, htu31d_handle_t);
    DRIVER_HTU31D_LINK_IIC_INIT(&gs_handle, htu31d_interface_iic_init);
    DRIVER_HTU31D_LINK_IIC_DEINIT(&gs_handle, htu31d_interface_iic_deinit);
    DRIVER_HTU31D_LINK_IIC_WRITE(&gs_handle, htu31d_interface_iic_write);
    DRIVER_HTU31D_LINK_IIC_READ(&gs_handle, htu31d_interface_iic_read);
    DRIVER_HTU31D_LINK_DELAY_MS(&gs_handle, htu31d_interface_delay_ms);
    DRIVER_HTU31D_LINK_DEBUG_PRINT(&gs_handle, htu31d_interface_debug_print);
    
    /* htu31d info */
    res = htu31d_info(&info);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        htu31d_interface_debug_print("htu31d: chip is %s.\n", info.chip_name);
        htu31d_interface_debug_print("htu31d: manufacturer is %s.\n", info.manufacturer_name);
        htu31d_interface_debug_print("htu31d: interface is %s.\n", info.interface);
        htu31d_interface_debug_print("htu31d: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        htu31d_interface_debug_print("htu31d: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        htu31d_interface_debug_print("htu31d: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        htu31d_interface_debug_print("htu31d: max current is %0.2fmA.\n", info.max_current_ma);
        htu31d_interface_debug_print("htu31d: max temperature is %0.1fC.\n", info.temperature_max);
        htu31d_interface_debug_print("htu31d: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    htu31d_interface_debug_print("htu31d: start read test.\n");
    
    /* set addr pin */
    res = htu31d_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init */
    res = htu31d_init(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: init failed.\n");
       
        return 1;
    }
    
    /* disable heater */
    res = htu31d_set_heater_off(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set heater off failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set very high */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_VERY_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set very high */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_VERY_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* output */
    htu31d_interface_debug_print("htu31d: set humidity osr very high.\n");
    htu31d_interface_debug_print("htu31d: set temperature osr very high.\n");
    
    for (i = 0; i < times; i++)
    {
        uint16_t temperature_raw;
        float temperature_s;
        uint16_t humidity_raw;
        float humidity_s;
        
        res = htu31d_read_temperature_humidity(&gs_handle,
                                               &temperature_raw, &temperature_s,
                                               &humidity_raw, &humidity_s
                                              );
        if (res != 0)
        {
            htu31d_interface_debug_print("htu31d: read temperature humidity failed.\n");
            (void)htu31d_deinit(&gs_handle);
           
            return 1;
        }
        
        /* output */
        htu31d_interface_debug_print("htu31d: temperature is %0.2fC.\n", temperature_s);
        htu31d_interface_debug_print("htu31d: humidity is %0.2f%%.\n", humidity_s);
        
        /* delay 2000ms */
        htu31d_interface_delay_ms(2000);
    }
    
    /* set low */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_LOW);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* set low */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_LOW);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
       
        return 1;
    }
    
    /* output */
    htu31d_interface_debug_print("htu31d: set humidity osr low.\n");
    htu31d_interface_debug_print("htu31d: set temperature osr low.\n");
    
    for (i = 0; i < times; i++)
    {
        uint16_t humidity_raw;
        float humidity_s;
        
        res = htu31d_read_humidity(&gs_handle, &humidity_raw, &humidity_s);
        if (res != 0)
        {
            htu31d_interface_debug_print("htu31d: read humidity failed.\n");
            (void)htu31d_deinit(&gs_handle);
           
            return 1;
        }
        
        /* output */
        htu31d_interface_debug_print("htu31d: humidity is %0.2f%%.\n", humidity_s);
        
        /* delay 2000ms */
        htu31d_interface_delay_ms(2000);
    }
    
    /* finish read test */
    htu31d_interface_debug_print("htu31d: finish read test.\n");
    (void)htu31d_deinit(&gs_handle);

    return 0;
}
