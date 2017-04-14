/******************************************************************//**
* @file		stm32f4_i2c_mpu9250.c
* @brief	Contains all functions support for I2C based MPU9250
*           9DOF Sensor library
* @version	1.0
* @date		23. Jan. 2017
* @author	Bhavin.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup MPU9250
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "stm32f4_i2c_mpu9250.h"


/** @defgroup STM32F4_I2C_MPU9250_Exported_Handle
 * @{
 */
I2C_HandleTypeDef hi2c1_mpu9250;

/**
  * @}
  */

/** @defgroup STM32F4_I2C_MPU9250_Private_variables
  * @{
  */
float SelfTest[6];
float gyroBias[3],accelBias[3]; // Bias corrections for gyro and accelerometer
float magCalibration[3], magbias[3];  // Factory mag calibration and mag bias

uint8_t Ascale=0;               // AFS_2G, AFS_4G, AFS_8G, AFS_16G
uint8_t Gscale=0;               // GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
uint8_t Mscale=0;               // MFS_14BITS or MFS_16BITS, 14-bit or 16-bit magnetometer resolution
uint8_t Mmode=0;                // Either 8 Hz 0x02) or 100 Hz (0x06) magnetometer data ODR
float aRes, gRes, mRes;       // scale resolutions per LSB for the sensors

int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
float q[4];           // vector to hold quaternion

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values
float pitch, yaw, roll;
float deltat=0;                             // integration interval for both filter schemes
int lastUpdate, firstUpdate, Now;    // used to calculate integration interval
int delt_t; // used to control display output rate
int count;  // used to control display output rate
int16_t tempCount;   // Stores the real internal chip temperature in degrees Celsius

/**
  * @}
  */


/** @defgroup STM32F4_I2C_MPU9250_Private_Functions
  * @{
  */

/**
 * @brief This function is support function for BSP_MPU9250_I2C1_Init
 *        This function configures GPIO of I2C1
 * @param hi2c  Provide I2C handle
 *      @arg  &hi2c1_mpu9250
 * @retval None
 */
static void MPU9250_I2C1_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    /* Peripheral interrupt init*/
//    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
//    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
//    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  }
}


/**
 * @brief To transmit data on I2C
 * @param i2c_Addr device address
 * @param Wr_Addr Register address on which Wr_Data will be written
 * @param Wr_Data data
 * @return  value   if data written successful, it will return 0 otherwise 1
 */
static uint8_t I2C_WriteRegister(uint8_t i2c_addr,uint8_t Wr_Addr, uint8_t Wr_Data)
{
  uint8_t I2C_Tx_Buf[2]={0};

  I2C_Tx_Buf[0] = Wr_Addr;
  I2C_Tx_Buf[1] = Wr_Data;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_mpu9250,i2c_addr,I2C_Tx_Buf,2,2000) == HAL_OK)
  {
    return (0);
  }
  else
  {
    return (1);
  }
}

/**
 * @brief To read data from the I2C based device
 * @param i2c_Addr device address
 * @param Rd_Addr Register address from data to be read
 * @return value  If data read successful, it will return data otherwise 1
 */
static uint8_t I2C_ReadRegister(uint8_t i2c_addr,uint8_t Rd_Addr)
{
  uint8_t I2C_Tx_Buf[2]={0};
  uint8_t I2C_Rx_Buf[2]={0};

  I2C_Tx_Buf[0] = Rd_Addr;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_mpu9250,i2c_addr,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_mpu9250,i2c_addr,I2C_Rx_Buf,1,2000) == HAL_OK)
  {
    return (I2C_Rx_Buf[0]);
  }
  else
  {
    return (1);
  }
}

/**
 * @brief To read data from the I2C based device
 * @param i2c_Addr device address
 * @param Rd_Addr Register address from data to be read
 * @return value  If data read successful, it will return data otherwise 1
 */
static uint8_t I2C_ReadBytes(uint8_t i2c_addr,uint8_t Rd_Addr,uint8_t* data_buf, uint16_t length)
{
  uint8_t I2C_Tx_Buf[2]={0};

  I2C_Tx_Buf[0] = Rd_Addr;

  /* write byte to addr  */
  if(HAL_I2C_Master_Transmit(&hi2c1_mpu9250,i2c_addr,I2C_Tx_Buf,1,2000) != HAL_OK)
  {
    return 1;
  }

  if (HAL_I2C_Master_Receive(&hi2c1_mpu9250,i2c_addr,data_buf,length,2000) == HAL_OK)
  {
    return (0);
  }
  else
  {
    return (1);
  }
}
/**
  * @}
  */


/** @defgroup STM32F4_I2C_MPU9250_Functions
  * @{
  */

/**
 * @brief This Function configures and Initializes I2C1
 * @retval None
 */
void BSP_MPU9250_I2C1_Init(void)
{
  __GPIOB_CLK_ENABLE();

  hi2c1_mpu9250.Instance = I2C1;
  hi2c1_mpu9250.Init.ClockSpeed = 100000;
  hi2c1_mpu9250.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1_mpu9250.Init.OwnAddress1 = 0;
  hi2c1_mpu9250.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_mpu9250.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1_mpu9250.Init.OwnAddress2 = 0;
  hi2c1_mpu9250.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1_mpu9250.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

  MPU9250_I2C1_MspInit(&hi2c1_mpu9250);
  HAL_I2C_Init(&hi2c1_mpu9250);
}

/**
 * @brief This function is for De-Initializing I2C1
 * @param hi2c Provide I2C handle
 *      @arg  &hi2c1_mpu9250
 * @retval None
 */
void BSP_MPU9250_I2C1_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* Peripheral interrupt Deinit*/
//    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
//    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
  }
}

/**
 * @brief Device connection test
 * @return device register id
 */
uint8_t MPU9250_ConnentTest(void)
{
  uint8_t whoami;
  whoami = I2C_ReadRegister(MPU9250_I2C_ADDRESS, WHO_AM_I_MPU9250);  // Read WHO_AM_I register for MPU-9250
  if(whoami == 1)
  {
    uprintf(&huart4,"Error! unable to connect MPU9250\r\n");
    while(1);
  }
  else
  {
    return whoami;
  }
}


/**
 * @brief Read INT pin status
 * @return value
 */
uint8_t MPU9250_INT_PinStatus(void)
{
  return I2C_ReadRegister(MPU9250_I2C_ADDRESS,INT_STATUS);
}

/**
 * @brief magnetometer scales (and their register bit settings)
 */
void MPU9250_GetMres(void)
{
  switch (Mscale)
  {
    // Possible magnetometer scales (and their register bit settings) are:
    // 14 bit resolution (0) and 16 bit resolution (1)
    case MFS_14BITS:
          mRes = 10.0*4219.0/8190.0; // Proper scale to return milliGauss
          break;
    case MFS_16BITS:
          mRes = 10.0*4219.0/32760.0; // Proper scale to return milliGauss
          break;
  }
}


/**
 *@brief gyro scales (and their register bit settings)
 */
void MPU9250_GetGres(void)
{
  switch (Gscale)
  {
    // Possible gyro scales (and their register bit settings) are:
    // 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case GFS_250DPS:
          gRes = 250.0/32768.0;
          break;
    case GFS_500DPS:
          gRes = 500.0/32768.0;
          break;
    case GFS_1000DPS:
          gRes = 1000.0/32768.0;
          break;
    case GFS_2000DPS:
          gRes = 2000.0/32768.0;
          break;
  }
}

/**
 * @brief accelerometer scales (and their register bit settings)
 */
void MPU9250_GetAres(void)
{
  switch (Ascale)
  {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case AFS_2G:
          aRes = 2.0/32768.0;
          break;
    case AFS_4G:
          aRes = 4.0/32768.0;
          break;
    case AFS_8G:
          aRes = 8.0/32768.0;
          break;
    case AFS_16G:
          aRes = 16.0/32768.0;
          break;
  }
}

/**
 * @brief Read accelerometer data
 * @param destination pointer to receiver buffer
 */
void MPU9250_ReadAccelData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here

  I2C_ReadBytes(MPU9250_I2C_ADDRESS,ACCEL_XOUT_H,rawData,6); // Read the six raw data registers sequentially into data array

  destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]);  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]);
  destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]);
}

/**
 * @brief Read Gyrometer data
 * @param destination pointer to receiver buffer
 */
void MPU9250_ReadGyroData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here

  I2C_ReadBytes(MPU9250_I2C_ADDRESS,GYRO_XOUT_H,rawData, 6);  // Read the six raw data registers sequentially into data array

  destination[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  destination[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
}

/**
 * @brief Read magnetometer data
 * @param destination pointer to receiver buffer
 */
void MPU9250_ReadMagData(int16_t * destination)
{
  uint8_t rawData[7];  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
  if(I2C_ReadRegister(AK8963_ADDRESS, AK8963_ST1) & 0x01)
  { // wait for magnetometer data ready bit to be set
    I2C_ReadBytes(AK8963_ADDRESS, AK8963_XOUT_L,rawData,7);  // Read the six raw data and ST2 registers sequentially into data array
    uint8_t c = rawData[6]; // End data read by reading ST2 register
    if(!(c & 0x08))
    { // Check if magnetic sensor overflow set, if not then report data
      destination[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);  // Turn the MSB and LSB into a signed 16-bit value
      destination[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]);  // Data stored as little Endian
      destination[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]);
    }
  }
}


/**
 * @brief Reset the device
 */
void MPU9250_ResetMPU9250(void)
{
  // reset device
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  HAL_Delay(100);
}

/**
 * @brief Initialize magnetometer
 * @param destination pointer to the calibration buffer
 */
void AK8963_Init(float * destination)
{
  uint8_t whoami;
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3];  // x/y/z gyro calibration data stored here
  whoami = I2C_ReadRegister(AK8963_ADDRESS,WHO_AM_I_AK8963);
  if(whoami != 0x48)
  {
    uprintf(&huart4,"Error! unable to connect AK8963\r\n");
    while(1);
  }
  I2C_WriteRegister(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  HAL_Delay(50);
  I2C_WriteRegister(AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
  HAL_Delay(50);
  I2C_ReadBytes(AK8963_ADDRESS, AK8963_ASAX,rawData,3);  // Read the x-, y-, and z-axis calibration values
//  rawData[0] = I2C_ReadRegister(AK8963_ADDRESS,AK8963_ASAX);
//  rawData[1] = I2C_ReadRegister(AK8963_ADDRESS,AK8963_ASAY);
//  rawData[2] = I2C_ReadRegister(AK8963_ADDRESS,AK8963_ASAZ);
  destination[0] =  (float)(rawData[0] - 128)/256 + 1;   // Return x-axis sensitivity adjustment values, etc.
  destination[1] =  (float)(rawData[1] - 128)/256 + 1;
  destination[2] =  (float)(rawData[2] - 128)/256 + 1;
  I2C_WriteRegister(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  HAL_Delay(10);
  // Configure the magnetometer for continuous read and highest resolution
  // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
  // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
  I2C_WriteRegister(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR
  HAL_Delay(10);
}

/**
 * @brief initialize accelerometer and gyrometer
 */
void MPU9250_Init(void)
{
 // Initialize MPU9250 device
 // wake up device
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
  HAL_Delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

 // get stable time source
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

 // Configure Gyro and Accelerometer
 // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
 // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
 // Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, CONFIG, 0x03);

 // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

 // Set gyroscope full scale range
 // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c =  I2C_ReadRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, c | Gscale << 3); // Set full scale range for the gyro

 // Set accelerometer configuration
  c =  I2C_ReadRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, c | Ascale << 3); // Set full scale range for the accelerometer

 // Set accelerometer sample rate configuration
 // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
 // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
  c = I2C_ReadRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG2);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG2, c & ~0x0F); // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG2, c | 0x03); // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz

 // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
 // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
  // can join the I2C bus and all can be controlled by the Arduino as master
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, INT_PIN_CFG, 0x22);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
}


/**
 * @brief accumulates gyro and accelerometer data after device initialization. It calculates the average
 * of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers
 * @param dest1 pointer to the gyroscope buffer
 * @param dest2 pointer to the accelerometer buffer
 */
void MPU9250_Calibrate(float * dest1, float * dest2)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

// reset device, reset all registers, clear gyro and accelerometer bias registers
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  HAL_Delay(10);

// get stable time source
// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x01);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_2, 0x00);
  HAL_Delay(10);

// Configure device for bias calculation
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
  HAL_Delay(15);

// Configure MPU9250 gyro and accelerometer for bias calculation
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

// Configure FIFO to capture accelerometer and gyro data for bias calculation
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO (max size 512 bytes in MPU-9250)
  HAL_Delay(40); // accumulate 40 samples in 80 milliseconds = 480 bytes

// At end of sample accumulation, turn off FIFO sensor read
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  I2C_ReadBytes(MPU9250_I2C_ADDRESS, FIFO_COUNTH, data, 2); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    I2C_ReadBytes(MPU9250_I2C_ADDRESS, FIFO_R_W, data, 12); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];

}
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;

  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) accelsensitivity;}

// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

/// Push gyro biases to hardware registers
/*  writeByte(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
  writeByte(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
  writeByte(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
  writeByte(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
  writeByte(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
  writeByte(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);
*/
  dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  I2C_ReadBytes(MPU9250_I2C_ADDRESS, XA_OFFSET_H, data, 2); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  I2C_ReadBytes(MPU9250_I2C_ADDRESS, YA_OFFSET_H, data, 2);
  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  I2C_ReadBytes(MPU9250_I2C_ADDRESS, ZA_OFFSET_H, data, 2);
  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];

  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for(ii = 0; ii < 3; ii++) {
    if(accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

// Apparently this is not working for the acceleration biases in the MPU-9250
// Are we handling the temperature correction bit properly?
// Push accelerometer biases to hardware registers
/*  writeByte(MPU9250_ADDRESS, XA_OFFSET_H, data[0]);
  writeByte(MPU9250_ADDRESS, XA_OFFSET_L, data[1]);
  writeByte(MPU9250_ADDRESS, YA_OFFSET_H, data[2]);
  writeByte(MPU9250_ADDRESS, YA_OFFSET_L, data[3]);
  writeByte(MPU9250_ADDRESS, ZA_OFFSET_H, data[4]);
  writeByte(MPU9250_ADDRESS, ZA_OFFSET_L, data[5]);
*/
// Output scaled accelerometer biases for manual subtraction in the main program
   dest2[0] = (float)accel_bias[0]/(float)accelsensitivity;
   dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
   dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;
}


/**
 * @brief returns X raised to the Y of power i.e. x^y
 * @param data  floating point base value.
 * @param power power value
 * @return  value
 */
static float power(float data, uint8_t power)
{
  uint8_t i;
  float data1 = data;
  for(i = 0 ;i<power-1;i++)
  {
    data = data*data1;
  }
  return data;
}

/**
 * @brief Accelerometer and gyroscope self test and check calibration factory settings
 * @param destination
 */
void MPU9250_SelfTest(float * destination) // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
   uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
   uint8_t selfTest[6];
   int16_t gAvg[3]={0}, aAvg[3]={0}, aSTAvg[3]={0}, gSTAvg[3]={0};
   float factoryTrim[6];
   uint8_t FS = 0;
   uint8_t i,ii;

  I2C_WriteRegister(MPU9250_I2C_ADDRESS, SMPLRT_DIV, 0x00); // Set gyro sample rate to 1 kHz
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, CONFIG, 0x02); // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, 1<<FS); // Set full scale range for the gyro to 250 dps
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, 1<<FS); // Set full scale range for the accelerometer to 2 g

  for(ii = 0; ii < 200; ii++)
  { // get average current values of gyro and acclerometer

  I2C_ReadBytes(MPU9250_I2C_ADDRESS, ACCEL_XOUT_H, rawData, 6); // Read the six raw data registers into data array
  aAvg[0] += ((rawData[0] << 8) | rawData[1]); // Turn the MSB and LSB into a signed 16-bit value
  aAvg[1] += ((rawData[2] << 8) | rawData[3]);
  aAvg[2] += ((rawData[4] << 8) | rawData[5]);

  I2C_ReadBytes(MPU9250_I2C_ADDRESS, GYRO_XOUT_H, rawData, 6); // Read the six raw data registers sequentially into data array
  gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
  gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for (ii =0; ii < 3; ii++)
  { // Get average of 200 values and store as average current readings
    aAvg[ii] /= 200;
    gAvg[ii] /= 200;
  }

// Configure the accelerometer for self-test
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, 0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
  HAL_Delay(25); // Delay a while to let the device stabilize

  for(ii = 0; ii < 200; ii++)
  { // get average self-test values of gyro and acclerometer
  I2C_ReadBytes(MPU9250_I2C_ADDRESS, ACCEL_XOUT_H, rawData, 6); // Read the six raw data registers into data array
  aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
  aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

  I2C_ReadBytes(MPU9250_I2C_ADDRESS, GYRO_XOUT_H, rawData, 6); // Read the six raw data registers sequentially into data array
  gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
  gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
  gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for ( ii=0; ii < 3; ii++)
  { // Get average of 200 values and store as average self-test readings
  aSTAvg[ii] /= 200;
  gSTAvg[ii] /= 200;
  }

 // Configure the gyro and accelerometer for normal operation
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, ACCEL_CONFIG, 0x00);
  I2C_WriteRegister(MPU9250_I2C_ADDRESS, GYRO_CONFIG, 0x00);
  HAL_Delay(25); // Delay a while to let the device stabilize

   // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
   selfTest[0] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_X_ACCEL); // X-axis accel self-test results
   selfTest[1] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_Y_ACCEL); // Y-axis accel self-test results
   selfTest[2] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_Z_ACCEL); // Z-axis accel self-test results
   selfTest[3] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_X_GYRO); // X-axis gyro self-test results
   selfTest[4] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_Y_GYRO); // Y-axis gyro self-test results
   selfTest[5] = I2C_ReadRegister(MPU9250_I2C_ADDRESS, SELF_TEST_Z_GYRO); // Z-axis gyro self-test results

  // Retrieve factory self-test value from self-test code reads
   factoryTrim[0] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
   factoryTrim[1] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
   factoryTrim[2] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
   factoryTrim[3] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
   factoryTrim[4] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
   factoryTrim[5] = (float)(2620/1<<FS)*(power( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation

 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get percent, must multiply by 100
   for (i = 0; i < 3; i++)
   {
     destination[i] = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i]; // Report percent differences
     destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3]; // Report percent differences
   }

}

void FloatToDecimal(float input, int16_t *buffer)
{
  float temp;
  int16_t temp1,temp2;

  temp1 = (int)input;
  temp = input - temp1;
  temp = temp * 10000;
  temp2 =((int)(temp)%10000);
  if(temp2<0)
  {
    temp2 = temp2 * (-1);
  }
  buffer[0] = temp1;
  buffer[1] = temp2;
}


void MPU9250_GetData(void)
{
  int16_t output[2]={0};

    if (MPU9250_INT_PinStatus() & 0x01) // On interrupt, check if data ready interrupt
     {
      MPU9250_ReadAccelData(accelCount);  // Read the x/y/z adc values
      // Now we'll calculate the accleration value into actual g's
      ax = (float) accelCount[0] * aRes - accelBias[0]; // get actual g value, this depends on scale being set
      ay = (float) accelCount[1] * aRes - accelBias[1];
      az = (float) accelCount[2] * aRes - accelBias[2];

      MPU9250_ReadGyroData(gyroCount);  // Read the x/y/z adc values
      // Calculate the gyro value into actual degrees per second
      gx = (float) gyroCount[0] * gRes - gyroBias[0]; // get actual gyro value, this depends on scale being set
      gy = (float) gyroCount[1] * gRes - gyroBias[1];
      gz = (float) gyroCount[2] * gRes - gyroBias[2];

      MPU9250_ReadMagData(magCount);  // Read the x/y/z adc values
      // Calculate the magnetometer values in milliGauss
      // Include factory calibration per data sheet and user environmental corrections
      mx = (float) magCount[0] * mRes * magCalibration[0] - magbias[0]; // get actual magnetometer value, this depends on scale being set
      my = (float) magCount[1] * mRes * magCalibration[1] - magbias[1];
      mz = (float) magCount[2] * mRes * magCalibration[2] - magbias[2];
     }
     ax = ax * 1000;
     ay = ay * 1000;
     az = az * 1000;

     FloatToDecimal(ax,output);
     uprintf(&huart4, "\x1b[3;1Hax = %d03.%d04", output[0], output[1]);

     FloatToDecimal(ay, output);
     uprintf(&huart4, " ay = %d03.%d04",output[0], output[1]);

     FloatToDecimal(az, output);
     uprintf(&huart4, " az = %d04.%d04 mg\r\n", output[0],output[1]);


     FloatToDecimal(gx,output);
     uprintf(&huart4, "\x1b[4;1Hgx = %d03.%d04",output[0], output[1]);

     FloatToDecimal(gy, output);
     uprintf(&huart4, " gy = %d03.%d04", output[0], output[1]);

     FloatToDecimal(gz, output);
     uprintf(&huart4, " gz = %d03.%d04 deg/s\r\n", output[0], output[1]);


     FloatToDecimal(mx, output);
     uprintf(&huart4, "\x1b[5;1Hmx = %d03.%d04", output[0], output[1]);

     FloatToDecimal(my, output);
     uprintf(&huart4, " my = %d03.%d04", output[0], output[1]);

     FloatToDecimal(mz, output);
     uprintf(&huart4, " mz = %d03.%d04 mG\r\n", output[0], output[1]);

}

/*
// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!

void MPU9250_MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
  float GyroMeasError = PI * (60.0f / 180.0f);     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
  float beta = sqrt(3.0f / 4.0f) * GyroMeasError;  // compute beta
  float GyroMeasDrift = PI * (1.0f / 180.0f);      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
  float zeta = sqrt(3.0f / 4.0f) * GyroMeasDrift;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
  q[0] = 1.0f;
  q[1] = 0.0f;
  q[2] = 0.0f;
  q[3] = 0.0f;
  float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
  float norm;
  float hx, hy, _2bx, _2bz;
  float s1, s2, s3, s4;
  float qDot1, qDot2, qDot3, qDot4;

  // Auxiliary variables to avoid repeated arithmetic
  float _2q1mx;
  float _2q1my;
  float _2q1mz;
  float _2q2mx;
  float _4bx;
  float _4bz;
  float _2q1 = 2.0f * q1;
  float _2q2 = 2.0f * q2;
  float _2q3 = 2.0f * q3;
  float _2q4 = 2.0f * q4;
  float _2q1q3 = 2.0f * q1 * q3;
  float _2q3q4 = 2.0f * q3 * q4;
  float q1q1 = q1 * q1;
  float q1q2 = q1 * q2;
  float q1q3 = q1 * q3;
  float q1q4 = q1 * q4;
  float q2q2 = q2 * q2;
  float q2q3 = q2 * q3;
  float q2q4 = q2 * q4;
  float q3q3 = q3 * q3;
  float q3q4 = q3 * q4;
  float q4q4 = q4 * q4;

  // Normalise accelerometer measurement
  norm = sqrt(ax * ax + ay * ay + az * az);
  if (norm == 0.0f) return; // handle NaN
  norm = 1.0f/norm;
  ax *= norm;
  ay *= norm;
  az *= norm;

  // Normalise magnetometer measurement
  norm = sqrt(mx * mx + my * my + mz * mz);
  if (norm == 0.0f) return; // handle NaN
  norm = 1.0f/norm;
  mx *= norm;
  my *= norm;
  mz *= norm;

  // Reference direction of Earth's magnetic field
  _2q1mx = 2.0f * q1 * mx;
  _2q1my = 2.0f * q1 * my;
  _2q1mz = 2.0f * q1 * mz;
  _2q2mx = 2.0f * q2 * mx;
  hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
  hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
  _2bx = sqrt(hx * hx + hy * hy);
  _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
  _4bx = 2.0f * _2bx;
  _4bz = 2.0f * _2bz;

  // Gradient decent algorithm corrective step
  s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
  s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
  s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
  s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
  norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
  norm = 1.0f/norm;
  s1 *= norm;
  s2 *= norm;
  s3 *= norm;
  s4 *= norm;

  // Compute rate of change of quaternion
  qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
  qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
  qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
  qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

  // Integrate to yield quaternion
  q1 += qDot1 * deltat;
  q2 += qDot2 * deltat;
  q3 += qDot3 * deltat;
  q4 += qDot4 * deltat;
  norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
  norm = 1.0f/norm;
  q[0] = q1 * norm;
  q[1] = q2 * norm;
  q[2] = q3 * norm;
  q[3] = q4 * norm;

}

*/

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

