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

Additionally, this dramatic improvement in cost-performance led to new calsses of computers. For example, personal computers and workstations emerged in the 1980s with the availability of the microprocessor. After that came the rise of smart cellhpones and tablet computers. These new classes of computers have been made possible by the dramatic improvements in cost-performance.



## 1.2 Classes of Computers

## 1.3 Defining Computer Architecture

## 1.4 Trends in Technology

## 1.5 Trends in Power and Energy in Integrated Circuits