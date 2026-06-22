#include "app_settings.h"
#include "logger.h"
#include "user_mb_app.h"

static const char *TAG = "Settings";

void app_setting_load(APP_Settings_t *setting)
{
    bsp_flash_read(APP_SETTING_ADDRESS, setting, sizeof(APP_Settings_t));
    if (setting->mg_number != MG_NUMBER)
    {
        log_error(TAG, "Setting area error, overwrite setting default");
        setting->id = 1;
        memset(setting->sn, 0, sizeof(setting->sn));
        strncpy((char *)setting->sn, PRODUCT_SN, SN_LENGTH);
        setting->com.baudrate = 115200;
        setting->com.databits = DATA8;
        setting->com.parity = PARITY_NONE;
        setting->com.stopbits = STOPBITS_1;

        setting->rf.baudrate = 115200;
        setting->rf.databits = DATA8;
        setting->rf.parity = PARITY_NONE;
        setting->rf.stopbits = STOPBITS_1;

        setting->rs485.baudrate = 115200;
        setting->rs485.databits = DATA8;
        setting->rs485.parity = PARITY_NONE;
        setting->rs485.stopbits = STOPBITS_1;
        setting->log_level = LOGGER_DEBUG;
        setting->app_mode = Parency_Modbus;
        setting->mg_number = MG_NUMBER;
        for(int i = 0;i < BSP_OUTPUT_NUM;i++){
            setting->input_mask[i] = 0x0000;
            setting->output_mask[i] = 0x0000;
        }
        // setting->input_mask = 0;
        app_setting_save(setting);
    }
    memcpy(input_reg,setting->sn,SN_LENGTH);
}

void app_setting_save(APP_Settings_t *setting)
{
    bsp_flash_unlock();
    // bsp_flash_erase(APP_SETTING_ADDRESS, 1);
    FLASH_EraseInitTypeDef erase = {.Banks = APP_SETTING_BANK, .Sector = APP_SETTING_SECTOR, .NbSectors = 1, .TypeErase = FLASH_TYPEERASE_SECTORS};
    uint32_t sector_err = 0;
    HAL_FLASHEx_Erase(&erase,&sector_err);
    FLASH_WaitForLastOperation(10000);
    bsp_flash_program(APP_SETTING_ADDRESS,(uint8_t*) setting, sizeof(APP_Settings_t));
    bsp_flash_lock();
}

void app_setting_printf(APP_Settings_t *setting){
    log_info(TAG,"Modbus Address : %d",setting->id);
    log_info(TAG,"S/N : %s",(char*)setting->sn);
    log_info(TAG,"Logger level : %d",setting->log_level);
    log_info(TAG, "RS485 :");
    log_info(TAG, "Baudrate : %lu", setting->rs485.baudrate);
    log_info(TAG, "Databits : %d", setting->rs485.databits == DATA8 ? 8 : 9);
    log_info(TAG, "Stopbits : %d", setting->rs485.stopbits == STOPBITS_1 ? 1 : 2);
    switch (setting->rs485.parity)
    {
    case PARITY_NONE:
        /* code */
        log_info(TAG, "Parity   : None");
        break;
    case PARITY_EVEN:
        log_info(TAG, "Parity   : Even");
        break;
    case PARITY_ODD:
        log_info(TAG, "Parity   : Odd");
        break;
    default:
        break;
    }

    log_info(TAG, "RF :");
    log_info(TAG, "Baudrate : %lu", setting->rf.baudrate);
    log_info(TAG, "Databits : %d", setting->rf.databits == DATA8 ? 8 : 9);
    log_info(TAG, "Stopbits : %d", setting->rf.stopbits == STOPBITS_1 ? 1 : 2);
    switch (setting->rf.parity)
    {
    case PARITY_NONE:
        /* code */
        log_info(TAG, "Parity   : None");
        break;
    case PARITY_EVEN:
        log_info(TAG, "Parity   : Even");
        break;
    case PARITY_ODD:
        log_info(TAG, "Parity   : Odd");
        break;
    default:
        break;
    }

    log_info(TAG, "USB :");
    log_info(TAG, "Baudrate : %lu", setting->com.baudrate);
    log_info(TAG, "Databits : %d", setting->com.databits == DATA8 ? 8 : 9);
    log_info(TAG, "Stopbits : %d", setting->com.stopbits == STOPBITS_1 ? 1 : 2);
    switch (setting->com.parity)
    {
    case PARITY_NONE:
        /* code */
        log_info(TAG, "Parity   : None");
        break;
    case PARITY_EVEN:
        log_info(TAG, "Parity   : Even");
        break;
    case PARITY_ODD:
        log_info(TAG, "Parity   : Odd");
        break;
    default:
        break;
    }

    log_info(TAG,"Mode : %d",setting->app_mode);
}

void app_setup_serial(int serial, Serial_t *config)
{
    switch (serial)
    {
    case BSP_DEBUG_COM_PORT:
        /* code */
        // huart1.Instance = LPUART1;
        // huart1.Init.BaudRate = config->baudrate;
        // huart1.Init.WordLength = UART_WORDLENGTH_8B;
        // huart1.Init.StopBits = UART_STOPBITS_1;
        // huart1.Init.Parity = UART_PARITY_NONE;
        // huart1.Init.Mode = UART_MODE_TX_RX;
        // huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        // huart1.Init.OverSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        // hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        // if (HAL_UART_Init(&hlpuart1) != HAL_OK)
        // {
        //     Error_Handler();
        // }
        // if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        // {
        //     Error_Handler();
        // }
        // if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        // {
        //     Error_Handler();
        // }
        // if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
        // {
        //     Error_Handler();
        // }
        break;
    case BSP_RS485_COM_PORT:
        huart1.Instance = USART1;
        huart1.Init.BaudRate = config->baudrate;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
        huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart1) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    case BSP_RF_COM_PORT:
        huart2.Instance = USART2;
        huart2.Init.BaudRate = config->baudrate;
        huart2.Init.WordLength = UART_WORDLENGTH_8B;
        huart2.Init.StopBits = UART_STOPBITS_1;
        huart2.Init.Parity = UART_PARITY_NONE;
        huart2.Init.Mode = UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart2.Init.OverSampling = UART_OVERSAMPLING_16;
        huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
        huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart2) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
        {
            Error_Handler();
        }
        break;
    default:
        break;
    }
}

APP_Settings_t app_setting;