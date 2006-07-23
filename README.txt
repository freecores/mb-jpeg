Open Source JPEG COdec based on Microblaze

1) Introduction

This is an open source JPEG codec, including encoder and decoder, for embedded systems. It can be fully synthesized and implemented on FPGA.

Different to a fully hardware implementation, this JPEG codec is designed based on Xilinx Microblaze processor with customized hardware accerlators. It is expected to achieve high flexibilty, low complexity at little cost of size and performance.

You can open the project with Xilinx EDK8.1 or higher. The verification hardware platform I use is Xilinx XUP board with a Xilinx XC2V30P on it. It also provides necessary peripherals such as CF card for image storage and video output. The board can be obtained at the cost of 300 euro if you are in a university.

At the same time, it can be used as a library or IP core for video applications, for instance, MPEG codec. The IP cores can be integrated immediately. It is actually part of my master project and I try to write down in detail how I design and how to use it. Enjoy!

2) Roadmap

For encoder
1. Setup the testbench and development environment
1.1 Simple environment with CF card and without external memory (Done)
1.2 Full environment with CF card, VGA in/out and external memory
2. Port reference code to microblaze
2.1 Port code to XUP2PRO platform and microblaze processor (Done)
2.2 Elaborate code for memory and speed (<- Current)
3. Design a simple FSL accelerator to evaluate the FSL design flow
3.1 Design a FSL accelerator for MAC operation (Done)
4. Design dct FSL accelerator
5. Design color conversion accelerator
6. Design vlc accelerator
7. Port code into and optimize for different platforms
7.1 Port to Xilinx Spartan III board
8. Experiment for Motion JPEG streaming
9. Start to design MPEG codec... :)

For decoder, it is roughly the same.

3) Features

    * Baseline JPEG encoder
    * Baseline JPEG decoder

4) Milestones

2006/07/05 Step 1.1 - Setup the testbench and development environment/Simple environment with CF card and without external memory (Sunwei) CVSTag: Step1_1
2006/07/18 Step 2.1 - Port reference code (Joris van Emden) to Microblaze and XUP2PRO board (Sunwei) CVSTag: Step2_1b
2006/07/20 Step 3.1 - Design a FSL accelerator to do MAC operation (Sunwei)

5) BitStream to Download

Bitstreams are in CVS/bitstreams directory (http://www.opencores.org/cvsweb.shtml/mb-jpeg/bitstreams/). To download a bitstream to Xilinx XUP2PRO board, you can use impact -batch download_XUP2PRO.cmd in EDK shell. Please note that the bit file to download is set in cmd file for the latest version. If you need to download an old version, it is necessary to modify cmd file.

1. V0.1 2006/07/19 (CVSTag Step2_1b)
You can download this bitstream to an Xilinx XUP2PRO board with CF card and it can compress image01.bmp on CF card to image01.jpg and write back to CF card. Due to current implementation limit, the BMP file size can not exceed 64KB for this version of bitstream. It is going to be fixed in next release.

6) Developers
Sunwei  sunwei388@gmail.com
Joris van Emden  joris.van.emden@gmail.com
Marcel Lauwerijssen mlauwerijssen@morphcore.com

http://www.opencores.org/projects.cgi/web/mb-jpeg/overview



