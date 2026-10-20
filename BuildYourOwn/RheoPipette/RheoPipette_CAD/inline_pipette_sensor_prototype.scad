// SPDX-License-Identifier: CERN-OHL-W-2.0
// INLINE PIPETTE PRESSURE ADAPTER -- FITTING PROTOTYPE, NOT CALIBRATED.
// Units: mm. Newly authored 2026-10-12. No external dependencies.
// Intended assembly: Socorex Acura 825.1000 -> adapter -> Fisherbrand tip.
// Side port: tubing -> SparkFun SEN-16476 pressure sensor.
// This replaces the earlier hose-fed aspirator concept; there is no hose inlet.
//
// VERIFIED REFERENCES
// Socorex 825.1000 is the 100-1000 uL Acura 825, using 1000 uL tips:
// https://www.socorex.com/en/laboratory/manual-pipettes/acura-manual/825-manual-micropipette
// User's tip reference matches Fisher 02-707-407, SureOne blue, non-filtered,
// universal fit; currently listed at 100-1250 uL and 89 mm long:
// https://www.fishersci.com/shop/products/02707407/02707407
// Neither reference supplies the mating taper dimensions used below.
// FEMALE SOCKET AND MALE CONE DIMENSIONS ARE INITIAL ESTIMATES. Fit-test first.
// SparkFun publishes a smooth male 2.5 mm-OD pressure port:
// https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html
// Honeywell Table 20 lists tubing IDs 0.093-0.094 in (2.36-2.39 mm):
// https://cdn.sparkfun.com/assets/2/e/8/0/9/honeywell-mpr-datasheet.pdf
// No external CAD was copied. No matching inline design was found in the
// Thingiverse search; original aspirator references do not define this socket.
//
// FIT WORKFLOW
// 1. Export part="fit_tests". Five FEMALE sockets and five MALE tip cones.
//    Print with the same settings and orientation as the full adapter.
//    1/2/3/4/5 rim notches = -0.30/-0.15/0/+0.15/+0.30 mm diameter offsets.
//    Female row y=0: positive = larger socket / looser pipette fit.
//    Male row y=20: positive = larger cone / tighter disposable-tip fit.
//    Nozzle fit must seat gently and release without force. Stop if it binds.
// 2. Enter the selected offsets in socket_diameter_adjust / tip_diameter_adjust.
//    A diameter sweep does not validate the taper angle; if nothing seals,
//    measure the actual nozzle/tip and edit the independent taper dimensions.
// 3. Re-export part="adapter" and leak-test the entire assembly.
// 4. Do NOT use the pipette ejector as a disposable-tip ejector with this part.
//    It may push the whole adapter off. Remove disposable tips manually while
//    supporting the adapter, with appropriate handling for the sample.
//
// PRINTING
// Model Z=0 is the female socket opening, placed on the build plate. Pipette
// enters this end; the male cone is at Z=20..30 and points up when printing.
// In use the assembly is normally inverted, disposable tip pointing down.
// PLA prototype: 0.12-0.16 mm layers, >=4 walls, 100% infill, external brim.
// Side nipple needs removable EXTERNAL support; no support inside any passage.
// Female socket tapers inward and uses a sloping transition to the main bore.
// A 0.25 mm nozzle is preferable for the side port; inspect thin-wall paths
// if using 0.4 mm. Clear any obstruction before attaching to the pipette.
// Keep the socket lip free of brim/elephant-foot material. Do not force a fit.
//
// VOLUME ACCURACY
// User target: +/-1% throughout 100-1000 uL. NOT DEMONSTRATED by this CAD.
// Socorex specifies inaccuracy limits of +/-1.5%, +/-1.0%, +/-0.5% at minimum,
// midpoint and maximum volume for 825.1000 under its stated test conditions.
// https://www.socorex.com/images/Documents/LAB/Operating%20Instructions/Acura%20manual/Acura_manual_815_825_835_855_Operating_Instructions_EN_DE_FR.pdf
// Added air in the adapter, residual socket gap, and sensor line affects
// dispensing. Nominal CAD volume is NOT a prediction of the dispensing error.
// Verify the unmodified pipette+tip first, then the COMPLETE sensor-connected
// assembly using repeated gravimetric measurements across the working range.
// Calibration/acceptance must include both bias and repeatability. A single
// successful dispense or a single calibration point cannot establish +/-1%.
// Use actual operating conditions and an appropriate balance/procedure;
// detailed professional pipette calibration is outside this geometry file.
// No certified accuracy, airtightness, sterility or pressure rating is claimed.
// Keep liquid in the disposable tip and out of the sensor branch.

/* [Output] */
part = "adapter"; // [adapter:Inline fitting prototype,fit_tests:Five sockets and five tip cones,socket_tests:Five female sockets,tip_tests:Five male cones,sensor_tests:Three sensor port coupons,section:Cutaway - do not print]
resolution = 128; // [64:16:256]

/* [Pipette socket - estimated, fit-test required] */
// Female opening at Z=0; tapered socket becomes narrower with insertion depth.
socket_depth = 12; // [8:0.5:16]
socket_entry_diameter = 8.00; // [7:0.05:9.5]
socket_deep_diameter = 6.60; // [5.5:0.05:8]
// Positive enlarges the hole. This adjustment is DIAMETER, not radius.
socket_diameter_adjust = 0; // [-0.6:0.05:0.6]
socket_entry_chamfer = 0.15; // [0:0.025:0.3]
// Sloped transition avoids a flat internal roof when printing socket-down.
socket_transition_length = 3; // [2.5:0.25:5]

/* [Disposable tip cone - estimated, fit-test required] */
tip_length = 10; // [8:0.5:14]
tip_base_diameter = 7.80; // [7:0.05:9]
tip_end_diameter = 6.60; // [5.5:0.05:8]
// Positive makes cone larger/tighter; independent of the female socket.
tip_diameter_adjust = 0; // [-0.6:0.05:0.6]
tip_entry_chamfer = 0.10; // [0:0.025:0.2]

/* [Compact body and airflow] */
body_diameter = 11; // [10:0.5:14]
// Socket depth + transition + junction length + tip = 30 mm by default.
junction_length = 5; // [5:0.5:8]
// Air path between pipette outlet and disposable tip; liquid stays in tip.
main_bore_diameter = 1.60; // [1.2:0.1:2.4]

/* [Pressure sensor side port] */
// Nominal mating OUTSIDE diameter, matching SparkFun's smooth male port.
sensor_outer_diameter = 2.50; // [2.3:0.05:3]
sensor_diameter_adjust = 0; // [-0.2:0.05:0.2]
sensor_bore_diameter = 1.00; // [0.8:0.05:1.2]
sensor_stem_length = 6; // [4:0.5:8]
sensor_boss_diameter = 5; // [4.5:0.25:6]
sensor_boss_projection = 2; // [1.5:0.5:3]
sensor_entry_chamfer = 0.10; // [0:0.025:0.15]

/* [Fit tests] */
// Five socket and cone sizes, indexed from smallest to largest.
fit_step = 0.15; // [0.05:0.05:0.2]
sensor_fit_step = 0.10; // [0.05:0.05:0.15]

/* [Sensor line volume estimate only - does not change the printed part] */
// Provisional layout: nearby sensor and 3 cm of flexible tube; adjust to actual.
sensor_tube_id = 2.38; // [1:0.01:3]
sensor_tube_length = 30; // [10:5:300]

/* [Hidden] */
$fn=resolution;
eps=0.02;
body_h=socket_depth+socket_transition_length+junction_length;
total_h=body_h+tip_length;
main_r=main_bore_diameter/2;
port_z=socket_depth+socket_transition_length+junction_length/2;
boss_start=body_diameter/2-1;
shoulder_x=body_diameter/2+sensor_boss_projection;
port_blend=0.4;
stem_x=shoulder_x+port_blend;
port_end=stem_x+sensor_stem_length;
edge_h=0.25;
coupon_pitch=body_diameter+7;
coupon_rows=body_diameter+9;
socket_coupon_h=socket_depth+socket_transition_length+2;
tip_coupon_h=2;

function sock_entry(delta=0)=(socket_entry_diameter+socket_diameter_adjust+delta)/2;
function sock_deep(delta=0)=(socket_deep_diameter+socket_diameter_adjust+delta)/2;
function cone_base(delta=0)=(tip_base_diameter+tip_diameter_adjust+delta)/2;
function cone_end(delta=0)=(tip_end_diameter+tip_diameter_adjust+delta)/2;
function port_r(delta=0)=(sensor_outer_diameter+sensor_diameter_adjust+delta)/2;
function frustum_volume(h,r1,r2)=PI*h*(r1*r1+r1*r2+r2*r2)/3;

module female_void(delta=0) {
    // One continuous revolved profile preserves the specified taper exactly.
    rotate_extrude(convexity=10)
        polygon([[0,-eps],[sock_entry(delta)+socket_entry_chamfer,-eps],
                 [sock_entry(delta)+socket_entry_chamfer,0],
                 [sock_entry(delta)+(sock_deep(delta)-sock_entry(delta))*edge_h/socket_depth,edge_h],
                 [sock_deep(delta),socket_depth],
                 [main_r,socket_depth+socket_transition_length],
                 [main_r,socket_depth+socket_transition_length+eps],
                 [0,socket_depth+socket_transition_length+eps]]);
}

module male_outer(z0,delta=0) {
    // Small end is chamfered; full nominal taper ends edge_h before the rim.
    translate([0,0,z0]) rotate_extrude(convexity=10)
        polygon([[0,0],[cone_base(delta),0],
                 [cone_end(delta)+(cone_base(delta)-cone_end(delta))*edge_h/tip_length,
                  tip_length-edge_h],
                 [cone_end(delta)-tip_entry_chamfer,tip_length],
                 [0,tip_length]]);
}

module sensor_outer(z,delta=0) {
    translate([0,0,z]) rotate([0,90,0]) rotate_extrude(convexity=10)
        polygon([[0,boss_start],[sensor_boss_diameter/2,boss_start],
                 [sensor_boss_diameter/2-0.35,shoulder_x],
                 [port_r(delta)+port_blend,shoulder_x],
                 [port_r(delta),stem_x],[port_r(delta),port_end-edge_h],
                 [port_r(delta)-sensor_entry_chamfer,port_end],[0,port_end]]);
}

module sensor_void(z) {
    translate([-eps,0,z]) rotate([0,90,0])
        cylinder(d=sensor_bore_diameter,h=port_end+2*eps);
}

module main_void(h) {
    translate([0,0,-eps]) cylinder(r=main_r,h=h+2*eps);
}

module adapter() {
    difference() {
        union() {
            cylinder(d=body_diameter,h=body_h);
            male_outer(body_h);
            sensor_outer(port_z);
        }
        female_void();
        main_void(total_h);
        sensor_void(port_z);
    }
}

// Notches sit away from the socket and mating cone. Count on each coupon's rim.
module notches(n,z) {
    for(i=[0:n-1]) rotate([0,0,180+(i-(n-1)/2)*18])
        translate([body_diameter/2,0,z-0.7]) cylinder(r=0.5,h=0.7+eps,$fn=24);
}

module socket_coupon(delta,n) {
    // Same socket-down print orientation as adapter. Turn over to insert pipette.
    difference() {
        cylinder(d=body_diameter,h=socket_coupon_h);
        female_void(delta);
        main_void(socket_coupon_h);
        notches(n,socket_coupon_h);
    }
}

module tip_coupon(delta,n) {
    difference() {
        union() {
            cylinder(d=body_diameter,h=tip_coupon_h);
            male_outer(tip_coupon_h,delta);
        }
        main_void(tip_coupon_h+tip_length);
        notches(n,tip_coupon_h);
    }
}

module socket_tests() {
    for(i=[0:4]) translate([(i-2)*coupon_pitch,0,0])
        socket_coupon((i-2)*fit_step,i+1);
}
module tip_tests() {
    for(i=[0:4]) translate([(i-2)*coupon_pitch,coupon_rows,0])
        tip_coupon((i-2)*fit_step,i+1);
}
module sensor_coupon(delta,n) {
    difference() {
        union() {
            cylinder(d=body_diameter,h=10);
            sensor_outer(5,delta);
        }
        main_void(10);
        sensor_void(5);
        notches(n,10);
    }
}
module sensor_tests() {
    for(i=[0:2]) translate([0,(i-1)*coupon_rows,0])
        sensor_coupon((i-1)*sensor_fit_step,i+1);
}

socket_testing=part=="socket_tests" || part=="fit_tests";
tip_testing=part=="tip_tests" || part=="fit_tests";
assert(part=="adapter" || part=="section" || part=="fit_tests" ||
       part=="socket_tests" || part=="tip_tests" || part=="sensor_tests", "Unknown part.");
assert(resolution>=32 && resolution==floor(resolution), "Use integer resolution >=32.");
assert(socket_depth>edge_h && tip_length>edge_h && socket_transition_length>0,
       "Invalid axial dimensions.");
assert(sock_entry()>sock_deep() && cone_base()>cone_end(), "Both fits must taper.");
assert(body_diameter/2-sock_entry(socket_testing ? 2*fit_step : 0)-socket_entry_chamfer>=1,
       "Socket wall below 1 mm. Increase body diameter or reduce socket.");
assert(sock_deep(socket_testing ? -2*fit_step : 0)>main_r,
       "Socket must be wider than the airflow bore.");
assert(socket_transition_length>=sock_deep(socket_testing ? 2*fit_step : 0)-main_r,
       "Increase socket transition length to keep internal slope at <=45 degrees.");
assert(body_diameter/2-cone_base(tip_testing ? 2*fit_step : 0)>=0.8,
       "Insufficient shoulder around disposable tip.");
assert(cone_end(tip_testing ? -2*fit_step : 0)-tip_entry_chamfer-main_r>=1,
       "Tip cone wall below 1 mm.");
assert(main_bore_diameter>sensor_bore_diameter && sensor_bore_diameter>0,
       "Sensor bore must be positive and smaller than main bore.");
assert(junction_length>=sensor_boss_diameter && boss_start>sock_entry(),
       "Increase junction length or adjust body/boss size.");
assert(port_r(part=="sensor_tests" ? -sensor_fit_step : 0)
       -sensor_entry_chamfer-sensor_bore_diameter/2>=0.55,
       "Sensor rim below 0.55 mm wall.");
assert(sensor_boss_diameter/2-0.35>
       port_r(part=="sensor_tests" ? sensor_fit_step : 0)+port_blend,
       "Increase sensor boss diameter.");
assert(sensor_stem_length>edge_h && sensor_entry_chamfer>=0 && tip_entry_chamfer>=0 &&
       socket_entry_chamfer>=0 && fit_step>0 && sensor_fit_step>0,
       "Invalid fit/chamfer settings.");
assert(sensor_tube_id>0 && sensor_tube_length>=0, "Invalid tubing estimate.");

echo("FITTING PROTOTYPE ONLY -- +/-1% liquid accuracy NOT verified");
echo("Overall adapter length / body length (mm)",total_h,body_h);
echo("Estimated female socket entry / deep diameters (mm)",2*sock_entry(),2*sock_deep());
echo("Estimated male tip base / end diameters (mm)",2*cone_base(),2*cone_end());
echo("Sensor port OD / passage diameter (mm)",2*port_r(),sensor_bore_diameter);
echo("Sensor port position from female opening (mm)",port_z);
echo("Main narrow-bore volume, excludes socket, transition, branch (uL)",
     PI*main_r*main_r*(junction_length+tip_length));
echo("Empty tapered socket volume BEFORE pipette insertion, no chamfer (uL)",
     frustum_volume(socket_depth,sock_entry(),sock_deep()));
echo("Empty transition volume (uL)",frustum_volume(socket_transition_length,sock_deep(),main_r));
echo("Assumed sensor tubing ID / length (mm)",sensor_tube_id,sensor_tube_length);
echo("Sensor tube volume alone (uL), ignores port intrusion",PI*pow(sensor_tube_id/2,2)*sensor_tube_length);
echo("Assembled trapped volume depends on nozzle seating; these are NOT predicted volume errors.");

color([0.88,0.10,0.39])
if(part=="adapter") adapter();
else if(part=="fit_tests") { socket_tests(); tip_tests(); }
else if(part=="socket_tests") socket_tests();
else if(part=="tip_tests") tip_tests();
else if(part=="sensor_tests") sensor_tests();
else difference() {
    adapter();
    translate([-body_diameter,0,-eps]) cube([2*body_diameter+port_end,body_diameter,total_h+2*eps]);
}
