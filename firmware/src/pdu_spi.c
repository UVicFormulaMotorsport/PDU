// BASED OFF EXAMPLE C CODE PAGE 171 AT90CAN32

// add part specific header


// DATA Direction Register controlling pins
DDR_SPI = DDA1; // example

// Data Direction bits for MOSI 
DD_MOSI = DDB2; // example

// Data Direction bits for SCK 
DD_SCK = DDRB; // example

void SPI_init() {
    // SET MOSI and SCK output, all others input
    
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);

    // enable SPI, set to master. Add clock rate????
    SPCR = (1 << SPE) | (1 << MSTR); 
}

void SPI_transmit() {
    // start transmittion?
    SPDR = cData;

    while(! SPSR & (1 << SPIF));
}