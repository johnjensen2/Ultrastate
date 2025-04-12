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


775_dia=60;
775_hub= 26;
775_screw_dia = 4.2;
775_screw_mount = 35;
775_mount_thick = 8;

775_washer_thick=1.5;
775_washer_dia =10;

775_screwhead_dia=7.1;


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




translate([0,0,9.5])
cube([82,60,1], center=true);

}

translate([10,0,14])
cylinder(h=50,d=43);


translate([10,0,9])
775_mount();


translate([-55,0,-1])
cylinder(h=50,d=50);



translate([-25,80,25])
rotate([90,0,0])
cylinder(h=160,d=PVC_OD);




//screw holes

translate([-100,30,25])
rotate([0,90,0])
cylinder(h=100,d=5);

translate([-100,-30,25])
rotate([0,90,0])
cylinder(h=100,d=5);
}




module 775_mount()
{
// 775 mount holes
    translate([0,0,-1])
    cylinder(h=775_mount_thick+2, d=775_hub);

    translate([775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=4.2);

    translate([-775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=4.2);

    translate([775_screw_mount/2,0,-1])
    cylinder(h=3, d=7.5);

    translate([-775_screw_mount/2,0,-1])
    cylinder(h=3, d=7.5);
    
}