#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "FlyCapture2.h"

#include "opencv2/legacy/legacy.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>


using namespace cv;
using namespace FlyCapture2;


#define SLEEP_US 33333 // 30FPS
#define MAGIC 0x17923349ab10ea9aL

static int64_t utime_now()
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (int64_t) tv.tv_sec * 1000000 + tv.tv_usec;
}

static void write_i32(uint8_t *buf, int32_t v)
{
    buf[0] = (v >> 24) & 0xFF;
    buf[1] = (v >> 16) & 0xFF;
    buf[2] = (v >>  8) & 0xFF;
    buf[3] = (v      ) & 0xFF;
}

static void write_i64(uint8_t *buf, int64_t v)
{
    uint32_t h = (uint32_t) (v >> 32);
    uint32_t l = (uint32_t) (v);

    write_i32(buf+0, h);
    write_i32(buf+4, l);
}
/*
static uint8_t* get_event_buffer(int *_len)
{
    int64_t magic = MAGIC;

    int64_t utime = utime_now();

    int32_t width  = 128;
    int32_t height =  64;

    char *format = "GRAY8";
    int32_t formatlen = strlen(format);

    int32_t imlen = width*height*sizeof(uint8_t);
    uint8_t *im = malloc(imlen);

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            im[y*width+x] = rand() & 0xFF;

    int32_t buflen = 8 + 8 + 4 + 4 + 4 + formatlen + 4 + imlen;
    uint8_t *buf = calloc(buflen, sizeof(uint8_t));
    uint8_t *ptr = buf;

    write_i64(ptr, magic);          ptr += 8;
    write_i64(ptr, utime);          ptr += 8;
    write_i32(ptr, width);          ptr += 4;
    write_i32(ptr, height);         ptr += 4;
    write_i32(ptr, formatlen);      ptr += 4;
    memcpy(ptr, format, formatlen); ptr += formatlen;
    write_i32(ptr, imlen);          ptr += 4;
    memcpy(ptr, im, imlen);         ptr += imlen;

    free(im);

    *_len = buflen;
    return buf;
}
*/
int main(int argc, char *argv[])
{
    setlinebuf(stdout);
    setlinebuf(stderr);

    if (argc != 3) {
        printf("Usage: tcptest <host> <port>\n");
        exit(1);
    }







    Error error;
    Camera camera;
    CameraInfo camInfo;

    // Connect the camera
    error = camera.Connect( 0 );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to connect to camera" << std::endl;     
        return false;
    }
    
    // Get the camera info and print it out
    error = camera.GetCameraInfo( &camInfo );
    if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to get camera info from camera" << std::endl;     
        return false;
    }
    std::cout << camInfo.vendorName << " "
              << camInfo.modelName << " " 
              << camInfo.serialNumber << std::endl;
    
    error = camera.StartCapture();
    if ( error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
    {
        std::cout << "Bandwidth exceeded" << std::endl;     
        return false;
    }
    else if ( error != PGRERROR_OK )
    {
        std::cout << "Failed to start image capture" << std::endl;     
        return false;
    } 












    char *host = argv[1];
    int   port = atoi(argv[2]);

    printf("Connecting to '%s' on port %d\n", host, port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock  < 0) {
        printf("Error opening socket\n");
        perror("socket: ");
        //goto cleanup;
    }

    struct hostent *server = gethostbyname(host);
    if (server == NULL) {
        printf("Error getting host by name\n");
        perror("gethostbyname: ");
       // goto cleanup;
    }

    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(port);
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error connecting to socket\n");
        perror("connect: ");
        //goto cleanup;
    }

    printf("Connected\n");

    char key = 0;

    while (1)
    {
        int len = -1;
        //uint8_t *buf = get_event_buffer(&len);



        // Get the image
        Image rawImage;
        Error error = camera.RetrieveBuffer( &rawImage );
        if ( error != PGRERROR_OK )
        {
            std::cout << "capture error" << std::endl;
            continue;
        }
        
        // convert to rgb
        Image rgbImage;
        rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );
       
        // convert to OpenCV Mat
        unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
        cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes);
        
        //cv::flip(image, flipped_image, -1);
        
           
        
        // convert opencv image into a buffer
        
        
        cv::Mat grey_mat;
        cv::cvtColor(image, grey_mat, CV_BGR2GRAY, 1);
        
        
        
        CV_Assert(grey_mat.type() == CV_8UC1);
        
        cv::imshow("image", grey_mat);
        key = cv::waitKey(30); 
        
        int64_t magic = MAGIC;

        int64_t utime = utime_now();

        int32_t width  = grey_mat.cols;
        int32_t height =  grey_mat.rows;

        char *format = "GREY8";
        int32_t formatlen = strlen(format);

        int32_t imlen = width*height*sizeof(uint8_t);

        int32_t buflen = 8 + 8 + 4 + 4 + 4 + formatlen + 4 + imlen;
        
        uint8_t *buf = (uint8_t*) calloc(buflen, sizeof(uint8_t));
        uint8_t *ptr = buf;

        write_i64(ptr, magic);          ptr += 8;
        write_i64(ptr, utime);          ptr += 8;
        write_i32(ptr, width);          ptr += 4;
        write_i32(ptr, height);         ptr += 4;
        write_i32(ptr, formatlen);      ptr += 4;
        memcpy(ptr, format, formatlen); ptr += formatlen;
        write_i32(ptr, imlen);          ptr += 4;
        memcpy(ptr, grey_mat.ptr(0), imlen);         ptr += imlen;

       // *_len = buflen;
        


        














        int bytes = send(sock, buf, buflen, 0);

        free(buf);

        if (bytes != len) {
            printf("Tried to send %d bytes, sent %d\n", buflen, bytes);
            perror("send: ");
           // goto cleanup;
        }

        usleep(SLEEP_US);
    }

    //cleanup:
    
    error = camera.StopCapture();
    if ( error != PGRERROR_OK )
    {
        // This may fail when the camera was removed, so don't show 
        // an error message
    }  
    
    camera.Disconnect();

    if (sock >= 0)
        close(sock);

    printf("Done\n");
    exit(0);
}
