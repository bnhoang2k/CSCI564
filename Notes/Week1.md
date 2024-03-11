---
title: "Fundamental of Quantitative Design and Analysis"
author: Bryan Hoang
date: 1/30/2024
geometry: margin=2cm
output: pdf_document
---
<!-- pandoc example.md -o example.pdf -->

## 1.1 Introduction

Today, less than $\$500$ will purchase a cell phone that performs as well as the world's fastest computer in 1993 for $\$50$ million. This rapid improvement comes both from advances in the technology *used to build computers* and *from innovations in computer design*.

Although technological improvements have been fairly steady historically, progress arising from better computer architectures has been much less consistent. For the first 25 years of computers, both forces made significant contrivution, delivering a 25% performance improvement per year.

For example, the late 1970s saw the emergence of the microprocessor. The ability of the microrpocessor to ride the improvements in IC technology led to a higher rate of performance improvement, about 35% per year. This growth rate, combined with the cost advantage of a mass-produced microprocessor, led to an incrasing fraction of the computer business being based on microprocessors.

In addition, two significant changes in the computer marketplace made it easier than ever before to succeed commerically with a new architecture. First, the virtual elimination of assembly language programming reduced the need for object-code compatibility. Second, the creation of standardized, vendor-independent OS, such as UNIX and its clone, Linux, lowered the cost and risk of bringing out a new architecture.

*I.e., there are two main reasons for the huge leap in performance and affordability: 1) improvements in materials and methods used to build computers, and 2) innovations in computer design like different more efficient ways to organize and structure a computer's components and operations to make it work more efficiently.*

*Computer Architecture improvements are less consistent, but yield great progress when they do occur.*

*Object-code compatibility refers to whether a program written on one computer can be run on a different computer without needing to be rewritten. In the past, changing computer designs (architecture) meant that old programs won't work on new machines. Moving away from assembly language made this less of an issue.*

These changes made it possible to develop a new set of architectures with simpler instruction sets, called RISC architectures. RISC-based machines focused the attention of designers on two critical performance techniques, the exploitation of instruction-level parallelism (initially through pipelining and later through multiple instruction issue) and the use of caches.

RISC based computers rasied the performance bar, forcing prior architectures to keep up or disappear. As transistor counters increased, the hardware overhead of translating more complex x86 architecture became negligible. In low-end applications, such as cell phones, the cost in power and silicon area of the x86-translation overhead helped lead to a RISC architecture, ARM, becoming the dominant architecture.

As a result of architectural and organizational enhancements, it led to over 17 years of sustained growth in performance at an annual rate of over $50\%$. This effect was fourfold during the $20$th century. As a result, it significantly enhanced hte capability available to computer users; for many applications, the highest-performance microprocessor outperformed the supercomputer of less than $20$ years ago.

Additionally, this dramatic improvement in cost-performance led to new classes of computers. For example, personal computers and workstations emerged in the 1980s with the availability of the microprocessor. After that came the rise of smart cellhpones and tablet computers. These new classes of computers have been made possible by the dramatic improvements in cost-performance.

Third, improvement of semiconductor manufacturing predicted by Moore's Law has led to the dominance of microprocessor-based computers across the entire range of computer design. Minicomputers, which were traditionally made from off-the-shelf logic or from gate arrays, were replaced by servers maade by using microprocessors . Even mainframe computers and high-performance supercomputers are now built from collections of microprocessors.

However, the hardware renaissance is slowly coming to end.The fundamental reason is that two characteristics of semiconductors proccesses responsible for the rapid improvement in cost-performance are no longer improving at historical rates.

In 1974, we observed that the power density was constant for a given area of silicon eve as you increased the number of transistors because of smaller dimensions of each transistor. Remarkably, transistors could go faster but use less power. This was called *Dennard scaling*. This scaling ended around 2004 because current and voltage couldn't keep dropping and still maintain the dependability of integrated circuits (IC). Because of this, it forced microprocessor industries to use multiple efficient processors/cores instead of a single inefficient one. Thie signaled a historic switch from relying solely on instruction-level parallelism, to data-level parallelism and thread-level parallelism.

Instruction-level parallelism (ILP) is the ability to execute multiple instructions in parallel. Data-level parallelism (DLP) is the ability to execute the same operation on multiple data elements in parallel. Thread-level parallelism (TLP) is the ability to execute multiple threads in parallel.

Amdahl's Law prescribes practical limits to the number of useful cores per chip. If 10% of a task is serial, then the maximum performance benefit from parallelism is 10 percent no matter how many cores you put on the chip.

The secon dobservation tht ended recently is Moore's Law. Moore's Law states that the number of transistors on a chip doubles every two year. This law has been true for the past 50 years, but it is no longer true.

## 1.3 Defining Computer Architecture

The task the computed designer faces is complex: determine what attributes are important, miaximize computer performance and energy efficiency, while minimizing cost. This task has many aspects, including instruct set design, functional organization, logic design, and implementation.

Optmizing design requires familiarity with a wide variety of technologies, from compilers and OS to logic design and packaging. A few dec ades ago, computer architecture generally referred to only instruction set design. Other aspects of computer design were called implementation, often insinuating that implementation is uninteresting/less important.

This is incorrect because the architect/designer's job is much more than instruct set design, and the technical hurdles in the other aspects of the project are likely more challenging than those encountered in instruction set design.

The only path left to improve energy-performance-cost is specialization. Future microprocessors will include several domain-specific cores that perform only one class of computations well, but they do so remarkably better than general-purpose cores.

### ISA: The Myopic View of Computer Architecture

Myopic: nearsighted, lacking imagination, foresight, or intellectual insight.

Within this book, ISA refers to the actual programmer-visible instruction set. The ISA serves as the boundary between software and hardware. RISC-V is a modern RISC instruction set architecture that is open source and free to use. It is a good example of a modern ISA. In addition to a full software stack (compiilers, OS, and simulators), there are several RISC-V implemntations freely available for use in custom chips or in field-programmable gate arrays.

Developed 30 years after the first RISC instruction sets, RISC-V inheritors a good chunks of its ancestors' design principles:

- A large number of registers
- Easy to pipeline instructions
- Lean set of operations while avoiding their omissions or mistakes

Nearly all ISAs today are classified as general-purpose register architectures, where the operands are either registers or memory locations. The 80x86 has 16 general purpose registers and 16 that can hold floating-point data, while RISC-V has 32 general-purpose and 32 floating-point registers.

Virtually all desktop and server computers use byte addressing to access memory operands. Some architectures require that objects must be aligned. An access to an object of size $s$ bytes at byte address $A$ is aligned if $A mod s = 0$.

## 1.4 Trends in Technology

## 1.5 Trends in Power and Energy in Integrated Circuits

