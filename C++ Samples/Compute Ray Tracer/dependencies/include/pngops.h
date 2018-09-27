#pragma once 
#include <png.h>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>

class Image{
  public:
    int w;
    int h;
    bool sixteenBit;
    uint8_t* data;
    Image(){
        w=h=0;
        sixteenBit=0;
    }
    Image(std::string infile){
        read(infile);
    }
    Image(int w, int h, bool sixteenBit){
        allocate(w,h,sixteenBit);
    }
    Image(const Image& i2){
        w=i2.w;
        h=i2.h;
        sixteenBit=i2.sixteenBit;
        datav = i2.datav;
        makePointer();
    }
    Image(Image&& i2){
        w=i2.w;
        h=i2.h;
        sixteenBit=i2.sixteenBit;
        datav.swap(i2.datav);
        makePointer();
    }
    void allocate(int w, int h, bool sixteenBit){
        this->w=w;
        this->h=h;
        this->sixteenBit=sixteenBit;
        datav.resize(w*h*4*(sixteenBit?2:1)+15);
        makePointer();
    }
    
    void read(std::string infile){
        using namespace std;
        //binary mode is important on Windows!
        FILE* fp = fopen(infile.c_str(), "rb");
        
        if(!fp)
            throw runtime_error("Cannot open input png "+infile);
        
        FileRAII closer(fp);
        
        //look at the header. Reject if it's not a PNG file.
        uint8_t hdr;
        fread(&hdr, 1, 1, fp);          //read some data
        if( png_sig_cmp(&hdr, 0, 1) ){      //compare to known png format
            throw runtime_error("File "+infile+": Not a png!");
        }
        
        //prepare the png library for reading
        png_structp ps = png_create_read_struct(
            PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL    //use default error handlers
            );
        
        if(!ps){
            throw runtime_error("Cannot allocate PNG struct");
        }
        
        //create a structure for receiving png info
        png_infop ip = NULL;
        ip = png_create_info_struct(ps);
        
        if(!ip ){
            png_destroy_read_struct(&ps, (ip? &ip : NULL), NULL );
            throw runtime_error("No PNG info struct");
        }
        
        png_init_io( ps, fp );
        png_set_sig_bytes(ps, 1);
        png_read_info(ps,ip);

        int colorType = png_get_color_type(ps,ip);
        int bitDepth = png_get_bit_depth(ps,ip);

        if( png_get_valid(ps,ip,PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(ps);

        if( bitDepth < 8 )
            png_set_packing(ps);
        
        switch(colorType){
            case PNG_COLOR_TYPE_PALETTE:
                png_set_palette_to_rgb(ps);
                png_set_filler(ps,0xffff,PNG_FILLER_AFTER);
                break;
            case PNG_COLOR_TYPE_GRAY:
            case PNG_COLOR_TYPE_GRAY_ALPHA:
            {
                png_set_gray_to_rgb(ps);
                if( bitDepth  < 8 )
                    png_set_expand_gray_1_2_4_to_8(ps);
                if( colorType == PNG_COLOR_TYPE_GRAY )
                    png_set_filler(ps,0xffff,PNG_FILLER_AFTER);
                break;
            }
            case PNG_COLOR_TYPE_RGB:
                png_set_filler(ps,0xffff,PNG_FILLER_AFTER);
                break;
        }
        
        if( png_get_interlace_type(ps,ip) != PNG_INTERLACE_NONE )
            throw runtime_error("Can't handle interlaced file");
            
            
        png_read_update_info(ps,ip);

        int bytesPerPixel;
        if( bitDepth == 8 )
            bytesPerPixel = 4;
        else if( bitDepth == 16 )
            bytesPerPixel = 8;
        else
            throw runtime_error("Internal error");
           
        allocate(
            png_get_image_width(ps,ip),
            png_get_image_height(ps,ip),
            bitDepth==16
        );
        
        Image& img = *this;
        int count = img.w*bytesPerPixel;
        uint8_t* D = img.data;
        for(int r=0;r<img.h;++r,D += count ){
            png_read_row(ps,D,NULL);
        }
        
        png_destroy_read_struct(&ps, &ip, NULL );
         
    }


    void write(std::string fname) const{
        const Image& img = *this;
        
        FILE* fp = fopen(fname.c_str(),"wb");
        if(!fp)
            throw new std::runtime_error("Could not open file");
            
        FileRAII closer(fp);
        
        png_structp ps = png_create_write_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);
        if(!ps )
            throw std::runtime_error("Cannot create write struct");
            
        png_infop ip = png_create_info_struct(ps);
        if(!ip){
            png_destroy_write_struct(&ps, (png_infopp)NULL);
            throw std::runtime_error("Cannot create info struct");
        }
        
        png_init_io(ps,fp);
        png_set_IHDR(ps, ip, img.w, img.h, 
            img.sixteenBit ? 16:8, PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);
            
        png_write_info(ps,ip);
        unsigned count = img.w*4*(img.sixteenBit?2:1);
        const unsigned char* P = img.data;
        for(int i=0;i<img.h;++i){
            png_write_row(ps,(png_bytep)P);
            P += count;
        }
        png_write_end(ps,ip);
        png_destroy_write_struct(&ps,&ip);
        return;
    }
    
  private:
    std::vector<uint8_t> datav;
    void makePointer(){
        uint64_t tmp = (uint64_t) datav.data();
        tmp += 15;
        tmp &= ~0xf;
        data = (uint8_t*) tmp;
    }

    struct FileRAII{
        FILE* fp;
        FileRAII(FILE* fp){
            this->fp=fp;
        }
        ~FileRAII(){
            fclose(fp);
        }
    };
};
        
     
