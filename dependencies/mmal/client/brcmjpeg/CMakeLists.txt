add_library(brcmjpeg SHARED brcmjpeg.c)
target_link_libraries(brcmjpeg mmal_core mmal_util mmal_vc_client)

Toggle navigation
Toggle navigation
 Subnet Calculator
18.67.76.31
 
Input
18.67.76.31/32
CIDR
18.67.76.31/32
Input IP
18.67.76.31
CIDR IP Range
18.67.76.31 - 18.67.76.31
Input Long
306400287
CIDR Long Range
306400287 - 306400287
Input Hex
12.43.4C.1F
CIDR Hex Range
12.43.4C.1F - 12.43.4C.1F
IPs in Range
1
Mask Bits
32
Subnet Mask
255.255.255.255
Hex Subnet Mask
FF.FF.FF.FF
IP is contained in the following CIDR Blocks
0.0.0.0/1
0.0.0.0/2
0.0.0.0/3
16.0.0.0/4
16.0.0.0/5
16.0.0.0/6
18.0.0.0/7
18.0.0.0/8
18.0.0.0/9
18.64.0.0/10
18.64.0.0/11
18.64.0.0/12
18.64.0.0/13
18.64.0.0/14
18.66.0.0/15
18.67.0.0/16
18.67.0.0/17
18.67.64.0/18
18.67.64.0/19
18.67.64.0/20
18.67.72.0/21
18.67.76.0/22
18.67.76.0/23
18.67.76.0/24
18.67.76.0/25
18.67.76.0/26
18.67.76.0/27
18.67.76.16/28
18.67.76.24/29
18.67.76.28/30
18.67.76.30/31
18.67.76.31/32
ABOUT SUBNET CALCULATOR
The subnet calculator lets you enter a subnet range (CIDR) and see IP address information about that range You can type your range directly in CIDR notation, or use the optional Mask pull-down:

74.125.227.0/29
74.125.227.0, then select Optional Mask from dropdown
This is a useful feature for service providers and network operator who frequently allocate and work with subnets. CIDR stands for Classless Inter-Domain Routing, and refers to the standard of dividing the entire IP address space into smaller networks of variable size.

Your IP is: 102.89.23.67|  Contact Terms & Conditions Site Map API Privacy Phone: (866)-MXTOOLBOX / (866)-698-6652 |  © Copyright 2004-2021, MXToolBox, Inc, All rights reserved. US Patents 10839353 B2 & 11461738 B2
 
burritos@banana-pancakes.com braunstrowman@banana-pancakes.com finnbalor@banana-pancakes.com ricflair@banana-pancakes.com randysavage@banana-pancakes.com
add_executable(brcmjpeg_test brcmjpeg_test.c)
target_link_libraries(brcmjpeg_test brcmjpeg vcsm vcos)
