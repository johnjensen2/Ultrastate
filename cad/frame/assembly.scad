
top=120;


frame_side();
translate([400,0,0])
mirror([1,0,0])
frame_side();
pipes();
deck_mounts();


module gears()
{
translate([-20,-130,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");

translate([-130,-130,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");



translate([-20,-350,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");

translate([-130,-350,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");



translate([-20,-550,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");

translate([-130,-550,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7.stl");


union()
{
translate([-110,-100,60])
rotate([0,90,0])
cylinder(h=90,d=22);

translate([-110,-170,60])
rotate([0,90,0])
cylinder(h=90,d=22);
}
translate([0,-215,0])

union()
{
translate([-110,-100,60])
rotate([0,90,0])
cylinder(h=90,d=22);

translate([-110,-170,60])
rotate([0,90,0])
cylinder(h=90,d=22);
}

translate([0,-430,0])

union()
{
translate([-110,-100,60])
rotate([0,90,0])
cylinder(h=90,d=22);

translate([-110,-170,60])
rotate([0,90,0])
cylinder(h=90,d=22);
}


}




module track()
{
translate([-75,-135,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");

translate([-75,-135,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");


translate([-75,-350,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");

translate([-75,-350,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");

translate([-75,-565,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");

translate([-75,-565,60])
rotate([0,-90,0])
import("/home/john/Documents/Water_Tank/gear_7_idler_new2.stl");


translate([-50,-100,35])
rotate([90,0,0])
cylinder(h=500,d=22);

translate([-50,-100,85])
rotate([90,0,0])
cylinder(h=500,d=22);


translate([-100,-100,35])
rotate([90,0,0])
cylinder(h=500,d=22);

translate([-100,-100,85])
rotate([90,0,0])
cylinder(h=500,d=22);


union()
{
translate([-75,-250,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/frame/track_90.stl");

translate([-122.5,-250,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/frame/track_90.stl");
}


translate([0,-200,0])
union()
{
translate([-75,-250,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/frame/track_90.stl");

translate([-122.5,-250,60])
rotate([0,90,0])
import("/home/john/Documents/Water_Tank/frame/track_90.stl");
}


}
module deck_mounts()
{

translate([75,0,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");

translate([250,0,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");


translate([50,-250,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");

translate([300,-250,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");



translate([50,-450,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");

translate([300,-450,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");

translate([50,-600,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");

translate([300,-600,0])
rotate([90,180,90])
import("/home/john/Documents/Water_Tank/frame/plate_mount.stl");
}

module pipes()
{
//top rails
translate([0,0,0])
rotate([90,0,0])
cylinder(h=600,d=22);

translate([400,0,0])
rotate([90,0,0])
cylinder(h=600,d=22);

// bottom rails
translate([0,-130,top])
rotate([90,0,0])
cylinder(h=470,d=22);

translate([400,-130,top])
rotate([90,0,0])
cylinder(h=470,d=22);



//cross bars

translate([0,0,0])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-130,top])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-250,0])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-250,top])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-450,0])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-450,top])
rotate([0,90,0])
cylinder(h=400,d=22);


translate([0,-600,0])
rotate([0,90,0])
cylinder(h=400,d=22);

translate([0,-600,top])
rotate([0,90,0])
cylinder(h=400,d=22);
}

module frame_side()
{
//base of assembly
track();
gears();

import("/home/john/Documents/Water_Tank/frame/3-way_45neg.stl");
// 45 rod 
translate([0,-5,0])
rotate([45,0,0])
cylinder(h=150,d=22);

mirror([1,0,0])
translate([0,-130,top])
rotate([0,180,0])
import("/home/john/Documents/Water_Tank/frame/3-way_45.stl");


//first cross all 200 mm

//bottom
translate([0,-250,0])
import("/home/john/Documents/Water_Tank/frame/4-way_90.stl");

//top
translate([0,-250,top])
rotate([180,0,0])
import("/home/john/Documents/Water_Tank/frame/4-way_90.stl");

// pipe
translate([0,-250,0])
cylinder(h=100,d=22);


translate([0,-250,60])
rotate([0,90,-90])
import("/home/john/Documents/Water_Tank/frame/3-way_flat.stl");



//bottom
translate([0,-450,0])
import("/home/john/Documents/Water_Tank/frame/4-way_90.stl");

//top
translate([0,-450,top])
rotate([180,0,0])
import("/home/john/Documents/Water_Tank/frame/4-way_90.stl");

// pipe
translate([0,-450,0])
cylinder(h=top,d=22);

// added

translate([0,-450,60])
rotate([0,90,-90])
import("/home/john/Documents/Water_Tank/frame/3-way_flat.stl");


// rear of tank

translate([0,-600,top])
rotate([180,0,0])
import("/home/john/Documents/Water_Tank/frame/3-way.stl");

translate([0,-600,0])
rotate([0,0,90])
import("/home/john/Documents/Water_Tank/frame/3-way.stl");
// pipe
translate([0,-600,0])
cylinder(h=top,d=22);
}

