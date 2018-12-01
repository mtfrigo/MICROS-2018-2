#ifndef I2C_H
#define I2C_H

int i2c_write_reg(int fd, unsigned char reg, unsigned char data);
void i2c_error(const char *msg);

#endif