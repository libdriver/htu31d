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
 * @file      driver_htu31d_basic.h
 * @brief     driver htu31d basic header file
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

#ifndef DRIVER_HTU31D_BASIC_H
#define DRIVER_HTU31D_BASIC_H

#include "driver_htu31d_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup htu31d_example_driver htu31d example driver function
 * @brief    htu31d example driver modules
 * @ingroup  htu31d_driver
 * @{
 */

/**
 * @brief htu31d basic example default definition
 */
#define HTU21D_BASIC_DEFAULT_HUMIDITY_OSR           HTU31D_HUMIDITY_OSR_VERY_HIGH           /**< very high */
#define HTU21D_BASIC_DEFAULT_TEMPERATURE_OSR        HTU31D_TEMPERATURE_OSR_VERY_HIGH        /**< very high */

/**
 * @brief     basic example init
 * @param[in] addr_pin addr pin
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t htu31d_basic_init(htu31d_addr_pin_t addr_pin);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t htu31d_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *temperature pointer to a converted temperature buffer
 * @param[out] *humidity pointer to a converted humidity buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t htu31d_basic_read(float *temperature, float *humidity);

/**
 * @brief      basic example get the serial number
 * @param[out] *number pointer to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t htu31d_basic_get_serial_number(uint8_t number[3]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
