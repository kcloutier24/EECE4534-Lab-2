# Lab 2 Assignment PWM Generation

We will generate PWM signals through various methods (busy loop,
software timer, hardware timer) and compare each method in terms of
quality of the generated PWM. In addition to the peripheral interfacing 
we will learn about scheduling effects and initial timing analysis.

## Makefile notes

Skeleton code in this repository uses a more full-featured Makefile that is able to cross-compile, upload to board, run on board, kill running program on board and remote debug using ```gdb```. If you feel comfortable, just take a look inside the Makefiles and also the included Makefile at *common/zed.mk* to see which targets are available.

## libzed 

Starting from Lab 2, the lab assignments take advantage of a libzed,
which provides convenient abstractions for many low level aspects. 
To save time, you'll be allowed to use it for controlling peripherals which you have *already worked with in past labs*, e.g. in this lab you may use the code for easier handling of GPIOs. You will only have access to the header files, though, and use the library as a shared object when compiling your code. You can find the headers in the 
SDK at `$OECORE_TARGET_SYSROOT/usr/include/libzed` and the Makfiles already reference it. In installations without SDK, see the binary repo [neu-ece-esl/libzed-bin](https://github.com/neu-ece-esl/libzed-bin). 


# Lab Instructions
## Pre-Lab 

We have limited time available for the lab. To make efficient use of
this time, you will need to prepare for it. First, go through the
assigned reading above and get an overview of the schematics and manuals
mentioned earlier.

Please go to Canvas and answer the questions that are posted for this
Pre-lab assignment online. Use the Reading List section below for references.

## Overview 

The instructions for ths lab are detailed in the following steps:

 1. (Reserved for feedback branch pull request. You will receive top level feedback there). 
 2. [Busy Loop PWM](.github/STARTING_ISSUES/2.%20Busy%20Loop%20PWM.md)
 3. [Software Timer PWM](.github/STARTING_ISSUES/3.%20Software%20Timer%20PWM.md)
 4. [Hardware Timer PWM](.github/STARTING_ISSUES/4.%20Hardware%20Timer%20PWM.md)
 5. [Edge Cases](.github/STARTING_ISSUES/5.%20Edge%20Cases.md)
 6. [Extra](.github/STARTING_ISSUES/6.%20Extra.md)
 7. [Post-Lab](.github/STARTING_ISSUES/7.%20Post-Lab.md)

After accepting this assignment in github classroom, each step is converted into a [github issue](https://docs.github.com/en/issues). Follow the issues in numerically increasing issue number (the first issue is typically on the bottom of the list). 

## General Rules

Please commit your code frequently or at e very logical break. Each commit should have a meaningful commit message and [cross reference](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/autolinked-references-and-urls#issues-and-pull-requests) the issue the commit belongs to. Ideally, there would be no commits without referencing to a github issue. 

Please comment on each issue with the problems faced and your approach to solve them. Close an issue when done. 

# Reading List

The following documents will help you complete the pre-lab assignment.
These will also help you in subsequent lab assignments.

Please complete the reading assignments that are marked as
***\[Required\]*** before attempting the pre-lab assignments.

Xilinx, **AXI TIMER v2.0 Product Guide**, October 5, 2016  
[`AXI Timer`](https://www.xilinx.com/support/documentation/ip_documentation/axi_timer/v2_0/pg079-axi-timer.pdf)  
Sections: *Register Space* and *Programming Sequence*
***\[Required\]*** 

Linux Programmer’s Manual, **NICE**, man-pages v4.14  
<http://man7.org/linux/man-pages/man1/nice.1.html> ***\[Required\]*** 

EECE4534, **The PulseCap module**, [link](https://neu-ece-4534.github.io/pulsecap.html)

**HINT:** Whenever you see the man-pages mentioned or a comment like
*see man \<name\>*, you can examine the documentation for that by simply
typing `man <name>` in a terminal inside the development environment.
Online man pages are available as well, such as
<https://linux.die.net/man/>.
