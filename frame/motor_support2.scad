
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
union()
{
body();
translate([145,0,0])
mirror([1,0,0])
body();
mount();


}
translate([55,0,50])
cylinder(h=15,d=5);

translate([90,0,50])
cylinder(h=15,d=5);

translate([-30,12.5,-50])
cube([200,5,200]);

translate([-30,-17.5,-50])
cube([200,5,200]);
}





module mount()
{
difference()
{
union()
{
hull()
{
translate([35,0,0])
rotate([0,90,0])
cylinder(h=75,d=pvc_od+5);


translate([0,9,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([0,-9,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([50,9,52])
rotate([0,90,0])
cylinder(h=45,d=7.5);

translate([50,-9,52])
rotate([0,90,0])
cylinder(h=45,d=7.5);
}
}

translate([0,37,0])
rotate([90,0,0])
cylinder(h=75,d=pvc_od);

translate([145,37,0])
rotate([90,0,0])
cylinder(h=75,d=pvc_od);



translate([72.5,12.5,30])
rotate([90,0,0])
 775_mount();

}

}

module body()
{
difference()
{
pos();
neg();



translate([0,0,0])
cylinder(h=pvc2+2,d=screw_hole_dia);

}

}

module pos()
{


hull()
{
translate([0,0,0])
rotate([0,90,0])
cylinder(h=35,d=pvc_od+5);

translate([0,12.5,0])
rotate([90,0,0])
cylinder(h=25,d=pvc_od+5);
}
}

module neg()
{


translate([0,37,0])
rotate([90,0,0])
cylinder(h=75,d=pvc_od);


}



module 775_mount()
{
// 775 mount holes
    translate([0,0,-1])
    cylinder(h=775_mount_thick+2, d=775_hub);

    //cut for motor
    translate([0,0,-10])
    cylinder(h=50, d=775_dia);
    
    translate([775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=4.2);

    translate([-775_screw_mount/2,0,-1])
    cylinder(h=775_mount_thick+2, d=4.2);

    translate([775_screw_mount/2,0,-1])
    cylinder(h=3, d=7.5);

    translate([-775_screw_mount/2,0,-1])
    cylinder(h=3, d=7.5);
    
}