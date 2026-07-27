// ============================================================
// HANGING TUBE WITH MALE LUER-LOCK CONNECTOR
//
// Connects to:
// Value Plastics FTLLB220-6005
// Female luer thread panel-mount fitting
//
// Rectangular/triangular side opening removed.
// ============================================================


// -------------------------
// General parameters
// -------------------------

resolution = 120;
$fn = resolution;

outer_radius = 6;
inner_radius = 2.75;

tube_height = 100;


// -------------------------
// Connector transition
// -------------------------

transition_height = 4;


// -------------------------
// Male luer taper
// -------------------------

// Nominal male luer taper is 6%.
luer_length = 8.2;
luer_taper  = 0.06;

// Negative = smaller/looser
// Positive = larger/tighter
luer_size_adjust = 0;

luer_tip_diameter =
    3.94 + luer_size_adjust;

luer_base_diameter =
    luer_tip_diameter +
    luer_taper * luer_length;

luer_bore_diameter = 1.6;


// -------------------------
// Male luer-lock collar
// -------------------------

lock_collar_outer_diameter = 10.5;
lock_collar_height         = 6.2;
lock_base_height           = 1.0;

thread_diametral_clearance = 0.20;

thread_crest_diameter =
    7.0 + thread_diametral_clearance;

thread_root_diameter =
    8.0 + thread_diametral_clearance;

// Double-start right-hand thread
thread_pitch = 2.5;
thread_lead  = thread_pitch * 2;

thread_start_height =
    lock_base_height + 0.20;

thread_height =
    lock_collar_height -
    thread_start_height +
    0.10;

thread_crest_half_width = 0.42;
thread_root_half_width  = 1.20;


// -------------------------
// Test-print option
// -------------------------

test_connector_only = false;


// ============================================================
// INTERNAL HELICAL THREAD RIDGE
// ============================================================

module internal_thread_ridge(start_angle = 0) {

    rotate([0, 0, start_angle])

        linear_extrude(
            height = thread_height,
            twist = -360 * thread_height / thread_lead,
            slices = 150,
            convexity = 12
        )

            polygon(points = [

                [
                    thread_crest_diameter / 2,
                    -thread_crest_half_width
                ],

                [
                    thread_root_diameter / 2 + 0.10,
                    -thread_root_half_width
                ],

                [
                    thread_root_diameter / 2 + 0.10,
                    thread_root_half_width
                ],

                [
                    thread_crest_diameter / 2,
                    thread_crest_half_width
                ]
            ]);
}


// ============================================================
// SOLID MALE LUER-LOCK CONNECTOR
// ============================================================

module male_luer_lock_solid() {

    union() {

        // Bottom plate connecting the collar and luer cone
        cylinder(
            h = lock_base_height,
            d = lock_collar_outer_diameter
        );


        // Hollow locking collar
        translate([0, 0, lock_base_height])

            difference() {

                cylinder(
                    h =
                        lock_collar_height -
                        lock_base_height,
                    d = lock_collar_outer_diameter
                );

                translate([0, 0, -0.1])

                    cylinder(
                        h =
                            lock_collar_height -
                            lock_base_height +
                            0.2,
                        d = thread_root_diameter
                    );
            }


        // First internal thread
        translate([
            0,
            0,
            thread_start_height
        ])
            internal_thread_ridge(0);


        // Second internal thread
        translate([
            0,
            0,
            thread_start_height
        ])
            internal_thread_ridge(180);


        // Male luer sealing cone
        translate([
            0,
            0,
            lock_base_height
        ])

            cylinder(
                h  = luer_length,
                d1 = luer_base_diameter,
                d2 = luer_tip_diameter
            );
    }
}


// ============================================================
// LUER FLOW PASSAGE
// ============================================================

module male_luer_lock_bore() {

    translate([0, 0, -0.2])

        cylinder(
            h =
                lock_base_height +
                luer_length +
                0.4,
            d = luer_bore_diameter
        );
}


// ============================================================
// COMPLETE TEST CONNECTOR
// ============================================================

module male_luer_lock() {

    difference() {

        male_luer_lock_solid();

        male_luer_lock_bore();
    }
}


// ============================================================
// MAIN TUBE SOLID
// ============================================================

module complete_solid() {

    union() {

        // Main hanging tube
        cylinder(
            h = tube_height,
            r = outer_radius
        );


        // Transition from tube to luer connector
        translate([0, 0, tube_height])

            cylinder(
                h  = transition_height,
                d1 = outer_radius * 2,
                d2 = lock_collar_outer_diameter
            );


        // Male luer-lock connector
        translate([
            0,
            0,
            tube_height + transition_height
        ])

            male_luer_lock_solid();
    }
}


// ============================================================
// COMPLETE INTERNAL FLOW PASSAGE
// ============================================================

module complete_bore() {

    // Main circular tube passage
    translate([0, 0, -1])

        cylinder(
            h = tube_height + 1.1,
            r = inner_radius
        );


    // Smooth reduction into luer bore
    translate([
        0,
        0,
        tube_height - 0.1
    ])

        cylinder(
            h =
                transition_height +
                lock_base_height +
                0.2,
            d1 = inner_radius * 2,
            d2 = luer_bore_diameter
        );


    // Passage through the male luer
    translate([
        0,
        0,
        tube_height + transition_height
    ])

        male_luer_lock_bore();
}


// ============================================================
// FINAL FULL MODEL
// ============================================================

module complete_model() {

    difference() {

        complete_solid();

        complete_bore();

        // Side groove removed.
    }
}


// ============================================================
// OUTPUT
// ============================================================

if (test_connector_only) {

    male_luer_lock();

} else {

    complete_model();
}