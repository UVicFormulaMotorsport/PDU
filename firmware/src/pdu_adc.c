void ADC_init() {

    // ADMUX - Choose Reference selection - VCC (+5V) selected
    ADMUX = (1 << REFS0);

    // Control and Status Register A - ADC Enable 
    ADCSRA = (1 << ADEN);
    
}

uint16_t ADC_read(uint8_t channel) {
    
}