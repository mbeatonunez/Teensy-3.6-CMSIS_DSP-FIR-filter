#include <SPI.h>
#include <SD.h>
#include "arm_math.h" 

// taps is the filder order N+1, sample blocks is the number of sample being filtered at a time
#define TAPS 129
#define TOTAL_SAMPLES 870400  //use matlab to find this information
#define BUFFER_SIZE 200 //sample block is a multiple of the total samples in the input file. Use matlab to verify

// input and output file
File outfile, infile;

//Using built-in SD card over SPI
const int chipSelect = BUILTIN_SDCARD;

// State buffer for the floating-point FIR filter.
static float32_t firStateF32[TAPS+BUFFER_SIZE];

// filter structure created using the CMSIS library
static arm_fir_instance_f32 FILTER;

//file position
uint64_t pos;
  
// I/O buffers. Holds the data as its being filtered
static float32_t input_buffer[BUFFER_SIZE];
static float32_t output_buffer[BUFFER_SIZE];

// coefficients 
const float32_t coeffs[TAPS] = {
      -0.0000000, 0.0001324, 0.0003211, 0.0004678, 0.0004726, 0.0002712,-0.0001325,-0.0006522,-0.0011353,-0.0014103,-0.0013552,
      -0.0009603,-0.0003539, 0.0002313, 0.0005485, 0.0004539,-0.0000000,-0.0005491,-0.0008023,-0.0004089, 0.0007552, 0.0024700,
       0.0041920, 0.0052311, 0.0050299, 0.0034338, 0.0008234,-0.0019745,-0.0039917,-0.0045334,-0.0035252,-0.0016235,-0.0000000,
       0.0001478,-0.0018231,-0.0055852,-0.0098053,-0.0125655,-0.0121198,-0.0076972, 0.0000000, 0.0088686, 0.0160951, 0.0192470,
       0.0173421, 0.0114229, 0.0043198,-0.0004067,-0.0000000, 0.0060802, 0.0154923, 0.0234927, 0.0245342, 0.0144939,-0.0072793,
      -0.0369387,-0.0666791,-0.0868432,-0.0890060,-0.0690223,-0.0289464, 0.0229464, 0.0741861, 0.1116232, 0.1253310, 0.1116232,
       0.0741861, 0.0229464,-0.0289464,-0.0690223,-0.0890060,-0.0868432,-0.0666791,-0.0369387,-0.0072793, 0.0144939, 0.0245342,
       0.0234927, 0.0154923, 0.0060802,-0.0000000,-0.0004067, 0.0043198, 0.0114229, 0.0173421, 0.0192470, 0.0160951, 0.0088686,
       0.0000000,-0.0076972,-0.0121198,-0.0125655,-0.0098053,-0.0055852,-0.0018231, 0.0001478,-0.0000000,-0.0016235,-0.0035252,
      -0.0045334,-0.0039917,-0.0019745, 0.0008234, 0.0034338, 0.0050299, 0.0052311, 0.0041920, 0.0024700, 0.0007552,-0.0004089,
      -0.0008023,-0.0005491,-0.0000000, 0.0004539, 0.0005485, 0.0002313,-0.0003539,-0.0009603,-0.0013552,-0.0014103,-0.0011353,
      -0.0006522,-0.0001325, 0.0002712, 0.0004726, 0.0004678, 0.0003211, 0.0001324,-0.0000000 };

union {      //this union converst the input data from binary to floats
  byte bytes[4];
  float f;
} databits;

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  SD.begin(chipSelect);
  infile = SD.open("input.txt"); 

  if ( !infile )
    Serial.printf( "error opening infile\n" );
  else
    Serial.printf( "opened infile for reading\n" );
    
  outfile = SD.open("filt.txt",FILE_WRITE); //creates the output file

  if ( !outfile )
    Serial.printf( "error opening outfile\n" );
  else
    Serial.printf( "opened outfile for writing\n" );
  
  pos = 0; // initial pointer position
  arm_fir_init_f32( &FILTER, TAPS, (float32_t *)&coeffs[0], &firStateF32[0], BUFFER_SIZE );
}


void loop()
{
  while(infile.read() != EOF)
  {
    infile.seek(pos); //sets the pointer to the next position in the file
    
    for (int j = 0; j < TOTAL_SAMPLES; j += BUFFER_SIZE) 
    {
      int i;
    
      for ( i = 0; i < BUFFER_SIZE; i++ )
      {
        infile.read( databits.bytes, 4 ); //read the binary data in chunks of  4 bytes 
        input_buffer[i] = databits.f;     //convert the 4 bytes into a float value
      }
      
      arm_fir_f32( &FILTER, &input_buffer[0], &output_buffer[0], BUFFER_SIZE );
    
      for ( i = 0; i < BUFFER_SIZE; i++ )
      {
        // uncomment this to see the filtered data printer in the serial monitor. This slows the process WAY down.
        //Serial.printf( "%0.7f\n", output_buffer[i] ); 
        databits.f = output_buffer[i];
        outfile.println(databits.f,7);  //export as float with 7 decimal points
      }
     pos = pos + 800; //move the cursor 200 samples forward (200*4 = 800 lines)
     }
  }
  delay(5000);  //this is a 5s grace period to make sure all data has been writen to the output file
  infile.close();
  outfile.close();
  Serial.printf( "finished!\n" );
  while(1);  //endless loop so data is not refiltered
}

