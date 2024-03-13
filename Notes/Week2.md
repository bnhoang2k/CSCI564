---
title: "Performance Metrics"
author: Bryan Hoang
date: 3/11/2024
geometry: margin=2cm
output: pdf_document
---
<!-- pandoc example.md -o example.pdf -->

## 1.5 Trends in Power and Energy in Integrated Circuits

Today energy is the biggest challenge faced by the designer for nearly every class of computer. Power must be brought in and distributed evenly throughout the chip, and modern microprocessors use hundreds of pins and multiple interconnect layers just for power and ground. Second, power is dissipated as heat and must be removed.

### Power and Energy: A Systems Perspective

How should system architects think about performance, power, and energy?

- What is the maximum power a processor ever requires? We must meet this demand to ensure and account for the worst-case scenario. For example, if processors attempt to draw more power than a power-supply system can provide (typically by drawing more current than a system can supply), the result is a voltage drop which can cause processors to malfunction.

- What is the sustained power consumption? Otherwise known as *thermal design power (TDP)*, it determines the cooling requirement. TDP is neither peak power, which is often 1.5 times higher, nor is it the actual average power consumed during a typical workload.
  
  A typical power supply for a system is typically sized to exceed the TDP, and a cooling system is usually designed to match or exceed TDP. Failure to provide adequate cooling allows junction temperature in the processor to exceed its maximum value and result in device failure or permanent damage.

- Designers must consider what energy and energy efficiency is. Recall, power is energy per time. Which metric is right for comparing processors? Energy or power? In general, energy is always a better metric because its tied to a specific task and the time required for that task.
  
  For example, the energy it takes to complete a workload is equal to the average power times the execution time for the workload.

  **Thus, if we want to know which of two processors is more efficient for a given task, we compare energy consumption (not power) for executing a task. For example, even if processor A has a 20% higher average power consumption than processor B, if A executes the task in only 70% of the needed time of B, its energy consumption will be $1.2 \times 0.7 = 0.84$, which is better.**

### Energy and Power within Microprocessors

For CMOS (a type of technology) chips, the traditional primary energy consumption has been in switching transistors, also called *dynamic energy.* The energy required per transistor is proportional to the product of the capacitive load driven by the transition and the square of the voltage swing.

$$\text{Energy}_{\text{dynamic}} \propto \text{Capacitive load} \times \text{Voltage}^2$$