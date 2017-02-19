void ADC_init() {
    // APB peripheral clock enable register
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    // Calibration through the control register
    ADC1->CR = ADC_CR_ADCAL;

    // wait for calibratrion to finish
    while (ADC1->CR == ADC_CR_ADCAL);

    // Set the ADC channel
    ADC1->CHSELR |= ADC_CHSELR_CHSEL3;

    // set the sampling time
    ADC1->SMPR |= ADC_SMPR_SMP;

    // Set AD_EN = 1 to enable the adc through the control register
    ADC1->CR |= ADC_CR_ADEN;

    // wait until AD_RDY = 1 in the ADC ISR register
    while ((ADC1->ISR & 0x1) != 0x1);
}