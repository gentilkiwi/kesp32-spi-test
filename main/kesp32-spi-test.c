/*	Benjamin DELPY `gentilkiwi`
	https://blog.gentilkiwi.com
	benjamin@gentilkiwi.com
	Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "driver/spi_master.h"
#include "driver/gpio.h"

#if CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define SPI             SPI2_HOST
#define GPIO_CLK        GPIO_NUM_12
#define GPIO_MOSI       GPIO_NUM_11
#define GPIO_MISO       GPIO_NUM_13
#define GPIO_CS         GPIO_NUM_10
#else
#error Target not supported	
#endif

#define SPICS_IO_NUM    -1 //GPIO_CS

const spi_bus_config_t SPI_BUS_CFG = {
    .mosi_io_num = GPIO_MOSI,
    .miso_io_num = GPIO_MISO,
    .sclk_io_num = GPIO_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .data4_io_num = -1,
    .data5_io_num = -1,
    .data6_io_num = -1,
    .data7_io_num = -1,
    .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
    .flags = 0,
    .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
    .intr_flags = 0,
};

const spi_device_interface_config_t SPI_DEV_CFG = {
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .mode = 0,
    .clock_source = SPI_CLK_SRC_DEFAULT,
    .duty_cycle_pos = 128,
    .cs_ena_pretrans = 0,
    .cs_ena_posttrans = 0,
    .clock_speed_hz = 8000000,
    .input_delay_ns = 0,
    .spics_io_num = SPICS_IO_NUM,
    .flags = 0, //SPI_DEVICE_BIT_LSBFIRST
    .queue_size = 1,
    .pre_cb = NULL,
    .post_cb = NULL,
};

const uint8_t TX_BUFFER_16[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

void app_main(void)
{
    esp_err_t ret;
    spi_device_handle_t hDevice;
    
    spi_transaction_t transaction = {
        .length = sizeof(TX_BUFFER_16) * 8,
        .tx_buffer = TX_BUFFER_16,
        .rxlength = 0,
        .rx_buffer = NULL,
    };

#if SPICS_IO_NUM == -1
    gpio_set_direction(GPIO_CS, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_CS, 1);
#endif

    ret = spi_bus_initialize(SPI, &SPI_BUS_CFG, SPI_DMA_DISABLED);
    if(ret == ESP_OK)
    {
        ret = spi_bus_add_device(SPI, &SPI_DEV_CFG, &hDevice);
        if(ret == ESP_OK)
        {
            spi_device_acquire_bus(hDevice, portMAX_DELAY);

            while(1) {
#if SPICS_IO_NUM == -1
                gpio_set_level(GPIO_CS, 0);
#endif
                /*ret = */spi_device_polling_transmit(hDevice, &transaction); // spi_device_transmit is slower
#if SPICS_IO_NUM == -1
                gpio_set_level(GPIO_CS, 1);
#endif
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            spi_device_release_bus(hDevice);

            /*ret = */spi_bus_remove_device(hDevice);
        }

        /*ret = */spi_bus_free(SPI);
    }
}
