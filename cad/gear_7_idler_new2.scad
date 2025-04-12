$fn=360;

/* Tank boat link starting at 22mm in ID
5 mm thick walls and M5 set screw
gear will use inner retainer as pivot point
*/


PVC_OD=22;
lock_thickness=25;
Wall_thickness=5;
screw_diameter=4.8;
angles=360/7;
fudge=2;



difference()
{
hull()
{
translate([-25,40,25])
rotate([90,0,0])
cylinder(h=80,d=PVC_OD+10);


translate([25,40,25])
rotate([90,0,0])
cylinder(h=80,d=PVC_OD+10);


translate([0,35,9])
cylinder(h=32,d=PVC_OD+10);


translate([0,-35,9])
cylinder(h=32,d=PVC_OD+10);

translate([0,0,9.5])
cube([82,80,1], center=true);

}

translate([0,0,38])
cylinder(h=lock_thickness,d=PVC_OD+Wall_thickness*2+5);


translate([0,0,-1])
cylinder(h=50,d=PVC_OD);


translate([0,35,-1])
cylinder(h=50,d=PVC_OD);


translate([0,-35,-1])
cylinder(h=50,d=PVC_OD);

translate([55,0,-1])
cylinder(h=50,d=50);


translate([-55,0,-1])
cylinder(h=50,d=50);



translate([-25,80,25])
rotate([90,0,0])
cylinder(h=160,d=PVC_OD);

translate([25,80,25])
rotate([90,0,0])
cylinder(h=160,d=PVC_OD);


//screw holes


translate([0,80,25])
rotate([90,0,0])
cylinder(h=160,d=5);


translate([-50,30,25])
rotate([0,90,0])
cylinder(h=100,d=5);

translate([-50,-30,25])
rotate([0,90,0])
cylinder(h=100,d=5);
}