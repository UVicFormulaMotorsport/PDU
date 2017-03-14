void ADC_init() {

    // ADMUX - Choose Reference selection - VCC (+5V) selected
    ADMUX = (1 << REFS0);

    // Control and Status Register A - ADC Enable 
    ADCSRA = (1 << ADEN);
    
}

uint16_t ADC_read(uint8_t channel) {
    
    // Keep the channel within [0..7] range
    channel &= 0x07;

    // clear bottom 3 bits before OR
    ADMUX = (ADMUX & 0xF8) | channel;

    // Control and Status Register A - Start Conversion
    ADCSRA |= (1 << ADSC);

    // Wait for converison to complete - ADSC becomes '0' upon completion 
    while (ADCSRA & (1 << ADSC));

    // Continue
    return (ADC);
}