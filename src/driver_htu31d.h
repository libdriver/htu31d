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
 * @file      driver_htu31d.h
 * @brief     driver htu31d header file
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

#ifndef DRIVER_HTU31D_H
#define DRIVER_HTU31D_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup htu31d_driver htu31d driver function
 * @brief    htu31d driver modules
 * @{
 */

/**
 * @addtogroup htu31d_base_driver
 * @{
 */

/**
 * @brief htu31d addr pin enumeration definition
 */
typedef enum
{
    HTU31D_ADDR_PIN_LOW  = (0x40 << 1),        /**< pin to gnd */
    HTU31D_ADDR_PIN_HIGH = (0x41 << 1),        /**< pin to vcc */
} htu31d_addr_pin_t;

/**
 * @brief htu31d humidity osr enumeration definition
 */
typedef enum
{
    HTU31D_HUMIDITY_OSR_VERY_HIGH = 3,        /**< 0.007%rh */
    HTU31D_HUMIDITY_OSR_HIGH      = 2,        /**< 0.010%rh */
    HTU31D_HUMIDITY_OSR_MIDDLE    = 1,        /**< 0.014%rh */
    HTU31D_HUMIDITY_OSR_LOW       = 0,        /**< 0.020%rh */
} htu31d_humidity_osr_t;

/**
 * @brief htu31d temperature osr enumeration definition
 */
typedef enum
{
    HTU31D_TEMPERATURE_OSR_VERY_HIGH = 3,        /**< 0.012°C */
    HTU31D_TEMPERATURE_OSR_HIGH      = 2,        /**< 0.016°C */
    HTU31D_TEMPERATURE_OSR_MIDDLE    = 1,        /**< 0.025°C */
    HTU31D_TEMPERATURE_OSR_LOW       = 0,        /**< 0.040°C */
} htu31d_temperature_osr_t;

/**
 * @brief htu31d diagnostic enumeration definition
 */
typedef enum
{
    HTU31D_DIAGNOSTIC_NVM_ERROR                 = (1 << 7),        /**< nvm error */
    HTU31D_DIAGNOSTIC_HUMIDITY_UNDER_OVERRUN    = (1 << 6),        /**< humidity under/overrun */
    HTU31D_DIAGNOSTIC_HUMIDITY_HIGH_ERROR       = (1 << 5),        /**< humidity high error */
    HTU31D_DIAGNOSTIC_HUMIDITY_LOW_ERROR        = (1 << 4),        /**< humidity low error */
    HTU31D_DIAGNOSTIC_TEMPERATURE_UNDER_OVERRUN = (1 << 3),        /**< temperature under/overrun */
    HTU31D_DIAGNOSTIC_TEMPERATURE_HIGH_ERROR    = (1 << 2),        /**< temperature high error */
    HTU31D_DIAGNOSTIC_TEMPERATURE_LOW_ERROR     = (1 << 1),        /**< temperature low error */
    HTU31D_DIAGNOSTIC_HEATER_ON                 = (1 << 0),        /**< heater on */
} htu31d_diagnostic_t;

/**
 * @brief htu31d handle structure definition
 */
typedef struct htu31d_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t humidity_osr;                                                               /**< humidity osr */
    uint8_t temperature_osr;                                                            /**< temperature osr */
} htu31d_handle_t;

/**
 * @brief htu31d information structure definition
 */
typedef struct htu31d_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} htu31d_info_t;

/**
 * @}
 */

/**
 * @defgroup htu31d_link_driver htu31d link driver function
 * @brief    htu31d link driver modules
 * @ingroup  htu31d_driver
 * @{
 */

/**
 * @brief     initialize htu31d_handle_t structure
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] STRUCTURE htu31d_handle_t
 * @note      none
 */
#define DRIVER_HTU31D_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_IIC_INIT(HANDLE, FUC)                 (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_IIC_DEINIT(HANDLE, FUC)               (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_IIC_WRITE(HANDLE, FUC)                (HANDLE)->iic_write = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_IIC_READ(HANDLE, FUC)                 (HANDLE)->iic_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_DELAY_MS(HANDLE, FUC)                 (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an htu31d handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_HTU31D_LINK_DEBUG_PRINT(HANDLE, FUC)              (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup htu31d_base_driver htu31d base driver function
 * @brief    htu31d base driver modules
 * @ingroup  htu31d_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an htu31d info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t htu31d_info(htu31d_info_t *info);

/**
 * @brief     set the address pin
 * @param[in] *handle pointer to an htu31d handle structure
 * @param[in] addr_pin addr pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t htu31d_set_addr_pin(htu31d_handle_t *handle, htu31d_addr_pin_t addr_pin);

/**
 * @brief      get the address pin
 * @param[in]  *handle pointer to an htu31d handle structure
 * @param[out] *addr_pin pointer to an addr pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t htu31d_get_addr_pin(htu31d_handle_t *handle, htu31d_addr_pin_t *addr_pin);

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
uint8_t htu31d_init(htu31d_handle_t *handle);

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
uint8_t htu31d_deinit(htu31d_handle_t *handle);

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
uint8_t htu31d_set_humidity_osr(htu31d_handle_t *handle, htu31d_humidity_osr_t osr);

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
uint8_t htu31d_get_humidity_osr(htu31d_handle_t *handle, htu31d_humidity_osr_t *osr);

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
uint8_t htu31d_set_temperature_osr(htu31d_handle_t *handle, htu31d_temperature_osr_t osr);

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
uint8_t htu31d_get_temperature_osr(htu31d_handle_t *handle, htu31d_temperature_osr_t *osr);

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
                                        );

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
uint8_t htu31d_read_humidity(htu31d_handle_t *handle, uint16_t *humidity_raw, float *humidity_s);

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
uint8_t htu31d_soft_reset(htu31d_handle_t *handle);

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
uint8_t htu31d_set_heater_on(htu31d_handle_t *handle);

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
uint8_t htu31d_set_heater_off(htu31d_handle_t *handle);

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
uint8_t htu31d_get_serial_number(htu31d_handle_t *handle, uint8_t number[3]);

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
uint8_t htu31d_get_diagnostic(htu31d_handle_t *handle, uint8_t *diagnostic);

/**
 * @}
 */

/**
 * @defgroup htu31d_extend_driver htu31d extend driver function
 * @brief    htu31d extend driver modules
 * @ingroup  htu31d_driver
 * @{
 */

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
uint8_t htu31d_set_reg(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

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
uint8_t htu31d_get_reg(htu31d_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
