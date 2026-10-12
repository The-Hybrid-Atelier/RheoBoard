// SPDX-License-Identifier: CERN-OHL-W-2.0
// Aspirator adapter for a 1 mL pipette tip and 1/4-inch-ID soft tubing.
// Original parametric reconstruction, 2026-10-12. Units: millimetres.
// Photo reference: AssayCell ACT-51003 / Fisher NC2884357 (RSPNKDPT).
// https://www.fishersci.com/shop/products/pink-aspirator-adaptor/NC2884357
// https://assaycell.com/shop/laboratory_supplies/aspirators/aspirator-adapater-for-1-ml-pipette-tip/
// Only tip capacity and tubing ID are published specifications. All other
// dimensions below are estimates/design choices, NOT manufacturer dimensions.
// No external libraries, meshes, fonts, or downloaded CAD are required.
// See aspirator_adapter_notes.md for sources, fit tuning, and print notes.
//
// QUICK START: F6, then File > Export > Export as STL, at 100% scale.
// First select part="fit_tests" to print six small independent coupons.
// Left-to-right: 1 notch = -fit_step, 2 = nominal, 3 = +fit_step.
// Front row: tip cones. Back row: tubing stems. Offsets are DIAMETERS.
// Add the winning offset to tip_diameter_adjust / hose_diameter_adjust.
// Re-export part="adapter". The fit tests use the same mating geometry.
//
// PLA prototype: dry fit and leak test before air or liquid experiments.
// This CAD does not establish airtightness, pressure rating, chemical
// compatibility, sterility, or the commercial part's autoclavability.

/* [Output] */
part = "adapter"; // [adapter:Full adapter,fit_tests:Six fit coupons,section:Longitudinal cutaway - do not print]
// Circumferential facets. 128 gives a smooth small connector.
resolution = 128; // [64:16:256]

/* [Pipette tip fit - estimated, test before use] */
// Male cone enters the OPEN, WIDE end of a disposable 1000 uL tip.
tip_length = 10; // [6:0.5:18]
tip_base_diameter = 7.80; // [7:0.05:9]
tip_end_diameter = 6.60; // [5.5:0.05:8]
// Positive = larger/tighter. Negative = smaller/looser. Diameter, not radius.
tip_diameter_adjust = 0; // [-0.6:0.05:0.6]
tip_edge_chamfer = 0.20; // [0:0.05:0.4]

/* [Tubing fit] */
// Verified intended tubing INNER diameter from Fisher. Not tubing OD.
tube_inner_diameter = 6.35; // [4:0.05:8]
// Stem grows gently toward handle; shallow retention rings mimic the photo.
hose_end_undersize = 0.20; // [0:0.05:0.5]
hose_root_oversize = 0.70; // [0:0.05:1]
hose_diameter_adjust = 0; // [-0.6:0.05:0.6]
hose_length = 30; // [24:1:40]
hose_shoulder_length = 3; // [2:0.5:5]
hose_ring_count = 3; // [1:1:4]
// Radial height above the local stem (0.25 means +0.50 on diameter).
hose_ring_height = 0.25; // [0:0.05:0.5]

/* [Handle - photo-based estimates] */
handle_length = 90; // [55:1:130]
handle_diameter = 10; // [9:0.25:14]
grip_ring_count = 11; // [0:1:16]
grip_ring_pitch = 1.70; // [1.4:0.1:2.5]
grip_ring_height = 0.25; // [0:0.05:0.5]

/* [Continuous passage] */
// Straight bore reduces trapped liquid and avoids an internal support roof.
bore_diameter = 3.20; // [2:0.1:4]
// Diameter compensation for printer undersized holes; independent of fits.
bore_diameter_adjust = 0; // [-0.2:0.05:0.4]
bore_edge_chamfer = 0.20; // [0:0.05:0.4]

/* [Fit coupons] */
// Three diameter offsets for each end: -step, 0, +step.
fit_step = 0.20; // [0.1:0.05:0.4]

/* [Hidden] */
$fn = resolution;
eps = 0.01;
total_length = hose_length + handle_length + tip_length;
tip_z = hose_length + handle_length;
stem_length = hose_length - hose_shoulder_length;
bore_r = (bore_diameter + bore_diameter_adjust) / 2;
tip_chamfer_length = 0.35;
grip_end = tip_z - 4;
grip_start = grip_end - max(0, grip_ring_count - 1) * grip_ring_pitch - 1.0;
coupon_base_h = 3;
coupon_base_r = max(7, handle_diameter / 2 + 1);
coupon_pitch = 2 * coupon_base_r + 8;
coupon_row_pitch = 2 * coupon_base_r + 10;

function tip_r(z, delta=0) =
    (tip_base_diameter + tip_diameter_adjust + delta
     + (tip_end_diameter-tip_base_diameter)*z/tip_length)/2;

function hose_r(z, delta=0) =
    (tube_inner_diameter + hose_diameter_adjust + delta
     - hose_end_undersize
     + (hose_root_oversize+hose_end_undersize)*z/stem_length)/2;

// Profile arrays are [radius, height], ordered from tubing end toward tip.
function hose_profile(delta=0) = concat(
    [[hose_r(0,delta)-0.15,0], [hose_r(1,delta),1]],
    [for(i=[0:hose_ring_count-1])
        let(z=3 + i*(stem_length-7)/hose_ring_count,
            w=min(4,(stem_length-7)/hose_ring_count-0.5))
        each [[hose_r(z,delta),z],
              [hose_r(z+w-0.5,delta)+hose_ring_height,z+w-0.5],
              [hose_r(z+w,delta),z+w]]],
    [[hose_r(stem_length,delta),stem_length],
     [handle_diameter/2,hose_length]]
);

function tip_profile(z0,delta=0) = [
    [tip_r(0,delta),z0],
    [tip_r(tip_length-tip_chamfer_length,delta),z0+tip_length-tip_chamfer_length],
    [tip_r(tip_length,delta)-tip_edge_chamfer,z0+tip_length]
];

function handle_profile() = concat(
    [[handle_diameter/2,grip_start-1]],
    grip_ring_count > 0 ?
        [for(i=[0:grip_ring_count-1])
            let(z=grip_start+i*grip_ring_pitch)
            each [[handle_diameter/2,z],
                  [handle_diameter/2+grip_ring_height,z+0.30],
                  [handle_diameter/2+grip_ring_height,z+0.65],
                  [handle_diameter/2,z+1.0]]] : [],
    [[handle_diameter/2,tip_z-1],
     [handle_diameter/2-0.5,tip_z]]
);

// Closing the section along the inside wall makes one continuous open bore.
// Both ends have small entry chamfers; nothing bridges/caps the passage.
module hollow_revolve(profile,height) {
    rotate_extrude(convexity=10)
        polygon(concat(profile,[
            [bore_r+bore_edge_chamfer,height],
            [bore_r,height-bore_edge_chamfer],
            [bore_r,bore_edge_chamfer],
            [bore_r+bore_edge_chamfer,0]
        ]));
}

module adapter() {
    hollow_revolve(concat(hose_profile(),handle_profile(),tip_profile(tip_z)),total_length);
}

// Countable notches on coupon rim identify the size without a font dependency.
// Keep them away from mating surfaces and from the central flow passage.
module rim_notches(count,radius,z) {
    for(i=[0:count-1])
        rotate([0,0,(i-(count-1)/2)*22])
            translate([radius,0,z-0.8])
                cylinder(r=0.75,h=0.8+eps,$fn=24);
}

module tip_coupon(delta,mark) {
    difference() {
        hollow_revolve(concat(
            [[coupon_base_r,0],[coupon_base_r,coupon_base_h]],
            tip_profile(coupon_base_h,delta)
        ),coupon_base_h+tip_length);
        rim_notches(mark,coupon_base_r,coupon_base_h);
    }
}

module hose_coupon(delta,mark) {
    difference() {
        hollow_revolve(concat(hose_profile(delta),
            [[handle_diameter/2,hose_length+5]]),hose_length+5);
        rim_notches(mark,handle_diameter/2,hose_length+5);
    }
}

module fit_tests() {
    for(i=[0:2]) {
        delta=(i-1)*fit_step;
        translate([(i-1)*coupon_pitch,0,0]) tip_coupon(delta,i+1);
        translate([(i-1)*coupon_pitch,coupon_row_pitch,0]) hose_coupon(delta,i+1);
    }
}

assert(part=="adapter" || part=="fit_tests" || part=="section", "Unknown part selection.");
assert(resolution>=32 && resolution==floor(resolution), "Resolution must be an integer >=32.");
assert(tip_length>tip_chamfer_length && tip_end_diameter<tip_base_diameter,
       "Tip must taper toward its free end.");
assert(bore_r>0 && bore_edge_chamfer>=0 && tip_edge_chamfer>=0, "Invalid bore/chamfer.");
assert(hose_ring_count>=1 && hose_ring_count==floor(hose_ring_count), "Use an integer hose ring count.");
assert(grip_ring_count>=0 && grip_ring_count==floor(grip_ring_count), "Use an integer grip ring count.");
assert(hose_ring_height>=0 && grip_ring_height>=0 && grip_ring_pitch>1,
       "Invalid ring size/spacing.");
assert(hose_shoulder_length>0 && stem_length>12 && (stem_length-7)/hose_ring_count>1,
       "Hose is too short for these rings.");
assert(hose_root_oversize>=0 && hose_end_undersize>=0 && fit_step>0,
       "Invalid fit parameter.");
assert(grip_start>hose_length+2, "Handle too short for the grip rings.");
assert(tip_r(0,part=="fit_tests" ? fit_step : 0)<handle_diameter/2-0.5,
       "Increase handle diameter or reduce tip diameter.");
assert(hose_r(stem_length,part=="fit_tests" ? fit_step : 0)+hose_ring_height<handle_diameter/2,
       "Increase handle diameter or reduce hose diameter.");
// Require at least a 1.0 mm radial wall, including the smallest fit coupons.
assert(min(tip_r(tip_length,part=="fit_tests" ? -fit_step : 0)-tip_edge_chamfer,
           hose_r(0,part=="fit_tests" ? -fit_step : 0)-0.15)
           -(bore_r+bore_edge_chamfer)>=1.0,
       "Connector mouth wall below 1.0 mm: reduce bore or enlarge connector.");

echo("Nominal adapter length (mm)",total_length);
echo("Tip cone diameters before end chamfer (mm)",
     2*tip_r(tip_length),2*tip_r(0));
echo("Continuous bore diameter (mm)",2*bore_r);
echo("CAD passage volume before small chamfers (mL)",PI*bore_r*bore_r*total_length/1000);
echo("Fit coupons: 1 notch=-step; 2=nominal; 3=+step. Step (diameter mm)",fit_step);

color([0.88,0.10,0.39])
    if(part=="adapter") adapter();
    else if(part=="fit_tests") fit_tests();
    else difference() {
        adapter();
        translate([-handle_diameter*2,0,-eps])
            cube([handle_diameter*4,handle_diameter*2,total_length+2*eps]);
    }
