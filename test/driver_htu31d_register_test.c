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
 * @file      driver_htu31d_register_test.c
 * @brief     driver htu31d register test source file
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
 
#include "driver_htu31d_register_test.h"

static htu31d_handle_t gs_handle;        /**< htu31d handle */

/**
 * @brief     register test
 * @param[in] addr_pin is the addr pin
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t htu31d_register_test(htu31d_addr_pin_t addr_pin)
{
    uint8_t res;
    htu31d_info_t info;
    htu31d_humidity_osr_t humi_osr;
    htu31d_temperature_osr_t temp_osr;
    htu31d_addr_pin_t addr_pin_check;
    uint8_t diagnostic;
    uint8_t number[3];
    
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
    
    /* start register test */
    htu31d_interface_debug_print("htu31d: start register test.\n");
    
    /* htu31d_set_addr_pin/htu31d_get_addr_pin test */
    htu31d_interface_debug_print("htu31d: htu31d_set_addr_pin/htu31d_get_addr_pin test.\n");
    
    /* set addr pin low */
    res = htu31d_set_addr_pin(&gs_handle, HTU31D_ADDR_PIN_LOW);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set addr pin failed.\n");
       
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set addr pin low.\n");
    res = htu31d_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get addr pin failed.\n");
       
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check addr pin %s.\n", (HTU31D_ADDR_PIN_LOW == addr_pin_check) ? "ok" : "error");
    
    /* set addr pin high */
    res = htu31d_set_addr_pin(&gs_handle, HTU31D_ADDR_PIN_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set addr pin failed.\n");
       
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set addr pin high.\n");
    res = htu31d_get_addr_pin(&gs_handle, &addr_pin_check);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get addr pin failed.\n");
       
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check addr pin %s.\n", (HTU31D_ADDR_PIN_HIGH == addr_pin_check) ? "ok" : "error");
    
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
    
    /* htu31d_set_humidity_osr/htu31d_get_humidity_osr test */
    htu31d_interface_debug_print("htu31d: htu31d_set_humidity_osr/htu31d_get_humidity_osr test.\n");
    
    /* set osr very high */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_VERY_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr very high.\n");
    res = htu31d_get_humidity_osr(&gs_handle, &humi_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check humidity osr %s.\n", (HTU31D_HUMIDITY_OSR_VERY_HIGH == humi_osr) ? "ok" : "error");
    
    /* set osr high */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr high.\n");
    res = htu31d_get_humidity_osr(&gs_handle, &humi_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check humidity osr %s.\n", (HTU31D_HUMIDITY_OSR_HIGH == humi_osr) ? "ok" : "error");
    
    /* set osr middle */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_MIDDLE);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr middle.\n");
    res = htu31d_get_humidity_osr(&gs_handle, &humi_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check humidity osr %s.\n", (HTU31D_HUMIDITY_OSR_MIDDLE == humi_osr) ? "ok" : "error");
    
    /* set osr low */
    res = htu31d_set_humidity_osr(&gs_handle, HTU31D_HUMIDITY_OSR_LOW);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr low.\n");
    res = htu31d_get_humidity_osr(&gs_handle, &humi_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get humidity osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check humidity osr %s.\n", (HTU31D_HUMIDITY_OSR_LOW == humi_osr) ? "ok" : "error");
    
    /* htu31d_set_temperature_osr/htu31d_get_temperature_osr test */
    htu31d_interface_debug_print("htu31d: htu31d_set_temperature_osr/htu31d_get_temperature_osr test.\n");
    
    /* set osr very high */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_VERY_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr very high.\n");
    res = htu31d_get_temperature_osr(&gs_handle, &temp_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check temperature osr %s.\n", (HTU31D_TEMPERATURE_OSR_VERY_HIGH == temp_osr) ? "ok" : "error");
    
    /* set osr high */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_HIGH);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr high.\n");
    res = htu31d_get_temperature_osr(&gs_handle, &temp_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check temperature osr %s.\n", (HTU31D_TEMPERATURE_OSR_HIGH == temp_osr) ? "ok" : "error");

    /* set osr middle */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_MIDDLE);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr middle.\n");
    res = htu31d_get_temperature_osr(&gs_handle, &temp_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check temperature osr %s.\n", (HTU31D_TEMPERATURE_OSR_MIDDLE == temp_osr) ? "ok" : "error");
    
    /* set osr low */
    res = htu31d_set_temperature_osr(&gs_handle, HTU31D_TEMPERATURE_OSR_LOW);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: set osr low.\n");
    res = htu31d_get_temperature_osr(&gs_handle, &temp_osr);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get temperature osr failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: check temperature osr %s.\n", (HTU31D_TEMPERATURE_OSR_LOW == temp_osr) ? "ok" : "error");
    
    /* htu31d_set_heater_on test */
    htu31d_interface_debug_print("htu31d: htu31d_set_heater_on test.\n");
    
    /* heater on */
    res = htu31d_set_heater_on(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set heater on failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: heater on.\n");
    htu31d_interface_debug_print("htu31d: heater on %s.\n", (res == 0) ? "ok" : "error");
    
    /* htu31d_set_heater_off test */
    htu31d_interface_debug_print("htu31d: htu31d_set_heater_off test.\n");
    
    /* heater off */
    res = htu31d_set_heater_off(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: set heater off failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: heater off.\n");
    htu31d_interface_debug_print("htu31d: heater off %s.\n", (res == 0) ? "ok" : "error");
    
    /* htu31d_get_serial_number test */
    htu31d_interface_debug_print("htu31d: htu31d_get_serial_number test.\n");
    
    /* get serial number */
    res = htu31d_get_serial_number(&gs_handle, number);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get serial number failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: serial number is 0x%02X 0x%02X 0x%02X.\n", number[0], number[1], number[2]);
    
    /* htu31d_get_diagnostic test */
    htu31d_interface_debug_print("htu31d: htu31d_get_diagnostic test.\n");
    
    /* get diagnostic */
    res = htu31d_get_diagnostic(&gs_handle, &diagnostic);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: get diagnostic failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    htu31d_interface_debug_print("htu31d: diagnostic is 0x%02X.\n", diagnostic);
    
    /* htu31d_soft_reset test */
    htu31d_interface_debug_print("htu31d: htu31d_soft_reset test.\n");
    
    /* soft reset */
    res = htu31d_soft_reset(&gs_handle);
    if (res != 0)
    {
        htu31d_interface_debug_print("htu31d: soft reset failed.\n");
        (void)htu31d_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish register test */
    htu31d_interface_debug_print("htu31d: finish register test.\n");
    (void)htu31d_deinit(&gs_handle);
    
    return 0;
}
