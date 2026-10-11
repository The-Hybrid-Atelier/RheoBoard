# What is REP

A REP (retraction-extrusion pulse) is the measurement RheoBoard runs.

The board draws a sample into a tube, then pushes it back out. One pump pulls. The valve switches. The other pump pushes. A pressure sensor on the air line records the trace. The shape of that trace is a short record of how the material flows.

## Reading the trace

Retraction draws the sample into the tube. Extrusion pushes it back out. The trace is the air pressure recorded through both moves.

A pass takes about a second. The vacuum pump runs at full flow, about 2 L/min, and the pressure is sampled for 250 ms. The other pump then pushes at the same flow for 250 ms. Pressure is read 20 times a second.

The line starts on a baseline, the pressure at rest. On a 10-bit reading that band is 490 to 510, which is atmospheric pressure. A pulse is the stretch of the trace that leaves the baseline and returns to it.

During retraction the pressure falls until the drop is deep enough to draw the sample into the tube. That low point is the retraction minimum. The pressure rises as the sample moves farther in. During extrusion the trace drops twice: a short drop while the sample leaves the tube, and a sharper drop as the pressure returns to the baseline.

Each pulse keeps twelve features: the pressure range and the duration of the baseline, the retraction, the extrusion, and equilibrium. Equilibrium is the line back at rest. The chart on this page marks those pressure points and the period bars.

On this board the record is longer: a fixed 1500 ms window of baseline, retract, extrude, and relax. The phase times are in the firmware note below. The trace is compared with p0, the mean pressure of the baseline. The peak vacuum, Δpr, is the minimum of (p − p0) during retract. The peak extrusion pressure, Δpe, is the maximum of (p − p0) during extrude. τ is the time constant of a single-exponential fit to the relax.

While the extrude pump is on, the drive u at time t after the pump turns on follows

```math
u(t) =
\begin{cases}
u_0 + (u_{\max} - u_0) \cdot \frac{t}{T_r} & \text{when } t < T_r \\
u_{\max} & \text{when } t \geq T_r
\end{cases}
```

The defaults are u0 = 40%, umax = 100%, and Tr = 300 ms. A command of u percent is sent as the duty cycle ⌊255u/100⌋.

The pressure trace below marks Δpr at the retraction low point, Δpe at the extrusion high point, and decay, τ on the settling side. The phase lengths on that figure are schematic, not measured.

For a set of pulses from one material, each pulse is compared with the mean pulse, written m̂. With t samples in the sum,

```math
\begin{aligned}
\mathrm{MSE}(m) &= \frac{1}{t} \sum_{i=1}^{t} (m_i - \hat{m}_i)^2 \\[0.75em]
\mathrm{PSNR}(m) &= 20 \cdot \log_{10} \left( \frac{\max(m)}{\sqrt{\mathrm{MSE}(m)}} \right)
\end{aligned}
```

MSE(m) is the average squared gap between the pulses and the mean pulse. PSNR(m) is that gap in decibels. max(m) is the maximum intensity of the signal.

<img src="images/rep-sensing-and-bench.jpg" width="720" alt="Sensing method on the left and the bench photo on the right. The diagram shows a pneumatic actuator, F retraction and F extrusion, an air pressure sensor, PV = nRT, a fluid segment, and a beaker with retraction, extrusion, and a pneumatic controller. The photo shows the airtube probe that retracts and extrudes the sample, a 1–2 second sensing routine that captures air pressure, cyclic sensing, an off-the-shelf pneumatic actuator, and an air pressure sensor">

*Sensing method.*

*Bench with the probe, actuator, and sensor.*

<img src="images/rep-pressure-trace.jpg" width="720" alt="Pressure versus time from 0 to 1500 ms, with retraction, extrusion, and stabilization. Δpr marks the retraction low point, Δpe the extrusion peak, and decay τ the settling side. The phase lengths are labeled schematic, not measured.">

*Pressure trace over the REP window.*

In the shared firmware, a REP runs `BASELINE → RETRACT → EXTRUDE → RELAX` in a fixed 1500 ms window. That timing is in [`../code/firmware/README.md`](../code/firmware/README.md).

**Next:** [Build options](build-options.md).

License: CC BY-SA 4.0 — see [Certification](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Certification).
