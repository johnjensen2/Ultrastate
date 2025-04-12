
$fn=360;

pvc_od=22;
wall=5;
clamp_lenght=50;
pvc2=pvc_od+wall;
plate_height=5;
screw_hole_dia=5;

775_dia=42;
775_hub= 26;
775_screw_dia = 4.2;
775_screw_mount = 35;
775_mount_thick = 8;

775_washer_thick=1.5;
775_washer_dia =10;

775_screwhead_dia=7.1;


inner_size = 34;
outter_size = 40;
fitting_height= 40;

difference()
{

translate([0,12.5,0])
rotate([90,0,0])
cylinder(h=25,d=pvc_od+5);

translate([0,13,0])
rotate([90,0,0])
cylinder(h=26,d=pvc_od);
}