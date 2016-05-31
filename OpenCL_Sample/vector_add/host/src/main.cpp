/**
* The VectorAdd host program implements an application that
* simply Vector Addition on the "Altera" FPGA using OpenCL 1.0.
*
* @author  Aswin Gunavelu Mohan <gmaswin@gmail.com>
* @version 1.0
* @since   2016-05-31 
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"

#define DATA_SIZE (4096)
using namespace aocl_utils;

    //Problem Data
    cl_int err;
    cl_uint numPlatforms;

    float data1[DATA_SIZE];              // original data set given to device
    float data2[DATA_SIZE];
    float results[DATA_SIZE];           // results returned from device
    unsigned int correct;               // number of correct results returned

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
  
  //OpenCL Runtime Configuration
    cl_platform_id platform;
    cl_device_id device;                // compute device id
    cl_context context;                 // compute context

    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel

    cl_mem input1,input2;               // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    
// Function prototypes
void cleanup();
bool init_opencl();
void run();
void init_problem();

//Main Program
int main(void)
{
    //Initialize OpenCL
    //
    if(!init_opencl()) {
    return -1;
    }    
    //Initialize Problem Data
    //
    init_problem();
    //Run the kernel
    // 
    run();
    // Shutdown and cleanup
    //
    cleanup();

    return 0;
}

bool init_opencl(){
    // Get the OpenCL platform.
    platform = findPlatform("Altera");
    if(platform == NULL) {
      printf("ERROR: Unable to find Altera OpenCL platform.\n");
      return false;
    }
  
    // Query the available OpenCL devices.
    scoped_array<cl_device_id> devices;
    cl_uint num_devices;
    devices.reset(getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices));
  
    // We'll just use the first device.
    device = devices[0];
    // Create a compute context
    //
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create the program for all device. Use the first device as the
    // representative device (assuming all device are of the same type).
    std::string binary_file = getBoardBinaryFile("vectorAdd", device);
    printf("Using AOCX: %s\n", binary_file.c_str());
    program = createProgramFromBinary(context, binary_file.c_str(), &device, 1);

    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "vectorAdd", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    //
    input1 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * DATA_SIZE, NULL, NULL);
    input2 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * DATA_SIZE, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * DATA_SIZE, NULL, NULL);
    if (!input1 ||!input2 || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }

    return true;

}

void init_problem(){
    
    for(int i = 0; i < DATA_SIZE; i++){
      data1[i] = rand() / (float)RAND_MAX;
      data2[i] = rand() / (float)RAND_MAX;
    }
}
void run(){
  // Write our data set into the input array in device memory
    //
    err = clEnqueueWriteBuffer(commands, input1, CL_TRUE, 0, sizeof(float) * DATA_SIZE, data1, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(float) * DATA_SIZE, data2, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    //
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    global = DATA_SIZE;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, NULL , 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        exit(1);
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(float) * DATA_SIZE, results, 0, NULL, NULL );
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    // Validate our results
    //
    correct = 0;
    for(int i = 0; i < DATA_SIZE; i++)
    {
        if(results[i] == data1[i] + data2[i])
            correct++;
    }

    // Print a brief summary detailing the results
    //
    printf("Computed '%d/%d' correct values!\n", correct, DATA_SIZE);

}
void cleanup(){

    clReleaseMemObject(input1);
    clReleaseMemObject(input2);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
  }