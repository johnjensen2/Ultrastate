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

775_dia=55;
775_hub= 17.5;
775_screw_dia = 4.2;
775_screw_mount = 29;
775_mount_thick = 15;

775_washer_thick=1.5;
775_washer_dia =10;

775_screwhead_dia=7.1;

nozzle_dia=55;
impeller_height=25;
stand_height=100;

difference()
{
hull()
{
cylinder(h=lock_thickness*2,d=PVC_OD+Wall_thickness*2);

translate([-16,10,0])
cube([82,10,lock_thickness*2]);

}

translate([0,0,-1])
cylinder(h=lock_thickness*2+2,d=PVC_OD);

//mount hole
translate([37.5,15,lock_thickness])
rotate([90,0,0])
cylinder(h=50,d=46);

translate([37.5,21,lock_thickness])
rotate([90,0,0])
775_holes();

}

module 775_holes()
{


// mount hub    
    translate([0,0,-1])
    cylinder(h=775_mount_thick+2, d=775_hub);

//screw holes
    translate([775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=775_screw_dia);

    translate([-775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=775_screw_dia);

    

}
