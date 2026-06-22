#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "iwdg.h"
#include <stdbool.h>
#include "tim.h"
#include "spi.h"
#include <string.h>
#include "logger.h"
#include "stm32h5xx_hal_flash.h"
#include "stm32h5xx_hal_flash_ex.h"

#define BSP_RS485_RF_TRANSFER_MODE 1
#define BSP_LOG_LEVEL LOGGER_DEBUG
#define BSP_BUTTON_EN 0
#define BSP_ANALOG_ENABLE 0
#define BSP_IO_ENABLE 1

#if BSP_ANALOG_ENABLE
#define BSP_NUM_ADC_CHANNEL 5
#endif

#if BSP_IO_ENABLE
/**
 * @def BSP_OUTPUT_NUM
 * @brief Defines the number of output channels available on the board.
 *
 * This macro specifies the total number of output channels (e.g., relays, digital outputs)
 * that are supported by the board hardware. Update this value if the hardware configuration changes.
 */
#define BSP_OUTPUT_NUM 4
/**
 * @def BSP_INPUT_NUM
 * @brief Defines the number of input channels available on the board.
 *
 * This macro specifies the total number of input lines (e.g., digital or analog inputs)
 * that are supported by the board hardware. It is used throughout the codebase to
 * reference the number of available input channels, ensuring consistency and ease of maintenance.
 */
#define BSP_INPUT_NUM 4
/**
 * @def BSP_LED_NUM
 * @brief Defines the number of LEDs available on the board.
 * 
 * This macro specifies the total count of LEDs that are present and can be controlled
 * via the board support package (BSP). Adjust this value according to the hardware configuration.
 */
#endif
#define BSP_LED_NUM 2
/**
 * @def BSP_ADDRESS_NUM
 * @brief Defines the number of board support package (BSP) addresses.
 *
 * This macro specifies the total number of addresses used by the BSP.
 * Adjust this value according to the number of supported addresses required by your hardware configuration.
 */
#define BSP_ADDRESS_NUM 5
/**
 * @def BSP_UART_NUM
 * @brief Defines the number of UART peripherals available on the board.
 *
 * This macro specifies the total count of UART (Universal Asynchronous Receiver/Transmitter)
 * interfaces supported by the board. It is used for configuring and managing UART resources
 * in the application.
 */
#define BSP_UART_NUM 3

/**
 * @def BSP_DEBUG_COM_PORT
 * @brief Defines the communication port number used for debugging purposes.
 * 
 * This macro specifies which COM port is designated as the debug port for the board support package (BSP).
 * The value should correspond to the port index used in the hardware or software configuration.
 */
#define BSP_DEBUG_COM_PORT 0
/**
 * @def BSP_RS485_COM_PORT
 * @brief Defines the communication port number used for RS485 interface.
 * 
 * This macro specifies which COM port is assigned for RS485 communication
 * on the board. Update this value if the RS485 interface is connected to a
 * different port.
 */
#define BSP_RS485_COM_PORT 1
/**
 * @def BSP_RF_COM_PORT
 * @brief Defines the communication port number used for RF (Radio Frequency) operations on the board.
 * 
 * This macro specifies which COM port (by number) is assigned for RF communication.
 * Adjust this value according to the hardware configuration and requirements.
 */
#define BSP_RF_COM_PORT 2

/**
 * @def TIMER0
 * @brief Macro definition for timer 0 identifier.
 *
 * This macro defines the identifier for Timer 0, typically used to reference
 * the first hardware timer in the system. The value assigned is 0.
 */
#define TIMER0 0
/**
 * @def TIMER1
 * @brief Macro definition for timer identifier 1.
 *
 * This macro defines a constant value representing Timer 1.
 * It can be used throughout the codebase to refer to Timer 1 in a clear and maintainable way.
 */
#define TIMER1 1

// MODBUS RS485 ADRESS GPIO
// #define BSP_ADDRESS_PORT GPIOB
// #define BSP_ADDRESS_BIT0_Pin GPIO_PIN_8
// #define BSP_ADDRESS_BIT1_Pin GPIO_PIN_7
// #define BSP_ADDRESS_BIT2_Pin GPIO_PIN_6
// #define BSP_ADDRESS_BIT3_Pin GPIO_PIN_5
// #define BSP_ADDRESS_BIT4_Pin GPIO_PIN_4

// LED INDICATOR GPIO
#define BSP_LED0_PORT LED0_GPIO_Port
#define BSP_LED0_Pin LED0_Pin

#define BSP_LED1_PORT LED1_GPIO_Port
#define BSP_LED1_Pin LED1_Pin

#define BSP_LED2_PORT LED2_GPIO_Port
#define BSP_LED2_Pin LED2_Pin
// INPUT GPIO
#define BSP_INPUT0_PORT IN0_GPIO_Port 
#define BSP_INPUT1_PORT IN1_GPIO_Port 
#define BSP_INPUT2_PORT IN2_GPIO_Port 
#define BSP_INPUT3_PORT IN3_GPIO_Port 
#define BSP_INPUT0_Pin IN0_Pin
#define BSP_INPUT1_Pin IN1_Pin
#define BSP_INPUT2_Pin IN2_Pin
#define BSP_INPUT3_Pin IN3_Pin

// OUTPUT GPIO
#define BSP_OUTPUT0_PORT OUT0_GPIO_Port 
#define BSP_OUTPUT1_PORT OUT1_GPIO_Port 
#define BSP_OUTPUT2_PORT OUT2_GPIO_Port 
#define BSP_OUTPUT3_PORT OUT3_GPIO_Port 

#define BSP_OUTPUT0_Pin OUT0_Pin
#define BSP_OUTPUT1_Pin OUT1_Pin
#define BSP_OUTPUT2_Pin OUT2_Pin
#define BSP_OUTPUT3_Pin OUT3_Pin
// BUTTTON GPIO

#define BSP_BUTTON_PORT GPIOB 
#define BSP_BUTTON_Pin  GPIO_PIN_9

#define SERIAL_WORDLENGTH_8B UART_WORDLENGTH_8B
#define SERIAL_WORDLENGTH_9B UART_WORDLENGTH_9B

#define SERIAL_STOPBITS_1     UART_STOPBITS_1
#define SERIAL_STOPBITS_2     UART_STOPBITS_2

#define SERIAL_PARITY_NONE    UART_PARITY_NONE
#define SERIAL_PARITY_EVEN    UART_PARITY_EVEN
#define SERIAL_PARITY_ODD     UART_PARITY_ODD

#define bsp_button_status() 0

/**
 * @brief Turns the status LED on.
 *
 * This macro sets the specified GPIO pin (BSP_LED0_Pin on BSP_LED0_PORT) to a low logic level (RESET),
 * which turns the status LED on. The macro wraps the operation in a do-while(0) construct for safe usage
 * in conditional statements and code blocks.
 */
#define bsp_led_status_on()     do{HAL_GPIO_WritePin(BSP_LED0_PORT,BSP_LED0_Pin,GPIO_PIN_RESET);}while(0)
/**
 * @brief Turns the status LED off.
 *
 * This macro sets the specified GPIO pin (BSP_LED0_Pin on BSP_LED0_PORT) to a high logic level,
 * which turns off the status LED according to the board's wiring.
 * It uses the HAL_GPIO_WritePin function to perform the operation.
 */
#define bsp_led_status_off()    do{HAL_GPIO_WritePin(BSP_LED0_PORT,BSP_LED0_Pin,GPIO_PIN_SET);}while(0)
/**
 * @brief Toggles the status LED.
 *
 * This macro toggles the state of the status LED connected to BSP_LED0_PORT and BSP_LED0_Pin.
 * It uses the HAL_GPIO_TogglePin function to change the LED state.
 */
#define bsp_led_status_toggle() do{HAL_GPIO_TogglePin(BSP_LED0_PORT,BSP_LED0_Pin);}while(0)
/**
 * @brief Turns on the network LED by setting the corresponding GPIO pin to RESET (active low).
 *
 * This macro controls the network status LED (BSP_LED1) by writing a RESET value to its GPIO pin,
 * effectively turning the LED on if it is configured as active low.
 *
 * Usage:
 *     bsp_led_net_on();
 *
 * Note:
 *     - Ensure BSP_LED1_PORT and BSP_LED1_Pin are properly defined.
 *     - The macro uses a do-while(0) construct for safe expansion in all contexts.
 */
#define bsp_led_net_on()        do{HAL_GPIO_WritePin(BSP_LED1_PORT,BSP_LED1_Pin,GPIO_PIN_RESET);}while(0)
/**
 * @brief Turns off the network LED by setting the corresponding GPIO pin.
 *
 * This macro sets the BSP_LED1_Pin on BSP_LED1_PORT to a high logic level (GPIO_PIN_SET),
 * which turns off the network LED indicator. It uses the HAL_GPIO_WritePin function
 * provided by the hardware abstraction layer.
 */
#define bsp_led_net_off()       do{HAL_GPIO_WritePin(BSP_LED1_PORT,BSP_LED1_Pin,GPIO_PIN_SET);}while(0)
/**
 * @brief Toggles the state of the network LED.
 *
 * This macro toggles the GPIO pin associated with the network LED (BSP_LED1).
 * It uses the HAL_GPIO_TogglePin function to invert the current state of the LED.
 * 
 * Usage:
 *     bsp_led_net_toggle();
 *
 * Note:
 *     Ensure that BSP_LED1_PORT and BSP_LED1_Pin are properly defined before using this macro.
 */
#define bsp_led_net_toggle()    do{HAL_GPIO_TogglePin(BSP_LED1_PORT,BSP_LED1_Pin) ;}while(0)

#define bsp_led_pow_on() HAL_GPIO_WritePin(BSP_LED2_PORT,BSP_LED2_Pin,GPIO_PIN_SET)
#define bsp_led_pow_off() HAL_GPIO_WritePin(BSP_LED2_PORT,BSP_LED2_Pin,GPIO_PIN_RESET)
#define bsp_led_pow_toggle() HAL_GPIO_TogglePin(BSP_LED2_PORT,BSP_LED2_Pin)
/**
 * @brief Macro to retrieve the current system tick value.
 *
 * This macro maps bsp_get_tick to the HAL_GetTick function, which returns
 * the number of milliseconds since the system started. It is typically used
 * for timing operations and delays.
 *
 * @see HAL_GetTick()
 */
#define bsp_get_tick() HAL_GetTick()

/**
 * @brief Refreshes (reloads) the Independent Watchdog (IWDG) timer.
 *
 * This macro calls HAL_IWDG_Refresh() with the handle to the IWDG peripheral,
 * preventing the watchdog from resetting the system. It should be called
 * periodically within the allowed time window to ensure normal operation.
 */
#define bsp_iwdg_refresh() HAL_IWDG_Refresh(&hiwdg)

/**
 * @brief Triggers a system reset using the NVIC (Nested Vectored Interrupt Controller).
 *
 * This macro provides a convenient way to restart the microcontroller by invoking
 * the NVIC_SystemReset() function. It can be used to perform a software reset of the system.
 */
#define bsp_restart()   NVIC_SystemReset()

/**
 * @brief Delays execution for a specified number of milliseconds and refreshes the independent watchdog timer.
 *
 * This macro wraps the HAL_Delay function to pause execution for 'x' milliseconds,
 * and then calls bsp_iwdg_refresh() to reset the watchdog timer, preventing a system reset.
 *
 * @param x Number of milliseconds to delay.
 */
#define bsp_delay(x) do{HAL_Delay(x);bsp_iwdg_refresh();}while (0)

/**
 * @brief  Enables the DE (Driver Enable) pin for UART2.
 *         This macro sets the USART2_DE pin high, typically used to control the direction
 *         of data flow in RS485 communication (enabling transmission mode).
 * @note   Make sure USART2_DE_GPIO_Port and USART2_DE_Pin are properly defined.
 */
#define bsp_uart2_de_on() HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin,GPIO_PIN_SET)
/**
 * @brief Disables the UART2 Driver Enable (DE) pin by setting it to a low logic level.
 *
 * This macro sets the USART2_DE_Pin on the USART2_DE_GPIO_Port to GPIO_PIN_RESET,
 * effectively turning off the driver enable signal for RS485 communication.
 */
#define bsp_uart2_de_off() HAL_GPIO_WritePin(USART1_DE_GPIO_Port,USART1_DE_Pin,GPIO_PIN_RESET)

/**
 * @brief Starts Timer 2 in interrupt mode.
 *
 * This macro initializes and starts the base timer (TIM2) with interrupt enabled
 * using the HAL library function HAL_TIM_Base_Start_IT. It is typically used to
 * begin periodic operations or timing events that require interrupt handling.
 *
 * @note Ensure that the timer handle 'htim2' is properly initialized before calling this macro.
 */
#define bsp_timer0_start() HAL_TIM_Base_Start_IT(&htim2)
/**
 * @brief Stops Timer 0 using interrupt mode.
 *
 * This macro calls HAL_TIM_Base_Stop_IT() with the handle to Timer 2 (htim2),
 * effectively stopping the timer's base generation in interrupt mode.
 *
 * @note Ensure that htim2 is properly initialized before calling this macro.
 */
#define bsp_timer0_stop() HAL_TIM_Base_Stop_IT(&htim2)

/**
 * @brief Starts Timer 1 using interrupt mode.
 *
 * This macro starts the base timer (TIM3) with interrupt enabled by calling HAL_TIM_Base_Start_IT.
 * Ensure that the timer handle 'htim3' is properly initialized before invoking this macro.
 */
#define bsp_timer1_start() HAL_TIM_Base_Start_IT(&htim3)
/**
 * @brief Stops Timer 1 using interrupt mode.
 *
 * This macro stops the base timer (TIM3) by calling HAL_TIM_Base_Stop_IT
 * with the handle to TIM3. It is typically used to halt timer operations
 * that are running in interrupt mode.
 *
 * @note Ensure that the timer handle 'htim3' is properly initialized before calling this macro.
 */
#define bsp_timer1_stop() HAL_TIM_Base_Stop_IT(&htim3)

void BSP_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#define BSP_FLASH_SECTOR_SIZE (uint32_t)(8 * 1024) // bytes
#define BSP_FLASH_BASE FLASH_BASE
#define BSP_FLASH_SIZE (224 * 1024)
#define BSP_FLASH_SECTOR_NUM (BSP_FLASH_SIZE / BSP_FLASH_SECTOR_SIZE)

#define bsp_flash_unlock() HAL_FLASH_Unlock()
/**
 * @brief   Locks the FLASH control register access.
 *
 * This macro calls the HAL_FLASH_Lock() function to disable write access
 * to the FLASH control registers, preventing accidental modification of
 * FLASH memory. It is typically used after FLASH operations are complete
 * to enhance system safety.
 */
#define bsp_flash_lock() HAL_FLASH_Lock()

    /**
     * @brief Erases one or more pages in flash memory starting from the specified address.
     *
     * This function erases a specified number of flash memory pages, beginning at the given page address.
     *
     * @param address The starting address of the flash page to erase.
     * @param num_sectors The number of consecutive pages to erase.
     * @return uint32_t Status code indicating success or failure of the erase operation.
     */
    static inline uint32_t bsp_flash_erase(uint32_t address, uint32_t num_sectors)
    {
        uint32_t bank = FLASH_BANK_1,sector = FLASH_SECTOR_0;

        sector = ((address - FLASH_BASE) / FLASH_SECTOR_SIZE ) % 15;
        
        bank = (((address - FLASH_BASE) / FLASH_SECTOR_SIZE ) > 15) ? FLASH_BANK_2 : FLASH_BANK_1;

        FLASH_EraseInitTypeDef erase = {.Banks = bank, .Sector = sector, .NbSectors = num_sectors, .TypeErase = FLASH_TYPEERASE_SECTORS};
        uint32_t page_err = 0;
        if (HAL_FLASHEx_Erase(&erase, &page_err) != HAL_OK)
        {
            return page_err;
        }
        return 0;
    }

    /**
     * @brief Programs data to the flash memory at the specified address.
     *
     * This function writes a block of data to the flash memory starting at the given address.
     *
     * @param address The starting address in flash memory where data will be programmed.
     * @param data Pointer to the data buffer to be written to flash.
     * @param len The number of bytes to program from the data buffer.
     * @return uint32_t Status code indicating success or failure of the operation.
     */
    static inline uint32_t bsp_flash_program(uint32_t addr,uint8_t *buf,uint32_t len)
    {
        // HAL_FLASH_Unlock();
        __disable_irq();

        for (uint32_t i = 0; i < len; i += 16)
        {
            uint8_t quadword[16];
            memset(quadword, 0xFF, 16);
            memcpy(quadword, &buf[i], (len - i >= 16) ? 16 : (len - i));
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, addr + i, (uint32_t)quadword) != HAL_OK)
            {
                __enable_irq();
                // HAL_FLASH_Lock();
                return -1;
            }
        }
        __enable_irq();
        // HAL_FLASH_Lock();
        return 0;
    }

    /**
     * @brief Reads data from flash memory at the specified address.
     *
     * @param address The starting address in flash memory to read from.
     * @param data Pointer to the buffer where the read data will be stored.
     * @param len Number of bytes to read from flash memory.
     * @return uint32_t Status code indicating success or failure of the read operation.
     */
    static inline uint32_t bsp_flash_read(uint32_t address, void *data, uint32_t len)
    {
        memcpy(data, (void *)address, len);
        return 0;
    }

typedef void (*timer_handle)();

/**
 * @brief Sets the handler function for a specified timer.
 *
 * Associates a handler function with a timer, allowing the timer to invoke
 * the specified handler when triggered.
 *
 * @param timer The identifier of the timer to set the handler for.
 * @param handle The handler function to be called when the timer event occurs.
 */
void bsp_timer_set_handle(int timer,timer_handle handle);

/**
 * @brief Enables the DE (Driver Enable) signal for the BSP (Board Support Package).
 *
 * This function is typically used to control the direction of data flow
 * in RS485 communication by enabling the driver output.
 *
 * @return true if the DE signal was successfully enabled, false otherwise.
 */
static inline bool bsp_de_on(){
    bsp_uart2_de_on();
    return true;
}
/**
 * @brief Turns off the DE (Driver Enable) signal for the BSP (Board Support Package).
 *
 * This function disables the driver enable line, typically used in RS485 communication
 * to control the direction of data flow. When DE is off, the transceiver is set to receive mode.
 *
 * @return true if the operation was successful, false otherwise.
 */
static inline bool bsp_de_off(){
    bsp_uart2_de_off();
    return true;
}

#if BSP_OUTPUT_NUM > 0 && BSP_INPUT_NUM > 0
/**
 * @brief Retrieves the state of a specified input.
 *
 * This function returns the current state (typically HIGH or LOW) of the input
 * specified by the input_num parameter.
 *
 * @param input_num The index or identifier of the input to read.
 * @return uint8_t The state of the specified input (e.g., 0 for LOW, 1 for HIGH).
 */
uint8_t bsp_get_input(int input_num);
uint8_t bsp_output_on(int output_num);
uint8_t bsp_output_off(int output_num);
uint8_t bsp_output_toggle(int output_num);
#endif
uint32_t bsp_com_write(int com_num,uint8_t *buff,uint32_t len);
uint32_t bsp_com_write_it(int com,uint8_t *buff,uint32_t len);
void bsp_com_set_tx_callback(int com,void (*callback)(void *arg),void *arg);
uint32_t bsp_com_read(int com_num,uint8_t *buff,uint32_t len);
uint32_t bsp_com_available(int com_num);
bool bsp_com_ready(int com);
void bsp_com_flush(int com);
uint32_t bsp_com_init();
uint8_t bsp_get_address();
void bsp_init();

#ifndef USB_ENABLE
#define USB_ENABLE 1
#endif

#if USB_ENABLE



#define BSP_USB_NUM NUMBER_OF_CDC

void bsp_usb_init();
int bsp_usb_transmit(int ch,uint8_t *buff,uint32_t len);
int bsp_usb_receiver(int ch,uint8_t *buff,uint32_t len);
int bsp_usb_available(int ch);
int bsp_usb_rx_callback(int ch,uint8_t *buff,uint32_t len);

#endif

#ifdef __cplusplus
}
#endif

#endif //BOARD_H