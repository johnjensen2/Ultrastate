
$fn=360;

pvc_od=22;
wall=5;
clamp_lenght=50;
pvc2=pvc_od+wall;
plate_height=5;
screw_hole_dia=5;




difference()
{
hull()
{
translate([0,-10,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([50,-10,50])
rotate([0,90,0])
cylinder(h=45,d=7.5);


translate([0,100,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([50,100,50])
rotate([0,90,0])
cylinder(h=45,d=7.5);
}

translate([0,0,-1])
hull()
{
translate([0,-11,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([51,-11,50])
rotate([0,90,0])
cylinder(h=43,d=7.5);


translate([0,115,0])
rotate([0,90,0])
cylinder(h=145,d=7.5);

translate([51,115,50])
rotate([0,90,0])
cylinder(h=43,d=7.5);
}


translate([-50,-25,-30])
cube([200,25,100]);

translate([-50,100,-30])
cube([200,25,100]);
hull()
{
translate([0,115,0])
rotate([90,0,0])
cylinder(h=140,d=pvc_od+5);

translate([150,115,0])
rotate([90,0,0])
cylinder(h=140,d=pvc_od+5);
}

translate([0,5,0])
ziptie();
translate([0,95,0])
ziptie();
}


module ziptie()
{
hull()
{
translate([110,0,0])
cylinder(h=50,d=5);

translate([115,0,])
cylinder(h=50,d=5);
}

hull()
{
translate([35,0,0])
cylinder(h=50,d=5);

translate([30,0,0])
cylinder(h=50,d=5);
}
}
