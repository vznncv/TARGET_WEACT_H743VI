/**
 * Internal ADC usage example.
 */
#include "mbed.h"

DigitalOut user_led(LED1);

/**
 * Helper STM32F4 specific function to calculate Vref voltage.
 *
 * @return reference voltage (unit: V)
 */
float calculate_reference_voltage()
{
    static AnalogIn v_ref_internal(ADC_VREF);
    uint16_t v_ref_internal_raw = v_ref_internal.read_u16() >> 4;
    uint16_t v_ref_mv = ((uint32_t)(*VREFINT_CAL_ADDR) * VREFINT_CAL_VREF) / v_ref_internal_raw;
    return v_ref_mv / 1000.0f;
}

/**
 * Helper STM32H7 specific function to read MCU temperature.
 *
 * @return MCU temperature (unit: degree Celsius)
 */
float read_mcu_temperature()
{
    static AnalogIn mc_temp_in(ADC_TEMP);
    static uint16_t v_ref_mv = calculate_reference_voltage() * 1000;

    uint16_t temp_adc_lsb;
    float temp_value;

    // read raw temperature value
    temp_adc_lsb = mc_temp_in.read_u16() >> 4;
    // adjust temperature values according reference voltage
    temp_adc_lsb = ((int32_t)temp_adc_lsb * v_ref_mv) / TEMPSENSOR_CAL_VREFANALOG;

    // convert lsb temperature value to degree Celsius
    temp_value =
            (float)((temp_adc_lsb - (int32_t)*TEMPSENSOR_CAL1_ADDR) * (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP)) /
            (float)(*TEMPSENSOR_CAL2_ADDR - *TEMPSENSOR_CAL1_ADDR) + TEMPSENSOR_CAL1_TEMP;

    return temp_value;
}


int main()
{
    float temp;

    printf("====== Start ======\n");

    while (true) {
        // read and show current MCU temperature
        temp = read_mcu_temperature();
        printf("MCU temperature:  %.1f C\n", temp);

        // delay and led blinking
        user_led = 0;
        ThisThread::sleep_for(50ms);
        user_led = 1;
        ThisThread::sleep_for(1550ms);
    }
}
