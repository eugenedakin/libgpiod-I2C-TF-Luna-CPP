//MdlTFLuna.h - module for the TF-Luna LiDAR
#include <gpiod.h>
#include <string>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>      // For open()
#include <unistd.h>     // For close()
#include <linux/i2c-dev.h>  // For I2C_SLAVE
#include <stdexcept>    // For std::runtime_error
#include <cstring>      // For strerror()
#include <errno.h>      // For errno
#include <sstream>
#include <iomanip>

class MdlTFLuna {
public:
    //TF-Luna I2C Register Addresses
    static const uint8_t REG_DIST_LOW = 0x00;
    static const uint8_t REG_DIST_HIGH = 0x01;
    static const uint8_t REG_RESET_VALUE = 0x02;
    static const uint8_t REG_AMP_LOW = 0x02;
    static const uint8_t REG_AMP_HIGH = 0x03;
    static const uint8_t REG_TEMP_LOW = 0x04;
    static const uint8_t REG_TEMP_HIGH = 0x05;
    static const uint8_t REG_TICK_LOW = 0x06;
    static const uint8_t REG_TICK_HIGH = 0x07;
    static const uint8_t REG_ERROR_LOW = 0x08;
    static const uint8_t REG_ERROR_HIGH = 0x09;
    
    static const uint8_t REG_VERSION_REVISION = 0x0A;
    static const uint8_t REG_VERSION_MINOR = 0x0B;
    static const uint8_t REG_VERSION_MAJOR = 0x0C;
    
    static const uint8_t REG_SER_NO01 = 0x10;
    static const uint8_t REG_SER_NO02 = 0x11;
    static const uint8_t REG_SER_NO03 = 0x12;
    static const uint8_t REG_SER_NO04 = 0x13;
    static const uint8_t REG_SER_NO05 = 0x14;
    static const uint8_t REG_SER_NO06 = 0x15;
    static const uint8_t REG_SER_NO07 = 0x16;
    static const uint8_t REG_SER_NO08 = 0x17;
    static const uint8_t REG_SER_NO09 = 0x18;
    static const uint8_t REG_SER_NO10 = 0x19;
    static const uint8_t REG_SER_NO11 = 0x1A;
    static const uint8_t REG_SER_NO12 = 0x1B;
    static const uint8_t REG_SER_NO13 = 0x1C;
    static const uint8_t REG_SER_NO14 = 0x1D;
    
    static const uint8_t REG_SHUTDOWN_REBOOT = 0x21;
    static const uint8_t REG_I2C_ADDRESS = 0x22;
    static const uint8_t REG_MODE = 0x23;
    static const uint8_t REG_ENABLE = 0x25;
    static const uint8_t REG_FRAME_RATE_LOW = 0x26;
    static const uint8_t REG_FRAME_RATE_HIGH = 0x27;
    static const uint8_t REG_LOW_POWER = 0x28;
    
    static const uint8_t REG_AMP_THR_LOW = 0x2A;
    static const uint8_t REG_AMP_THR_HIGH = 0x2B;
    static const uint8_t REG_DUMMY_DIST_LOW = 0x2C;
    static const uint8_t REG_DUMMY_DIST_HIGH = 0x2D;
    static const uint8_t REG_MIN_DIST_LOW = 0x2E;
    static const uint8_t REG_MIN_DIST_HIGH = 0x2F;
    static const uint8_t REG_MAX_DIST_LOW = 0x30;
    static const uint8_t REG_MAX_DIST_HIGH = 0x31;
    
    //Variable declaration
    int MyChipHandle;
    int I2C_ADDR;
    const char* I2C_BUS; 
    
    //Method declaration
    int addtwo(int a, int b);
    MdlTFLuna();   // Constructor
    ~MdlTFLuna();  // Destructor
    bool ReadBytes(uint8_t reg, uint8_t* buffer, size_t length);
    uint8_t ReadByte(uint8_t reg);
    uint8_t WriteByte(uint8_t reg, uint8_t ByteVal);
    int16_t GetDistance();  
    float_t GetTemperature();  
    float_t GetAmplitude();
    float_t GetFrameRate();
    std::string GetFirmwareVersion();
    std::string GetSerialNumber();
    int16_t GetCounterTicks();
    int16_t GetErrorCode();
    int8_t GetOperatingMode();
    int8_t GetLowPowerMode();
    int8_t GetEnabledStatus();
    int16_t GetAmplitudeThreshold();
    int16_t GetDummyDistance();
    int16_t GetMinimumDistance();
    int16_t GetMaximumDistance();
    bool SetI2CAddress(uint8_t NewAddress);
    std::string DecimalToHex(int decimal, int width=0, bool addPrefix=false);
    bool SetMinDistance(uint16_t MinDist);
    bool SetMaxDistance(uint16_t MaxDist);
    bool Reset();
    uint8_t WriteBytes(uint8_t* buffer, size_t length);
};

// Constructor implementation
MdlTFLuna::MdlTFLuna() {
    // Initialize member variables
    MyChipHandle = 0;
    I2C_ADDR = 0x10;
    I2C_BUS = "/dev/i2c-1";
    
    // Open I2C device
    MyChipHandle = open(I2C_BUS, O_RDWR);
    if (MyChipHandle < 0) {
        throw std::runtime_error("Failed to open I2C device: " + std::string(I2C_BUS) + 
                                 " - " + std::string(strerror(errno)));
    }
    
    // Set I2C slave address
    if (ioctl(MyChipHandle, I2C_SLAVE, I2C_ADDR) < 0) {
        close(MyChipHandle);
        throw std::runtime_error("Failed to set I2C address: " + std::string(strerror(errno)));
    }
    
    std::cout << "TF-Luna initialized on " << I2C_BUS << " at address 0x"
              << std::hex << I2C_ADDR << std::dec << std::endl;
}

// Destructor implementation
MdlTFLuna::~MdlTFLuna() {
    if (MyChipHandle > 0) {
        close(MyChipHandle);
        std::cout << "TF-Luna connection closed" << std::endl;
    }
}

uint8_t MdlTFLuna::ReadByte(uint8_t reg) {
	// Write the register address we want to read from
    if (write(MyChipHandle, &reg, 1) != 1) {
        throw std::runtime_error("Failed to write register address in ReadByte");
    }
    // Read the byte data from that register
    uint8_t buffer;
    uint8_t length = 1;
    if (read(MyChipHandle, &buffer, length) != 1) {
        throw std::runtime_error("Failed to read from I2C device in ReadByte");
    }
    return buffer;
}
uint8_t MdlTFLuna::WriteByte(uint8_t reg, uint8_t ByteVal) {
    if (MyChipHandle < 0) {
        throw std::runtime_error("Invalid file handle in WriteByte");
        return 0;
    }
    
    // Set the I2C slave address
    if (ioctl(MyChipHandle, I2C_SLAVE, I2C_ADDR) < 0) {
        throw std::runtime_error("Failed to set I2C slave address in WriteByte");
        return 0;
    }
    
    // Create a buffer with register address and value
    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = ByteVal;
    
    // Write both bytes in one transaction
    if (write(MyChipHandle, buffer, 2) != 2) {
        throw std::runtime_error("Failed to write to I2C device in WriteByte");
        return 0;
    }
    return 1;
}

// ReadBytes implementation - must include class scope
bool MdlTFLuna::ReadBytes(uint8_t reg, uint8_t* buffer, size_t length) {
    // Write the register address we want to read from
    if (write(MyChipHandle, &reg, 1) != 1) {
        return false;
    }
    // Read the data from that register
    if (read(MyChipHandle, buffer, length) != (ssize_t)length) {
        return false;
    }
    return true;
}

uint8_t MdlTFLuna::WriteBytes(uint8_t* buffer, size_t length) {
    if (MyChipHandle < 0) {
        throw std::runtime_error("Invalid file handle in WriteBytes");
    }
     
    // Write all bytes in one transaction
    ssize_t bytesWritten = write(MyChipHandle, buffer, length);
    if (bytesWritten != static_cast<ssize_t>(length)) {
        throw std::runtime_error("Failed to write to I2C device in WriteBytes");
    }
    
    return 1;  // Success
}

// GetDistance implementation
int16_t MdlTFLuna::GetDistance() {
    uint8_t Low = ReadByte(REG_DIST_LOW);
    uint8_t High = ReadByte(REG_DIST_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error (Distance can't be negative)
	}
	return Total;
}
// GetDistance implementation
float_t MdlTFLuna::GetTemperature() {
    uint8_t Low = ReadByte(REG_TEMP_LOW);
    uint8_t High = ReadByte(REG_TEMP_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error (Distance can't be negative)
	}
	return (Total/100);
}

float_t MdlTFLuna::GetAmplitude() {
    uint8_t Low = ReadByte(REG_AMP_LOW);
    uint8_t High = ReadByte(REG_AMP_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error (Distance can't be negative)
	}
    return Total; // Return -1 to indicate error (Amplitude can't be negative)
}

float_t MdlTFLuna::GetFrameRate() {
	uint8_t Low = ReadByte(REG_FRAME_RATE_LOW);
    uint8_t High = ReadByte(REG_FRAME_RATE_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error (Frame Rate can't be negative)
	}
    return Total; // Return -1 to indicate error (Frame Rate can't be negative)
}

std::string MdlTFLuna::GetFirmwareVersion() {
    uint8_t FMajor = ReadByte(REG_VERSION_MAJOR);
    uint8_t FMinor = ReadByte(REG_VERSION_MINOR);
    uint8_t FRevision = ReadByte(REG_VERSION_REVISION);
    std::stringstream ss;
    ss << (int)FMajor << "." << (int)FMinor << "." << (int)FRevision;
    std::string RetVal = ss.str();
    return RetVal;
}

std::string MdlTFLuna::GetSerialNumber() {
    uint8_t SerialBytes[14];  // Changed size to 14
    std::stringstream ss;
    
    for (int i = 0; i < 14; i++) {
        SerialBytes[i] = ReadByte(REG_SER_NO01 + i);
        ss << std::hex << std::setw(1) << std::setfill('0') 
           << static_cast<char>(SerialBytes[i]);
    }
    return ss.str();
}
int16_t MdlTFLuna::GetCounterTicks() {
	uint8_t Low = ReadByte(REG_TICK_LOW);
    uint8_t High = ReadByte(REG_TICK_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error 
	}
    return Total; 
}
int16_t MdlTFLuna::GetErrorCode() {
	uint8_t Low = ReadByte(REG_ERROR_LOW);
    uint8_t High = ReadByte(REG_ERROR_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return Total; 
}
int8_t MdlTFLuna::GetOperatingMode() {
	int8_t RetVal = ReadByte(REG_MODE);
    
    if (RetVal < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return RetVal; 
}
int8_t MdlTFLuna::GetLowPowerMode() {
	int8_t RetVal = ReadByte(REG_LOW_POWER);
    
    if (RetVal < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return RetVal; 
}
int8_t MdlTFLuna::GetEnabledStatus() {
	int8_t RetVal = ReadByte(REG_ENABLE);
    
    if (RetVal < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return RetVal; 
}
int16_t MdlTFLuna::GetAmplitudeThreshold() {
	uint8_t Low = ReadByte(REG_AMP_THR_LOW);
    uint8_t High = ReadByte(REG_AMP_THR_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return Total; 
}
int16_t MdlTFLuna::GetDummyDistance() {
	uint8_t Low = ReadByte(REG_DUMMY_DIST_LOW);
    uint8_t High = ReadByte(REG_DUMMY_DIST_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return Total; 
}
int16_t MdlTFLuna::GetMinimumDistance() {
	uint8_t Low = ReadByte(REG_MIN_DIST_LOW);
    uint8_t High = ReadByte(REG_MIN_DIST_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return Total; 
}
int16_t MdlTFLuna::GetMaximumDistance() {
	uint8_t Low = ReadByte(REG_MAX_DIST_LOW);
    uint8_t High = ReadByte(REG_MAX_DIST_HIGH);
    int16_t Total = Low + (High*256);
    if (Total < 0 ) {
		return -1; // Return -1 to indicate error
	}
    return Total; 
}

bool MdlTFLuna::SetI2CAddress(uint8_t NewAddress){
    if (NewAddress < 0x08 || NewAddress > 0x77){
        throw std::runtime_error("Change to I2C address not successful in SetI2CAddress with NewAddress out-of-range.");
    }
    
    uint8_t ResVal = WriteByte(REG_I2C_ADDRESS, NewAddress);
    if (ResVal == 1) {
        I2C_ADDR = NewAddress;
        return true;
    }
    throw std::runtime_error("Change to I2C address not successful in SetI2CAddress");
}
 std::string DecimalToHex(int decimal, int width=0, bool addPrefix=false){
    std::stringstream ss;
    if (addPrefix){
        ss << "0x";
    }
    ss << std::hex << std::uppercase;
    if (width > 0) {
        ss << std::setw(width) << std::setfill('0');
    }
    ss << decimal;
    return ss.str();
}
bool MdlTFLuna::SetMinDistance(uint16_t MinDist){
    if (MinDist < 0){
        throw std::runtime_error("Minimum distance must be greater than 0.");
    }

	uint8_t Low = MinDist & 0xFF; //Get lower 8 bits
    uint8_t High = (MinDist >>8) & 0xFF; //Get the upper 8 bits
    int16_t Total = Low + (High*256);
	
    uint8_t ResVal = WriteByte(REG_MIN_DIST_LOW, Low);
    ResVal = WriteByte(REG_MIN_DIST_HIGH, High);
    
	return true;
}
bool MdlTFLuna::SetMaxDistance(uint16_t MaxDist){
    if (MaxDist > 9000){
        throw std::runtime_error("Maximum value should be lower than 9000 cm.");
    }
    if (MaxDist < 20){
        throw std::runtime_error("Maximum value should be greater than 20 cm.");
    }

	uint8_t Low = MaxDist & 0xFF; //Get lower 8 bits
    uint8_t High = (MaxDist >>8) & 0xFF; //Get the upper 8 bits
    int16_t Total = Low + (High*256);
	
    uint8_t ResVal = WriteByte(REG_MAX_DIST_LOW, Low);
    ResVal = WriteByte(REG_MAX_DIST_HIGH, High);
    
	return true;
}
bool MdlTFLuna::Reset() {
    // Soft reset the TF-Luna sensor
    uint8_t WriteBuffer[2] = {REG_SHUTDOWN_REBOOT, REG_RESET_VALUE};
    
    if (!WriteBytes(WriteBuffer, 2)) {
        throw std::runtime_error("Failed to reset TF-Luna sensor.");
    }
    
    // Optional: Add a small delay for the sensor to reset
    usleep(100000);  // 100ms delay
    
    return true;
}
