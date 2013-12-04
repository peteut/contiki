// -*-coding: utf-8 -*-  vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
#ifndef SPI_H
#define SPI_H

void spi_init(void);

#define FASTSPI_TX(x) do { \
  SPI_TXBUF = x; \
  SPI_WAITFOREOTx(); \
} while (0)

#define FASTSPI_TX_DISCARD_RX(x) do { \
  FASTSPI_TX(x); \
  READ_AND_DISCARD(SPI_RXBUF); \
} while (0)

#define FASTSPI_RX(x) do { \
  SPI_TXBUF = 0; \
  SPI_WAITFOREORx(); \
  x = SPI_RXBUF; \
} while(0)

#define FASTSPI_CLEAR_RX() do{ \
  if (BF == 1) READ_AND_DISCARD(SPI_RXBUF); \
} while (0)

#define FASTSPI_RX_GARBAGE() do { \
  SPI_TXBUF = 0; \
  SPI_WAITFOREORx(); \
  READ_AND_DISCARD(SPI_RXBUF); \
} while (0)

#define FASTSPI_TX_MANY(p, c) do { \
  uint8_t spiCnt; \
  for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
    FASTSPI_TX_DISCARD_RX(((uint8_t*)(p))[spiCnt]); \
  } \
} while (0)

#define FASTSPI_RX_WORD(x) do { \
  SPI_TXBUF = 0; \
  SPI_WAITFOREORx(); \
  x = SPI_RXBUF << 8; \
  SPI_TXBUF = 0; \
  SPI_WAITFOREORx(); \
  x |= SPI_RXBUF; \
} while (0)

#endif /* SPI_H */
