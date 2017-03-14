// BASED OFF EXAMPLE C CODE PAGE 171 AT90CAN32

// add part specific header


// DATA Direction Register controlling pins
DDR_SPI_1 = DDRB;

// DATA Direction Register controlling pins
DDR_SPI_2 = DDRA;

// Data Direction bits for MOSI 
DD_MOSI = DDA3;

// Data Direction bits for MISO 
DD_MISO = DDB3;

// Data Direction bits for SCK 
DD_SCK = DDRB; // example

void SPI_init() {
    
    // SET MOSI output, all others input
    DDR_SPI_1 = (1 << DD_MOSI);

    // SET MISO output, all others input
    DDR_SPI_1 = (1 << DD_MISO));

    // enable SPI.
    SPCR = (1 << SPE); 
}

char SPI_Receive() {
    
    // wait for reception complete

    while(! SPSR & (1 << SPIF));

    // return data register
    return SPDR
}